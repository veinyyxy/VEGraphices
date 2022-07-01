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
�ļ�����DelSymbol.h DelSymbol.cpp 
�� ���� CDelSymbol
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

#include "DelSymbol.h"
#include "SymbolPoint.h"
#include "SymbolLine.h"
#include "SymbolContourLine.h"
#include "SymbolMultiContourLine.h"
#include "NormalSymbol.h"
#include <QtGui/QCursor>
#include <QtWidgets/QDockWidget>
//#include "..//QE_lib/QTextureFont.h"
#include "QEText.h"
/************************************************************************/
/* �������ܣ� Ĭ�Ϲ��캯��						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
CDelSymbol::CDelSymbol(void)//:m_pUndoSymbolPointEdit(0), m_pUndoLineMoveEdit(0)
{
	m_pPickObj = NULL;
}

/************************************************************************/
/* �������ܣ� Ĭ����������						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
CDelSymbol::~CDelSymbol(void)
{
}

/************************************************************************/
/* �������ܣ� �ƶ�����¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/* �޸�����:  renxue													*/
/* �޸�����:  2011 �� 06 �� 22 ��										*/
/* �޸�����:  wuenp											     		*/
/* �޸�����:  2011 �� 06 �� 10 ��										*/
/************************************************************************/
void CDelSymbol::MouseEvent(QMouseEvent* event) 
{
	if(m_pPickObj) 
		ReleaseSymbol(m_pPickObj);
	else
	{
		//����ʰȡ����
		QVector3D screenPoint(event->x(),event->y(), 0);	
		screenPoint.setY(m_pOpenglView->height() - screenPoint.y());
		m_pPickObj = m_ParentLayer->PickUp(screenPoint);

		QVector3D scPoint(event->x(),event->y(), m_ParentLayer->LayerZ()), glPoint;
		ScreenToGL(scPoint, glPoint);
		SelectSymbol(m_pPickObj);
	}	
}

/************************************************************************/
/* �������ܣ� ��������¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/* �޸�����:  renxue													*/
/* �޸�����:  2011 �� 06 �� 22 ��										*/
/* �޸�����:  wuenp												    	*/
/* �޸�����:  2011 �� 06 �� 10 ��										*/
/************************************************************************/
void CDelSymbol::MousePress(QMouseEvent *event)
{
	QVector3D screenPoint(event->x(),event->y(), 0);	
	screenPoint.setY(m_pOpenglView->height() - screenPoint.y());
	m_pPickObj = m_ParentLayer->PickUp(screenPoint);
	if(m_pPickObj)
	{
		DeleteSymbol(m_pPickObj);
	}
}

/************************************************************************/
/* �������ܣ� ̧������¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CDelSymbol::MouseRelease(QMouseEvent *event)
{
}

/************************************************************************/
/* �������ܣ� ����뿪�¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 06 �� 29 ��										*/
/************************************************************************/
void CDelSymbol::LeaveEvent()
{
}

/************************************************************************/
/* �������ܣ� ���˫���¼�						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CDelSymbol::keyPressEvent(QKeyEvent * event )
{
	if(NULL != m_pPickObj)			//��ʾ�з��ű�ѡ��
	{
		if((event->key() & Qt::Key_Delete) || (event->key() & Qt::Key_Backspace))
		{
			DeleteSymbol(m_pPickObj);
			emit DelButtonStatus(false);
		}
	}
}

/************************************************************************/
/* �������ܣ� ���˫���¼�						                        */
/* ����˵��:  NULL														*/
/* �޸�����:  renxue													*/
/* �޸�����:  2011 �� 04 �� 26 ��										*/
/************************************************************************/
void CDelSymbol::MouseDoubleClick(QMouseEvent *event)
{
}

/************************************************************************/
/* �������ܣ� ���ö�Ӧ�ķ���					                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CDelSymbol::SetSymbol(Symbol *pSymbol)
{
}

/************************************************************************/
/* �������ܣ� ��������ֵ					                            */
/* ����˵��:  NULL														*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 06 �� 22 ��										*/
/************************************************************************/
void CDelSymbol::setILayerAttrib(ILayerAttribute *Ilayer)
{
}

/************************************************************************/
/* �������ܣ� ѡ�з���						                            */
/* ����˵��:  QRenderable												*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 04 �� 26 ��										*/
/************************************************************************/
void CDelSymbol::SelectSymbol(QRenderable *pPickObj)
{
// 	Line* pTroughLine = dynamic_cast<Line*>(pPickObj);
// 	if(pTroughLine)
// 	{
// 		QVector3DArray *pArray = pTroughLine->getVectexArray();
// 		QVector4DArray *pColor = pTroughLine->getColorArray();
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
// 
// 		m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
// 		m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
// 		m_pUndoLineMoveEdit->SetRenderUnit(pTroughLine);
// 
// 		return;
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
	/*	if(pData)
		{
			m_pUndoSymbolPointEdit->SetUndoPostion(pData->at(iI));
		}*/
		//--------------end--------------------

		return ;
	}

	//20110510���
	NormalSymbol* pSymNormal   = dynamic_cast<NormalSymbol*>(pPickObj);
	if(pSymNormal)
	{
		pSymNormal->SetSelected(true);
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

	CSymbolMultiContourLine *pMultiContourLine = dynamic_cast<CSymbolMultiContourLine *>(pPickObj);//��ֵ��
	if(pMultiContourLine)
	{
		pMultiContourLine->SetSelected(true);
		//----------udno����-----------------
		//m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
		//m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
		//m_pUndoLineMoveEdit->SetRenderUnit(pMultiContourLine);
		//--------------end------------------
		return;		
	}

	//����20110620���
	/*QTextureFont *pText = dynamic_cast<QTextureFont *>(pPickObj);
	if(pText)
	{
		pText->SetSelected(true);
		return;
	}
*/
	QEText *pText1 = dynamic_cast<QEText *>(pPickObj);
	if(pText1)
	{
		pText1->SetSelected(true);
		return;
	}
}

