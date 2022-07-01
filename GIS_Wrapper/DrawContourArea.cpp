/*============================================================ 
文件名：DrawContourLine.cpp 
类 名： DrawContourLine
父 类： ManualDraw
子 类： 无 
功能说明：画等值线的算法
调用说明：
1、获取用户的PING命令鼠标坐标点，调用下层算法，返回计算得到的点集；
2、将屏幕点转换成OPENGL的坐标，并传给相应的绘图管道；

-----------------------------版本更新-------------------------
============================================================*/
#include "Const.h"
#include "DrawContourArea.h"
#include <QtCore/QSettings>
//#include <QtCore/QTextCodec>
#include "ILayerAttribute.h"
#include <QtWidgets/QInputDialog>
#include "SaveColorTableToXML.h"
#include "ReadColorTableFromXML.h"
#include <QtWidgets/QMessageBox>


typedef struct
{
	double dLon;
	double dLat;
	double dValue;
}ContourValue;

int DrawContourArea::m_nCount = 0;

DrawContourArea::DrawContourArea(Layer::ELayerType type):m_layertype(type)
{
	pline = new Line;
	m_bFinished = false;
	pArray = NULL;
	m_pContourLine = NULL;
	m_bFirst = true;
	m_pSpLine =  new CCardinalSpline;//创建基数样条对象
	m_pSymbolData = new SymbolData;
	pOriginalArray = new QVector3DArray;
	m_fStep = 0.1f/100000;	
	m_LineColor.setRed(240.00f);
	m_LineColor.setGreen(0);
	m_LineColor.setBlue(10.00f);
	m_LineColor.setAlpha(255);
	if (colortablelist.isEmpty())
	{
		InitColorTable();		
	}
	if (m_layertype == Layer::ELayerType::Temperature_Layer) //温度距平预报
	{ 
		m_fStep = 0.0001;
		SetCurrentColorTable(QString("0"));
	
	}
	if (m_layertype == Layer::ELayerType::Rain_Layer) //降水距平预报
	{
		m_fStep = 0.0001;
		SetCurrentColorTable(QString("1"));	
	}	
	if (m_layertype == Layer::ELayerType::Rain_Yubao_Layer)//降水预报
	{
		m_fStep = 0.000001;
		SetCurrentColorTable(QString("2"));		
	}
	m_bCloseLine = true;
}

