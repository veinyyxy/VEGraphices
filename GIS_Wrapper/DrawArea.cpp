/*============================================================ 
文件名：DrawArea.cpp 
类 名： DrawArea
父 类： ManualDraw
子 类： 无 
功能说明：绘制灾害图
调用说明：
1、获取用户的PING命令鼠标坐标点，调用下层算法，返回计算得到的点集；
2、将屏幕点转换成OPENGL的坐标，并传给相应的绘图管道；

-----------------------------版本更新-------------------------
============================================================*/
#include "DrawArea.h"
#include <QtCore/QSettings>
//#include <QtCore/QTextCodec>
#include "ILayerAttribute.h"
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>
#include "ReadColorTableFromXML.h"

// 
// typedef struct
// {
// 	double dLon;
// 	double dLat;
// 	double dValue;
// }ContourValue;

int DrawArea::m_nCount = 1;

DrawArea::DrawArea(QString str):m_strname(str)
{
	pline = new Line;
	m_bFinished = false;
	pArray = NULL;
//	m_pContourLine = NULL;
	m_pMultiContourLine = NULL;
	m_bFirst = true;
	m_pSpLine =  new CCardinalSpline;//创建基数样条对象
	m_pSymbolData = new SymbolData;
	pOriginalArray = new QVector3DArray;
	m_fStep = 0.1f/10000;	
	m_bCloseLine = true;	

	if (colortablelist.isEmpty())
	{
		InitColorlist();		
	}	
	SetCurrentColorTable(tr("灾害"));

	m_layertype = Layer::ELayerType::Disaster_Layer;
}

DrawArea::~DrawArea(void)
{
	//销毁基数样条对象
	if(NULL != m_pSpLine)
	{
		delete m_pSpLine;
		m_pSpLine = NULL; 
	}
}

