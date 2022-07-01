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
�ļ�����ModifySymbol.h ModifySymbol.cpp 
�� ���� CModifySymbol
�� �ࣺ NULL
�� �ࣺ �� 
����˵���� �޸ķ�����
����˵���� 

-------------------------------------�汾����------------------------------------------------ 
V 1.2 
�� �ߣ�������
������ڣ�2011��4��28��
����˵������ӶԸ�����ŵı༭��������
V 1.1 
�� �ߣ�renxue
������ڣ�2011��04��28��
����˵�������ӶԷ����ߡ���ֵ�ߵ�ʰȡ���� 
V 1.0 
ԭ���� ��lugsh
������ڣ�2011�ꣿ�£���
============================================================*/

#include "ModifySymbol.h"
#include "SymbolPoint.h"
#include "SymbolLine.h"
#include "SymbolMultiLine.h"
#include "SymbolContourLine.h"
#include "SymbolMultiContourLine.h"
#include "NormalSymbol.h"
#include <QtGui/QCursor>
#include <QtWidgets/QDockWidget>
//#include "QTextureFont.h"
#include "QEText.h"
/************************************************************************/
/* �������ܣ� Ĭ�Ϲ��캯��						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
CModifySymbol::CModifySymbol(void):m_pPickObj(NULL),m_bCanMove(false)//, m_pUndoSymbolPointEdit(0), m_pUndoLineMoveEdit(0),m_pundoSymbolMarkEdit(0)
{
	m_pShowLine = new Line;
	m_iFlag = 0 ;
	m_bEdit = false;
	m_bEditStart = false;
	nPosStart = nPosEnd = -1;
	pArray = NULL;
	pColor = NULL;
	pTempArray = NULL;
	pTempOriginalArray = NULL;
	m_pPickObj = NULL;
	m_pEditObj = NULL;
	m_pAttribute = NULL;

}

/************************************************************************/
/* �������ܣ� Ĭ����������						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
CModifySymbol::~CModifySymbol(void)
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
void CModifySymbol::MouseEvent(QMouseEvent* event) 
{
	//��꿪�ö���ѡ��ȡ��
	if(NULL!= m_pPickObj && m_bCanMove )
	{
		QVector3D screenPoint(event->x(),event->y(), m_ParentLayer->LayerZ());
		MoveSymbol(m_pPickObj, screenPoint);
	}else if(NULL != m_pPickObj && !m_bCanMove)
	{
		ReleaseSymbol(m_pPickObj);
	}
	else if(!m_pPickObj)
	{
		//����ʰȡ����
		QVector3D screenPoint(event->x(),event->y(), 0);	
		screenPoint.setY(m_pOpenglView->height() - screenPoint.y());
		m_pPickObj = m_ParentLayer->PickUp(screenPoint);

		QVector3D scPoint(event->x(),event->y(), m_ParentLayer->LayerZ()), glPoint;
		ScreenToGL(scPoint, glPoint);
		if(m_pPickObj)
			SelectSymbol(m_pPickObj);
	}
}

/************************************************************************/
/* �������ܣ� ��������¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/* �޸�����:  renxue													*/
/* �޸�����:  2011 �� 04 �� 26 ��										*/
/************************************************************************/
void CModifySymbol::MousePress(QMouseEvent *event)
{
	QVector3D screenPoint(event->x(),event->y(), 0);	
	screenPoint.setY(m_pOpenglView->height() - screenPoint.y());
	m_pPickObj = m_ParentLayer->PickUp(screenPoint);

	QVector3D scPoint(event->x(),event->y(), m_ParentLayer->LayerZ()), glPoint;
	ScreenToGL(scPoint, glPoint);

	if(event->buttons() & Qt::LeftButton)				//��������༭״̬
	{
		if(m_pEditObj && m_bEdit)//�༭��ֵ��
		{
			if(!m_bEditStart)
			{
				m_PressPoints.Clear();
				CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine*>(m_pEditObj);
				if(pContourLine)
				{
					QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pContourLine->VertexArray());
					int nPos = pContourLine->GetPosition();
					if(nPos<0 || nPos>(pData->size()-1)) return;//û��ʰȡ����
					nPosStart = nPos;
					m_PressPoints.push_back(pData->at(nPos));
					m_bEditStart = true;
				}
			}
			else 
			{
				m_PressPoints.push_back(glPoint);
			}
			//��ʾ
			pArray = m_pShowLine->getVectexArray();
			pColor = m_pShowLine->getColorArray();		
			pArray->Clear();
			pColor->Clear();
			for(int i=0; i<m_PressPoints.size(); i++)
			{
				pArray->push_back(m_PressPoints[i]);
				pColor->push_back(QVector4D(1, 0, 0, 1));
			}
			m_pShowLine->SetColorArray();
			if(m_PressPoints.size()>1)
				m_ParentLayer->GetLayerData()->InsertRenderable(m_pShowLine);
		}
		else if(m_pPickObj)
		{
			m_bCanMove = true;
			SelectSymbol(m_pPickObj);
		}
	}
	else if(event->buttons() & Qt::RightButton)			//ѡ�а����Ҽ�����ɾ��
	{
		if(NULL != m_pPickObj && !m_bEdit)
		{
			DeleteSymbol(m_pPickObj);
			m_bCanMove = false;
		}
		else if(NULL != m_pPickObj )					//�༭״̬ȡ��
		{
			m_bEdit = false;
			emit DelButtonStatus(false);
		}

		if(NULL != m_pEditObj)
		{
			if(NULL != pArray && NULL != pColor)
			{
				pArray->Clear();
				pColor->Clear();
			}
			
			m_bEdit = false;
			emit DelButtonStatus(false);
			if(!m_bEdit && m_bEditStart)
			{
				CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine*>(m_pEditObj);
				if(pContourLine)
				{
					QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pContourLine->VertexArray());
					int nPos = pContourLine->GetPosition();
					if(nPos<0 || nPos>(pData->size()-1)) return;//û��ʰȡ����
					nPosEnd = nPos;
					m_PressPoints.push_back(pData->at(nPos));
					m_bEditStart = false;
				}
			}
			m_SpLine.GetSmoothPoints(m_PressPoints, m_SmoothPoints);
			if(nPosStart>0 && nPosEnd>0)
			{
				EditSymbol(m_pEditObj, &m_SmoothPoints, nPosStart, nPosEnd);//�༭��ֵ��
			}
			ReleaseSymbol(m_pEditObj);			
			m_pEditObj = NULL;
		}
	}
}

