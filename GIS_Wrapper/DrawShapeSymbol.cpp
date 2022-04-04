
#include "math.h"
#include "DrawShapeSymbol.h"
#include "ILayerAttribute.h"
#include "SymbolAttribute.h"
//#include "LinesSymbol.h"

CDrawShapeSymbol::CDrawShapeSymbol( int nType ) : m_nType(nType)
{
	m_bisStart = false;
	m_pSymboldata = NULL;
	initAttribute(nType);
	m_nType = nType;
//	m_pLineTemp = new Line;
	m_pRectTemp = new Line;
	m_pLineTemp = new Line;
	m_vectorSreen = QVector3D(0.0f,0.0f,0.0f);
}

CDrawShapeSymbol::~CDrawShapeSymbol(void)
{
// 	delete []m_pRectTemp;
// 	m_pRectTemp = 0;
// 	delete []m_pLineTemp;
// 	m_pLineTemp = 0;
}

void CDrawShapeSymbol::SetShadeSymbol(Symbol *pSymbol)
{
	m_symbolShade = pSymbol; 
	m_vectorSreen = QVector3D(3.0f,3.0f,3.0f);
}

void CDrawShapeSymbol::DrawRect(int x1,int x2,int y1,int y2,QVector4D color)
{
	if (m_pRectTemp)
	{
		QVector3D glPoint1,glPoint2,glPoint3,glPoint4;
		//QVector4D geoPoint1,geoPoint2,geoPoint1,geoPoint1;
		m_pRectTemp->getVectexArray()->Clear(); 
		m_pRectTemp->OriginalArray()->Clear();
		m_pRectTemp->getColorArray()->Clear();
		
		ScreenToGL(QVector3D(x1,y1,0.0f),glPoint1);
		ScreenToGL(QVector3D(x1,y2,0.0f),glPoint2);
		ScreenToGL(QVector3D(x2,y2,0.0f),glPoint3);
		ScreenToGL(QVector3D(x2,y1,0.0f),glPoint4);

		m_pRectTemp->getVectexArray()->push_back(glPoint1);
		m_pRectTemp->getVectexArray()->push_back(glPoint2);
		m_pRectTemp->getVectexArray()->push_back(glPoint3);
		m_pRectTemp->getVectexArray()->push_back(glPoint4);	

		for (int i=0;i<4;i++)
		{		
			m_pRectTemp->getColorArray()->push_back(color);
		}
		
		m_pRectTemp->SetColorArray();
		m_pRectTemp->SetLineStyle(1, 1, 0xFFFF); //设置"0"
		m_pRectTemp->Mode(GL_LINE_LOOP);		
	}
}

void CDrawShapeSymbol::DrawEclipse(int x1,int x2,int y1,int y2,QVector4D color)
{
	int dx,dy;
	QVector3D scPoint,glPoint;
	QVector3D tempPos;

	if (m_pLineTemp)
	{
		scPoint=  QVector3D((x1+x2)/2,(y1+y2)/2,0.0f); //得到椭圆中心点
		dx = abs(x1-x2)/2; //椭圆长半轴
		dy = abs(y1-y2)/2; //椭圆短半轴		

		m_pLineTemp->getVectexArray()->Clear(); 
		m_pLineTemp->OriginalArray()->Clear();
		m_pLineTemp->getColorArray()->Clear();
		SreenPointArray.clear();

		for (int i=0;i<360;i++)
		{				
			tempPos = scPoint + QVector3D(dx*cosf(i*MPI/180),dy*sinf(i*MPI/180),0.0f); 
			SreenPointArray.push_back(tempPos);
			ScreenToGL(tempPos,glPoint);
			m_pLineTemp->getColorArray()->push_back(QVector4D(0,1,0,1));
			m_pLineTemp->getVectexArray()->push_back(glPoint);
		}	

		m_pLineTemp->SetColorArray();
		m_pLineTemp->SetLineStyle(1, 1, 0xFFFF); //设置"0"
		m_pLineTemp->Mode(GL_LINE_LOOP);		
	}
}

void CDrawShapeSymbol::MouseEvent(QMouseEvent* event)
{
	QVector3D scPoint(event->x(), event->y(), 0.0f);
	m_posnew = scPoint;		

	if (m_bisStart&&m_Symbol)
	{
		m_ParentLayer->GetLayerData()->RemoveRenderable(m_pRectTemp);
		m_ParentLayer->GetLayerData()->RemoveRenderable(m_pLineTemp);
		DrawRect(m_posold.x(),m_posnew.x(),m_posold.y(),m_posnew.y(),QVector4D(0.0f,1.0f,0.0f,1.0f));
		DrawEclipse(m_posold.x(),m_posnew.x(),m_posold.y(),m_posnew.y(),QVector4D(0.0f,1.0f,0.0f,1.0f));			
		m_ParentLayer->GetLayerData()->InsertRenderable(m_pRectTemp);
		m_ParentLayer->GetLayerData()->InsertRenderable(m_pLineTemp);	
	}
}

