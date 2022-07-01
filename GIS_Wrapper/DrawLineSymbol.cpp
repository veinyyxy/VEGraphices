/*============================================================ 
文件名：DrawLineSymbol.h DrawLineSymbol.cpp 
类 名： CDrawLineSymbol
父 类： ManualDraw
子 类： 无 
功能说明：画矢量符号线的算法
调用说明：
1、获取用户的PING命令鼠标坐标点，调用下层算法，返回计算得到的点集；
2、将屏幕点转换成OPENGL的坐标，并传给相应的绘图管道；

----------------------------版本更新--------------------------
V 1.0 
原作者 ：任雪
完成日期：2011年4月2日

V 1.1 
作 者：杨晓宇
完成日期：2011年4月25日
更新说明：加入undo机制

V 1.2 
作 者：杨东
完成日期：2011年5月28日
更新说明：分离箭头符号

============================================================*/


#include "DrawLineSymbol.h"
#include "ILayerAttribute.h"
#include "SymbolAttribute.h"

CDrawLineSymbol::CDrawLineSymbol( int nType ) : m_nType(nType)
{
	m_pDisplayLine = new Line;
	m_bFinished = false;
	m_pArray = NULL;
	m_pSymbolLine = NULL;
	m_bFirst = true;
	m_pSymboldata = new SymbolData;
	InitAttribute(nType);
	m_nType = nType;

	if (nType == 1201)
	{
		cardline = true;
		m_pSpLine =  new CCardinalSpline;
	}
	else
		cardline = false;
}

CDrawLineSymbol::CDrawLineSymbol( int nType,CoordinateTransform* proj )
{
	m_pDisplayLine = new Line;
	m_bFinished = false;
	m_pArray = NULL;
	m_pSymbolLine = NULL;
	m_bFirst = true;
	m_pSymboldata = new SymbolData;
	InitAttribute(nType);
	m_nType = nType;

	if (nType == 1201)
	{
		cardline = true;
		m_pSpLine =  new CCardinalSpline;
	}
	else
		cardline = false;
	m_proj = dynamic_cast<RadProjTransform*> (proj);
}


CDrawLineSymbol::~CDrawLineSymbol(void)
{
	if(NULL != m_pSpLine)
	{
		delete m_pSpLine;
		m_pSpLine = NULL;
	}
}

/************************************************************************/
/* 函数功能： 响应鼠标移动事件					                        */
/* 参数说明:  QRenderable* volume  : 绘制								*/
/*			  QMouseEvent* event   : 鼠标点								*/	
/* 返 回 值:  NULL														*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 07 月 28 日										*/
/************************************************************************/
void CDrawLineSymbol::MouseEvent(QMouseEvent* event)
{
	QVector3D  glPoint, scPoint(event->x(), event->y(), 0.0f);
	if (cardline == true)
	{
		if(m_bFinished)
		{
			if(m_InputData.size()<3) return;
			//取得平滑后的点数组
			m_InputData<<scPoint;
			m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);
			QVector4DArray *pColor = m_pDisplayLine->getColorArray();
			QVector3DArray *pArray = m_pDisplayLine->getVectexArray();
			//清除上次的点数组
			pArray->Clear();			
			pColor->clear();
			int j=0;
			QVector3D topLefPoint, topRightPoint, bottomLeftPoint, bottomRightPoint;
			for(int i = 0 ; i < m_OutputData.count(); i++)
			{
				ScreenToGL(m_OutputData[i], glPoint);
				pColor->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
				glPoint.setZ(0.52f);
				pArray->push_back(glPoint);
			}
			m_pDisplayLine->SetColorArray();

			if(pArray->count() <2)  return ;
			if(m_bFirst)
			{
				m_ParentLayer->GetLayerData()->InsertRenderable(m_pDisplayLine);
			}
			m_InputData.pop_back();
			m_OutputData.Clear();
		}
	}
	else if (cardline == false)
	{
	}
}