/************************************************************************/
/* �������ܣ� �ͷ�ѡ�еķ���						                    */
/* ����˵��:  QRenderable												*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 05 �� 10 ��										*/
/************************************************************************/
void CDelSymbol::ReleaseSymbol(QRenderable *pPickObj)
{
// 	Line* pTroughLine = dynamic_cast<Line*>(pPickObj);
// 	if(pTroughLine)
// 	{
// 		QVector3DArray *pArray = pTroughLine->getVectexArray();
// 		QVector4DArray *pColor = pTroughLine->getColorArray();
// 		if(pArray && pColor)
// 		{
// 			pColor->Clear();
// 			for(int i=0; i<pArray->size();i++)
// 			{
// 				pColor->push_back(SourceColor);
// 			}
// 		}	
// 		//----------udno����-----------------
// 		m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
// 		m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
// 		m_pUndoLineMoveEdit->SetRenderUnit(pTroughLine);
// 		//--------------end------------------
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
	//��ֵ��
	CSymbolContourLine* pContourLine = dynamic_cast<CSymbolContourLine*>(pPickObj);//��ֵ��
	if(pContourLine)
	{
		pContourLine->SetSelected(false);
		m_pPickObj = NULL;							//���³�ʼ��ʰȡ����
		return ;
	}
	
	//����20110621���
	/*QTextureFont *pText = dynamic_cast<QTextureFont *>(pPickObj);
	if(pText)
	{
	pText->SetSelected(false);
	pText = NULL;
	return;
	}*/

	QEText *pText1 = dynamic_cast<QEText *>(pPickObj);
	if(pText1)
	{
		pText1->SetSelected(false);
		return;
	}
}

