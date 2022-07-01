/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� lugsh
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����ResizeSymbol.h ResizeSymbol.cpp 
�� ���� CResizeSymbol
�� �ࣺ NULL
�� �ࣺ �� 
����˵���� �޸ķ�����
����˵���� 

-------------------------------------�汾����------------------------------------------------ 

V 1.0 
ԭ���� ��lugsh
������ڣ�2011�ꣿ�£���
============================================================*/

#include "ScaleSymbolLine.h"
#include "SymbolPoint.h"
#include "SymbolLine.h"
#include "SymbolContourLine.h"
#include "NormalSymbol.h"
#include <QtGui/QCursor>
//#include "QTextureFont.h"

/************************************************************************/
/* �������ܣ� Ĭ�Ϲ��캯��						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
CScaleSymbolLine::CScaleSymbolLine(void):m_pPickObj(NULL),m_bCanMove(false)//, m_pUndoSymbolPointEdit(0), m_pUndoLineMoveEdit(0),m_pundoSymbolMarkEdit(0)
{
	m_pShowLine = new Line;
	m_iFlag = 0 ;
	m_bEdit = false;
	nPosStart = nPosEnd = -1;
	pArray = NULL;
	pColor = NULL;
	pTempArray = NULL;
	pTempOriginalArray = NULL;
	m_pPickObj = NULL;
	m_pEditObj = NULL;
	m_pPickObjShade = NULL;
	m_pAttribute = NULL;

	m_pRectTemp = new Line;	
	m_rect1 = new Line;
	m_rect2 = new Line;
	m_rect3 = new Line;
	m_rect4 = new Line;
	m_rect5 = new Line;
	m_rect6 = new Line;
	m_rect7 = new Line;
	m_rect8 = new Line;	
}

/************************************************************************/
/* �������ܣ� Ĭ����������						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
CScaleSymbolLine::~CScaleSymbolLine(void)
{
	if(m_pShowLine)
	{
		delete m_pShowLine;
		m_pShowLine = NULL;
	}

	if(pTempArray)
	{
		delete pTempArray;
		pTempArray = NULL;
	}

	if(pTempOriginalArray)
	{
		delete pTempOriginalArray;
		pTempOriginalArray = NULL;
	}

	if (m_pAttribute)
	{
		delete m_pAttribute;
		m_pAttribute = NULL;
	}
}

/************************************************************************/
/* �������ܣ� �ƶ�����¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/* �޸�����:  renxue													*/
/* �޸�����:  2011 �� 04 �� 26 ��										*/
/************************************************************************/
void CScaleSymbolLine::MouseEvent(QMouseEvent* event) 
{
	static float lastDistance = 9999;
	float CurrentDistance;
	
// 	int dx = screenPoint.x() - lastPos.x();
// 	int dy = screenPoint.y()- lastPos.y();	
	
	if(m_pPickObj && m_bEdit )
	{		
	   CSymbolLine* pSymLine = dynamic_cast<CSymbolLine*>(m_pPickObj);

	   QVector3D screenPoint(event->x(),event->y(), m_ParentLayer->LayerZ());    
	   QVector3D GlPos;

	   ScreenToGL(screenPoint,GlPos); 
	   CurrentDistance = GlPos.distanceToLine(pSymLine->GetglCenter(),QVector3D(0.0f,0.0f,0.0f));
	   m_ScCurrentPoint = screenPoint;

	   bool isYScale = pSymLine->isInYScale(GlPos,30);
	   bool isXScale = pSymLine->isInXScale(GlPos,30);	
	   bool isXYScale = pSymLine->isInXYScale(GlPos,30);	

	   float scaletemp = CurrentDistance/lastDistance;

	   m_ParentLayer->GetLayerData()->RemoveRenderable(m_pRectTemp);
// 	   m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect1);
// 	   m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect2);
// 	   m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect3);
// 	   m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect4);
// 	   m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect5);
// 	   m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect6);
// 	   m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect7);
// 	   m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect8);
	   DrawSymbolLineBound(m_pPickObj,QVector4D(0.0f,1.0f,0.0f,1.0f));
	   m_ParentLayer->GetLayerData()->InsertRenderable(m_pRectTemp);
// 	   m_ParentLayer->GetLayerData()->InsertRenderable(m_rect1);
// 	   m_ParentLayer->GetLayerData()->InsertRenderable(m_rect2);
// 	   m_ParentLayer->GetLayerData()->InsertRenderable(m_rect3);
// 	   m_ParentLayer->GetLayerData()->InsertRenderable(m_rect4);
// 	   m_ParentLayer->GetLayerData()->InsertRenderable(m_rect5);
// 	   m_ParentLayer->GetLayerData()->InsertRenderable(m_rect6);
// 	   m_ParentLayer->GetLayerData()->InsertRenderable(m_rect7);
// 	   m_ParentLayer->GetLayerData()->InsertRenderable(m_rect8);

// 	   QVector3D SymLineCenter = pSymLine->GetglCenter();
// 	   float Height = pSymLine->GetBoundH();
// 	   float Width = pSymLine->GetBoundW();
// 
// 	   QVector3D leftButtomPoint = QVector3D(SymLineCenter.x()-Width/2,SymLineCenter.y()-Height/2,SymLineCenter.z());
// 	   QVector3D RightTopPoint = QVector3D(SymLineCenter.x()+Width/2,SymLineCenter.y()+Height/2,SymLineCenter.z());
// 	 
// 	   m_ParentLayer->GetLayerData()->RemoveRenderable(m_pRectTemp);
// 	   DrawRect(leftButtomPoint.x(),RightTopPoint.x(),leftButtomPoint.y(),RightTopPoint.y(),QVector4D(0.0f,1.0f,0.0f,1.0f));
// 	   m_ParentLayer->GetLayerData()->InsertRenderable(m_pRectTemp);

	   if (isLmousePressing)
	   {
		    if(isYScale) 
		   {
			   pSymLine->ScaleLine(1.00,scaletemp);			  
           }
		   if (isXScale)
		   {
			    pSymLine->ScaleLine(scaletemp,1.00);
		   }   	
		   if (isXYScale)
		   {
			     pSymLine->ScaleLine(scaletemp,scaletemp);
		   } 

		   //Scale objShade
		   if (pSymLine->GetShadeLine())
		   {
			  //  CSymbolLine* pSymLineShade = dynamic_cast<CSymbolLine*>(m_pPickObjShade);
				
				if(isYScale) 
				{
					pSymLine->GetShadeLine()->ScaleLine(1.00,scaletemp);			  
				}
				if (isXScale)
				{
					pSymLine->GetShadeLine()->ScaleLine(scaletemp,1.00);
				}   	
				if (isXYScale)
				{
					pSymLine->GetShadeLine()->ScaleLine(scaletemp,scaletemp);
				} 
		   }

		   m_ParentLayer->GetLayerData()->RemoveRenderable(m_pRectTemp);
// 		   m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect1);
// 		   m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect2);
// 		   m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect3);
// 		   m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect4);
// 		   m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect5);
// 		   m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect6);
// 		   m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect7);
// 		   m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect8);
		   DrawSymbolLineBound(pSymLine,QVector4D(0.0f,1.0f,0.0f,1.0f));
		   m_ParentLayer->GetLayerData()->InsertRenderable(m_pRectTemp);
// 		   m_ParentLayer->GetLayerData()->InsertRenderable(m_rect1);
// 		   m_ParentLayer->GetLayerData()->InsertRenderable(m_rect2);
// 		   m_ParentLayer->GetLayerData()->InsertRenderable(m_rect3);
// 		   m_ParentLayer->GetLayerData()->InsertRenderable(m_rect4);
// 		   m_ParentLayer->GetLayerData()->InsertRenderable(m_rect5);
// 		   m_ParentLayer->GetLayerData()->InsertRenderable(m_rect6);
// 		   m_ParentLayer->GetLayerData()->InsertRenderable(m_rect7);
// 		   m_ParentLayer->GetLayerData()->InsertRenderable(m_rect8);
	   }
		 lastDistance =  GlPos.distanceToLine(pSymLine->GetglCenter(),QVector3D(0.0f,0.0f,0.0f));      
	}
	else if(m_pPickObj && !m_bEdit)
	{
		ReleaseSymbol(m_pPickObj);
	}

	//lastPos = event->pos();
// 	else if(!m_pPickObj)
// 	{
// 		//����ʰȡ����
// 		QVector3D screenPoint(event->x(),event->y(), 0);	
// 		screenPoint.setY(m_pOpenglView->height() - screenPoint.y());
// 		m_pPickObj = m_ParentLayer->PickUp(screenPoint);
// 
// 		QVector3D scPoint(event->x(),event->y(), m_ParentLayer->LayerZ()), glPoint;
// 		ScreenToGL(scPoint, glPoint);
// 		if(m_pPickObj)
// 			SelectSymbol(m_pPickObj);
// 	}
}