/************************************************************************/
/* 函数功能： 鼠标左键：收集点集，并绘制显示；鼠标右键：结束收集       	*/
/* 参数说明:  QMouseEvent 鼠标事件                 						*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 25 日										*/
/************************************************************************/
void DrawArea::MousePress(QMouseEvent *event)
{
	QVector3D glPoint, scPoint(event->x(), event->y(), 0.0f);	

	Layer::ELayerType type = GetParentLayer()->LayerType();

	if(type == Layer::ELayerType::Station_Layer)				//站点数据是错误
	{
		GetParentLayer()->LayerWarning();
		return;
	}
	
	if(type != Layer::ELayerType::Symbol_Layer && type != m_layertype)
	{
		GetParentLayer()->LayerWarning();
		return;
	}
	else if(type == Layer::ELayerType::Symbol_Layer) //若当前图层为符号图层，修改为灾害图层
	{
		GetParentLayer()->LayerType(m_layertype);
	}

	if (event->buttons() & Qt::LeftButton)
	{
		//按下左键
		if(!m_bFinished)
		{
			m_InputData.Clear();
			m_CrossPoints.clear();
			m_bFinished = true;
			pArray = NULL;
		}

		pArray = pline->getVectexArray();
		m_InputData.push_back(scPoint);

		if(m_InputData.size()<3)
		{
			m_OutputData.Clear();
			for(int i=0; i<m_InputData.size(); i++)
				m_OutputData<<QVector3D(m_InputData[i].x(), m_InputData[i].y(), m_InputData[i].z());
		}
		else
		{	//取得平滑后的点数组
			m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);
		}

		QVector4DArray *pColor = pline->getColorArray();

		//清除上次的点数组
		pArray->Clear();
		pColor->clear();
		for(int i = 0 ; i < m_OutputData.count(); i++)
		{
			ScreenToGL(m_OutputData[i], glPoint);
			pColor->push_back(QVector4D(0.0, 0.0, 0.0, 1.0));
			pArray->push_back(glPoint);
		}
		pline->SetColorArray();		
		if(m_bFirst)
		{
			m_ParentLayer->GetLayerData()->InsertRenderable(pline);
			m_bFirst = false;
			//m_InputData.pop_back();
			//m_bFirst=false;
		}
	}
	else if(event->buttons() & Qt::RightButton)
	{
		if(m_bFinished)				//按下右键，开始画双线
		{
			m_bFinished = false;
			if(pArray->count() <2)  return;          //只有一个点，退出
			/*
			m_pContourLine = new CSymbolContourLine;
			QVector3DArray * ArrayTemp = m_pContourLine->GetArray(); 
			pOriginalArray = m_pContourLine->OriginalArray();	
			m_pContourLine->IsMark(false);
			GetAttribute(m_pContourLine);
			m_pContourLine->SetCloseLine(m_bCloseLine);
			*/

			m_pMultiContourLine = new CSymbolMultiContourLine;
			QVector3DArray * ArrayTemp = m_pMultiContourLine->GetArray(); 
			pOriginalArray = m_pMultiContourLine->OriginalArray();	
			m_pMultiContourLine->IsMark(false);
			GetAttribute(m_pMultiContourLine);
			m_pMultiContourLine->SetCloseLine(m_bCloseLine);
			m_pMultiContourLine->SetName(m_strname);
			m_pMultiContourLine->SetKey(m_strkeymap.value(m_strname));
			m_pMultiContourLine->SetColor(m_colorlist.value(m_strname));

			QVector3D temp;
			if(m_bCloseLine)
			{
				temp = m_InputData[0];
				m_InputData<<temp;				
			    int n0 = m_InputData.count();
				//取得平滑后的点数组
				m_OutputData.Clear();
				m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);			

				if (m_lineanalysis.isPolygonintersect(m_OutputData))
				{
					QMessageBox msg;
					msg.setText("多边形中出现线段相交！");
					msg.exec();
					m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
					m_bFirst = true;
					return;
				}
				for(int i = 0 ; i < m_OutputData.count(); i++)
				{		
					ScreenToGL(m_OutputData[i], glPoint);
					glPoint.setZ(m_nCount*m_fStep);
					ArrayTemp->push_back(glPoint);
					temp = TransGLtoWGS84(glPoint);
					temp.setZ(m_nCount* m_fStep);
					pOriginalArray->push_back(temp);
				}
				pArray->Clear();	

				if(!m_bFirst)
				{
					m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
					m_bFirst = true;
				}	

				//m_ParentLayer->SetColorIndex(m_currentcolorindex);

				{
					//undo机制
		/*			CUndoAddDrawCommand* addCommand = new CUndoAddDrawCommand();
					addCommand->SetUndoLayer(m_ParentLayer);
					addCommand->SetUndoRenderable(m_pMultiContourLine);
					addCommand->SetUndoGLWidget(m_pOpenglView);*/
					//if(m_undoStack)
					//{
					//	m_undoStack->push(addCommand);
					//}	
					m_nCount++; 
				}
				
			}			
		}
	}	
	m_ParentLayer->TransformLayer();
}

/************************************************************************/
/* 函数功能： 响应鼠标移动事件					                        */
/* 参数说明:  QRenderable* volume  : 绘制								*/
/*			  QMouseEvent* event   : 鼠标点								*/	
/* 返 回 值:  NULL														*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 05 月 10 日										*/
/************************************************************************/
void DrawArea::MouseEvent(QMouseEvent* event)
{
	QVector3D  glPoint, scPoint(event->x(), event->y(), 1.0f);

	if(m_bFinished)
	{
		//取得平滑后的点数组
		m_InputData<<scPoint;
		m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);
		QVector4DArray *pColor = pline->getColorArray();
		pArray = pline->getVectexArray();
		//清除上次的点数组
		pArray->Clear();			
		pColor->clear();
		int j=0;
		QVector3D topLefPoint, topRightPoint, bottomLeftPoint, bottomRightPoint;
		for(int i = 0 ; i < m_OutputData.count(); i++)
		{
			ScreenToGL(m_OutputData[i], glPoint);
			glPoint.setZ(1.0f);
			pColor->push_back(QVector4D(0.0, 0.0, 0.0, 1.0));
			pArray->push_back(glPoint);
		}
		pline->SetColorArray();

		if(m_bFirst)
		{
			m_ParentLayer->GetLayerData()->InsertRenderable(pline);
		}
		m_InputData.pop_back();
		m_OutputData.Clear();
	}
}