DrawContourArea::~DrawContourArea(void)
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
void DrawContourArea::MousePress(QMouseEvent *event)
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
	else
	{
		//设置图层类型
		if (m_layertype == Layer::ELayerType::Temperature_Layer)
		{
			GetParentLayer()->LayerType(Layer::ELayerType::Temperature_Layer);
			bool bDraw= GetParentLayer()->GetIsDraw();
			if(!bDraw)
			{
				AddTempCountorLine();	
				GetParentLayer()->SetIsDraw(true);
			}
		}
		if (m_layertype == Layer::ELayerType::Rain_Layer)
		{
			GetParentLayer()->LayerType(Layer::ELayerType::Rain_Layer);
			bool bDraw= GetParentLayer()->GetIsDraw();
			if(!bDraw)
			{
				AddRainCountorLine();		
				GetParentLayer()->SetIsDraw(true);
			}
		}		
		if (m_layertype == Layer::ELayerType::Rain_Yubao_Layer)
		{
			GetParentLayer()->LayerType(Layer::ELayerType::Rain_Yubao_Layer);
			bool bDraw= GetParentLayer()->GetIsDraw();
			if(!bDraw)
			{
				AddRainYUBAOCountorLine();		
				GetParentLayer()->SetIsDraw(true);
			}
		}	
	}

	///开始绘制线条
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
			pColor->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
			pArray->push_back(glPoint);
		}
		pline->SetColorArray();

		if(m_bFirst)
		{
			m_ParentLayer->GetLayerData()->InsertRenderable(pline);
			m_bFirst = false;
		}

		//绘制控制点
		DrawCrossArray(m_InputData);
	}
	else if(event->buttons() & Qt::RightButton)
	{
		if(m_bFinished)				//按下右键，开始画双线
		{
			m_bFinished = false;
			if(pArray->count() <2)  return;          //只有一个点，退出

			m_pContourLine = new CSymbolContourLine;
			QVector3DArray * ArrayTemp = m_pContourLine->GetArray(); 
			pOriginalArray = m_pContourLine->OriginalArray();
			m_pContourLine->IsMark(true);
			m_pContourLine->HideBound(false);
			m_pContourLine->SetContourType((int)m_layertype);

			bool ok;
			double valueDouble = 0;
			QString strvalue;
		
			strvalue = QInputDialog::getItem(NULL, tr("数值"),	tr("选择数值:"), m_stringitems, 0, false, &ok);
			
			if (ok && !strvalue.isEmpty())
			{
				if (m_currentmap.contains(strvalue))
				{
                   valueDouble = m_currentmap.value(strvalue);
				}
			}

			if (!GetContourAttribute(valueDouble)) //如果没有获得属性数据则返回,清除临时数据
			{
				m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
				return;
			}

			m_ParentLayer->SetColorIndex(m_currentcolorindex);
			m_pContourLine->SetCloseLine(m_bCloseLine);

			QVector3D temp;
			if(m_bCloseLine)
			{
				temp = m_InputData[0];
				m_InputData<<temp;
				
				//取得平滑后的点数组
				m_OutputData.Clear();
				m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);

				if (m_lineanalysis.isPolygonintersect(m_OutputData))
				{
					QMessageBox msg;
					msg.setText(tr("多边形中出现线段相交！"));
					msg.exec();
					ClearCrossArray(m_InputData);
					m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
					m_bFirst = true;
					return;
				}

				float zValue = m_CoutourZArray.value(valueDouble);

				for(int i = 0 ; i < m_OutputData.count(); i++)
				{					
					ScreenToGL(m_OutputData[i], glPoint);
					glPoint.setZ(zValue);
					ArrayTemp->push_back(glPoint);
					temp = TransGLtoWGS84(glPoint);
					temp.setZ(zValue);
					pOriginalArray->push_back(temp);		
				}
			}
			pArray->Clear();

			ClearCrossArray(m_InputData);
			//undo机制
			//CUndoAddDrawCommand* addCommand = new CUndoAddDrawCommand();
			//addCommand->SetUndoLayer(m_ParentLayer);		
			//addCommand->SetUndoRenderable(m_pContourLine);
			//addCommand->SetUndoGLWidget(m_pOpenglView);
			//if(m_undoStack)
			//{
			//	m_undoStack->push(addCommand);
			//}

			//将绘制等值线的对象保存在符号层中
			SymbolLayer *sl = dynamic_cast<SymbolLayer*>(m_ParentLayer);
			if (sl == NULL)
			{
				return;
			}

			if(!m_bFirst)
			{
				m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
				m_bFirst = true;
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
void DrawContourArea::MouseEvent(QMouseEvent* event)
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
			pColor->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
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
void DrawContourArea::MouseRelease(QMouseEvent *event)
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
void DrawContourArea::LeaveEvent()
{
	if(m_bFinished)				//没有按下右键，鼠标移开
	{
		m_bFinished = false;

		//清除记录
		if(pArray != NULL)
			pArray->Clear();

		ClearCrossArray(m_InputData);
		if(!m_bFirst)
		{
			m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
			m_bFirst = true;
		}
	}
	return;
}


void DrawContourArea::setILayerAttrib( ILayerAttribute *Ilayer )
{
//	ILayerAttribute *Ilayer = m_layerAttribute->value(QString("cFill"));

}

void DrawContourArea::InitAttribute(bool b)
{
	m_layerAttribute = new LAYER_ATTRIBUTE;	

	if (b)
	{
		QStringList strFill;
		strFill.append(tr("是"));
		strFill.append(tr("否"));
		ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill",tr("填充"),0,QVariant::Bool,0,0,strFill);
		m_layerAttribute->insert(QString("cFill"),cFill);
	}

}
/************************************************************************/
/* 函数功能： 根据属性值设置颜色数据       			        					*/
/* 参数说明:  无                 										*/
/* 返 回 值:  无                 										*/
/* 作    者:  zhanglei													*/
/* 日    期:  2011 年 04 月 25 日										*/
/************************************************************************/
bool  DrawContourArea::GetContourAttribute(double index)
{
	CSymbolAttribute * pAttr = m_pContourLine->GetAttribute();	
	if (m_currentColorTable.contains(index))
	{
		QColor tempcolor = m_currentColorTable.value(index)._color;
		int value = index;
		if(m_layertype ==  Layer::ELayerType::Rain_Layer)
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
		pAttr->SetColor(QVector4D(m_LineColor.redF(),m_LineColor.greenF(),m_LineColor.blueF(),m_LineColor.alphaF()));
		pAttr->SetFillType(false);	
		pAttr->SetFillColor(QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),tempcolor.alphaF()));
		pAttr->SetLineWidth(2.0f);	
		pAttr->SetLineType(QString(tr("0")));
		pAttr->SetLayerZ(0);
		return true;
	}
	else
	{
		return false;
	}
}