/************************************************************************/
/* �������ܣ� ��������¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/* �޸�����:  renxue													*/
/* �޸�����:  2011 �� 04 �� 26 ��										*/
/************************************************************************/
void CScaleSymbolLine::MousePress(QMouseEvent *event)
{
	QVector3D screenPoint(event->x(),event->y(), 0);	
	QVector3D scPoint(event->x(),event->y(), m_ParentLayer->LayerZ()), glPoint;

	if(event->buttons() & Qt::LeftButton)	
	{
		isLmousePressing = true;
		if(!m_bEdit)
		{			
			screenPoint.setY(m_pOpenglView->height() - screenPoint.y());
			m_pPickObj = m_ParentLayer->PickUp(screenPoint);		
			ScreenToGL(scPoint, glPoint);
		    m_ScOldPoint = m_ScCurrentPoint = scPoint;

			if(m_pPickObj) //ʰȡ�����ţ����Կ�ʼ�任
			{
				SelectSymbol(m_pPickObj);
				m_bEdit = true;
				isLmousePressing = true;
				CSymbolLine* pSymLine = dynamic_cast<CSymbolLine*>(m_pPickObj);
				pSymLine->ScaleLine(1.0,1.0);
				
				if (pSymLine->GetShadeLine()!=NULL)
				{
					pSymLine->GetShadeLine()->ScaleLine(1.0,1.0);
					pSymLine->GetShadeLine()->SetSelected(false);
				}
				else if (pSymLine->isShadeLine())
				{
					pSymLine->SetSelected(false);
					m_bEdit = false;
					CSymbolLine* pSymLineParent = pSymLine->GetParentLine();
					
					if (pSymLineParent)
					{
						m_pPickObj = dynamic_cast<QRenderable*>(pSymLineParent);
						SelectSymbol(m_pPickObj);
						m_bEdit = true;
						pSymLineParent->ScaleLine(1.0,1.0);
					}
				}

			//	ResizeSymbol(m_pPickObj,2.0f);
				//��������ı߿�
			}
		}
		else if (m_bEdit)
		{
			m_ScCurrentPoint = scPoint;
		}
	}
	else if(event->buttons() & Qt::RightButton)			//ѡ�а����Ҽ�����ɾ��
	{
		m_bEdit = false; //����Ҽ��������༭

		m_ParentLayer->GetLayerData()->RemoveRenderable(m_pRectTemp);
// 		m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect1);
// 		m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect2);
// 		m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect3);
// 		m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect4);
// 		m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect5);
// 		m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect6);
// 		m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect7);
// 		m_ParentLayer->GetLayerData()->RemoveRenderable(m_rect8);
	}
}

