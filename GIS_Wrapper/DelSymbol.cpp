/*============================================================ 
项目名： 人机交互平台
作 者： lugsh
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：DelSymbol.h DelSymbol.cpp 
类 名： CDelSymbol
父 类： NULL
子 类： 无 
功能说明： 拾取修改类
调用说明： 

-------------------------------------版本更新------------------------------------------------ 
V 1.2 
作 者：杨晓宇
完成日期：2011年4月28日
更新说明：添加对各类符号的编辑撤销功能
V 1.1 
作 者：renxue
完成日期：2011年04月28日
更新说明：增加对符号线、等值线的拾取功能 
V 1.0 
原作者 ：lugsh
完成日期：2011年？月？日
============================================================*/

#include "DelSymbol.h"
#include "SymbolPoint.h"
#include "SymbolLine.h"
#include "SymbolContourLine.h"
#include "SymbolMultiContourLine.h"
#include "NormalSymbol.h"
#include <QtGui/QCursor>
#include <QDockWidget>
//#include "..//QE_lib/QTextureFont.h"
#include "QEText.h"
/************************************************************************/
/* 函数功能： 默认构造函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
CDelSymbol::CDelSymbol(void)//:m_pUndoSymbolPointEdit(0), m_pUndoLineMoveEdit(0)
{
	m_pPickObj = NULL;
}

/************************************************************************/
/* 函数功能： 默认析构函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
CDelSymbol::~CDelSymbol(void)
{
}

/************************************************************************/
/* 函数功能： 移动鼠标事件						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/* 修改作者:  renxue													*/
/* 修改日期:  2011 年 06 月 22 日										*/
/* 修改作者:  wuenp											     		*/
/* 修改日期:  2011 年 06 月 10 日										*/
/************************************************************************/
void CDelSymbol::MouseEvent(QMouseEvent* event) 
{
	if(m_pPickObj) 
		ReleaseSymbol(m_pPickObj);
	else
	{
		//重新拾取对象
		QVector3D screenPoint(event->x(),event->y(), 0);	
		screenPoint.setY(m_pOpenglView->height() - screenPoint.y());
		m_pPickObj = m_ParentLayer->PickUp(screenPoint);

		QVector3D scPoint(event->x(),event->y(), m_ParentLayer->LayerZ()), glPoint;
		ScreenToGL(scPoint, glPoint);
		SelectSymbol(m_pPickObj);
	}	
}

/************************************************************************/
/* 函数功能： 按下鼠标事件						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/* 修改作者:  renxue													*/
/* 修改日期:  2011 年 06 月 22 日										*/
/* 修改作者:  wuenp												    	*/
/* 修改日期:  2011 年 06 月 10 日										*/
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
/* 函数功能： 抬起鼠标事件						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void CDelSymbol::MouseRelease(QMouseEvent *event)
{
}

/************************************************************************/
/* 函数功能： 鼠标离开事件						                        */
/* 参数说明:  NULL														*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 06 月 29 日										*/
/************************************************************************/
void CDelSymbol::LeaveEvent()
{
}

/************************************************************************/
/* 函数功能： 鼠标双击事件						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void CDelSymbol::keyPressEvent(QKeyEvent * event )
{
	if(NULL != m_pPickObj)			//表示有符号被选中
	{
		if((event->key() & Qt::Key_Delete) || (event->key() & Qt::Key_Backspace))
		{
			DeleteSymbol(m_pPickObj);
			emit DelButtonStatus(false);
		}
	}
}

/************************************************************************/
/* 函数功能： 鼠标双击事件						                        */
/* 参数说明:  NULL														*/
/* 修改作者:  renxue													*/
/* 修改日期:  2011 年 04 月 26 日										*/
/************************************************************************/
void CDelSymbol::MouseDoubleClick(QMouseEvent *event)
{
}