/************************************************************************/
/* 函数功能： 获取鼠标点         			        					*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 02 日										*/
/************************************************************************/
void CDrawLineSymbol::MousePress(QMouseEvent *event)
{
	if(!m_Symbol) return;
	QVector3D glPoint, scPoint(event->x(), event->y(), 0.0f);
	if (cardline == true)
	{
		if (event->buttons() & Qt::LeftButton)
		{
			if(!m_bFinished)
			{
				m_InputData.Clear();
				m_bFinished = true;
				m_pArray = NULL;
			}		
			m_InputData.push_back(scPoint);

			if(m_InputData.size()<3)
			{
				m_OutputData.Clear();
				for(int i=0; i<m_InputData.size(); i++)
					m_OutputData<<QVector3D(m_InputData[i].x(), m_InputData[i].y(), m_InputData[i].z());
			}
			else
			{
				m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);//取得平滑后的点数组
			}

			QVector4DArray *pColor = m_pDisplayLine->getColorArray();
			m_pArray = m_pDisplayLine->getVectexArray();		
			m_pArray->Clear();	//清除上次的点数组		
			pColor->clear();

			for(int i = 0 ; i < m_OutputData.count(); i++)
			{
				ScreenToGL(m_OutputData[i], glPoint);
				pColor->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
				m_pArray->push_back(glPoint);
			}

			m_pDisplayLine->SetColorArray();

			if(m_bFirst)
			{
				m_ParentLayer->GetLayerData()->InsertRenderable(m_pDisplayLine);
				m_bFirst = false;
			}
		}
		else if(event->buttons() & Qt::RightButton)
		{
			if(m_bFinished)				//按下右键，开始画双线
			{
				m_bFinished = false;
				if(m_pArray->count() <2)  return;          //只有一个点，退出

				m_pSymbolLine = new CSymbolLine;
				QVector3DArray * ArrayTemp = m_pSymbolLine->GetArray(); 
				QVector3DArray * pOriginalArray = m_pSymbolLine->OriginalArray();
				GetSymbolAttribute();
				for(int i = 0 ; i < m_pArray->size(); i++)
				{
					ArrayTemp->push_back (m_pArray->at(i));
					QVector3D temp = m_pArray->at(i);
					CoordTrans(m_proj);
					pOriginalArray->push_back(TransGLtoWGS84(temp));
				}

				m_pArray->Clear();
				m_pSymbolLine->SetSymbolLineStyle(m_Symbol);

				m_ParentLayer->GetLayerData()->InsertRenderable(m_pSymbolLine);
				m_pSymbolLine->SetColorArray();
				//undo机制
				//CUndoAddDrawCommand* addCommand = new CUndoAddDrawCommand();
				//addCommand->SetUndoLayer(m_ParentLayer);
				//addCommand->SetUndoRenderable(m_pSymbolLine);
				//addCommand->SetUndoGLWidget(m_pOpenglView);
				//if(m_undoStack)
				//{
				//	m_undoStack->push(addCommand);
				//}
				SymbolLayer *sLayer = dynamic_cast<SymbolLayer*>(m_ParentLayer);
				if (sLayer == NULL) return;
				//SymbolData *m_pSymboldata = sLayer->getSymbolData();
				m_pSymboldata = sLayer->getSymbolData();
				m_pSymboldata->getSymbolLine()->push_back(m_pSymbolLine);

				if(!m_bFirst)
				{
					m_ParentLayer->GetLayerData()->RemoveRenderable(m_pDisplayLine);
					m_bFirst = true;
				}
			}
		}
	}
	else if (cardline == false)
	{
		if (event->buttons() & Qt::LeftButton)
		{
			if(!m_bFinished)
			{
				m_InputData.Clear();
				m_bFinished = true;
				m_pArray = NULL;
			}		
			m_InputData.push_back(scPoint);

			//if(m_InputData.size()<3)
			{
				m_OutputData.Clear();
				for(int i=0; i<m_InputData.size(); i++)
					m_OutputData<<QVector3D(m_InputData[i].x(), m_InputData[i].y(), m_InputData[i].z());
			}

			QVector4DArray *pColor = m_pDisplayLine->getColorArray();
			m_pArray = m_pDisplayLine->getVectexArray();		
			m_pArray->Clear();	//清除上次的点数组		
			pColor->clear();

			for(int i = 0 ; i < m_OutputData.count(); i++)
			{
				ScreenToGL(m_OutputData[i], glPoint);
				pColor->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
				m_pArray->push_back(glPoint);
			}

			m_pDisplayLine->SetColorArray();

			if(m_bFirst)
			{
				m_ParentLayer->GetLayerData()->InsertRenderable(m_pDisplayLine);
				m_bFirst = false;
			}
		}
		else if(event->buttons() & Qt::RightButton)
		{
			if(m_bFinished)				//按下右键，开始画双线
			{
				m_bFinished = false;
				if(m_pArray->count() <2)  return;          //只有一个点，退出

				m_pSymbolLine = new CSymbolLine;
				QVector3DArray * ArrayTemp = m_pSymbolLine->GetArray(); 
				QVector3DArray * pOriginalArray = m_pSymbolLine->OriginalArray();
				GetSymbolAttribute();
				for(int i = 0 ; i < m_pArray->size(); i++)
				{
					ArrayTemp->push_back (m_pArray->at(i));
					QVector3D temp = m_pArray->at(i);
					CoordTrans(m_proj);
					pOriginalArray->push_back(TransGLtoWGS84(temp));
				}

				m_pArray->Clear();
				m_pSymbolLine->SetSymbolLineStyle(m_Symbol);

				m_ParentLayer->GetLayerData()->InsertRenderable(m_pSymbolLine);
				m_pSymbolLine->SetColorArray();
				//undo机制
				//CUndoAddDrawCommand* addCommand = new CUndoAddDrawCommand();
				//addCommand->SetUndoLayer(m_ParentLayer);
				//addCommand->SetUndoRenderable(m_pSymbolLine);
				//addCommand->SetUndoGLWidget(m_pOpenglView);
				//if(m_undoStack)
				//{
				//	m_undoStack->push(addCommand);
				//}
				SymbolLayer *sLayer = dynamic_cast<SymbolLayer*>(m_ParentLayer);
				if (sLayer == NULL) return;
				//SymbolData *m_pSymboldata = sLayer->getSymbolData();
				m_pSymboldata = sLayer->getSymbolData();
				m_pSymboldata->getSymbolLine()->push_back(m_pSymbolLine);

				if(!m_bFirst)
				{
					m_ParentLayer->GetLayerData()->RemoveRenderable(m_pDisplayLine);
					m_bFirst = true;
				}
			}
		}
	}
}