/************************************************************************/
/* �������ܣ� ̧������¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CModifySymbol::MouseRelease(QMouseEvent *event)
{
		m_pOpenglView->setCursor(Qt::ArrowCursor);	//���ָ�ԭʼ״̬
		m_bCanMove = false;							//�ƶ���־��Ϊfalse
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
void CModifySymbol::LeaveEvent()
{
}


/************************************************************************/
/* �������ܣ� ���˫���¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CModifySymbol::keyPressEvent(QKeyEvent * event )
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
void CModifySymbol::MouseDoubleClick(QMouseEvent *event)
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
void CModifySymbol::SetSymbol(Symbol *pSymbol)
{
}

/************************************************************************/
/* �������ܣ� ��������ֵ					                            */
/* ����˵��:  NULL														*/
/* ��    ��:  renxue														*/
/* ��    ��:  2011 �� 04 �� 29 ��										*/
/************************************************************************/
void CModifySymbol::setILayerAttrib(ILayerAttribute *Ilayer)
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
void CModifySymbol::SetContourAttrib(CSymbolAttribute *pAttribute)
{
	m_layerAttribute->clear();
	QStringList strValue;

	const int nShowValue =(int) pAttribute->GetValue();
	ILayerAttribute *cValue = new ILayerAttribute("cValue","cValue",tr("��ֵ����ֵ"),nShowValue,QVariant::Int,0,10000,strValue);
	m_layerAttribute->insert(QString("cValue"),cValue);

	QStringList strKinds;
	strKinds.append(QString(tr("ʵ��")));
	strKinds.append(QString(tr("�̻���")));
	strKinds.append(QString(tr("����")));
	QString strLineType = m_pAttribute->GetLineType();
	QString strShow;
	if("1" == strLineType)
	{
		strShow = QString(tr("�̻���"));
	}
	else if("2" == strLineType)
	{
		strShow = QString(tr("����"));
	}
	ILayerAttribute *cKinds = new ILayerAttribute("cKinds","cKinds",tr("��ֵ������"),strShow,QVariant::String,0,0,strKinds);
	m_layerAttribute->insert(QString("cKinds"),cKinds);

	QVector4D tempColor = pAttribute->GetColor();
	tempColor.setX(tempColor.x()*255);
	tempColor.setY(tempColor.y()*255);
	tempColor.setZ(tempColor.z()*255);
	QStringList strColor;
	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor",tr("��ֵ����ɫ"),tempColor,QVariant::Color,0,0,strColor);
	m_layerAttribute->insert(QString("cColor"),cColor);

	QStringList strWidth;
	for (int i=1; i<=10; i++)
	{
		strWidth.append(QString::number(i));
	}
	ILayerAttribute *cWidth = new ILayerAttribute("cWidth","cWidth",tr("��ֵ���߿�"),pAttribute->GetLineWidth(),QVariant::Int,1,10,strWidth);
	m_layerAttribute->insert(QString("cWidth"), cWidth);

	QStringList strFill;
	strFill.append(tr("��"));
	strFill.append(tr("��"));
	ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill",tr("��ֵ���Ƿ����"),pAttribute->GetFillType(),QVariant::Bool,0,0,strFill);
	m_layerAttribute->insert(QString("cFill"),cFill);
}