void DrawContourArea::InitColorTable()
{
	ReadColorTableFromXML reader;	
	reader.readFile(CIPASApplication::ConfigPath()+ "BasicInfo/CipasColorTable.xml",colortablelist);
}


void DrawContourArea::SetCurrentColorTable(QString str)
{
	if (colortablelist.contains(str))
	{
		m_currentColorTable = colortablelist.value(str);
		
		m_currentcolorindex.clear();
		m_stringitems.clear();
		m_currentmap.clear();
		m_CoutourZArray.clear();
		
		COLORTABLE::Iterator it;
		for (it=m_currentColorTable.begin();it!=m_currentColorTable.end();++it)
		{
			m_currentcolorindex.insert(it.key(),COLORITEM(it.value()._color,QString("")));
			m_currentmap.insert(it.value()._Name,it.key());
			
			if (it.value()._Zcount!=0)
			{
				m_stringitems.push_back(it.value()._Name);	
			}			
			m_CoutourZArray.insert(it.key(),it.value()._Zcount*m_fStep);
		}
		
	}
}

void DrawContourArea::AddRainCountorLine()
{

	CSymbolContourLine *pContourLine = new CSymbolContourLine;
	QVector3DArray * ArrayTemp = pContourLine->GetArray();
	QVector3DArray * pOriginalArray = pContourLine->OriginalArray();
	pContourLine->SetCloseLine(true);
	pContourLine->IsMark(false);

	//设置属性值
	CSymbolAttribute * pAttr = pContourLine->GetAttribute();
	pAttr->SetValue(-10);
	QColor tempcolor =QColor(255.0f,255.0f,64.0f,255.0f);
	pAttr->SetColor(QVector4D(m_LineColor.redF(),m_LineColor.greenF(),m_LineColor.blueF(),m_LineColor.alphaF()));
	pAttr->SetFillType(false);
	pAttr->SetFillColor( QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),tempcolor.alphaF()));
	pAttr->SetLineWidth(2.0f);
	pAttr->SetLineType(QString(tr("0")));
	pAttr->SetLayerZ(0);

	for(int i = 0 ; i < 221; i++)
	{
		QVector3D temp(contourArray[i][0], contourArray[i][1],0);
		ArrayTemp->push_back(temp);
		pOriginalArray->push_back(temp);
	}

	Layer* layer = GetParentLayer();
	SymbolLayer * sl = dynamic_cast<SymbolLayer*> (layer);

	if (sl!=NULL)
	{
 		SymbolData * pSymbolData = sl->getSymbolData();	// 取得符号层的对象
 		if(pSymbolData)
 		{
 			pSymbolData->getSymbolContourLine()->push_back(pContourLine);
 		}

		GetParentLayer()->GetLayerData()->InsertRenderable(pContourLine);
		GetParentLayer()->TransformLayer();
	}
}

void DrawContourArea::AddRainYUBAOCountorLine()
{
	CSymbolContourLine *pContourLine = new CSymbolContourLine;
	QVector3DArray * ArrayTemp = pContourLine->GetArray();
	QVector3DArray * pOriginalArray = pContourLine->OriginalArray();
	pContourLine->SetCloseLine(true);
		pContourLine->IsMark(true);
	//设置属性值
	CSymbolAttribute * pAttr = pContourLine->GetAttribute();
	pAttr->SetValue(10);
	QColor tempcolor =QColor(255.0f,255.0f,0.0f,255.0f);
	pAttr->SetColor(QVector4D(m_LineColor.redF(),m_LineColor.greenF(),m_LineColor.blueF(),m_LineColor.alphaF()));
	pAttr->SetFillType(false);
	pAttr->SetFillColor( QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),tempcolor.alphaF()));
	pAttr->SetLineWidth(2.0f);
	pAttr->SetLineType(QString("0"));
	pAttr->SetLayerZ(0);

	for(int i = 0 ; i < 221; i++)
	{
		QVector3D temp(contourArray[i][0], contourArray[i][1],0);
		ArrayTemp->push_back(temp);
		pOriginalArray->push_back(temp);
	}

	Layer* layer = GetParentLayer();
	SymbolLayer * sl = dynamic_cast<SymbolLayer*> (layer);

	if (sl!=NULL)
	{
 		SymbolData * pSymbolData = sl->getSymbolData();	// 取得符号层的对象
 		if(pSymbolData)
 		{
 			pSymbolData->getSymbolContourLine()->push_back(pContourLine);
 		}

		GetParentLayer()->GetLayerData()->InsertRenderable(pContourLine);
		GetParentLayer()->TransformLayer();
	}
}

