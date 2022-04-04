#include "DrawWindSymbol.h"
#include <math.h>

const GLfloat Pi = 3.1415926536f;

CDrawWindSymbol::CDrawWindSymbol(void)
{
	m_bStart= false;
	m_Circular = new Line;
	m_fAngle = 0.0f;
	m_symbolpoint = NULL;
	m_bFirst = true;
	m_pColorAttri = new ColorAttribute;
    //m_text = new QTextureFont;
	
	//m_text->initFont(QString(tr("楷体")),QString(""),17,17,0); 
	m_nSymbolData = NULL;
	m_nSymbolLayer = NULL;
    m_bFirst = true;
}


CDrawWindSymbol::~CDrawWindSymbol(void)
{
	//m_Circular->Clear();
	if(m_Circular != NULL)
	{
		delete m_Circular;
		m_Circular = NULL;
	}

	/*if(m_text != NULL)
	{
		delete m_text;
		m_text = NULL;
	}*/

	if(NULL!= m_pColorAttri)
	{
	  delete m_pColorAttri;
	  m_pColorAttri = NULL;
	}
}

 void CDrawWindSymbol::MouseEvent(QMouseEvent* event) 
 {	 
	 if(!m_bStart)
		 return;
	 
	 QVector3DArray *pLineArray  = m_line.getVectexArray();
	 QVector4DArray *pLineColor  = m_line.getColorArray();
	 pLineArray->Clear();
	 pLineColor->Clear();
	 QVector3D glPoint, scPoint(event->x(), event->y(), 0.0f);
	 ScreenToGL(scPoint,glPoint);
	 pLineColor->push_back(QVector4D(1.0,0.0,0.0,1));
	 pLineArray->push_back(glPoint);
	 pLineColor->push_back(QVector4D(1.0,0.0,0.0,1));
	 ScreenToGL(m_CircularCenter,glPoint);
	 pLineArray->push_back(glPoint);
	 m_line.SetColorArray();
	 m_ParentLayer->GetLayerData()->InsertRenderable(&m_line);

	 //Get the angle by two points
	 m_LineEndPoint.setX(scPoint.x());
	 m_LineEndPoint.setY(scPoint.y());
	 m_LineEndPoint.setZ(scPoint.z());
	 

	 double dAngle = CalculateAngle();
	 m_fAngle = dAngle;
	 //clear front text
	 //int ntextcount = m_TextArray.count();
// 	 if (ntextcount !=0)
// 	 {
// 		 for (int i=0;i<ntextcount;i++)
// 		 {
// 			 m_ParentLayer->GetLayerData()->RemoveRenderable(m_TextArray.at(0));
// 			 m_TextArray.at(i)->StringArray()->clear();
// 			 m_TextArray.at(i)->Pointion()->Clear();
// 			 m_TextArray.at(i)->Clear();
// 		 }
// 		 m_TextArray.clear();
// 	 }
	 //set the text on the line
	 SetTextOnLine(dAngle);
/*	 m_TextArray.push_back(m_text);*/
	// m_ParentLayer->GetLayerData()->InsertRenderable(m_text);

 }
 void CDrawWindSymbol::MousePress(QMouseEvent *event)
 {
	 QVector3DArray *pCircularArray = m_Circular->getVectexArray();
	 QVector4DArray *pCircularColor = m_Circular->getColorArray();
	 QVector3D glPoint,tempPoint, scPoint(event->x(), event->y(), 0.0f);
	 pCircularArray->Clear();
	 pCircularColor->Clear();
	 //CUndoAddPointCommand* pUndoAddPointCommand = 0;
	 //Draw the circle when the mouse press
	 if(!m_bStart)
	 {
		 m_bStart = true;
		 // save the center point of circle
		 m_CircularCenter.setX(scPoint.x());
		 m_CircularCenter.setY(scPoint.y());
		 m_CircularCenter.setZ(scPoint.z());
		 double x  ; 
		 double y  ;
		 for(int i = 0;i <=90; i++)
		 {
			 pCircularColor->push_back(QVector4D(1.0,0.0,0.0,1));
			 x = scPoint.x() + 4* cos(8*Pi/360*i);
			 y = scPoint.y() + 4* sin(8*Pi/360*i);
			 tempPoint.setX(x);tempPoint.setY(y);tempPoint.setZ(glPoint.z());
			 ScreenToGL(tempPoint,glPoint);
			 pCircularArray->push_back(glPoint);
		 }
		 m_Circular->Mode(GL_LINE_LOOP);
		 m_Circular->SetColorArray();
		 m_ParentLayer->GetLayerData()->InsertRenderable(m_Circular);
	  }
	 else
	 {
		 //pUndoAddPointCommand = new CUndoAddPointCommand;
		 m_bStart = false;
		 // save the line end point of circle
		 m_LineEndPoint.setX(scPoint.x());
		 m_LineEndPoint.setY(scPoint.y());
		 m_LineEndPoint.setZ(scPoint.z());
		 QVector3DArray *pLineArray  = m_line.getVectexArray();
		 QVector4DArray *pLineColor  = m_line.getColorArray();	
		 pLineArray->Clear();
		 pLineColor->Clear();
		 //take he texture
		 if(m_Symbol)
		 {
			m_symbolpoint = new SymbolPoint();
			//pUndoAddPointCommand->SetSymbolPoint(m_symbolpoint);
			//pUndoAddPointCommand->SetGLWdiget(m_pOpenglView);
			
			 ScreenToGL(m_CircularCenter, glPoint);
			 //m_symbolpoint->SetPosition(glPoint);
			 m_symbolpoint->SetPointStyle(m_Symbol); 
			 //m_symbolpoint->SetAngle(m_fAngle);

			// pUndoAddPointCommand->SetUndoPoint(glPoint);
			 //pUndoAddPointCommand->SetRedoPoint(glPoint);
			 //pUndoAddPointCommand->SetAngle(m_fAngle);
			 //m_undoStack->push(pUndoAddPointCommand);
			 //pUndoAddPointCommand->SetIndex(m_symbolpoint->VertexArray());

			 //bak Original point for transform
			 QVector3DArray *pOriginal = m_symbolpoint->OriginalArray();
			 QVector3D OriginalPoint = TransGLtoWGS84(glPoint);
			 pOriginal->push_back(OriginalPoint);

			 m_ParentLayer->GetLayerData()->InsertRenderable(m_symbolpoint);

			 SymbolLayer *m_nSymbolLayer = dynamic_cast<SymbolLayer*>(m_ParentLayer);
			 if (m_symbolpoint == NULL)
			 {
				 return;
			 }

			 if(m_bFirst) 
			 {				
				 m_bFirst = false ;
			 }

			 m_nSymbolData = m_nSymbolLayer->getSymbolData();
			 m_nSymbolData->getSymbolPoint()->push_back(m_symbolpoint);

			 m_ParentLayer->GetLayerData()->RemoveRenderable(m_Circular);
			 //m_ParentLayer->GetLayerData()->RemoveRenderable(m_text);
			 m_ParentLayer->GetLayerData()->RemoveRenderable(&m_line);
		 }
	 }
 }

 void CDrawWindSymbol::MouseRelease(QMouseEvent *event)
 {
 }

 /************************************************************************/
 /* 函数功能： 鼠标离开事件         			        				 */
 /* 参数说明:  鼠标事件           										 */
 /* 返 回 值:  无                 										 */
 /* 作    者:  renxue													 */
 /* 日    期:  2011 年 06 月 29 日										 */
 /************************************************************************/
 void CDrawWindSymbol::LeaveEvent()
 {
	 if(m_bStart)
	 {
		 m_bStart = false;
		 m_ParentLayer->GetLayerData()->RemoveRenderable(m_Circular);
		 //m_ParentLayer->GetLayerData()->RemoveRenderable(m_text);
		 m_ParentLayer->GetLayerData()->RemoveRenderable(&m_line);		 
	 }	 
 }

 double CDrawWindSymbol::CalculateAngle()
 {
	 double x1,x2,y1,y2,t,c2,angle=0.0;
	 x1 = m_CircularCenter.x();
	 y1 = m_CircularCenter.y();
	 x2 = m_LineEndPoint.x();
	 y2 = m_LineEndPoint.y();
	 if (x2 < x1 && y2 == y1)
	 {
		 return 270.0;
	 }
	 else if (x2 > x1 && y2 == y1)
	 {
		 return 90.0;
	 }
	 else if (x2 == x1 && y2 < y1)
	 {
		 return 360.0;
	 }
	 else if (x2 == x1 && y2 > y1)
	 {
		 return 180.0;
	 } 
	 c2 = (y2-y1)*(y2-y1)+(x2-x1)*(x2-x1);
	 t = abs(y2-y1)/sqrt(c2);
	 angle = asin(t);
	 double tempangle;
	 tempangle = angle*180/Pi;
	 if (x2 > x1 && y2 < y1)
	 {
		 return 90.0-tempangle;
	 }
	 else if (x2 > x1 && y2 > y1)
	 {
		 return 90.0+tempangle;
	 }
	 else if (x2 < x1 && y2 > y1)
	 {
		 return 270.0-tempangle;
	 }
	 else if (x2 < x1 && y2 < y1)
	 {
		 return 270.0+tempangle;
	 }
	 m_fAngle = (GLfloat)tempangle;
	 return tempangle;
 }

 void CDrawWindSymbol::SetTextOnLine(double dAngle)
 {
	 QString strText;

	 GLfloat  CurrentZero = 0 ; 
	 GLfloat  k = 0 ; 
	 QVector3D Origin1,Origin2, glPoint1, glPoint2,glPoint;

	 ScreenToGL(m_CircularCenter,glPoint);
	 Origin1 = TransGLtoWGS84(glPoint);
	 Origin2.setX(Origin1.x());
	 Origin2.setY(Origin1.y()+10);
	 Origin2.setZ(Origin1.z());

	 glPoint1 = TransWGS84toGL(Origin1);
	 glPoint2 = TransWGS84toGL(Origin2);

	 if(glPoint2.x() == glPoint1.x())
	 {
		 CurrentZero = 0;
	 }else
	 {
		 k =( glPoint1.y() - glPoint2.y() )/( glPoint1.x() - glPoint2.x());
		 CurrentZero = atan(k);
		 CurrentZero = CurrentZero * 180 / 3.1415926;
		 if(k < 0)
			 CurrentZero =(90 - abs(CurrentZero));
		 else
			 CurrentZero = 90 - abs(CurrentZero);
	 }

	 int temp = 0 ;
	 if(k < 0)
	 {
		 if(dAngle > (360 - CurrentZero))
		 {
			 temp = int(CurrentZero) -(360 - int(dAngle));
		 }else
		 {
			  temp = int(CurrentZero) + int(dAngle);
		 }
	 }else
	 {
		 if(dAngle <= CurrentZero)
		 {
			 temp = 360 - CurrentZero+ dAngle;
		 }else
		 {
			 temp = int(dAngle)  - int(CurrentZero);
		 }
	 }
	
	 
	 int tempAngle = temp;
	 if (tempAngle == 0)
	 {
		 strText = "N/0℃";
	 } 
	 else if (tempAngle == 180)
	 {
		 strText = "S/180℃";
	 }	 
	 else if (tempAngle == 90)
	 {
		 strText = "E/90℃";
	 }
	 else if (tempAngle == 270)
	 {
		 strText = "W/270℃";
	 }
	 else if (tempAngle == 45)
	 {
		 strText = "NE/45℃";
	 }
	 else if (tempAngle == 135)
	 {
		 strText = "SE/135℃";
	 }
	 else if (tempAngle == 225)
	 {
		 strText = "SW/225℃";
	 }
	 else if (tempAngle == 315)
	 {
		 strText = "NW/315℃";
	 }
	 else if (tempAngle > 0 && tempAngle < 45)
	 {
		 strText.append(QString("NNE/%1℃").arg(tempAngle));
	 }
	 else if (tempAngle > 45 && tempAngle < 90)
	 {
		 strText.append(QString("ENE/%1℃").arg(tempAngle));
	 }
	 else if (tempAngle > 90 && tempAngle < 135)
	 {
		 strText.append(QString("ESE/%1℃").arg(tempAngle));
	 } 
	 else if (tempAngle > 135 && tempAngle < 180)
	 {
		 strText.append(QString("SSE/%1℃").arg(tempAngle));
	 }
	 else if (tempAngle > 180 && tempAngle < 225)
	 {
		 strText.append(QString("SSW/%1℃").arg(tempAngle));
	 }
	 else if (tempAngle > 225 && tempAngle < 270)
	 {
		 strText.append(QString("WSW/%1℃").arg(tempAngle));
	 }
	 else if (tempAngle > 270 && tempAngle < 315)
	 {
		 strText.append(QString("WNW/%1℃").arg(tempAngle));
	 } 
	 else if (tempAngle > 315 && tempAngle < 360)
	 {
		 strText.append(QString("NWN/%1℃").arg(tempAngle));
	 }

	//m_text->DisplayList(true);	

	QVector3D glpoint;
	ScreenToGL(m_LineEndPoint,glpoint);
	//m_text->SetColor(QVector4D(1.0,0.0,0.0,1));
	//m_text->SetPosition(glpoint);
	//m_text->StringArray(strText);
 }
 void CDrawWindSymbol::SetSymbol(Symbol *pSymbol)
 {
	 m_Symbol = pSymbol;
 }

 void CDrawWindSymbol::setILayerAttrib( ILayerAttribute *Ilayer )
 {

 }