/************************************************************************/
/* �������ܣ� ѡ�з���						                            */
/* ����˵��:  QRenderable												*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 06 �� 21 ��										*/
/************************************************************************/
void CModifySymbol::SelectSymbol(QRenderable *pPickObj)
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
		//int iI = pSp->GetPickIndex();
		//QVector3DArray* pData = dynamic_cast<QVector3DArray*>(pSp->VertexArray());
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

	//���Ŷ���
	CSymbolMultiLine *pSymMultiLine = dynamic_cast<CSymbolMultiLine *>(pPickObj);
	if(pSymMultiLine)
	{
		pSymMultiLine->SetSelected(true);
		//----------udno����-----------------
		//m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
		//m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
		//m_pUndoLineMoveEdit->SetRenderUnit(pSymMultiLine);
		//--------------end------------------
		return;		
	}
	
	CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine *>(pPickObj);//��ֵ��
	if(pContourLine)
	{
		pContourLine->SetSelected(true);
		pContourLine->ReDraw();
		//----------udno����-----------------
		//m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
		//m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
		//m_pUndoLineMoveEdit->SetRenderUnit(pContourLine);
		//--------------end------------------
		return;		
	}

	CSymbolMultiContourLine *pMultiContourLine = dynamic_cast<CSymbolMultiContourLine *>(pPickObj);//��ֵ��
	if(pMultiContourLine)
	{
		pMultiContourLine->SetSelected(true);
		pMultiContourLine->ReDraw();
		//----------udno����-----------------
		//m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
		//m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
		//m_pUndoLineMoveEdit->SetRenderUnit(pMultiContourLine);
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
	}

	QEText *pText1 = dynamic_cast<QEText *>(pPickObj);
	if(pText1)
	{
		pText1->SetSelected(true);	

		int iI = pText1->GetPickUpIndex();
		QVector3DArray* pData = dynamic_cast<QVector3DArray*>(pText1->VertexArray());
		m_pundoSymbolMarkEdit = new CUndoSymbolMarkMoveCommand;
		m_pundoSymbolMarkEdit->SetEditSymbolMark((QRenderUnit*)pText1);
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
void CModifySymbol::ReleaseSymbol(QRenderable *pPickObj)
{
// 	Line* pTroughLine = dynamic_cast<Line*>(pPickObj);
// 	if(pTroughLine)
// 	{
// 		pArray = pTroughLine->getVectexArray();
// 		pColor = pTroughLine->getColorArray();
// 		if(pArray && pColor)
// 		{
// 			pColor->Clear();
// 			for(int i=0; i<pArray->size();i++)
// 			{
// 				pColor->push_back(SourceColor);
// 			}
// 		}	
// 		//pTroughLine->SetColorArray();
// 		//----------udno����-----------------
// 		m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
// 		m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
// 		m_pUndoLineMoveEdit->SetRenderUnit(pTroughLine);
// 		//--------------end------------------
// 		m_pPickObj = NULL;
// 		return;
// 	}

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

	//������
	CSymbolMultiLine* pSymMultiLine = dynamic_cast<CSymbolMultiLine*>(pPickObj);
	if(pSymMultiLine)
	{
		pSymMultiLine->SetSelected(false);
		m_pPickObj = NULL;							//���³�ʼ��ʰȡ����
		return ;
	}
	//��ֵ��
	CSymbolContourLine* pContourLine = dynamic_cast<CSymbolContourLine*>(pPickObj);//��ֵ��
	if(pContourLine)
	{
		pContourLine->SetSelected(false);
		pContourLine->ReDraw();
		m_pPickObj = NULL;							//���³�ʼ��ʰȡ����
		return ;
	}

	//���ֵ��
	CSymbolMultiContourLine* pMultiContourLine = dynamic_cast<CSymbolMultiContourLine*>(pPickObj);//��ֵ��
	if(pMultiContourLine)
	{
		pMultiContourLine->SetSelected(false);
		pMultiContourLine->ReDraw();
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
void CModifySymbol::DeleteSymbol(QRenderable *pPickObj)
{	
}

/************************************************************************/
/* �������ܣ� �ƶ�����						                            */
/* ����˵��:  QRenderable												*/
/*            scPoint���������������Ļ����							*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 04 �� 26 ��										*/
/************************************************************************/
void CModifySymbol::MoveSymbol(QRenderable *pPickObj,QVector3D &scPoint)
{
	QVector3D glPoint;
	int nIndex = -1;			//ѡ����������
	if(m_iFlag == 0)
	{
		m_pOpenglView->setCursor(Qt::ClosedHandCursor);
		m_iFlag  =1;
	}
	//���ŵ�����ƶ�
	SymbolPoint* pSp = dynamic_cast<SymbolPoint*>(pPickObj);
	if(pSp)
	{
		nIndex = pSp->GetPickIndex();
		QVector3DArray* pData = dynamic_cast<QVector3DArray*>(pSp->VertexArray());
		QVector3DArray* pOriginal = dynamic_cast<QVector3DArray*>(pSp->OriginalArray());
		ScreenToGL(scPoint, glPoint);
		//-----------undo----------
		//if(m_pUndoSymbolPointEdit && pData)
		//{
		//	m_pUndoSymbolPointEdit->SetRedoPostion(glPoint);
		//}
		//-----------end-----------
		pData->replace(nIndex, glPoint);
		pOriginal->replace(nIndex,TransGLtoWGS84(glPoint));
		return;
	}

	//20110510���--ͨ�÷��Ŷ����ƶ�
	NormalSymbol* pSymNormal = dynamic_cast<NormalSymbol*>(pPickObj);
	if(pSymNormal)
	{
		nIndex = pSymNormal->GetPickIndex();
		QVector3DArray* pData = dynamic_cast<QVector3DArray*>(pSymNormal->VertexArray());
		QVector3DArray* pOriginal = dynamic_cast<QVector3DArray*>(pSymNormal->OriginalArray());
		ScreenToGL(scPoint, glPoint);
		//-----------undo----------
		/*if(m_pUndoSymbolPointEdit && pData)
		{
			m_pUndoSymbolPointEdit->SetRedoPostion(glPoint);
		}*/
		//-----------end-----------
		pData->replace(nIndex, glPoint);
		pOriginal->replace(nIndex,TransGLtoWGS84(glPoint));
		return;
	}

	//�������ƶ�
	CSymbolLine *pSymLine = dynamic_cast<CSymbolLine*>(pPickObj);
	if(pSymLine)
	{
		QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pSymLine->VertexArray());
		QVector3DArray *pOrgData = dynamic_cast<QVector3DArray*>(pSymLine->OriginalArray());
		ScreenToGL(scPoint, glPoint);
		int nPos = pSymLine->GetPosition();
		if(nPos < 0) return;//û�л��ʰȡ���ĵ��λ��

		float fTransformedX, fTransformedY, fTempX, fTempY;
		fTransformedX = glPoint.x() - pData->at(nPos).x();
		fTransformedY = glPoint.y() - pData->at(nPos).y();

		fTempX = TransGLtoWGS84(glPoint).x() - pOrgData->at(nPos).x();
		fTempY = TransGLtoWGS84(glPoint).y() - pOrgData->at(nPos).y();

		//if(m_pUndoLineMoveEdit)
		//{
		//	m_pUndoLineMoveEdit->SetUndoPosition(QVector3D(fTransformedX, fTransformedY, 0));
		//	m_pUndoLineMoveEdit->SetUndoOriginalPos(QVector3D(fTempX, fTempY, 0));
		//}
		pSymLine->SetSelected(false);
		return;
	}

	//��������ƶ�
	CSymbolMultiLine *pSymMultiLine = dynamic_cast<CSymbolMultiLine*>(pPickObj);
	if(pSymMultiLine)
	{
		QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pSymMultiLine->VertexArray());
		QVector3DArray *pOrgData = dynamic_cast<QVector3DArray*>(pSymMultiLine->OriginalArray());
		ScreenToGL(scPoint, glPoint);
		int nPos = pSymMultiLine->GetPosition();
		if(nPos < 0) return;//û�л��ʰȡ���ĵ��λ��

		float fTransformedX, fTransformedY, fTempX, fTempY;
		fTransformedX = glPoint.x() - pData->at(nPos).x();
		fTransformedY = glPoint.y() - pData->at(nPos).y();

		fTempX = TransGLtoWGS84(glPoint).x() - pOrgData->at(nPos).x();
		fTempY = TransGLtoWGS84(glPoint).y() - pOrgData->at(nPos).y();

		//if(m_pUndoLineMoveEdit)
		//{
		//	m_pUndoLineMoveEdit->SetUndoPosition(QVector3D(fTransformedX, fTransformedY, 0));
		//	m_pUndoLineMoveEdit->SetUndoOriginalPos(QVector3D(fTempX, fTempY, 0));
		//}
		return;
	}
	
	//��ֵ��
	CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine*>(pPickObj);
	if(pContourLine)
	{
		QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pContourLine->VertexArray());
		QVector3DArray *pOrgData = dynamic_cast<QVector3DArray*>(pContourLine->OriginalArray());
		ScreenToGL(scPoint, glPoint);
		int nPos = pContourLine->GetPosition();
		if(nPos < 0) return;//û�л��ʰȡ���ĵ��λ��

		float fTransformedX, fTransformedY, fTempX, fTempY;
		fTransformedX = glPoint.x() - pData->at(nPos).x();
		fTransformedY = glPoint.y() - pData->at(nPos).y();

		fTempX = TransGLtoWGS84(glPoint).x() - pOrgData->at(nPos).x();
		fTempY = TransGLtoWGS84(glPoint).y() - pOrgData->at(nPos).y();

		//if(m_pUndoLineMoveEdit) 
		//{
		//	m_pUndoLineMoveEdit->SetUndoPosition(QVector3D(fTransformedX, fTransformedY, 0));
		//	m_pUndoLineMoveEdit->SetUndoOriginalPos(QVector3D(fTempX, fTempY, 0));
		//}
		pContourLine->SetSelected(false);
		return;
	}

	//���ֵ��
	CSymbolMultiContourLine *pMultiContourLine = dynamic_cast<CSymbolMultiContourLine*>(pPickObj);
	if(pMultiContourLine)
	{
		QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pMultiContourLine->VertexArray());
		QVector3DArray *pOrgData = dynamic_cast<QVector3DArray*>(pMultiContourLine->OriginalArray());
		ScreenToGL(scPoint, glPoint);
		int nPos = pMultiContourLine->GetPosition();
		if(nPos < 0) return;//û�л��ʰȡ���ĵ��λ��

		float fTransformedX, fTransformedY, fTempX, fTempY;
		fTransformedX = glPoint.x() - pData->at(nPos).x();
		fTransformedY = glPoint.y() - pData->at(nPos).y();

		fTempX = TransGLtoWGS84(glPoint).x() - pOrgData->at(nPos).x();
		fTempY = TransGLtoWGS84(glPoint).y() - pOrgData->at(nPos).y();

		//if(m_pUndoLineMoveEdit) 
		//{
		//	m_pUndoLineMoveEdit->SetUndoPosition(QVector3D(fTransformedX, fTransformedY, 0));
		//	m_pUndoLineMoveEdit->SetUndoOriginalPos(QVector3D(fTempX, fTempY, 0));
		//}
		pMultiContourLine->SetSelected(false);
		return;
	}

	//�����ƶ�--20110621���
//	QTextureFont *pText = dynamic_cast<QTextureFont *>(pPickObj);
//	if(pText)
//	{
//		ScreenToGL(scPoint, glPoint);
//		QVector3D Position = pText->GetPosition();
//
//		float fTransformedX, fTransformedY;
//// 		fTransformedX = glPoint.x() - Position.x();
//// 		fTransformedY = glPoint.y() - Position.y();
//		//if(m_pUndoLineMoveEdit) m_pUndoLineMoveEdit->SetUndoPosition(QVector3D(fTransformedX, fTransformedY, 0));
//
//		QVector3DArray *pTextPositions = pText->GetTextsPositions();
//		QVector3DArray *pOriginalArray = pText->OriginalArray();
//
//		//-----------undo----------
//		if(m_pundoSymbolMarkEdit && pTextPositions)
//		{
//			m_pundoSymbolMarkEdit->SetRedoPostion(glPoint);
//		}		
//
//		int iPos = pText->GetPickUpIndex();
//		pTextPositions->replace(iPos,glPoint);
//		pOriginalArray->replace(iPos,TransGLtoWGS84(glPoint));
//		
//		return;
//// 		for(int i=0; i<pTextPositions->size(); i++)
//// 		{
//// 			pTemp = pTextPositions->at(i);
//// 			
//// 		}
//		//pText->SetPosition(pTemp);
//		//m_ParentLayer->TransformLayer();
//		return;
//	}

	//�����ƶ�--20110621���
	QEText *pText1 = dynamic_cast<QEText *>(pPickObj);
	if(pText1)
	{
		ScreenToGL(scPoint, glPoint);
		QVector3D Position = pText1->GetPosition();

		float fTransformedX, fTransformedY;
		// 		fTransformedX = glPoint.x() - Position.x();
		// 		fTransformedY = glPoint.y() - Position.y();
		//if(m_pUndoLineMoveEdit) m_pUndoLineMoveEdit->SetUndoPosition(QVector3D(fTransformedX, fTransformedY, 0));

		QVector3DArray *pTextPositions = pText1->GetPositionArray();
		QVector3DArray *pOriginalArray = pText1->OriginalArray();

		//-----------undo----------
		//if(m_pundoSymbolMarkEdit && pTextPositions)
		//{
		//	m_pundoSymbolMarkEdit->SetRedoPostion(glPoint);
		//}		

		int iPos = pText1->GetPickUpIndex();
		pTextPositions->replace(iPos,glPoint);
		pOriginalArray->replace(iPos,TransGLtoWGS84(glPoint));

		return;
		// 		for(int i=0; i<pTextPositions->size(); i++)
		// 		{
		// 			pTemp = pTextPositions->at(i);
		// 			
		// 		}
		//pText->SetPosition(pTemp);
		//m_ParentLayer->TransformLayer();
		return;
	}
	//����--20110629