/************************************************************************/
/* �������ܣ� ̧������¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CScaleSymbolLine::MouseRelease(QMouseEvent *event)
{
	//	m_pOpenglView->setCursor(Qt::ArrowCursor);	//���ָ�ԭʼ״̬
		m_bCanMove = false;							//�ƶ���־��Ϊfalse
		
		if(Qt::LeftButton)			//ѡ�а����Ҽ�����ɾ��
		{
			isLmousePressing = false;
		}
		
		m_iFlag  =0;

		//-----------------undo-------------------
		//if(m_pUndoSymbolPointEdit)
		//{
		//	if(m_pUndoSymbolPointEdit->GetRedoPostion() == QVector3D(0,0,0))
		//	{
		//		delete m_pUndoSymbolPointEdit;
		//	}
		//	else
		//	{
		//		//m_undoStack->push(m_pUndoSymbolPointEdit);
		//	}
		//	m_pUndoSymbolPointEdit = 0;
		//}
		//if(m_pUndoLineMoveEdit)
		//{
		//	if(m_pUndoLineMoveEdit->GetUndoPosition() == QVector3D(0,0,0))
		//	{
		//		delete m_pUndoLineMoveEdit;
		//	}
		//	else
		//	{
		//		//m_undoStack->push(m_pUndoLineMoveEdit);
		//	}
		//	m_pUndoLineMoveEdit = 0;
		//}

		//if(m_pundoSymbolMarkEdit)
		//{
		//	if(m_pundoSymbolMarkEdit->GetRedoPostion() == QVector3D(0,0,0))
		//	{
		//		delete m_pundoSymbolMarkEdit;
		//	}
		//	else
		//	{
		//		//m_undoStack->push(m_pundoSymbolMarkEdit);
		//	}
		//	m_pundoSymbolMarkEdit = 0;
		//}
		//----------------------------------------
}

/************************************************************************/
/* �������ܣ� ����뿪�¼�  					                        */
/* ����˵��:  NULL														*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 06 �� 29 ��										*/
/************************************************************************/
void CScaleSymbolLine::LeaveEvent()
{
}