/************************************************************************/
/* �������ܣ� ɾ������						                            */
/* ����˵��:  QRenderable												*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 06 �� 21 ��										*/
/************************************************************************/
void CDelSymbol::DeleteSymbol(QRenderable *pPickObj)
{
	int iIndex = -1;
	//�Եȷ��Ų���
	SymbolPoint* pSp = dynamic_cast<SymbolPoint*>(pPickObj);
	if(pSp)
	{
		iIndex = pSp->GetPickIndex();
		QVector3DArray* pData = dynamic_cast<QVector3DArray*>(pSp->VertexArray());
		QVector3DArray* pOriginal = dynamic_cast<QVector3DArray*>(pSp->OriginalArray());
		QVector<GLfloat>* pVF = pSp->AngleArray();
		//pData->remove(iIndex);
		//pOriginal->remove(iIndex);
		//CUndoDelPointCommand* pDelPointCommand = new CUndoDelPointCommand();
		//pDelPointCommand->SetIndex(iIndex);
		//pDelPointCommand->SetSymbolPoint(pSp);
		//pDelPointCommand->SetGLWdiget(m_pOpenglView);
		//pDelPointCommand->SetUndoPoint(pData->at(iIndex));
		//pDelPointCommand->SetUndoOriginalPoint(pOriginal->at(iIndex));
		//pDelPointCommand->SetAngle(pVF->at(iIndex));
		//m_undoStack->push(pDelPointCommand);
		pSp->SetPickIndex();
		m_pPickObj = NULL;
		return;
	}

	//20110510���--��ͨ�÷��Ų���
	NormalSymbol* pSymNormal = dynamic_cast<NormalSymbol*>(pPickObj);
	if(pSymNormal)
	{
		iIndex = pSymNormal->GetPickIndex();
		QVector3DArray* pData = dynamic_cast<QVector3DArray*>(pSymNormal->VertexArray());
		QVector3DArray* pOriginal = dynamic_cast<QVector3DArray*>(pSymNormal->OriginalArray());

		//CUndoDelPointCommand* pDelPointCommand = new CUndoDelPointCommand();
		//pDelPointCommand->SetIndex(iIndex);
		//pDelPointCommand->SetSymbolPoint(pSymNormal);
		//pDelPointCommand->SetGLWdiget(m_pOpenglView);
		//pDelPointCommand->SetUndoPoint(pData->at(iIndex));
		//pDelPointCommand->SetUndoOriginalPoint(pOriginal->at(iIndex));
		//m_undoStack->push(pDelPointCommand);
		pSymNormal->SetPickIndex();
		m_pPickObj = NULL;
		return;
	}
	
	//������
	CSymbolLine* pSymLine = dynamic_cast<CSymbolLine*>(pPickObj);
	if(pSymLine)                       
	{
		//---------------undo----------------------
		//CUndoDeleteDrawCommand* pUndoDelCommand = new CUndoDeleteDrawCommand();
		//pUndoDelCommand->SetUndoGLWidget(m_pOpenglView);
		//pUndoDelCommand->SetUndoLayer(m_ParentLayer);
		//pUndoDelCommand->SetUndoRenderable(pSymLine);
		//m_undoStack->push(pUndoDelCommand);
		m_ParentLayer->GetLayerData()->RemoveRenderable(pSymLine);
		//---------------------------------------------
		m_pPickObj = NULL;
		pSymLine = NULL;
		return;
	}
	//��ֵ��
	CSymbolContourLine* pContourLine = dynamic_cast<CSymbolContourLine*>(pPickObj); //��ֵ��
	if(pContourLine)                       
	{
		pContourLine->SetSelected(false);
		//---------------undo----------------------
		//CUndoDeleteDrawCommand* pUndoDelCommand = new CUndoDeleteDrawCommand();
		//pUndoDelCommand->SetUndoGLWidget(m_pOpenglView);
		//pUndoDelCommand->SetUndoLayer(m_ParentLayer);
		//pUndoDelCommand->SetUndoRenderable(pContourLine);
		//m_undoStack->push(pUndoDelCommand);
		//m_ParentLayer->GetLayerData()->RemoveRenderable(pSymLine);
		//---------------------------------------------
		//m_ParentLayer->GetLayerData()->RemoveRenderable(pContourLine);
		m_pPickObj = NULL;
		pContourLine = NULL;
		return;
	}

	//���ֵ��
	CSymbolMultiContourLine* pMultiContourLine = dynamic_cast<CSymbolMultiContourLine*>(pPickObj); //��ֵ��
	if(pMultiContourLine)                       
	{
		pMultiContourLine->SetSelected(false);
		//---------------undo----------------------
		//CUndoDeleteDrawCommand* pUndoDelCommand = new CUndoDeleteDrawCommand();
		//pUndoDelCommand->SetUndoGLWidget(m_pOpenglView);
		//pUndoDelCommand->SetUndoLayer(m_ParentLayer);
		//pUndoDelCommand->SetUndoRenderable(pMultiContourLine);
		//m_undoStack->push(pUndoDelCommand);
		//m_ParentLayer->GetLayerData()->RemoveRenderable(pSymLine);
		//---------------------------------------------
		//m_ParentLayer->GetLayerData()->RemoveRenderable(pContourLine);
		m_pPickObj = NULL;
		pMultiContourLine = NULL;
		return;
	}

	//����20110621���
	/*QTextureFont *pText = dynamic_cast<QTextureFont *>(pPickObj);
	if(pText)
	{
	CUndoDeleteDrawCommand* pUndoDelCommand = new CUndoDeleteDrawCommand();
	pUndoDelCommand->SetUndoGLWidget(m_pOpenglView);
	pUndoDelCommand->SetUndoLayer(m_ParentLayer);
	pUndoDelCommand->SetUndoRenderable(pText);
	m_undoStack->push(pUndoDelCommand);
	pPickObj = NULL;
	pText = NULL;
	return;
	}

	QEText *pText1 = dynamic_cast<QEText *>(pPickObj);
	if(pText1)
	{
	CUndoDeleteDrawCommand* pUndoDelCommand = new CUndoDeleteDrawCommand();
	pUndoDelCommand->SetUndoGLWidget(m_pOpenglView);
	pUndoDelCommand->SetUndoLayer(m_ParentLayer);
	pUndoDelCommand->SetUndoRenderable(pText1);
	m_undoStack->push(pUndoDelCommand);
	pPickObj = NULL;
	return;
	}*/

	Line* pTroughLine = dynamic_cast<Line*>(pPickObj);
	if(pTroughLine)
	{
		//CUndoDeleteDrawCommand* pUndoDelCommand = new CUndoDeleteDrawCommand();
		//pUndoDelCommand->SetUndoGLWidget(m_pOpenglView);
		//pUndoDelCommand->SetUndoLayer(m_ParentLayer);
		//pUndoDelCommand->SetUndoRenderable(pTroughLine);
		//m_undoStack->push(pUndoDelCommand);
		
		m_pPickObj = NULL;
		pTroughLine = NULL;
		return;
	}
}

void CDelSymbol::DelSymbolImmediately( QVector3D delPoint, Layer* parentLayer )
{
	//QVector3D screenPoint(event->x(),event->y(), 0);
	m_ParentLayer = parentLayer;
	delPoint.setY(m_pOpenglView->height() - delPoint.y());
	m_pPickObj = parentLayer->PickUp(delPoint);
	if(m_pPickObj)
	{
		DeleteSymbol(m_pPickObj);
	}
}
