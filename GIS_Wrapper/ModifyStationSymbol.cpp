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

#include "ModifyStationSymbol.h"
#include "SymbolPoint.h"
#include "SymbolLine.h"
#include "SymbolContourLine.h"
#include "NormalSymbol.h"
#include <QtGui/QCursor>
#include <QDockWidget>
//#include "QTextureFont.h"

/************************************************************************/
/* �������ܣ� Ĭ�Ϲ��캯��						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
CModifyStationSymbol::CModifyStationSymbol(void):m_pPickObj(NULL),m_bCanMove(false)//, m_pUndoSymbolPointEdit(0), m_pUndoLineMoveEdit(0),m_pundoSymbolMarkEdit(0)
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
CModifyStationSymbol::~CModifyStationSymbol(void)
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
void CModifyStationSymbol::MouseEvent(QMouseEvent* event) 
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
void CModifyStationSymbol::MousePress(QMouseEvent *event)
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
	
}

/************************************************************************/
/* �������ܣ� ̧������¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CModifyStationSymbol::MouseRelease(QMouseEvent *event)
{
	/*	m_pOpenglView->setCursor(Qt::ArrowCursor);	//���ָ�ԭʼ״̬
		m_bCanMove = false;							//�ƶ���־��Ϊfalse
		m_iFlag  =0;

		//-----------------undo-------------------
		if(m_pUndoSymbolPointEdit)
		{
			if(m_pUndoSymbolPointEdit->GetRedoPostion() == QVector3D(0,0,0))
			{
				delete m_pUndoSymbolPointEdit;
			}
			else
			{
				m_undoStack->push(m_pUndoSymbolPointEdit);
			}
			m_pUndoSymbolPointEdit = 0;
		}
		if(m_pUndoLineMoveEdit)
		{
			if(m_pUndoLineMoveEdit->GetUndoPosition() == QVector3D(0,0,0))
			{
				delete m_pUndoLineMoveEdit;
			}
			else
			{
				m_undoStack->push(m_pUndoLineMoveEdit);
			}
			m_pUndoLineMoveEdit = 0;
		}

		if(m_pundoSymbolMarkEdit)
		{
			if(m_pundoSymbolMarkEdit->GetRedoPostion() == QVector3D(0,0,0))
			{
				delete m_pundoSymbolMarkEdit;
			}
			else
			{
				m_undoStack->push(m_pundoSymbolMarkEdit);
			}
			m_pundoSymbolMarkEdit = 0;
		}
		//----------------------------------------
		*/
}

/************************************************************************/
/* �������ܣ� ����뿪�¼�  					                        */
/* ����˵��:  NULL														*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 06 �� 29 ��										*/
/************************************************************************/
void CModifyStationSymbol::LeaveEvent()
{
}


/************************************************************************/
/* �������ܣ� ���˫���¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CModifyStationSymbol::keyPressEvent(QKeyEvent * event )
{

/*	if(NULL != m_pPickObj)			//��ʾ�з��ű�ѡ��
	{
		if((event->key() & Qt::Key_Delete) || (event->key() & Qt::Key_Backspace))
		{
			m_iFlag = 0;
			m_bEdit = false;
			DeleteSymbol(m_pPickObj);
			emit DelButtonStatus(false);
		}
	}
	*/
}

/************************************************************************/
/* �������ܣ� ���˫���¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/* �޸�����:  renxue													*/
/* �޸�����:  2011 �� 04 �� 26 ��										*/
/************************************************************************/
void CModifyStationSymbol::MouseDoubleClick(QMouseEvent *event)
{
	/*QVector3D screenPoint(event->x(),event->y(), 0);
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
	*/
}

/************************************************************************/
/* �������ܣ� ���ö�Ӧ�ķ���					                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CModifyStationSymbol::SetSymbol(Symbol *pSymbol)
{

}

/************************************************************************/
/* �������ܣ� ��������ֵ					                            */
/* ����˵��:  NULL														*/
/* ��    ��:  renxue														*/
/* ��    ��:  2011 �� 04 �� 29 ��										*/
/************************************************************************/
void CModifyStationSymbol::setILayerAttrib(ILayerAttribute *Ilayer)
{
	/*
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
				m_pAttribute->SetColor(QVector3D(tColor.red()/255.0, tColor.green()/255.0, tColor.blue()/255.0));
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
	*/
}

/************************************************************************/
/* �������ܣ� ��������ֵ--Ŀǰֻ�����ڵ�ֵ��						    */
/* ����˵��:  NULL														*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 05 �� 10 ��										*/
/************************************************************************/
void CModifyStationSymbol::SetContourAttrib(CSymbolAttribute *pAttribute)
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

void CModifyStationSymbol::SelectSymbol(QRenderable *pPickObj)
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
	//	pSp->SetSelected(true);
		m_stationvalueArray = pSp->StationValueArray();
	//	int countNum = m_stationvalueArray->count();

		if (m_stationvalueArray->count()>0)
		{
		//	StationValues temp = m_stationvalueArray->at(pSp->GetPickIndex());
		//	emit IdentiferEnd(&temp);
			emit IdentiferEnd(pSp,pSp->GetPickIndex());
		}
		
		/*
		//isVaule = true;
		//----------udno����-----------------
		int iI = pSp->GetPickIndex();
		QVector3DArray* pData = dynamic_cast<QVector3DArray*>(pSp->VertexArray());
		m_pUndoSymbolPointEdit = new CUndoSymbolPointMoveCommand;
		m_pUndoSymbolPointEdit->SetEditSymbolPoint(pSp);
		m_pUndoSymbolPointEdit->SetIndex(iI);
		m_pUndoSymbolPointEdit->SetGLWidget(m_pOpenglView);
		if(pData)
		{
			m_pUndoSymbolPointEdit->SetUndoPostion(pData->at(iI));
		}
		//--------------end--------------------
		return ;
		*/
	}
}

void CModifyStationSymbol::ReleaseSymbol(QRenderable *pPickObj)
{

}

void CModifyStationSymbol::DeleteSymbol(QRenderable *pPickObj)
{	
}

void CModifyStationSymbol::MoveSymbol(QRenderable *pPickObj,QVector3D &scPoint)
{
	
}

void CModifyStationSymbol::EditSymbol(QRenderable *pPickObj, QVector3DArray *pPoints, int nPosStart, int nPosEnd)
{
	
}