/************************************************************************/
/* �������ܣ� ���˫���¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CScaleSymbolLine::keyPressEvent(QKeyEvent * event )
{
	if(NULL != m_pPickObj)			//��ʾ�з��ű�ѡ��
	{
		if((event->key() & Qt::Key_Delete) || (event->key() & Qt::Key_Backspace))
		{
			m_iFlag = 0;
			m_bEdit = false;
			DeleteSymbol(m_pPickObj);
			emit DelButtonStatus(false);
		}
	}
}

/************************************************************************/
/* �������ܣ� ���˫���¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/* �޸�����:  renxue													*/
/* �޸�����:  2011 �� 04 �� 26 ��										*/
/************************************************************************/
void CScaleSymbolLine::MouseDoubleClick(QMouseEvent *event)
{
	QVector3D screenPoint(event->x(),event->y(), 0);
	//˫����ʾѡ�ж���
	if(m_ParentLayer)
	{
		screenPoint.setY(m_pOpenglView->height() - screenPoint.y());
		m_pPickObj = m_ParentLayer->PickUp(screenPoint);
		if(m_pPickObj)
		{
			if(NULL != m_pEditObj)//����ͬʱѡ�ж���༭����
			{
				ReleaseSymbol(m_pEditObj);
				m_pEditObj = NULL;
			}
			m_pEditObj = m_pPickObj;
			m_bEdit = true;
			m_bCanMove = true;
			SelectSymbol(m_pPickObj);

			emit DelButtonStatus(true);

			//������ֵ�ߵ����Կ�
			CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine *>(m_pEditObj);
			if(pContourLine)
			{
				InitAttribute(true);
				//emit  UpdateAttribute(this);
				//����ʰȡ�����ߵ�����
				m_pAttribute = pContourLine->GetAttribute();
				SetContourAttrib(m_pAttribute);
				emit UpdateAttribute(this);
				m_pAttribute = NULL;
			}
		}
	}
}

/************************************************************************/
/* �������ܣ� ���ö�Ӧ�ķ���					                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CScaleSymbolLine::SetSymbol(Symbol *pSymbol)
{
}

/************************************************************************/
/* �������ܣ� ��������ֵ					                            */
/* ����˵��:  NULL														*/
/* ��    ��:  renxue														*/
/* ��    ��:  2011 �� 04 �� 29 ��										*/
/************************************************************************/
void CScaleSymbolLine::setILayerAttrib(ILayerAttribute *Ilayer)
{
	if(NULL != m_pEditObj && m_bEdit)
	{
		CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine *>(m_pEditObj);
		if(pContourLine)
		{
			m_pAttribute = pContourLine->GetAttribute();
		}

		if(Ilayer != NULL) 
		{
			QString strName = Ilayer->Name();
			if(strName == QString("cValue"))
			{
				m_pAttribute->SetValue(Ilayer->Value().toDouble());
			}
			else if(strName == QString("cColor"))
			{
				QColor tColor = Ilayer->Value().value<QColor>();
                m_pAttribute->SetColor(QVector4D(tColor.red()/255.0, tColor.green()/255.0, tColor.blue()/255.0, 1.0));
			}
			else if(strName == QString("cFill"))
			{
				m_pAttribute->SetFillType(Ilayer->Value().toBool());
			}
			else if(strName == QString("cWidth"))
			{
				m_pAttribute->SetLineWidth(Ilayer->Value().toInt());
			}
			else if(strName == QString("cKinds"))
			{
				m_pAttribute->SetLineType(Ilayer->Value().toString());
			}
		}
		m_pAttribute = NULL;
		pContourLine->Render();
		ReleaseSymbol(m_pEditObj);
		m_pEditObj = NULL;
		m_bEdit = false;
		emit DelButtonStatus(false);
	}
}

