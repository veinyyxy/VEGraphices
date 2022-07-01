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
����˵���� ʰȡ�޸���
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

#include "ModifyContour.h"
//#include "UndoContourEditCommand.h"
#include <QtGui/QColor>
#include <QtWidgets/QMessageBox>
#include "CIPASMessageBox.h"

/************************************************************************/
/* �������ܣ� Ĭ�Ϲ��캯��						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
CModifyContour::CModifyContour(void)
{
	m_pPickObj = NULL;
	//m_pUndoSymbolPointEdit = NULL;
	//m_pUndoLineMoveEdit = NULL;
	//m_pUndoContourEdit = NULL;
	//m_pUndoContourEdit2 = NULL;
	//m_pUndoMultiContourEdit = NULL;
	m_bEditStart = false;				//Ĭ��û�п����༭
	m_pShowLine = new Line;				//�޸ĵ�ֵ��ʱ������ʾ
	pArray = NULL;
	pColor = NULL;
	m_pAttribute=NULL;
	m_bClose = true;
	m_currentZ = 0 ;					//��ʼ��Ϊ��
}

/************************************************************************/
/* �������ܣ� Ĭ����������						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
CModifyContour::~CModifyContour(void)
{
}

/************************************************************************/
/* �������ܣ� Ĭ�Ϲ��캯��						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CModifyContour::MouseEvent(QMouseEvent* event)
{

	//�ͷ�ǰ��ʰȡ�Ķ���
	if(!m_bEditStart)
	{
		ReleaseSymbol(m_pPickObj);
		//����ʰȡ����
		QVector3D screenPoint(event->x(),event->y(), 0);	
		screenPoint.setY(m_pOpenglView->height() - screenPoint.y());
		m_pPickObj = m_ParentLayer->PickUp(screenPoint);

		QVector3D scPoint(event->x(),event->y(), m_ParentLayer->LayerZ()), glPoint;
		ScreenToGL(scPoint, glPoint);
		SelectSymbol(m_pPickObj);
	}else   //�ռ��ƶ������еĵ�
	{
		QVector3D  glPoint, scPoint(event->x(), event->y(), 1.0f);
		ScreenToGL(scPoint, glPoint);

		m_PressPoints.push_back(glPoint);
		DrawControlLine();
		m_PressPoints.pop_back();
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
void CModifyContour::MouseDoubleClick(QMouseEvent *event)
{
	//����ʰȡ
	ReleaseSymbol(m_pPickObj);

	QVector3D screenPoint(event->x(),event->y(), 0);	
	screenPoint.setY(m_pOpenglView->height() - screenPoint.y());
	m_pPickObj = m_ParentLayer->PickUp(screenPoint);

	QVector3D scPoint(event->x(),event->y(), m_ParentLayer->LayerZ()), glPoint;
	ScreenToGL(scPoint, glPoint);
	SelectSymbol(m_pPickObj);

	if(m_pPickObj)
	{
		SelectSymbol(m_pPickObj);
		emit DelButtonStatus(true);

		//������ֵ�ߵ����Կ�
		CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine *>(m_pPickObj);
		CSymbolMultiContourLine *pMultiContourLine = dynamic_cast<CSymbolMultiContourLine *>(m_pPickObj);
		if(pContourLine)
		{
			InitAttribute(pContourLine->GetClosedLine());
			m_bClose = pContourLine->GetClosedLine();

			//����ʰȡ�����ߵ�����
			m_pAttribute = pContourLine->GetAttribute();
			SetContourAttrib(m_pAttribute);
			emit UpdateAttribute(this);
		}
		if (pMultiContourLine)
		{
			InitAttribute(pMultiContourLine->GetClosedLine());
			m_bClose = pMultiContourLine->GetClosedLine();

			//����ʰȡ�����ߵ�����
			m_pAttribute = pMultiContourLine->GetAttribute();
			SetContourAttrib(m_pAttribute);
			emit UpdateAttribute(this);

		}
	}
}

/************************************************************************/
/* �������ܣ� Ĭ�Ϲ��캯��						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CModifyContour::MousePress(QMouseEvent *event)
{
	if(NULL !=m_pPickObj)
	{
		QVector3D scPoint(event->x(),event->y(), m_ParentLayer->LayerZ()), glPoint;
		ScreenToGL(scPoint, glPoint);		
		m_lastScreen = scPoint;
		if(event->buttons() & Qt::LeftButton)				//��������༭״̬
		{
			if(!m_bEditStart)
			{
				m_PressPoints.Clear();
				CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine*>(m_pPickObj);
				CSymbolMultiContourLine *pMultiContourLine = dynamic_cast<CSymbolMultiContourLine*>(m_pPickObj);

				QVector3DArray *pData = NULL;

				//�����ĵ�ֵ�߹���
				if(pContourLine)
				{
					pData = dynamic_cast<QVector3DArray*>(pContourLine->VertexArray());
					int nPos = pContourLine->GetPosition();
					if(nPos<0 || nPos>(pData->size()-1)) return;//û��ʰȡ����
					nPosStart = nPos;
					m_bEditStart = true;
				}

				//�����ֺ��Ĵ�����߿��ͼ��
				if(pMultiContourLine)
				{
					pData = dynamic_cast<QVector3DArray*>(pMultiContourLine->VertexArray());
					int nPos = pMultiContourLine->GetPosition();
					if(nPos<0 || nPos>(pData->size()-1)) return;//û��ʰȡ����
					nPosStart = nPos;
					m_bEditStart = true;
				}

				//���ӵ�һ�εĿ��Ƶ�
				if(pData == NULL)
				{
					m_bEditStart = false;
					return ;
				}

				m_bPositive = m_DrawOrder.isConvert(pData,true);

				QVector3D temp1,temp2;
				if(nPosStart > 1 && pData != NULL)
				{
					temp1 = pData->at(nPosStart-2);
					temp2 = pData->at(nPosStart);
					temp1.setZ(0);
					temp2.setZ(0);
					nPosStart = nPosStart -2;
				}else if(nPosStart == 1 && pData != NULL)
				{
					temp1 = pData->at(pData->count()-1);
					temp2 = pData->at(nPosStart);
					temp1.setZ(0);
					temp2.setZ(0);
				}

				m_PressPoints.push_back(temp1);
				m_PressPoints.push_back(temp2);
				m_currentZ = pData->at(nPosStart).z() + 0.0001;
			}
			else 
			{
				m_PressPoints.push_back(glPoint);
			}	
            
			if( m_PressPoints.count() == 3)
			{
				bool bOrder = m_DrawOrder.isConvert(&m_PressPoints,false);
				if(bOrder != m_bPositive)
				{
					m_bEditStart = false;
					ClearControlLine();
					ClearCrossArray(m_PressPoints);
					QString strName1,strName2;

					if(bOrder)
					{
						strName2 = tr("��ʱ��");
					}else
					{
						strName2 = tr("˳ʱ��");
					}

					if(m_bPositive)
					{
						strName1 = tr("��ʱ��");
					}else
					{
						strName1 = tr("˳ʱ��");
					}

					QString strContext;
					strContext.append(tr("��ֵ��Ϊ"));
					strContext.append(strName1);
					strContext.append(",");
					strContext.append("�༭��Ϊ");
					strContext.append(strName2);
					strContext.append(",");
					strContext.append(tr("�뱣�ַ�����ͬ!"));
					CCIPASMessageBox::CIPASInformation(0, CCIPASMessageBox::CIPAS_OK,tr("����"),strContext);
					return;
				}
			}
			//���ƿ�����
			DrawControlLine();

		}
		else if(event->buttons() & Qt::RightButton)				//�����Ҽ����б༭
		{
			if(NULL != pArray && NULL != pColor)
			{
				pArray->Clear();
				pColor->Clear();
			}
			
			//�Ƴ�������
			ClearControlLine();

			if(!CheckDirection())
			{
				return;
			}
			//����ʰȡ��Ϊ�˵õ��������λ��
			QVector3D screenPoint(event->x(),event->y(), 0);	
			screenPoint.setY(m_pOpenglView->height() - screenPoint.y());
			m_lastScreen.setY(m_pOpenglView->height() - m_lastScreen.y());
			//����ʰȡǰ�����ͷ���ǰ��pickobj
			ReleaseSymbol(m_pPickObj);	
			m_pPickObj = m_ParentLayer->PickUp(m_lastScreen);
			m_bEditStart = false;				//�����Ƿ�ɹ�ʰȡ������Ϊfalse

			//�������һ�����Ƶ�
			m_PressPoints.pop_back();
			//���Ʊ༭����
			CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine*>(m_pPickObj);
			CSymbolMultiContourLine *pMultiContourLine = dynamic_cast<CSymbolMultiContourLine*>(m_pPickObj);
			if(pContourLine)
			{
				m_bEditStart = false;
				QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pContourLine->VertexArray());
				QVector3DArray *pDataOrigin = pContourLine->OriginalArray();
				int nPos = pContourLine->GetPosition();
				if(nPos<0 || nPos>(pData->size()-1)) 
				{	
					//û��ʰȡ���� ,�ͷ�ԭ����ʰȡ�Ķ���
					ReleaseSymbol(m_pPickObj);		
					return;
				}

				nPosEnd = nPos+2;
				
				QVector3D temp1,temp2;
				temp1 = pData->at(nPos);
				temp2 = pData->at(nPosEnd);
				temp1.setZ(0);
				temp2.setZ(0);
				m_PressPoints.push_back(temp1);
				m_PressPoints.push_back(temp2);
				m_SpLine.GetSmoothPoints(m_PressPoints, m_SmoothPoints);
				float tempz = pData->at(0).z();
				if(nPosStart>0 && nPosEnd>0)
				{
				if (EditSymbol(m_pPickObj, &m_SmoothPoints, nPosStart, nPosEnd))
				{
				//m_undoStack->push(m_pUndoContourEdit);
				}									
				//m_pUndoContourEdit = 0;
				}	
			}
			else if(pMultiContourLine)
			{
				m_bEditStart = false;
				QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pMultiContourLine->VertexArray());
				QVector3DArray *pDataOrigin = pMultiContourLine->OriginalArray();
				int nPos = pMultiContourLine->GetPosition();
				if(nPos<0 || nPos>(pData->size()-1))
				{	
					ReleaseSymbol(m_pPickObj);		
					return;
				}//û��ʰȡ����
				nPosEnd = nPos;
				m_PressPoints.push_back(pData->at(nPos));

				m_SpLine.GetSmoothPoints(m_PressPoints, m_SmoothPoints);
				float tempz = pData->at(0).z();
				
				if(nPosStart>0 && nPosEnd>0)
				{
					if (EditSymbol(m_pPickObj, &m_SmoothPoints, nPosStart, nPosEnd))
					{
						//m_undoStack->push(m_pUndoMultiContourEdit);
					}					
					//m_pUndoMultiContourEdit = 0;
				}				
			}
			ReleaseSymbol(m_pPickObj);				
		}
	}
}

/************************************************************************/
/* �������ܣ� �����Ƶı༭���Ƿ����ֵ�߷�����ͬ				        */
/* ����˵��:  NULL														*/
/*																		*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2012 �� 01 �� 11 ��										*/
/************************************************************************/
bool CModifyContour::CheckDirection()
{
	bool bOrder = m_DrawOrder.isConvert(&m_PressPoints,false);
	if(bOrder != m_bPositive)
	{
		m_bEditStart = false;
		ClearControlLine();
		ClearCrossArray(m_PressPoints);
		QString strName1,strName2;

		if(bOrder)
		{
			strName2 = tr("��ʱ��");
		}else
		{
			strName2 = tr("˳ʱ��");
		}

		if(m_bPositive)
		{
			strName1 = tr("��ʱ��");
		}else
		{
			strName1 = tr("˳ʱ��");
		}

		QString strContext;
		strContext.append(tr("��ֵ��Ϊ"));
		strContext.append(strName1);
		strContext.append(",");
		strContext.append("�༭��Ϊ");
		strContext.append(strName2);
		strContext.append(",");
		strContext.append(tr("�뱣�ַ�����ͬ!"));
		CCIPASMessageBox::CIPASInformation(0, CCIPASMessageBox::CIPAS_OK,tr("��ʾ"),strContext);

		return false;
	}
	return true;
}