/************************************************************************/
/* 函数功能： 鼠标释放         			        		    			*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 02 日										*/
/************************************************************************/
void CDrawLineSymbol::MouseRelease(QMouseEvent *event)
{
}

/************************************************************************/
/* 函数功能： 获取鼠标点，没有按下右键，开始清理轨迹线         			*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 02 日										*/
/************************************************************************/
void CDrawLineSymbol::LeaveEvent()
{
	if(m_bFinished)				
	{
		m_bFinished = false;
		m_pArray->Clear();
		if(!m_bFirst)
		{
			m_ParentLayer->GetLayerData()->RemoveRenderable(m_pDisplayLine);
			m_bFirst = true;
		}
	}
}

void CDrawLineSymbol::setILayerAttrib( ILayerAttribute *Ilayer )
{

}
/************************************************************************/
// 函数功能： 初始化箭头线属性窗
// 返 回 值:  void
// 作    者:  杨东		
// 日    期:  2011 年 4 月 13 日
/************************************************************************/
void CDrawLineSymbol::InitAttribute( int nType )
{
	m_layerAttribute = new LAYER_ATTRIBUTE;
	
	//颜色
	QStringList nColorNum;
	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor",tr("颜色"),QColor(0,0,255),QVariant::Color,0,0,nColorNum);
	m_layerAttribute->insert(QString("cColor"),cColor);
	
	//线型
	QStringList strKinds;
	strKinds.append(QString(tr("实线")));
	strKinds.append(QString(tr("短划线")));
	strKinds.append(QString(tr("点线")));
	ILayerAttribute *cKinds = new ILayerAttribute("cKinds","cKinds",tr("线型"),0,QVariant::StringList,0,0,strKinds);
	m_layerAttribute->insert(QString("cKinds"), cKinds);

	//线宽
	QStringList strWidth;
	for (int i=1; i<=10; i++)
	{
		strWidth.append(QString::number(i));
	}
	ILayerAttribute *cWidth = new ILayerAttribute("cWidth","cWidth",tr("线宽"),1,QVariant::Int,1,50,strWidth);
	m_layerAttribute->insert(QString("cWidth"),cWidth);

	
	if(nType == 1101)//等值线的标值
	{
		QStringList nContourValue;
		ILayerAttribute *cValue = new ILayerAttribute("cValue","cValue",tr("标值"),0,QVariant::Double,-10000.000000,10000.000000,nContourValue);
		m_layerAttribute->insert(QString("cValue"),cValue);
	}	
	else if(nType == 1308)//多边形的区域标值
	{
		QStringList strRegion;
		strRegion.append(QString(tr("雨区")));
		strRegion.append(QString(tr("雪区")));
		strRegion.append(QString(tr("雾区")));
		strRegion.append(QString(tr("大风区")));
		strRegion.append(QString(tr("沙暴区")));
		strRegion.append(QString(tr("雷暴区")));
		ILayerAttribute *cValue = new ILayerAttribute("cValue","cValue",tr("区域类型"),0,QVariant::StringList,-0,0,strRegion);
		m_layerAttribute->insert(QString("cValue"),cValue);
	}
	
	if (nType==1101 || nType==1308)//1101表示等值线，1308表示多边形
	{
		QStringList strFill;
		strFill.append(tr("是"));
		strFill.append(tr("否"));
		ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill",tr("填充"),0,QVariant::Bool,0,0,strFill);
		m_layerAttribute->insert(QString("cFill"),cFill);
	}
}

