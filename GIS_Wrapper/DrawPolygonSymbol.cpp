/*============================================================ 
项目名： 人机交互平台
文件名： DrawPolygonSymbol.cpp 
类 名：  CDrawPolygonSymbol
父 类：  ManulDraw
子 类：  无 
功能说明：绘制雨区、雪区等多边形区域，用户可以设置其属性
调用说明：

-------------------------------------版本更新------------------
V 1.1 
修改作者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：任雪
完成日期：2011年07月22日
============================================================*/

#include "DrawPolygonSymbol.h"
#include "ILayerAttribute.h"
#include "SymbolAttribute.h"

//************************************************************************/
// 函数功能： 初始化各个成员变量
// 返 回 值:  void
// 作    者:  renxue	
// 日    期:  2011 年 7 月 22 日
//************************************************************************/
CDrawPolygonSymbol::CDrawPolygonSymbol( int nType ) : m_nType(nType)
{
	m_pLine = new Line;
	m_pPolygonLine = NULL;
	m_bFinished = false;
	m_bFirst = true;
	m_pSymboldata = NULL;
	initAttribute(nType);
	m_nType = nType;
	m_PolygonAttribute.SetSymbolType(1038);
}

CDrawPolygonSymbol::~CDrawPolygonSymbol(void)
{
}

void CDrawPolygonSymbol::MouseEvent(QMouseEvent* event)
{
}

void CDrawPolygonSymbol::MouseRelease(QMouseEvent *event)
{
}

/************************************************************************/
/* 函数功能： 单击左键时获取鼠标点，单击右键绘制      					*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 07 月 22 日										*/
/************************************************************************/
void CDrawPolygonSymbol::MousePress(QMouseEvent *event)
{
	QVector3D glPoint, scPoint(event->x(), event->y(), 0.0f);
	QVector3DArray  *pArray = m_pLine->getVectexArray();
	QVector4DArray  *pColor = m_pLine->getColorArray();

	if(m_Symbol)
	{
		if (event->buttons() & Qt::LeftButton)
		{
			if(!m_bFinished)
			{
				m_InputData.Clear();
				m_bFinished = true;
				pArray->Clear();
				pColor->clear();
			}			
			
			ScreenToGL(scPoint, glPoint);
			pColor->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
			pArray->push_back(glPoint);			
			m_pLine->SetColorArray();

			if(m_bFirst)
			{
				m_ParentLayer->GetLayerData()->InsertRenderable(m_pLine);
				m_bFirst = false;
			}
		}
		else if(event->buttons() & Qt::RightButton)
		{
			//按下右键，开始画双线
			if(m_bFinished)				
			{
				//只有一个点，无法组成多边形
				if(pArray->count() < 2)  return;    

				m_bFinished = false;
				m_pPolygonLine = new Line;
				QVector3DArray * pPolygonArray = m_pPolygonLine->getVectexArray(); 
				QVector3DArray * pOriginalPolygonArray = m_pPolygonLine->OriginalArray();
				QVector4DArray * pPolygonColor = m_pPolygonLine->getColorArray();
				GetUIAttribute();

				//设置绘制颜色
				pPolygonColor->Clear();
				QVector4D tColor=m_PolygonAttribute.GetColor();
				if(tColor.x()>255)
				{
					tColor = QVector4D(0.0, 0.0, 1.0 , 1.0);
				}

				//设置绘制顶点
				QVector3D tPoint;
				pPolygonArray->Clear();
				pOriginalPolygonArray->Clear();
				for(int i = 0 ; i < pArray->size(); i++)
				{
					pPolygonArray->push_back (pArray->at(i));
					tPoint = pArray->at(i);
					pOriginalPolygonArray->push_back(TransformData(tPoint));
					pPolygonColor->push_back(tColor);
				}
				m_pPolygonLine->SetColorArray();
				pArray->Clear();

				//设置线型、线宽
				int  nLineWidth = m_PolygonAttribute.GetLineWidth();
				QString strLineType = m_PolygonAttribute.GetLineType();
				if("0" == strLineType)
				{
					m_pPolygonLine->SetLineStyle(nLineWidth, 1, 0xFFFF);
				}
				else if("1" == strLineType)
				{
					m_pPolygonLine->SetLineStyle(nLineWidth, 2, 0xAAAA);
				}
				else if("2" == strLineType)
				{
					m_pPolygonLine->SetLineStyle(nLineWidth, 1, 0x0101);
				}
				
				//设置填充设置
				if(m_PolygonAttribute.GetFillType())
				{
					m_pPolygonLine->Mode(GL_POLYGON);	
				}
				else
				{
					m_pPolygonLine->Mode(GL_LINE_STRIP);	
				}
				
				//undo机制
				CUndoAddDrawCommand* addCommand = new CUndoAddDrawCommand();
				addCommand->SetUndoLayer(m_ParentLayer);
				addCommand->SetUndoRenderable(m_pPolygonLine);
				addCommand->SetUndoGLWidget(m_pOpenglView);
				if(m_undoStack)
				{
					m_undoStack->push(addCommand);
				}
				SymbolLayer *sLayer = dynamic_cast<SymbolLayer*>(m_ParentLayer);
				if (sLayer == NULL) return;

				if(!m_bFirst)
				{
					m_pSymboldata = sLayer->getSymbolData();
					m_pSymboldata->getTroughLine()->push_back(m_pPolygonLine);
					m_ParentLayer->GetLayerData()->RemoveRenderable(m_pLine);
					m_bFirst = true;
				}
			}
		}
	}
}
/************************************************************************/
/* 函数功能： 鼠标离开绘制区域，清除原来未完成的多边形点         		*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 07 月 22 日										*/
/************************************************************************/
void CDrawPolygonSymbol::LeaveEvent()
{
	//没有按下右键，开始清理轨迹线
	if(m_bFinished)				
	{
		m_bFinished = false;
		QVector3DArray *pArray = m_pLine->getVectexArray();
		pArray->Clear();
		if(!m_bFirst)
		{
			m_ParentLayer->GetLayerData()->RemoveRenderable(m_pLine);
			m_bFirst = true;
		}
	}
}