void DrawContourArea::AddTempCountorLine()
{
	CSymbolContourLine *pContourLine = new CSymbolContourLine;
	QVector3DArray * ArrayTemp = pContourLine->GetArray();
	QVector3DArray * pOriginalArray = pContourLine->OriginalArray();
	pContourLine->SetCloseLine(true);
	pContourLine->HideBound(false);			
	pContourLine->IsMark(true);
	//设置属性值
	CSymbolAttribute * pAttr = pContourLine->GetAttribute();
	pAttr->SetValue(0);
	QColor tempcolor =QColor(255.0f,255.0f,0.0f,255.0f);
	pAttr->SetColor(QVector4D(m_LineColor.redF(),m_LineColor.greenF(),m_LineColor.blueF(),m_LineColor.alphaF()));
	pAttr->SetFillType(false);
	pAttr->SetFillColor( QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),tempcolor.alphaF()));
	pAttr->SetLineWidth(2.0f);
	pAttr->SetLineType(QString(tr("实线")));
	pAttr->SetLayerZ(0);

	for(int i = 0 ; i < 221; i++)
	{
		QVector3D temp(contourArray[i][0], contourArray[i][1],0);
		ArrayTemp->push_back(temp);
		pOriginalArray->push_back(temp);
	}

	Layer* layer = GetParentLayer();
	SymbolLayer * sl = dynamic_cast<SymbolLayer*> (layer);
	
	if (sl!=NULL)
	{
 		SymbolData * pSymbolData = sl->getSymbolData();	// 取得符号层的对象
 		if(pSymbolData)
 		{
 			pSymbolData->getSymbolContourLine()->push_back(pContourLine);
 		}

		GetParentLayer()->GetLayerData()->InsertRenderable(pContourLine);
		GetParentLayer()->TransformLayer();		
	}
}

/************************************************************************/
/*绘制十字线工具                                                        */
/************************************************************************/
void DrawContourArea::DrawCrossArray(QVector3DArray&  InputData)
{

	QVector3D glPos1,glPos2,glPos3,glPos4;
	QVector3D srPos1,srPos2,srPos3,srPos4;
	int iCount = InputData.count() -1;
	Line* temp1 = new Line;
	Line* temp2 = new Line;
	srPos1 = QVector3D(InputData[iCount].x()-3.0f,InputData[iCount].y()-3.0f,InputData[iCount].z()) ;
	srPos2 = QVector3D(InputData[iCount].x()-3.0f,InputData[iCount].y()+3.0f,InputData[iCount].z()) ;
	srPos3 = QVector3D(InputData[iCount].x()+3.0f,InputData[iCount].y()-3.0f,InputData[iCount].z()) ;
	srPos4 = QVector3D(InputData[iCount].x()+3.0f,InputData[iCount].y()+3.0f,InputData[iCount].z()) ;

	ScreenToGL(srPos1,glPos1);
	ScreenToGL(srPos2,glPos2);
	ScreenToGL(srPos3,glPos3);
	ScreenToGL(srPos4,glPos4);	


	temp1->getColorArray()->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
	temp1->getVectexArray()->push_back(glPos1);
	temp1->getColorArray()->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
	temp1->getVectexArray()->push_back(glPos4);
	temp1->SetColorArray();

	temp2->getColorArray()->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
	temp2->getVectexArray()->push_back(glPos2);
	temp2->getColorArray()->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
	temp2->getVectexArray()->push_back(glPos3);
	temp2->SetColorArray();


	m_CrossArray.push_back(temp1);
	m_CrossArray.push_back(temp2);

	m_ParentLayer->GetLayerData()->InsertRenderable(temp1);
	m_ParentLayer->GetLayerData()->InsertRenderable(temp2);
}

/************************************************************************/
/* 清除十字线功能                                                                     */
/************************************************************************/
void DrawContourArea::ClearCrossArray(QVector3DArray&  InputData)
{
	if (InputData.count()>0)
	{
		for (int j=0;j<m_CrossArray.count();j++)
		{
			m_ParentLayer->GetLayerData()->RemoveRenderable(m_CrossArray.at(j));
			delete m_CrossArray.at(j);
		}
		m_CrossArray.clear();
	}
}