// 	Line *pTroughLine = dynamic_cast<Line*>(pPickObj);
// 	if(pTroughLine)
// 	{
// 		QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pTroughLine->VertexArray());
// 		ScreenToGL(scPoint, glPoint);
// 		int nPos = pTroughLine->GetPosition();
// 		if(nPos < 0) return;//û�л��ʰȡ���ĵ��λ��
// 
// 		float fTransformedX, fTransformedY;
// 		fTransformedX = glPoint.x() - pData->at(nPos).x();
// 		fTransformedY = glPoint.y() - pData->at(nPos).y();
// 
// 		if(m_pUndoLineMoveEdit) m_pUndoLineMoveEdit->SetUndoPosition(QVector3D(fTransformedX, fTransformedY, 0));
// 		return;
// 	}
}

/************************************************************************/
/* �������ܣ� �༭����---Ŀǰֻ�����ڵ�ֵ��						        */
/* ����˵��:  QRenderable												*/
/*            scPoint���������������Ļ����							*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 04 �� 26 ��										*/
/************************************************************************/
void CModifySymbol::EditSymbol(QRenderable *pPickObj, QVector3DArray *pPoints, int nPosStart, int nPosEnd)
{
	if (!pPickObj && !pPoints) return ;
	
	if(nPosStart == nPosEnd) return ;
	int nTemp;
	if(nPosEnd < nPosStart)
	{
		nTemp = nPosStart;
		nPosStart = nPosEnd;
		nPosEnd = nTemp;
	}

	//��ֵ��
	CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine*>(pPickObj);
	if(!pContourLine) return;

	QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pContourLine->VertexArray());
	QVector3DArray *pOriginalData = dynamic_cast<QVector3DArray*>(pContourLine->OriginalArray());
	if(!pData) return ;

	//�༭����
	pTempArray = new QVector3DArray;
	pTempOriginalArray = new QVector3DArray;
	int i;

	for(i=pData->size()-1; i>nPosStart; i--)	
	{
		if(i >= nPosEnd)
		{
			pTempArray->push_back(pData->at(i));//����浽��ʱ������
			pTempOriginalArray->push_back(pOriginalData->at(i));
		}
		pData->pop_back();
		pOriginalData->pop_back();
	}
	pData->pop_back();
	pOriginalData->pop_back();

	//�����±༭�ĵ�
	for(i=0; i<pPoints->size(); i++)
	{
		QVector3D temp = pPoints->at(i);
		pData->push_back(temp);		
		pOriginalData->push_back(TransGLtoWGS84(temp));
	}
	//����ԭ��ֵ�ߺ�벿�ֵĵ�
	for(i=pTempArray->size()-1; i>=0; i--)
	{
		pData->push_back(pTempArray->at(i));
		pOriginalData->push_back(pTempOriginalArray->at(i));
	}
	return;
}