/************************************************************************/
/* �������ܣ� ��������ֵ--Ŀǰֻ�����ڵ�ֵ��						    */
/* ����˵��:  NULL														*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 05 �� 10 ��										*/
/************************************************************************/
void CScaleSymbolLine::SetContourAttrib(CSymbolAttribute *pAttribute)
{
	m_layerAttribute->clear();
	QStringList strValue;

	const int nShowValue =(int) pAttribute->GetValue();
	ILayerAttribute *cValue = new ILayerAttribute("cValue","cValue","��ֵ����ֵ",nShowValue,QVariant::Int,0,10000,strValue);
	m_layerAttribute->insert(QString("cValue"),cValue);

	QStringList strKinds;
	strKinds.append(QString("ʵ��"));
	strKinds.append(QString("�̻���"));
	strKinds.append(QString("����"));
	QString strLineType = m_pAttribute->GetLineType();
	QString strShow;
	if("1" == strLineType)
	{
		strShow = QString("�̻���");
	}
	else if("2" == strLineType)
	{
		strShow = QString("����");
	}
	ILayerAttribute *cKinds = new ILayerAttribute("cKinds","cKinds","��ֵ������",strShow,QVariant::String,0,0,strKinds);
	m_layerAttribute->insert(QString("cKinds"),cKinds);

	QVector4D tempColor = pAttribute->GetColor();
	tempColor.setX(tempColor.x()*255);
	tempColor.setY(tempColor.y()*255);
	tempColor.setZ(tempColor.z()*255);
	QStringList strColor;
	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor","��ֵ����ɫ",tempColor,QVariant::Color,0,0,strColor);
	m_layerAttribute->insert(QString("cColor"),cColor);

	QStringList strWidth;
	for (int i=1; i<=10; i++)
	{
		strWidth.append(QString::number(i));
	}
	ILayerAttribute *cWidth = new ILayerAttribute("cWidth","cWidth","��ֵ���߿�",pAttribute->GetLineWidth(),QVariant::Int,1,10,strWidth);
	m_layerAttribute->insert(QString("cWidth"), cWidth);

	QStringList strFill;
	strFill.append("��");
	strFill.append("��");
	ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill","��ֵ���Ƿ����",pAttribute->GetFillType(),QVariant::Bool,0,0,strFill);
	m_layerAttribute->insert(QString("cFill"),cFill);
}