/************************************************************************/
/* 函数功能： 设置对应的符号					                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void CDelSymbol::SetSymbol(Symbol *pSymbol)
{
}

/************************************************************************/
/* 函数功能： 设置属性值					                            */
/* 参数说明:  NULL														*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 06 月 22 日										*/
/************************************************************************/
void CDelSymbol::setILayerAttrib(ILayerAttribute *Ilayer)
{
}

/************************************************************************/
/* 函数功能： 选中符号						                            */
/* 参数说明:  QRenderable												*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 26 日										*/
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
// 		//----------udno机制-----------------
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
		//----------udno机制-----------------
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

	//20110510添加
	NormalSymbol* pSymNormal   = dynamic_cast<NormalSymbol*>(pPickObj);
	if(pSymNormal)
	{
		pSymNormal->SetSelected(true);
		return ;
	}

	//符号线
	CSymbolLine *pSymLine = dynamic_cast<CSymbolLine *>(pPickObj);
	if(pSymLine)
	{
		pSymLine->SetSelected(true);
		//----------udno机制-----------------
		//m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
		//m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
		//m_pUndoLineMoveEdit->SetRenderUnit(pSymLine);
		//--------------end------------------
		return;		
	}
	
	CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine *>(pPickObj);//等值线
	if(pContourLine)
	{
		pContourLine->SetSelected(true);
		//----------udno机制-----------------
		//m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
		//m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
		//m_pUndoLineMoveEdit->SetRenderUnit(pContourLine);
		//--------------end------------------
		return;		
	}

	CSymbolMultiContourLine *pMultiContourLine = dynamic_cast<CSymbolMultiContourLine *>(pPickObj);//等值线
	if(pMultiContourLine)
	{
		pMultiContourLine->SetSelected(true);
		//----------udno机制-----------------
		//m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
		//m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
		//m_pUndoLineMoveEdit->SetRenderUnit(pMultiContourLine);
		//--------------end------------------
		return;		
	}

	//文字20110620添加
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
/* 函数功能： 释放选中的符号						                    */
/* 参数说明:  QRenderable												*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 05 月 10 日										*/
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
// 		//----------udno机制-----------------
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
		m_pPickObj = NULL;							//重新初始化拾取对象
		return ;
	}

	//20110510添加
	NormalSymbol* pSymNormal = dynamic_cast<NormalSymbol*>(pPickObj);
	if(pSymNormal)
	{
		nIndex = pSymNormal->GetPickIndex();
		QVector3DArray* pData = dynamic_cast<QVector3DArray*>(pSymNormal->VertexArray());
		pSymNormal->SetPickIndex();
		pSymNormal->SetSelected(false);
		m_pPickObj = NULL;							//重新初始化拾取对象
		return ;
	}

	//符号线
	CSymbolLine* pSymLine = dynamic_cast<CSymbolLine*>(pPickObj);
	if(pSymLine)
	{
		pSymLine->SetSelected(false);
		m_pPickObj = NULL;							//重新初始化拾取对象
		return ;
	}
	//等值线
	CSymbolContourLine* pContourLine = dynamic_cast<CSymbolContourLine*>(pPickObj);//等值线
	if(pContourLine)
	{
		pContourLine->SetSelected(false);
		m_pPickObj = NULL;							//重新初始化拾取对象
		return ;
	}
	
	//文字20110621添加
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
/* 函数功能： 删除符号						                            */
/* 参数说明:  QRenderable												*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 06 月 21 日										*/
/************************************************************************/
void CDelSymbol::DeleteSymbol(QRenderable *pPickObj)
{
	int iIndex = -1;
	//对等符号操作
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

	//20110510添加--对通用符号操作
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
	
	//符号线
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
	//等值线
	CSymbolContourLine* pContourLine = dynamic_cast<CSymbolContourLine*>(pPickObj); //等值线
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

	//多等值线
	CSymbolMultiContourLine* pMultiContourLine = dynamic_cast<CSymbolMultiContourLine*>(pPickObj); //等值线
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

	//文字20110621添加
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