/************************************************************************/
/* 函数功能： 获取符号属性         			        					*/
/* 参数说明:  无                 										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 19 日										*/
/************************************************************************/
void  CDrawLineSymbol::GetSymbolAttribute()
{
	LAYER_ATTRIBUTE *pLA = getILayerAttrib();
	if(!pLA) return;

	//设置类型编号
	ILayerAttribute *pLayAttribute;	
	m_pSymbolLine->m_LineAttribute.SetSymbolType(m_nType);
	
	//设置颜色
	pLayAttribute = pLA->value(QString("cColor"));
	if(!pLayAttribute) return;
	QColor tColor = pLayAttribute->Value().value<QColor>();;
    m_pSymbolLine->m_LineAttribute.SetColor(QVector4D(tColor.red()/255.0, tColor.green()/255.0, tColor.blue()/255.0, 1.0f));

	//设置线型
	pLayAttribute = pLA->value(QString("cKinds"));
	if(!pLayAttribute) return;
	m_pSymbolLine->m_LineAttribute.SetLineType(pLayAttribute->Value().toString());

	//设置线宽
	pLayAttribute = pLA->value(QString("cWidth"));
	if(!pLayAttribute) return;
	m_pSymbolLine->m_LineAttribute.SetLineWidth(pLayAttribute->Value().toInt());
	
	
	if(m_nType == 1101)//等值线的标值
	{
		pLayAttribute = pLA->value(QString("cValue"));
		if(!pLayAttribute) return;
		m_pSymbolLine->m_LineAttribute.SetValue(pLayAttribute->Value().toDouble());
	}	
	else if(m_nType == 1308)//多边形的区域标识
	{
		pLayAttribute = pLA->value(QString("cValue"));
		if(!pLayAttribute) return;
		int nRegion = pLayAttribute->Value().toInt();
		QString strRegion;
		switch(nRegion)
		{
		case 0:
			strRegion = tr("雨区");
			break;
		case 1:
			strRegion = tr("雪区");
			break;
		case 2:
			strRegion = tr("雾区");
			break;
		case 3:
			strRegion = tr("大风区");
			break;
		case 4:
			strRegion = tr("沙暴区");
			break;
		case 5:
			strRegion = tr("雷暴区");
			break;
		}
		m_pSymbolLine->m_LineAttribute.SetValue(strRegion);
	}	
	
	//设置填充设置
	if(m_nType == 1003 || m_nType ==1005)//填充的箭头
	{
		m_pSymbolLine->m_LineAttribute.SetFillType(true);
	}
	else if(m_nType == 1101 || m_nType ==1308)//等值线 ， 多边形
	{
		pLayAttribute = pLA->value(QString("cFill"));
		if(!pLayAttribute) return;
		m_pSymbolLine->m_LineAttribute.SetFillType(pLayAttribute->Value().toBool());
	}
}