bool CScaleSymbolLine::SelectSymbolBound(QRenderable *pPickObj)
{
	return true;
}
/************************************************************************/
/* �������ܣ� ѡ�з���						                            */
/* ����˵��:  QRenderable												*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 06 �� 21 ��										*/
/************************************************************************/
void CScaleSymbolLine::SelectSymbol(QRenderable *pPickObj)
{
// 	Line* pTroughLine = dynamic_cast<Line*>(pPickObj);
// 	if(pTroughLine)
// 	{
// 		pArray = pTroughLine->getVectexArray();
// 		pColor = pTroughLine->getColorArray();
// 		QVector4D selectColor(0.5, 1.0, 0.0, 1.0);
// 		if(pArray && pColor)
// 		{
// 			SourceColor.setX(pColor->at(0).x());
// 			SourceColor.setY(pColor->at(0).y());
// 			SourceColor.setZ(pColor->at(0).z());
// 			SourceColor.setW(pColor->at(0).w());
// 			pColor->Clear();
// 			for(int i=0; i<pArray->size();i++)
// 			{
// 				pColor->push_back(selectColor);
// 			}
// 		}
// 		//----------udno����-----------------
// 		m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
// 		m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
// 		m_pUndoLineMoveEdit->SetRenderUnit(pTroughLine);
// 		//--------------end------------------
// 	}

	SymbolPoint* pSp = dynamic_cast<SymbolPoint*>(pPickObj);
	if(pSp)
	{
		pSp->SetSelected(true);
		//----------udno����-----------------
		int iI = pSp->GetPickIndex();
		QVector3DArray* pData = dynamic_cast<QVector3DArray*>(pSp->VertexArray());
		//m_pUndoSymbolPointEdit = new CUndoSymbolPointMoveCommand;
		//m_pUndoSymbolPointEdit->SetEditSymbolPoint(pSp);
		//m_pUndoSymbolPointEdit->SetIndex(iI);
		//m_pUndoSymbolPointEdit->SetGLWidget(m_pOpenglView);
		//if(pData)
		//{
		//	m_pUndoSymbolPointEdit->SetUndoPostion(pData->at(iI));
		//}
		//--------------end--------------------
		return ;
	}

	//20110510���
	NormalSymbol* pSymNormal   = dynamic_cast<NormalSymbol*>(pPickObj);
	if(pSymNormal)
	{
		pSymNormal->SetSelected(true);
		//----------udno����-----------------
		/*int iI = pSymNormal->GetPickIndex();
		QVector3DArray* pData = dynamic_cast<QVector3DArray*>(pSymNormal->VertexArray());
		m_pUndoSymbolPointEdit = new CUndoSymbolPointMoveCommand;
		m_pUndoSymbolPointEdit->SetEditSymbolPoint(pSymNormal);
		m_pUndoSymbolPointEdit->SetIndex(iI);
		m_pUndoSymbolPointEdit->SetGLWidget(m_pOpenglView);
		if(pData)
		{
			m_pUndoSymbolPointEdit->SetUndoPostion(pData->at(iI));
		}*/
		//--------------end------------------
		return ;
	}

	//������
	CSymbolLine *pSymLine = dynamic_cast<CSymbolLine *>(pPickObj);
	if(pSymLine)
	{
		pSymLine->SetSelected(true);
		//----------udno����-----------------
		//m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
		//m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
		//m_pUndoLineMoveEdit->SetRenderUnit(pSymLine);
		//--------------end------------------
		return;		
	}
	
	CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine *>(pPickObj);//��ֵ��
	if(pContourLine)
	{
		pContourLine->SetSelected(true);
		//----------udno����-----------------
		//m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
		//m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
		//m_pUndoLineMoveEdit->SetRenderUnit(pContourLine);
		//--------------end------------------
		return;		
	}

	//����20110621���
	/*QTextureFont *pText = dynamic_cast<QTextureFont *>(pPickObj);
	if(pText)
	{
	pText->SetSelected(true);	

	int iI = pText->GetPickUpIndex();
	QVector3DArray* pData = dynamic_cast<QVector3DArray*>(pText->VertexArray());
	m_pundoSymbolMarkEdit = new CUndoSymbolMarkMoveCommand;
	m_pundoSymbolMarkEdit->SetEditSymbolMark(pText);
	m_pundoSymbolMarkEdit->SetIndex(iI);
	m_pundoSymbolMarkEdit->SetGLWidget(m_pOpenglView);
	if(pData)
	{
	m_pundoSymbolMarkEdit->SetUndoPostion(pData->at(iI));
	}

	return;
	}*/
}

/************************************************************************/
/* �������ܣ� �ͷ�ѡ�еķ���						                    */
/* ����˵��:  QRenderable												*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 05 �� 10 ��										*/
/************************************************************************/
void CScaleSymbolLine::ReleaseSymbol(QRenderable *pPickObj)
{

	int nIndex = -1;
	SymbolPoint* pSp = dynamic_cast<SymbolPoint*>(pPickObj);
	if(pSp)
	{
		nIndex = pSp->GetPickIndex();
		QVector3DArray* pData = dynamic_cast<QVector3DArray*>(pSp->VertexArray());
		pSp->SetPickIndex();
		pSp->SetSelected(false);
		m_pPickObj = NULL;							//���³�ʼ��ʰȡ����
		return ;
	}

	//20110510���
	NormalSymbol* pSymNormal = dynamic_cast<NormalSymbol*>(pPickObj);
	if(pSymNormal)
	{
		nIndex = pSymNormal->GetPickIndex();
		QVector3DArray* pData = dynamic_cast<QVector3DArray*>(pSymNormal->VertexArray());
		pSymNormal->SetPickIndex();
		pSymNormal->SetSelected(false);
		m_pPickObj = NULL;							//���³�ʼ��ʰȡ����
		return ;
	}

	//������
	CSymbolLine* pSymLine = dynamic_cast<CSymbolLine*>(pPickObj);
	if(pSymLine)
	{
		pSymLine->SetSelected(false);
		m_pPickObj = NULL;							//���³�ʼ��ʰȡ����
		return ;
	}
	//��ֵ��
	CSymbolContourLine* pContourLine = dynamic_cast<CSymbolContourLine*>(pPickObj);//��ֵ��
	if(pContourLine)
	{
		pContourLine->SetSelected(false);
		m_pPickObj = NULL;							//���³�ʼ��ʰȡ����
		return ;
	}

	//����20110621���
	//QTextureFont *pText = dynamic_cast<QTextureFont *>(pPickObj);
	//if(pText)
	//{
	//	pText->SetSelected(false);
	////	pText->SetColor(QVector4D())
	//	m_pPickObj = NULL;
	//	return;
	//}
}