/************************************************************************/
/* 函数功能： 响应鼠标弹起事件					                        */
/* 参数说明:  QRenderable* volume  : 绘制								*/
/*			  QMouseEvent* event   : 鼠标点								*/	
/* 返 回 值:   NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
void DrawArea::MouseRelease(QMouseEvent *event)
{
	return;
}

/************************************************************************/
/* 函数功能： 鼠标离开事件					                            */
/* 参数说明:  QRenderable* volume  : 绘制								*/
/*			  QMouseEvent* event   : 鼠标点								*/	
/* 返 回 值:   NULL														*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 06 月 29 日										*/
/************************************************************************/
void DrawArea::LeaveEvent()
{
	if(m_bFinished)				//没有按下右键，鼠标移开
	{
		m_bFinished = false;

		//清除记录
		if(pArray != NULL)
			pArray->Clear();

		if(!m_bFirst)
		{
			m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
			m_bFirst = true;
		}
	}
	return;
}


void DrawArea::setILayerAttrib( ILayerAttribute *Ilayer )
{
	
}

void DrawArea::GetAttribute(CSymbolMultiContourLine *pContourLine)
{
	if (pContourLine!=NULL)
	{
		CSymbolAttribute * pAttr = pContourLine->GetAttribute();

		if (m_colorlist.contains(m_strname))
		{
			int r,g,b;
			QColor tempcolor = m_colorlist.value(m_strname);
			pAttr->SetColor(QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),1.0f));
			pAttr->SetFillType(true);	
			//pAttr->SetFillColor(QVector4D(255,0,0,1));
			pAttr->SetFillColor( QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),1.0f));
			pAttr->SetLineWidth(1.0f);	
			pAttr->SetLineType(QString("实线"));
			pAttr->SetLayerZ(0);		
		}
	}		
}

/************************************************************************/
/* 函数功能： 根据属性值设置颜色数据       			        					*/
/* 参数说明:  无                 										*/
/* 返 回 值:  无                 										*/
/* 作    者:  zhanglei													*/
/* 日    期:  2011 年 04 月 25 日										*/
/************************************************************************/
/*
bool  DrawArea::GetContourAttribute(double index)
{
	CSymbolAttribute * pAttr = m_pContourLine->GetAttribute();
	if (m_currentColorTable.contains(index))
	{
		QColor tempcolor = m_currentColorTable.value(index);
	//	QVector4D color = QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),tempcolor.alphaF());
		int value = index;
		if(m_areaType ==RAIN_AREA)
		{
			switch(value)
			{
			case 50:
				index = 50;
				break;
			case 20:
				index = 20;
				break;
			case 0:
				index = 10;
				break;
			case -10:
				index = -10;
				break;
			case -20:
				index = -20;
				break;
			case -50:
				index = -50;
				break;
			default:
				break;
			}
		}

		pAttr->SetValue(index);
		pAttr->SetColor(QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),0.0f));
		pAttr->SetFillType(true);	
		//pAttr->SetFillColor(QVector4D(255,0,0,1));
		pAttr->SetFillColor( QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),1.0f));
		pAttr->SetLineWidth(1.0f);	
		pAttr->SetLineType(QString("实线"));
		pAttr->SetLayerZ(0);

		return true;
	}
	else
	{
		return false;
	}
}
*/
/************************************************************************/
/* 函数功能： 在输入的控制点上画十字		        					*/
/* 参数说明:  无                 										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 05 月 10 日										*/
/************************************************************************/
void DrawArea::GetCrossLineData(QVector3D scPoint, int i)
{
	QVector3D topLeftPoint, topRightPoint, bottomLeftPoint, bottomRightPoint;

	topLeftPoint.setX(scPoint.x()-6);
	topLeftPoint.setY(scPoint.y()+6);
	topLeftPoint.setZ(0.0f);
	
	topRightPoint.setX(scPoint.x()+6);
	topRightPoint.setY(scPoint.y()+6);
	topRightPoint.setZ(0.0f);
		
	bottomLeftPoint.setX(scPoint.x()-6);
	bottomLeftPoint.setY(scPoint.y()-6);
	bottomLeftPoint.setZ(0.0f);
	
	bottomRightPoint.setX(scPoint.x()+6);
	bottomRightPoint.setY(scPoint.y()-6);
	bottomRightPoint.setZ(0.0f);
	m_CrossPoints[i]<<topLeftPoint<<topRightPoint<<bottomLeftPoint<<bottomRightPoint;
}