void CDrawPolygonSymbol::setILayerAttrib( ILayerAttribute *Ilayer )
{
}

/************************************************************************/
// 函数功能： 初始化多边形的属性窗
// 返 回 值:  void
// 作    者:  renxue	
// 日    期:  2011 年 7 月 22 日
/************************************************************************/
void CDrawPolygonSymbol::initAttribute( int nType )
{
	m_layerAttribute = new LAYER_ATTRIBUTE;
	QStringList colorNum, strRegion;

	strRegion.append(QString("雨区"));
	strRegion.append(QString("雪区"));
	strRegion.append(QString("雾区"));
	strRegion.append(QString("大风区"));
	strRegion.append(QString("沙暴区"));
	strRegion.append(QString("雷暴区"));
	ILayerAttribute *cValue = new ILayerAttribute("cValue","cValue","区域类型",0,QVariant::StringList,-0,0,strRegion);
	m_layerAttribute->insert(QString("cValue"),cValue);
	
	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor","颜色",QColor(0,0,255),QVariant::Color,0,0,colorNum);
	m_layerAttribute->insert(QString("cColor"),cColor);	

	QStringList strKinds;
	strKinds.append(QString("实线"));
	strKinds.append(QString("短划线"));
	strKinds.append(QString("点线"));
	ILayerAttribute *cKinds = new ILayerAttribute("cKinds","cKinds","线型",0,QVariant::StringList,0,0,strKinds);
	m_layerAttribute->insert(QString("cKinds"), cKinds);

	QStringList strWidth;
	for (int i=1; i<=10; i++)
	{
		strWidth.append(QString::number(i));
	}
	ILayerAttribute *cWidth = new ILayerAttribute("cWidth","cWidth","线宽",1,QVariant::Int,1,50,strWidth);
	m_layerAttribute->insert(QString("cWidth"),cWidth);

	QStringList strFill;
	strFill.append("是");
	strFill.append("否");
	ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill","是否填充",0,QVariant::Bool,0,0,strFill);
	m_layerAttribute->insert(QString("cFill"),cFill);

}

//************************************************************************/
//* 函数功能： 获取界面上的多边形属性         			        		*/
//* 参数说明:  无                 										*/
//* 返 回 值:  无                 										*/
//* 作    者:  renxue													*/
//* 日    期:  2011 年 07 月 22 日										*/
//************************************************************************/
void  CDrawPolygonSymbol::GetUIAttribute()
{
	LAYER_ATTRIBUTE *pLA = getILayerAttrib();
	if(!pLA) return;
	QColor tColor;

	//区域值
	ILayerAttribute *pLayAttribute = pLA->value(QString("cValue"));
	pLayAttribute = pLA->value(QString("cValue"));
	m_PolygonAttribute.SetValue(pLayAttribute->Value().toString());

	//颜色值
	pLayAttribute = pLA->value(QString("cColor"));
	tColor = pLayAttribute->Value().value<QColor>();
	m_PolygonAttribute.SetColor(QVector4D(tColor.redF(), tColor.greenF(), tColor.blueF(),tColor.alphaF()));

	//线宽设置
	pLayAttribute = pLA->value(QString("cWidth"));
	m_PolygonAttribute.SetLineWidth(pLayAttribute->Value().toInt());

	//线型设置
	pLayAttribute = pLA->value(QString("cKinds"));
	m_PolygonAttribute.SetLineType(pLayAttribute->Value().toString());

	//填充
	pLayAttribute = pLA->value(QString("cFill"));
	if(NULL != pLayAttribute)
	{
		m_PolygonAttribute.SetFillType(pLayAttribute->Value().toBool());
	}
}