/************************************************************************/
/* �������ܣ� ɾ������						                            */
/* ����˵��:  QRenderable												*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 06 �� 22 ��										*/
/************************************************************************/
void CScaleSymbolLine::DeleteSymbol(QRenderable *pPickObj)
{	
}

inline QVector3D CScaleSymbolLine::GetScalePoint(QVector3D glCenter,QVector3D glPosOrigin,float scale)
{
	if (scale==1) 
		return glPosOrigin;
	else
	{		
		float xtemp,ytemp;		
		xtemp = scale*(glPosOrigin.x()-glCenter.x())+glCenter.x();
		ytemp = scale*(glPosOrigin.y()-glCenter.y())+glCenter.y();
		
		return QVector3D(xtemp,ytemp,0.0f);
	}
}

void CScaleSymbolLine::DrawSymbolLineBound(QRenderable* m_pPickObj,QVector4D color)
{
	if (m_pRectTemp && m_pPickObj)
	{
		CSymbolLine* pSymLine = dynamic_cast<CSymbolLine*>(m_pPickObj);

		QVector3D SymLineCenter = pSymLine->GetglCenter();
		float Height = pSymLine->GetBoundH();
		float Width = pSymLine->GetBoundW();
		// 
//		QVector3D leftButtomPoint = QVector3D(SymLineCenter.x()-Width/2,SymLineCenter.y()-Height/2,SymLineCenter.z());
//		QVector3D RightTopPoint = QVector3D(SymLineCenter.x()+Width/2,SymLineCenter.y()+Height/2,SymLineCenter.z());

		QVector3D glPoint1 = QVector3D(SymLineCenter.x()-Width/2,SymLineCenter.y()-Height/2,0.0f);
		QVector3D glPoint2 = QVector3D(SymLineCenter.x()-Width/2,SymLineCenter.y()+Height/2,0.0f);
		QVector3D glPoint3 = QVector3D(SymLineCenter.x()+Width/2,SymLineCenter.y()+Height/2,0.0f);
		QVector3D glPoint4 = QVector3D(SymLineCenter.x()+Width/2,SymLineCenter.y()-Height/2,0.0f);

		//QVector4D geoPoint1,geoPoint2,geoPoint1,geoPoint1;
		m_pRectTemp->getVectexArray()->Clear(); 
		m_pRectTemp->OriginalArray()->Clear();
		m_pRectTemp->getColorArray()->Clear();

		m_pRectTemp->getVectexArray()->push_back(glPoint1);
		m_pRectTemp->getVectexArray()->push_back(glPoint2);
		m_pRectTemp->getVectexArray()->push_back(glPoint3);
		m_pRectTemp->getVectexArray()->push_back(glPoint4);	

		for (int i=0;i<4;i++)
		{		
			m_pRectTemp->getColorArray()->push_back(color);
		}

		m_pRectTemp->SetColorArray();
		m_pRectTemp->SetLineStyle(2, 1, 0xFFFF); //����"0"
		m_pRectTemp->Mode(GL_LINE_LOOP);	
		/*
		//Draw 8 rect in the bound
		DrawRect(m_rect1,QVector3D(SymLineCenter.x()-Width/2,SymLineCenter.y()-Height/2,0.0f),5,color);
		DrawRect(m_rect2,QVector3D(SymLineCenter.x()-Width/2,SymLineCenter.y(),0.0f),5,color);
		DrawRect(m_rect3,QVector3D(SymLineCenter.x()-Width/2,SymLineCenter.y()+Height/2,0.0f),5,color);
		DrawRect(m_rect4,QVector3D(SymLineCenter.x(),SymLineCenter.y()-Height/2,0.0f),5,color);

		DrawRect(m_rect5,QVector3D(SymLineCenter.x(),SymLineCenter.y()+Height/2,0.0f),5,color);
		DrawRect(m_rect6,QVector3D(SymLineCenter.x()+Width/2,SymLineCenter.y()-Height/2,0.0f),5,color);
		DrawRect(m_rect7,QVector3D(SymLineCenter.x()+Width/2,SymLineCenter.y(),0.0f),5,color);
		DrawRect(m_rect8,QVector3D(SymLineCenter.x()+Width/2,SymLineCenter.y()+Height/2,0.0f),5,color);
		*/
	}
	
	// 	 
	// 	   m_ParentLayer->GetLayerData()->RemoveRenderable(m_pRectTemp);
	// 	   DrawRect(leftButtomPoint.x(),RightTopPoint.x(),leftButtomPoint.y(),RightTopPoint.y(),QVector4D(0.0f,1.0f,0.0f,1.0f));
	// 	   m_ParentLayer->GetLayerData()->InsertRenderable(m_pRectTemp);


}