void DrawArea::InitColorlist()
{	
	/*
	if (m_colorlist.isEmpty())
	{
		m_colorlist.insert(QString("雪灾"),QColor(255.0f,255.0f,255.0f,255.0f));
		m_colorlist.insert(QString("气象干旱"),QColor(226.0f,202.0f,128.0f,255.0f));
		m_colorlist.insert(QString("低温雨雪冰冻"),QColor(161.0f,198.0f,180.0f,255.0f));
		m_colorlist.insert(QString("森林火线气象干旱"),QColor(224.0f,101.0f,83.0f,255.0f));
		m_colorlist.insert(QString("高温"),QColor(221.0f,156.0f,124.0f,255.0f));
		m_colorlist.insert(QString("地质灾害"),QColor(178.0f,168.0f,133.0f,255.0f));
		m_colorlist.insert(QString("伏旱"),QColor(221.0f,156.0f,124.0f,255.0f));
		m_colorlist.insert(QString("台风雨涝"),QColor(95.0f,101.0f,213.0f,255.0f));
		m_colorlist.insert(QString("初霜偏早"),QColor(209.0f,216.0f,200.0f,255.0f));
		m_colorlist.insert(QString("9-10月秋雨偏多"),QColor(95.0f,102.0f,209.0f,255.0f));
		m_colorlist.insert(QString("初夏阶段性低温"),QColor(146.0f,156.0f,205.0f,255.0f));
		m_colorlist.insert(QString("夏季洪涝"),QColor(95.0f,102.0f,209.0f,255.0f));
	}
	else if (!m_colorlist.contains(m_strname))
	{
		m_colorlist.insert(m_strname,QColor(51.0f,151.0f,255.0f,255.0f));
	}
	*/
	ReadColorTableFromXML reader;	
	reader.readFile(CIPASApplication::ConfigPath()+ "BasicInfo/CipasColorTable.xml",colortablelist);	
	
}

void DrawArea::SetCurrentColorTable(QString str)
{
	if (colortablelist.contains(str))
	{
		m_curcolortable = colortablelist.value(str);

		m_colorlist.clear();
		m_strkeymap.clear();
		m_currentcolorindex.clear();
		COLORTABLE::Iterator it;
		for (it=m_curcolortable.begin();it!=m_curcolortable.end();++it)
		{
			m_colorlist.insert(it.value()._Name,it.value()._color);	
			m_strkeymap.insert(it.value()._Name,it.key());
	//		m_currentcolorindex.insert(it.key(),COLORITEM(it.value()._color,it.value()._Name));
		}
	}	
}

void DrawArea::SetLayerType(Layer::ELayerType type)
{
	m_layertype = type;
}

/************************************************************************/
/* 根据绘制类型                                                                     */
/************************************************************************/
/*
void DrawArea::InitCoutourArray()
{
	m_CoutourZArray.clear();

	float fStep = 0.01 / 10;
	if(m_areaType == TEMPRATRUE_AREA)
	{
		m_CoutourZArray.insert(0.0, 0);
		m_CoutourZArray.insert(-1.0,  fStep);
		m_CoutourZArray.insert(-2.0, fStep * 2);
		m_CoutourZArray.insert(-3.0,  fStep * 3);
		m_CoutourZArray.insert(1.0,  fStep * 4);
		m_CoutourZArray.insert(2.0, fStep * 5);
	}else if(m_areaType == RAIN_AREA)
	{
		m_CoutourZArray.insert(-10.0, 0);
		m_CoutourZArray.insert(-20.0, fStep);
		m_CoutourZArray.insert(-50.0, fStep * 2);
		m_CoutourZArray.insert(0.0, fStep * 3);
		m_CoutourZArray.insert(20.0,  fStep * 4);
		m_CoutourZArray.insert(50.0,  fStep * 5);
	}
	else if(m_areaType == RAIN_YUBAO_AREA)
	{
		m_CoutourZArray.insert(1.0, 0);
		m_CoutourZArray.insert(0.0, fStep);		
		m_CoutourZArray.insert(10.0, fStep*2 );
		m_CoutourZArray.insert(25.0, fStep * 3);
		m_CoutourZArray.insert(50.0,  fStep * 4);
		m_CoutourZArray.insert(100.0,  fStep * 5);
		m_CoutourZArray.insert(200.0, fStep * 6);
		m_CoutourZArray.insert(300.0, fStep * 7);
		m_CoutourZArray.insert(400.0,  fStep * 8);
		m_CoutourZArray.insert(600.0,  fStep *9);
	}
}
*/