/************************************************************************/
/* �������ܣ� ���Ʊ༭ʱ�Ŀ����߹���							        */
/* ����˵��:  NULL														*/
/*																		*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2012 �� 01 �� 11 ��										*/
/************************************************************************/
void CModifyContour::DrawControlLine()
{
	pArray = m_pShowLine->getVectexArray();
	pColor = m_pShowLine->getColorArray();		
	pArray->Clear();
	pColor->Clear();

	m_SpLine.GetSmoothPoints(m_PressPoints, m_SmoothPoints);

	DrawCrossArray(m_PressPoints);
	for(int i=0; i<m_SmoothPoints.size(); i++)
	{
		m_SmoothPoints[i].setZ(m_currentZ); 
		pArray->push_back(m_SmoothPoints[i]);
		pColor->push_back(QVector4D(1,0, 0, 1));
	}
	m_pShowLine->SetColorArray();
	if(m_PressPoints.size()>1)
		m_ParentLayer->GetLayerData()->InsertRenderable(m_pShowLine);
}

/************************************************************************/
/* �������ܣ� �Ƴ��༭ʱ�Ŀ����߹���							        */
/* ����˵��:  NULL														*/
/*																		*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2012 �� 01 �� 11 ��										*/
/************************************************************************/
void CModifyContour::ClearControlLine()
{
	if(m_pShowLine)
	{
		m_ParentLayer->GetLayerData()->RemoveRenderable(m_pShowLine);
		emit DelButtonStatus(false);
	}
	ClearCrossArray(m_PressPoints);
}