void CScaleSymbolLine::DrawRect(Line* rect,QVector3D& glpos,int srcD,QVector4D color)
{
	if (rect)
	{
// 		QVector3D glPoint1 = QVector3D(x1,y1,0.0f);
// 		QVector3D glPoint2 = QVector3D(x1,y2,0.0f);
// 		QVector3D glPoint3 = QVector3D(x2,y1,0.0f);
// 		QVector3D glPoint4 = QVector3D(x2,y2,0.0f);
	
		QVector3D glPoint1,glPoint2,glPoint3,glPoint4;
		QVector3D screenPos;

		rect->getVectexArray()->Clear(); 
		rect->OriginalArray()->Clear();
		rect->getColorArray()->Clear();
		
	//	GLToScreen(glpos,screenPos);
		rect->ogTOs(glpos,screenPos);
		float height = 1280;
		QViewport* viewport = (this->Observer()->Viewport());
		if (viewport)
		{
			height = viewport->Height();
		}

 		ScreenToGL(QVector3D(screenPos.x()-srcD,height-screenPos.y()+srcD,0.0f),glPoint1);
 		ScreenToGL(QVector3D(screenPos.x()+srcD,height-screenPos.y()+srcD,0.0f),glPoint2);
 		ScreenToGL(QVector3D(screenPos.x()+srcD,height-screenPos.y()-srcD,0.0f),glPoint3);
 		ScreenToGL(QVector3D(screenPos.x()-srcD,height-screenPos.y()-srcD,0.0f),glPoint4);		

		rect->getVectexArray()->push_back(glPoint1);
		rect->getVectexArray()->push_back(glPoint2);
		rect->getVectexArray()->push_back(glPoint3);
		rect->getVectexArray()->push_back(glPoint4);	

		for (int i=0;i<4;i++)
		{		
			rect->getColorArray()->push_back(color);
		}

		rect->SetColorArray();
		rect->SetLineStyle(4, 1, 0xFFFF); //����"0"
		rect->Mode(GL_LINE_LOOP);		
	}
}

void CScaleSymbolLine::ResizeSymbol(QRenderable *pPickObj,float scale)
{
	 CSymbolLine* pSymLine = dynamic_cast<CSymbolLine*>(pPickObj);

	 if (pSymLine)
	 {
		 QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pSymLine->VertexArray());
		 QVector3DArray *pOrgData = dynamic_cast<QVector3DArray*>(pSymLine->OriginalArray());

		 //��ȡsymbolline�ı߽���Ϣ
		 float fxmin,fymin,fxmax,fymax,xtemp,ytemp;
		 QVector3D glsymbolCenter,glposTemp;
		 fxmin = fxmax = pData->at(0).x();
		 fymin = fymax = pData->at(0).y();

		 for (int i=0;i<pData->count();i++)
		 {
			 xtemp = pData->at(i).x();
			 ytemp = pData->at(i).y();

			 if(xtemp>fxmax) fxmax = xtemp;
			 if(xtemp<fxmin) fxmin = xtemp;
			 if(ytemp>fymax) fymax = ytemp;
			 if(ytemp<fymin) fymin = ytemp;			 
		 }
		 glsymbolCenter = QVector3D((fxmin+fxmax)/2,(fymin+fymax)/2,0);
		 //�滻��
		 for (int i=0;i<pData->count();i++)
		 {
			 glposTemp = GetScalePoint(glsymbolCenter,pData->at(i),scale);
			 pData->replace(i,glposTemp);
			
			 if (i<pOrgData->count())
			 {
				 pOrgData->replace(i,TransGLtoWGS84(glposTemp));
			 }			 
		 }

	 }
	// pSp->Scales(2.0f);
}