void CDrawShapeSymbol::MouseRelease(QMouseEvent *event)
{
	if (m_bisStart)
	{
		m_ParentLayer->GetLayerData()->RemoveRenderable(m_pRectTemp);
		m_ParentLayer->GetLayerData()->RemoveRenderable(m_pLineTemp);	

		m_posnew = QVector3D(event->x(), event->y(), 0.0f);				

		m_PSymbolMultiLine = new CSymbolMultiLine;
		QVector3DArray * ArrayTemp = m_PSymbolMultiLine->GetArray(); 
		QVector3DArray * pOriginalArray = m_PSymbolMultiLine->OriginalArray();
		GetSymbolAttribute(m_PSymbolMultiLine);
		QVector3DArray  *m_pArray = m_pLineTemp->getVectexArray();
		for(int i = 0 ; i < m_pArray->size(); i++)
		{
			ArrayTemp->push_back (m_pArray->at(i));
			QVector3D temp = m_pArray->at(i);
			pOriginalArray->push_back(TransGLtoWGS84(temp));
		}

		m_pArray->Clear();
		m_PSymbolMultiLine->SetSymbolLineStyle(m_Symbol);
		
		m_ParentLayer->GetLayerData()->InsertRenderable(m_PSymbolMultiLine);
		m_PSymbolMultiLine->SetColorArray();
		//undo机制
		//CUndoAddDrawCommand* addCommand = new CUndoAddDrawCommand();
		//addCommand->SetUndoLayer(m_ParentLayer);
		//addCommand->SetUndoRenderable(m_PSymbolMultiLine);
		//addCommand->SetUndoGLWidget(m_pOpenglView);
		//if(m_undoStack)
		//{
		//	m_undoStack->push(addCommand);
		//}

		SymbolLayer *m_nSymbolLayer = dynamic_cast<SymbolLayer*>(m_ParentLayer);
		
		m_pSymboldata = m_nSymbolLayer->getSymbolData();
		m_pSymboldata->getSymbolMultiLine()->push_back(m_PSymbolMultiLine);			
				
		m_bisStart = false; 
	}
}

void CDrawShapeSymbol::MouseMove(QMouseEvent *event)
{

}

void CDrawShapeSymbol::MousePress(QMouseEvent *event)
{
	QVector3D scPoint(event->x(), event->y(), 0.0f);	

	if(m_Symbol)
	{
		if (event->buttons() & Qt::LeftButton)
		{
			m_bisStart = true; //点击鼠标左键，开始绘制
		    m_posold = m_posnew = scPoint;	
			DrawRect(m_posold.x(),m_posnew.x(),m_posold.y(),m_posnew.y(),QVector4D(0.0f,0.0f,0.0f,1.0f));
			DrawEclipse(m_posold.x(),m_posnew.x(),m_posold.y(),m_posnew.y(),QVector4D(0.0f,1.0f,0.0f,1.0f));			
			m_ParentLayer->GetLayerData()->InsertRenderable(m_pRectTemp);
			m_ParentLayer->GetLayerData()->InsertRenderable(m_pLineTemp);
		}
		else if(event->buttons() & Qt::RightButton)
		{
			m_bisStart = false; //点击鼠标右键，撤销本次绘制
		}
	}
}

void CDrawShapeSymbol::LeaveEvent()
{
	
}

void CDrawShapeSymbol::setILayerAttrib( ILayerAttribute *Ilayer )
{
}

void CDrawShapeSymbol::initAttribute( int nType )
{
	m_layerAttribute = new LAYER_ATTRIBUTE;
	QStringList colorNum, strRegion;

	if (nType ==2001) strRegion.append(QString(tr("干旱区")));
	if (nType ==801) strRegion.append(QString(tr("洪涝区")));	

	ILayerAttribute *cValue = new ILayerAttribute("cValue","cValue",tr("区域类型"),0,QVariant::StringList,-0,0,strRegion);
	m_layerAttribute->insert(QString("cValue"),cValue);
	
// 	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor","颜色",QColor(0,0,255),QVariant::Color,0,0,colorNum);
// 	m_layerAttribute->insert(QString("cColor"),cColor);	

// 	QStringList strKinds;
// 	strKinds.append(QString("实线"));
// 	strKinds.append(QString("短划线"));
// 	strKinds.append(QString("点线"));
// 	ILayerAttribute *cKinds = new ILayerAttribute("cKinds","cKinds","线型",0,QVariant::StringList,0,0,strKinds);
// 	m_layerAttribute->insert(QString("cKinds"), cKinds);
// 
// 	QStringList strWidth;
// 	for (int i=1; i<=10; i++)
// 	{
// 		strWidth.append(QString::number(i));
// 	}
// 	ILayerAttribute *cWidth = new ILayerAttribute("cWidth","cWidth","线宽",1,QVariant::Int,1,50,strWidth);
// 	m_layerAttribute->insert(QString("cWidth"),cWidth);

// 	QStringList strFill;
// 	strFill.append("是");	
// 	ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill","是否填充",0,QVariant::Bool,0,0,strFill);
// 	m_layerAttribute->insert(QString("cFill"),cFill);	

}

void  CDrawShapeSymbol::GetUIAttribute()
{
}

void CDrawShapeSymbol::GetSymbolAttribute(CSymbolMultiLine *pSymbolLine)
{
	LAYER_ATTRIBUTE *pLA = getILayerAttrib();
	if(!pLA) return;

	//设置类型编号
	ILayerAttribute *pLayAttribute;	
	pSymbolLine->m_LineAttribute.SetSymbolType(m_nType);

	//设置线型
	pLayAttribute = pLA->value(QString("cKinds"));
	if(!pLayAttribute) return;
	pSymbolLine->m_LineAttribute.SetLineType(pLayAttribute->Value().toString());

	//设置线宽
	pLayAttribute = pLA->value(QString("cWidth"));
	if(!pLayAttribute) return;
	pSymbolLine->m_LineAttribute.SetLineWidth(pLayAttribute->Value().toInt());

	if(m_nType == 2001)//多边形的区域标识
	{
		pSymbolLine->m_LineAttribute.SetValue(tr("干旱区"));
	}		
	if(m_nType == 801)//多边形的区域标识
	{
		pSymbolLine->m_LineAttribute.SetValue(tr("洪灾区"));
	}	
}