/************************************************************************/
/* �������ܣ� ���ƿ��Ƶ�										        */
/* ����˵��:  NULL														*/
/*																		*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2012 �� 01 �� 11 ��										*/
/************************************************************************/
void CModifyContour::DrawCrossArray(QVector3DArray&  InputData)
{
	if (InputData.count()>0)
	{
		QVector3D glPos1,glPos2,glPos3,glPos4;
		QVector3D srPos1,srPos2,srPos3,srPos4;
		int iCount = InputData.count() -1;
		Line* temp1 = new Line;
		Line* temp2 = new Line;
		srPos1 = QVector3D(m_lastScreen.x()-3.0f,m_lastScreen.y()-3.0f,m_currentZ) ;
		srPos2 = QVector3D(m_lastScreen.x()-3.0f,m_lastScreen.y()+3.0f,m_currentZ) ;
		srPos3 = QVector3D(m_lastScreen.x()+3.0f,m_lastScreen.y()-3.0f,m_currentZ) ;
		srPos4 = QVector3D(m_lastScreen.x()+3.0f,m_lastScreen.y()+3.0f,m_currentZ) ;

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
}

/************************************************************************/
/* �������ܣ� ������ƵĿ��Ƶ�										        */
/* ����˵��:  NULL														*/
/*																		*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2012 �� 01 �� 11 ��										*/
/************************************************************************/
void CModifyContour::ClearCrossArray(QVector3DArray&  InputData)
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
//
//bool CModifyContour::EditSymbol(QRenderable *pPickObj,QVector3DArray *pPoints, int nPosStart, int nPosEnd)
//{
//	if (!pPickObj && !pPoints) return false;
//	if(nPosStart == nPosEnd) return false;
//	int nTemp;
//
//	//��ֵ��
//	CSymbolMultiContourLine *pContourLine = dynamic_cast<CSymbolMultiContourLine*>(pPickObj);
//	if(!pContourLine) return false;
//
//	QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pContourLine->VertexArray());
//	QVector3DArray *pOriginalData = dynamic_cast<QVector3DArray*>(pContourLine->OriginalArray());
//
//	if (pData==NULL || pOriginalData==NULL)
//	{
//		return false;
//	}
//
//	float pDataZ,pOriginalDataZ;
//	pDataZ = pData->at(0).z();
//	pOriginalDataZ = pOriginalData->at(0).z();
//
//	//����undo������;
//
//	
//	if(!pData || pData->count() < 3 || pPoints->count() <3) return false ;     //�����Ƶĵ�ֵ������������ʱ���˳�
//	
//	//�༭����
//	QVector3DArray *pTempArray, *pTempOriginalArray;
//	pTempArray = new QVector3DArray;
//	pTempOriginalArray = new QVector3DArray;
//	int i;
//	
//	//����ȸ�������ʱ�����
//	if(m_bPositive == true) 
//	{
//		if(nPosStart < nPosEnd  )//ͬ����
//		{   
//			bool bModify = m_DrawOrder.isConvert(pPoints,false);
//			if(bModify == m_bPositive)  
//			{
//				for(i=pData->size()-1; i>nPosStart; i--)	
//				{
//					if(i >= nPosEnd)
//					{
//						pTempArray->push_back(pData->at(i));//����浽��ʱ������
//						pTempOriginalArray->push_back(pOriginalData->at(i));
//					}
//					pData->pop_back();
//					pOriginalData->pop_back();
//				}
//				pData->pop_back();
//				pOriginalData->pop_back();
//
//				//�����±༭�ĵ�
//				for(i=0; i<pPoints->size(); i++)
//				{
//					QVector3D temp = pPoints->at(i);
//					temp.setZ(pDataZ);
//					pData->push_back(temp);		
//					pOriginalData->push_back(TransGLtoWGS84(temp));
//				}
//
//				//����ԭ��ֵ�ߺ�벿�ֵĵ�
//				for(i=pTempArray->size()-2; i>=0; i--)
//				{   
//					QVector3D temp =pTempOriginalArray->at(i);
//					temp.setZ(pOriginalDataZ);
//					pData->push_back(pTempArray->at(i));
//					pOriginalData->push_back(temp);
//				}
//			} 
//			else
//			{
//				//������0�㵽nposEnd�ĵ�
//				for(i = pData->count()-1 ; i>nPosEnd; i--)
//				{
//					pData->pop_back();
//					pOriginalData->pop_back();
//				}
//
//				//������nposstart֮��ĵ�
//				for(i = 0 ; i <nPosStart; i++)
//				{
//					pData->pop_front();
//					pOriginalData->pop_front();
//				}
//
//				//����������
//				for(i=0; i< pPoints->count()-1; i++)
//				{
//					pData ->push_front(pPoints->at(i));
//					QVector3D temp  = pPoints->at(i);
//					temp.setZ(pDataZ);
//					pOriginalData->push_front(TransGLtoWGS84(temp));					
//				}
//			}
//		}
//		else if (nPosStart > nPosEnd)    //�����෴
//		{
//		  bool bModify = m_DrawOrder.isConvert(pPoints,false);
//
//			if(bModify == m_bPositive)     //��ʼ���ڽ�����ǰ��
//			{
//				//������0�㵽nposEnd�ĵ�
//				for(i = 0 ; i< nPosEnd; i++)
//				{
//					pData->pop_front();
//					pOriginalData->pop_front();
//				}
//
//				//������nposstart֮��ĵ�
//				for(i = pData->count()-1 ; i >(nPosStart - nPosEnd); i--)
//				{
//					pData->pop_back();
//					pOriginalData->pop_back();
//				}
//
//				//����������
//				for(i = 0; i< pPoints->count(); i++)
//				{
//					pData ->push_back(pPoints->at(i));
//					QVector3D temp  = pPoints->at(i);
//					temp.setZ(pDataZ);
//					pOriginalData->push_back(TransGLtoWGS84(temp));
//				}
//			}
//			else
//			{
//				for(i=pData->size()-1; i>nPosEnd; i--)	
//				{
//					if(i >= nPosStart)
//					{
//						pTempArray->push_back(pData->at(i));//����浽��ʱ������
//						pTempOriginalArray->push_back(pOriginalData->at(i));
//					}
//					pData->pop_back();
//					pOriginalData->pop_back();
//				}
//				pData->pop_back();
//				pOriginalData->pop_back();
//				
//				//�����±༭�ĵ�
//				for(i=pPoints->size()-1; i>0; i--)
//				{
//					QVector3D temp = pPoints->at(i);
//					temp.setZ(pDataZ);
//					pData->push_back(temp);		
//					pOriginalData->push_back(TransGLtoWGS84(temp));
//				}
//				
//				//����ԭ��ֵ�ߺ�벿�ֵĵ�
//				for(i=pTempArray->size()-2; i>=0; i--)
//				{
//					pData->push_back(pTempArray->at(i));
//					QVector3D temp = pTempOriginalArray->at(i);
//					temp.setZ(pOriginalDataZ);
//					pOriginalData->push_back(temp);
//				}
//			}
//		}
//	}
//    else if(m_bPositive == false)  //�����ֵ����˳ʱ�����
//	{
//		if(nPosStart < nPosEnd  )//ͬ����
//		{   
//			bool bModify = m_DrawOrder.isConvert(pPoints,false);
//
//			if(bModify == m_bPositive)  
//			{
//				for(i=pData->size()-1; i>nPosStart; i--)	
//				{
//					if(i >= nPosEnd)
//					{
//						pTempArray->push_back(pData->at(i));//����浽��ʱ������
//						pTempOriginalArray->push_back(pOriginalData->at(i));
//					}
//					pData->pop_back();
//					pOriginalData->pop_back();
//				}
//				pData->pop_back();
//				pOriginalData->pop_back();
//				
//				//�����±༭�ĵ�
//				for(i=0; i<pPoints->size(); i++)
//				{
//					QVector3D temp = pPoints->at(i);
//					temp.setZ(pDataZ);
//					pData->push_back(temp);		
//					pOriginalData->push_back(TransGLtoWGS84(temp));
//				}
//
//				//����ԭ��ֵ�ߺ�벿�ֵĵ�
//				for(i=pTempArray->size()-2; i>=0; i--)
//				{
//					QVector3D temp = pTempOriginalArray->at(i);
//					pData->push_back(pTempArray->at(i));
//					pOriginalData->push_back(temp);
//				}
//			}
//			else
//			{
//				//������0�㵽nposEnd�ĵ�
//				for(i = pData->count()-1 ; i>nPosEnd; i--)
//				{
//					pData->pop_back();
//					pOriginalData->pop_back();
//				}
//
//				//������nposstart֮��ĵ�
//				for(i = 0 ; i <nPosStart; i++)
//				{
//					pData->pop_front();
//					pOriginalData->pop_front();
//				}
//
//				//����������
//				for(i=0; i< pPoints->count()-1; i++)
//				{
//					pData ->push_front(pPoints->at(i));
//					QVector3D temp  = pPoints->at(i);
//					temp.setZ(pDataZ);
//					pOriginalData->push_front(TransGLtoWGS84(temp));
//				}
//			}
//		}
//		else if (nPosStart > nPosEnd)    //�����෴
//		{
//			bool bModify = m_DrawOrder.isConvert(pPoints,false);
//
//			if(bModify == m_bPositive)     //��ʼ���ڽ�����ǰ��
//			{
//				//������0�㵽nposEnd�ĵ�
//				for(i = 0 ; i< nPosEnd; i++)
//				{
//					pData->pop_front();
//					pOriginalData->pop_front();
//				}
//
//				//������nposstart֮��ĵ�
//				for(i = pData->count()-1 ; i >(nPosStart - nPosEnd); i--)
//				{
//					pData->pop_back();
//					pOriginalData->pop_back();
//				}
//
//				//����������
//				for(i = 0; i< pPoints->count(); i++)
//				{
//					pData ->push_back(pPoints->at(i));
//					QVector3D temp  = pPoints->at(i);
//					temp.setZ(pDataZ);
//					pOriginalData->push_back(TransGLtoWGS84(temp));
//				}
//			}
//			else
//			{
//				for(i=pData->size()-1; i>nPosEnd; i--)	
//				{
//					if(i >= nPosStart)
//					{
//						pTempArray->push_back(pData->at(i));//����浽��ʱ������
//						pTempOriginalArray->push_back(pOriginalData->at(i));
//					}
//					pData->pop_back();
//					pOriginalData->pop_back();
//				}
//				pData->pop_back();
//				pOriginalData->pop_back();
//
//				//�����±༭�ĵ�
//				for(i=pPoints->size()-1; i>0; i--)
//				{
//					QVector3D temp = pPoints->at(i);
//					temp.setZ(pDataZ);
//					pData->push_back(temp);		
//					pOriginalData->push_back(TransGLtoWGS84(temp));
//				}
//				
//				//����ԭ��ֵ�ߺ�벿�ֵĵ�
//				for(i=pTempArray->size()-2; i>=0; i--)
//				{
//					pData->push_back(pTempArray->at(i));
//					QVector3D temp = pTempOriginalArray->at(i);
//					temp.setZ(pOriginalDataZ);
//					pOriginalData->push_back(temp);
//				}
//			}
//		}
//	}
//
//	if (m_lineanalysis.isPolygonintersect(*pData))
//	{
//	
//		CCIPASMessageBox::CIPASInformation(0, CCIPASMessageBox::CIPAS_OK,tr("��ʾ"),tr("�ü��������г����߶��ཻ,�����²ü���"));
//		*pData = *(pUndoContourEdit->GetLinexVertex(CUndoMultiContourEditCommand::OldLine));
//		*pOriginalData = *(pUndoContourEdit->GetLinexVertex(CUndoMultiContourEditCommand::OldOriginalLine));
//		return false;
//	}	
//
//	if(pUndoContourEdit)
//	{
//		pUndoContourEdit->SetLineVertex(pData, CUndoMultiContourEditCommand::NewLine);
//		pUndoContourEdit->SetLineVertex(pOriginalData, CUndoMultiContourEditCommand::NewOriginalLine);
//	}	
//
//	pContourLine->ReDraw();
//	return true;
//}
/************************************************************************/
/* �������ܣ� �༭����---Ŀǰֻ�����ڵ�ֵ��						        */
/* ����˵��:  QRenderable												*/
/*            scPoint���������������Ļ����							*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 04 �� 26 ��										*/
/************************************************************************/
bool CModifyContour::EditSymbol(QRenderable *pPickObj, QVector3DArray *pPoints, int nPosStart, int nPosEnd)
{
	if (!pPickObj && !pPoints) return false ;
	if(nPosStart == nPosEnd) return false;
	int nTemp;

	//��ֵ��
	CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine*>(pPickObj);
	if(!pContourLine) return false;

	QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pContourLine->VertexArray());
	QVector3DArray *pOriginalData = dynamic_cast<QVector3DArray*>(pContourLine->OriginalArray());
	
	//����undo������;
	//if(pUndoContourEdit)
	//{
	//	pUndoContourEdit->SetLineVertex(pData, CUndoContourEditCommand::OldLine);
	//	pUndoContourEdit->SetLineVertex(pOriginalData, CUndoContourEditCommand::OldOriginalLine);
	//}
	//
	if(!pData || pData->count() < 3 || pPoints->count() <3) return false ;     //�����Ƶĵ�ֵ������������ʱ���˳�

	//�༭����
	QVector3DArray *pTempArray, *pTempOriginalArray;
	pTempArray = new QVector3DArray;
	pTempOriginalArray = new QVector3DArray;
	int i;

	//����ȸ�������ʱ�����
	if(m_bPositive == true) 
	{
		if(nPosStart < nPosEnd  )//ͬ����
		{   
			bool bModify = m_DrawOrder.isConvert(pPoints,false);
			if(bModify == m_bPositive)  
			{
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
				for(i=pTempArray->size()-2; i>=0; i--)
				{   
					QVector3D temp =pTempOriginalArray->at(i);
					pData->push_back(pTempArray->at(i));
				//	pOriginalData->push_back(TransGLtoWGS84(temp));
					pOriginalData->push_back(temp);
				}
			} 
			else
			{
				//������0�㵽nposEnd�ĵ�
				for(i = pData->count()-1 ; i>nPosEnd; i--)
				{
					pData->pop_back();
					pOriginalData->pop_back();
				}

				//������nposstart֮��ĵ�
				for(i = 0 ; i <nPosStart; i++)
				{
					pData->pop_front();
					pOriginalData->pop_front();
				}

				//����������
				for(i=0; i< pPoints->count()-1; i++)
				{
					pData ->push_front(pPoints->at(i));
					QVector3D temp  = pPoints->at(i);
					pOriginalData->push_front(TransGLtoWGS84(temp));
				}
			}
		}
		else if (nPosStart > nPosEnd)    //�����෴
		{
		  bool bModify = m_DrawOrder.isConvert(pPoints,false);

			if(bModify == m_bPositive)     //��ʼ���ڽ�����ǰ��
			{
				//������0�㵽nposEnd�ĵ�
				for(i = 0 ; i< nPosEnd; i++)
				{
					pData->pop_front();
					pOriginalData->pop_front();
				}

				//������nposstart֮��ĵ�
				for(i = pData->count()-1 ; i >(nPosStart - nPosEnd); i--)
				{
					pData->pop_back();
					pOriginalData->pop_back();
				}

				//����������
				for(i = 0; i< pPoints->count(); i++)
				{
					pData ->push_back(pPoints->at(i));
					QVector3D temp  = pPoints->at(i);
					pOriginalData->push_back(TransGLtoWGS84(temp));
				}
			}
			else
			{
				for(i=pData->size()-1; i>nPosEnd; i--)	
				{
					if(i >= nPosStart)
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
				for(i=pPoints->size()-1; i>0; i--)
				{
					QVector3D temp = pPoints->at(i);
					pData->push_back(temp);		
					pOriginalData->push_back(TransGLtoWGS84(temp));
				}
				
				//����ԭ��ֵ�ߺ�벿�ֵĵ�
				for(i=pTempArray->size()-2; i>=0; i--)
				{
					pData->push_back(pTempArray->at(i));
					QVector3D temp = pTempOriginalArray->at(i);
				//	pOriginalData->push_back(TransGLtoWGS84(temp));
					pOriginalData->push_back(temp);
				}
			}
		}
	}
    else if(m_bPositive == false)  //�����ֵ����˳ʱ�����
	{
		if(nPosStart < nPosEnd  )//ͬ����
		{   
			bool bModify = m_DrawOrder.isConvert(pPoints,false);

			if(bModify == m_bPositive)  
			{
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
				for(i=pTempArray->size()-2; i>=0; i--)
				{
					QVector3D temp = pTempOriginalArray->at(i);
					pData->push_back(pTempArray->at(i));
					pOriginalData->push_back(temp);
				}
			}
			else
			{
				//������0�㵽nposEnd�ĵ�
				for(i = pData->count()-1 ; i>nPosEnd; i--)
				{
					pData->pop_back();
					pOriginalData->pop_back();
				}

				//������nposstart֮��ĵ�
				for(i = 0 ; i <nPosStart; i++)
				{
					pData->pop_front();
					pOriginalData->pop_front();
				}

				//����������
				for(i=0; i< pPoints->count()-1; i++)
				{
					pData ->push_front(pPoints->at(i));
					QVector3D temp  = pPoints->at(i);
					pOriginalData->push_front(TransGLtoWGS84(temp));
				}
			}
		}
		else if (nPosStart > nPosEnd)    //�����෴
		{
			bool bModify = m_DrawOrder.isConvert(pPoints,false);

			if(bModify == m_bPositive)     //��ʼ���ڽ�����ǰ��
			{
				//������0�㵽nposEnd�ĵ�
				for(i = 0 ; i< nPosEnd; i++)
				{
					pData->pop_front();
					pOriginalData->pop_front();
				}

				//������nposstart֮��ĵ�
				for(i = pData->count()-1 ; i >(nPosStart - nPosEnd); i--)
				{
					pData->pop_back();
					pOriginalData->pop_back();
				}

				//����������
				for(i = 0; i< pPoints->count(); i++)
				{
					pData ->push_back(pPoints->at(i));
					QVector3D temp  = pPoints->at(i);
					pOriginalData->push_back(TransGLtoWGS84(temp));
				}
			}
			else
			{
				for(i=pData->size()-1; i>nPosEnd; i--)	
				{
					if(i >= nPosStart)
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
				for(i=pPoints->size()-1; i>0; i--)
				{
					QVector3D temp = pPoints->at(i);
					pData->push_back(temp);		
					pOriginalData->push_back(TransGLtoWGS84(temp));
				}
				
				//����ԭ��ֵ�ߺ�벿�ֵĵ�
				for(i=pTempArray->size()-2; i>=0; i--)
				{
					pData->push_back(pTempArray->at(i));
					QVector3D temp = pTempOriginalArray->at(i);
				//	pOriginalData->push_back(TransGLtoWGS84(temp));
					pOriginalData->push_back(temp);
				}
			}
		}
	}

	if (m_lineanalysis.isPolygonintersect(*pData))
	{
		QMessageBox msg;
		msg.setText(tr("�ü��������г����߶��ཻ,�����²ü���"));
		msg.exec();	
		//*pData = *(pUndoContourEdit->GetLinexVertex(CUndoContourEditCommand::OldLine));
		//*pOriginalData = *(pUndoContourEdit->GetLinexVertex(CUndoContourEditCommand::OldOriginalLine));
		return false;
	}

	//if(pUndoContourEdit)
	//{
	//	pUndoContourEdit->SetLineVertex(pData, CUndoContourEditCommand::NewLine);
	//	pUndoContourEdit->SetLineVertex(pOriginalData, CUndoContourEditCommand::NewOriginalLine);
	//}	

	pContourLine->ReDraw();
	return true;
}
/************************************************************************/
/* �������ܣ� Ĭ�Ϲ��캯��						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CModifyContour::MouseRelease(QMouseEvent *event)
{
	return;
}

/************************************************************************/
/* �������ܣ� ����뿪�¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 06 �� 29 ��										*/
/************************************************************************/
void CModifyContour::LeaveEvent()
{
	return;
}

/************************************************************************/
/* �������ܣ� Ĭ�Ϲ��캯��						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CModifyContour::setILayerAttrib(ILayerAttribute *Ilayer)
{
	if(NULL != m_pPickObj&&m_bEditStart)
	{
		CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine *>(m_pPickObj);
		CSymbolMultiContourLine *pMultiContourLine = dynamic_cast<CSymbolMultiContourLine *>(m_pPickObj);
		
		//���������Ⱑ�������޸�ֵʱ�� �õ���line �ǿյ�
		if(pContourLine)
		{
			m_pAttribute = pContourLine->GetAttribute();

			if(Ilayer != NULL && m_pAttribute != NULL) 
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
				}else if(strName == QString("cFillColor"))
				{
					QColor tColor = Ilayer->Value().value<QColor>();
					m_pAttribute->SetFillColor(QVector4D(tColor.redF(), tColor.greenF(), tColor.blueF(),tColor.alphaF()));
				}
			}
			m_pAttribute = NULL;
			pContourLine->Render();
			ReleaseSymbol(m_pPickObj);
			m_bEditStart = false;
			emit DelButtonStatus(false);
		}		
		else if(pMultiContourLine)
		{
			m_pAttribute = pMultiContourLine->GetAttribute();

			if(Ilayer != NULL && m_pAttribute != NULL) 
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
				}else if(strName == QString("cFillColor"))
				{
					QColor tColor = Ilayer->Value().value<QColor>();
					m_pAttribute->SetFillColor(QVector4D(tColor.redF(), tColor.greenF(), tColor.blueF(),tColor.alphaF()));
				}
			}
			m_pAttribute = NULL;
			pMultiContourLine->Render();
			ReleaseSymbol(m_pPickObj);
			m_bEditStart = false;
			emit DelButtonStatus(false);
		}		
	}
}

/************************************************************************/
/* �������ܣ� ��������ֵ                    						    */
/* ����˵��:  NULL														*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 05 �� 10 ��										*/
/************************************************************************/
void CModifyContour::SetContourAttrib(CSymbolAttribute *pAttribute)
{
	m_layerAttribute->clear();
	QStringList strValue;

	const int nShowValue =(int) pAttribute->GetValue();
	ILayerAttribute *cValue = new ILayerAttribute("cValue","cValue",tr("��ֵ����ֵ"),nShowValue,QVariant::Double,-10000.00000,10000.00000,strValue);
	m_layerAttribute->insert(QString("cValue"),cValue);

	QStringList strKinds;
	strKinds.append(QString("ʵ��"));
	strKinds.append(QString("�̻���"));
	strKinds.append(QString("����"));
	QString strLineType = m_pAttribute->GetLineType();
	int nLineType = strLineType.toInt();

	ILayerAttribute *cKinds = new ILayerAttribute("cKinds","cKinds",tr("��ֵ������"),nLineType,QVariant::StringList,0,0,strKinds);
	m_layerAttribute->insert(QString("cKinds"),cKinds);

	QVector4D tempColor = pAttribute->GetColor();
	tempColor.setX(tempColor.x()*255);
	tempColor.setY(tempColor.y()*255);
	tempColor.setZ(tempColor.z()*255);
	tempColor.setW(tempColor.w());
	QColor temp(tempColor.x(),tempColor.y(),tempColor.z(),tempColor.w() );
	QStringList strColor;
	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor",tr("��ֵ����ɫ"),temp,QVariant::Color,0,0,strColor);
	m_layerAttribute->insert(QString("cColor"),cColor);

	QStringList strWidth;
	for (int i=1; i<=10; i++)
	{
		strWidth.append(QString::number(i));
	}
	ILayerAttribute *cWidth = new ILayerAttribute("cWidth","cWidth",tr("��ֵ���߿�"),pAttribute->GetLineWidth(),QVariant::Int,1,10,strWidth);
	m_layerAttribute->insert(QString("cWidth"), cWidth);

	if(m_bClose)
	{
		QStringList strFill;
		strFill.append(tr("��"));
		strFill.append(tr("��"));
		ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill",tr("��ֵ���Ƿ����"),pAttribute->GetFillType(),QVariant::Bool,0,0,strFill);
		m_layerAttribute->insert(QString("cFill"),cFill);

		QVector4D tempColor = pAttribute->GetFillColor();
		tempColor.setX(tempColor.x()*255);
		tempColor.setY(tempColor.y()*255);
		tempColor.setZ(tempColor.z()*255);
		tempColor.setW(tempColor.w()*255);
		QColor temp(tempColor.x(),tempColor.y(),tempColor.z(),tempColor.w() );
		QStringList strColor;
		ILayerAttribute *cFillColor = new ILayerAttribute("cFillColor","cFillColor",tr("�����ɫ"),temp,QVariant::Color,0,0,strColor);
		m_layerAttribute->insert(QString("cFillColor"), cFillColor);
	}
	

}

/************************************************************************/
/* �������ܣ� ѡ�з���						                            */
/* ����˵��:  QRenderable												*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 04 �� 26 ��										*/
/************************************************************************/
void CModifyContour::SelectSymbol(QRenderable *pPickObj)
{
	CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine *>(pPickObj);//��ֵ��
	CSymbolMultiContourLine *pContourMultiLine = dynamic_cast<CSymbolMultiContourLine *>(pPickObj);//��ֵ��
	if(pContourLine)
	{
           pContourLine->SetSelected(true);
		   pContourLine->ReDraw();

		  //----------udno����-----------------
		  /*m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
		  m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
		  m_pUndoLineMoveEdit->SetRenderUnit(pContourLine);*/
	/*	  m_pUndoContourEdit = new CUndoContourEditCommand;
		  m_pUndoContourEdit->SetUndoGLWidget(m_pOpenglView);
		  m_pUndoContourEdit->SetUndoRenderable(pContourLine);
		  m_pUndoContourEdit->SetContour(pContourLine);*/
		  //--------------end------------------
		return;		
	}
	else if(pContourMultiLine)
	{
           pContourMultiLine->SetSelected(true);
		   pContourMultiLine->ReDraw();

		  //----------udno����-----------------
		  /*m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
		  m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
		  m_pUndoLineMoveEdit->SetRenderUnit(pContourLine);*/
		  //m_pUndoMultiContourEdit = new CUndoMultiContourEditCommand;
		  //m_pUndoMultiContourEdit->SetUndoGLWidget(m_pOpenglView);
		  //m_pUndoMultiContourEdit->SetUndoRenderable(pContourMultiLine);
		  //m_pUndoMultiContourEdit->SetContour(pContourMultiLine);
		  //--------------end------------------
		return;		
	}
}

/************************************************************************/
/* �������ܣ� �ͷ�ѡ�еķ���						                    */
/* ����˵��:  QRenderable												*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 05 �� 10 ��										*/
/************************************************************************/
void CModifyContour::ReleaseSymbol(QRenderable *pPickObj)
{
	CSymbolContourLine* pContourLine = dynamic_cast<CSymbolContourLine*>(pPickObj);
	CSymbolMultiContourLine* pMultiContourLine = dynamic_cast<CSymbolMultiContourLine*>(pPickObj);
	if(pContourLine)
	{
		pContourLine->SetSelected(false);
		pContourLine->ReDraw();
		m_pPickObj = NULL;				  //���³�ʼ��ʰȡ����
		return ;
	}
	if(pMultiContourLine)
	{
		pMultiContourLine->SetSelected(false);
		pMultiContourLine->ReDraw();
		m_pPickObj = NULL;				  //���³�ʼ��ʰȡ����
		return ;
	}	
}
