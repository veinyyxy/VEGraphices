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
文件名：ModifySymbol.h ModifySymbol.cpp 
类 名： CModifySymbol
父 类： NULL
子 类： 无 
功能说明： 修改符号类
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
/* 函数功能： 默认构造函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
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
/* 函数功能： 默认析构函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
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
/* 函数功能： 移动鼠标事件						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/* 修改作者:  renxue													*/
/* 修改日期:  2011 年 04 月 26 日										*/
/************************************************************************/
void CModifySymbol::MouseEvent(QMouseEvent* event) 
{
	//鼠标开该对象，选择取消
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
		//重新拾取对象
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
/* 函数功能： 按下鼠标事件						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/* 修改作者:  renxue													*/
/* 修改日期:  2011 年 04 月 26 日										*/
/************************************************************************/
void CModifySymbol::MousePress(QMouseEvent *event)
{
	QVector3D screenPoint(event->x(),event->y(), 0);	
	screenPoint.setY(m_pOpenglView->height() - screenPoint.y());
	m_pPickObj = m_ParentLayer->PickUp(screenPoint);

	QVector3D scPoint(event->x(),event->y(), m_ParentLayer->LayerZ()), glPoint;
	ScreenToGL(scPoint, glPoint);

	if(event->buttons() & Qt::LeftButton)				//按下左键编辑状态
	{
		if(m_pEditObj && m_bEdit)//编辑等值线
		{
			if(!m_bEditStart)
			{
				m_PressPoints.Clear();
				CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine*>(m_pEditObj);
				if(pContourLine)
				{
					QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pContourLine->VertexArray());
					int nPos = pContourLine->GetPosition();
					if(nPos<0 || nPos>(pData->size()-1)) return;//没有拾取到点
					nPosStart = nPos;
					m_PressPoints.push_back(pData->at(nPos));
					m_bEditStart = true;
				}
			}
			else 
			{
				m_PressPoints.push_back(glPoint);
			}
			//显示
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
	else if(event->buttons() & Qt::RightButton)			//选中按下右键进行删除
	{
		if(NULL != m_pPickObj && !m_bEdit)
		{
			DeleteSymbol(m_pPickObj);
			m_bCanMove = false;
		}
		else if(NULL != m_pPickObj )					//编辑状态取消
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
					if(nPos<0 || nPos>(pData->size()-1)) return;//没有拾取到点
					nPosEnd = nPos;
					m_PressPoints.push_back(pData->at(nPos));
					m_bEditStart = false;
				}
			}
			m_SpLine.GetSmoothPoints(m_PressPoints, m_SmoothPoints);
			if(nPosStart>0 && nPosEnd>0)
			{
				EditSymbol(m_pEditObj, &m_SmoothPoints, nPosStart, nPosEnd);//编辑等值线
			}
			ReleaseSymbol(m_pEditObj);			
			m_pEditObj = NULL;
		}
	}
}

/************************************************************************/
/* 函数功能： 抬起鼠标事件						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void CModifySymbol::MouseRelease(QMouseEvent *event)
{
		m_pOpenglView->setCursor(Qt::ArrowCursor);	//鼠标恢复原始状态
		m_bCanMove = false;							//移动标志置为false
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
/* 函数功能： 鼠标离开事件  					                        */
/* 参数说明:  NULL														*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 06 月 29 日										*/
/************************************************************************/
void CModifySymbol::LeaveEvent()
{
}


/************************************************************************/
/* 函数功能： 鼠标双击事件						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void CModifySymbol::keyPressEvent(QKeyEvent * event )
{
	if(NULL != m_pPickObj)			//表示有符号被选中
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
/* 函数功能： 鼠标双击事件						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/* 修改作者:  renxue													*/
/* 修改日期:  2011 年 04 月 26 日										*/
/************************************************************************/
void CModifySymbol::MouseDoubleClick(QMouseEvent *event)
{
	QVector3D screenPoint(event->x(),event->y(), 0);
	//双击表示选中对象
	if(m_ParentLayer)
	{
		screenPoint.setY(m_pOpenglView->height() - screenPoint.y());
		m_pPickObj = m_ParentLayer->PickUp(screenPoint);
		if(m_pPickObj)
		{
			if(NULL != m_pEditObj)//避免同时选中多个编辑符号
			{
				ReleaseSymbol(m_pEditObj);
				m_pEditObj = NULL;
			}
			m_pEditObj = m_pPickObj;
			m_bEdit = true;
			m_bCanMove = true;
			SelectSymbol(m_pPickObj);

			emit DelButtonStatus(true);

			//弹出等值线的属性框
			CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine *>(m_pEditObj);
			if(pContourLine)
			{
				InitAttribute(true);
				//emit  UpdateAttribute(this);
				//设置拾取到的线的属性
				m_pAttribute = pContourLine->GetAttribute();
				SetContourAttrib(m_pAttribute);
				emit UpdateAttribute(this);
				m_pAttribute = NULL;
			}
		}
	}
}

/************************************************************************/
/* 函数功能： 设置对应的符号					                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void CModifySymbol::SetSymbol(Symbol *pSymbol)
{
}

/************************************************************************/
/* 函数功能： 设置属性值					                            */
/* 参数说明:  NULL														*/
/* 作    者:  renxue														*/
/* 日    期:  2011 年 04 月 29 日										*/
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
/* 函数功能： 设置属性值--目前只适用于等值线						    */
/* 参数说明:  NULL														*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 05 月 10 日										*/
/************************************************************************/
void CModifySymbol::SetContourAttrib(CSymbolAttribute *pAttribute)
{
	m_layerAttribute->clear();
	QStringList strValue;

	const int nShowValue =(int) pAttribute->GetValue();
	ILayerAttribute *cValue = new ILayerAttribute("cValue","cValue",tr("等值线数值"),nShowValue,QVariant::Int,0,10000,strValue);
	m_layerAttribute->insert(QString("cValue"),cValue);

	QStringList strKinds;
	strKinds.append(QString(tr("实线")));
	strKinds.append(QString(tr("短划线")));
	strKinds.append(QString(tr("点线")));
	QString strLineType = m_pAttribute->GetLineType();
	QString strShow;
	if("1" == strLineType)
	{
		strShow = QString(tr("短划线"));
	}
	else if("2" == strLineType)
	{
		strShow = QString(tr("点线"));
	}
	ILayerAttribute *cKinds = new ILayerAttribute("cKinds","cKinds",tr("等值线类型"),strShow,QVariant::String,0,0,strKinds);
	m_layerAttribute->insert(QString("cKinds"),cKinds);

	QVector4D tempColor = pAttribute->GetColor();
	tempColor.setX(tempColor.x()*255);
	tempColor.setY(tempColor.y()*255);
	tempColor.setZ(tempColor.z()*255);
	QStringList strColor;
	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor",tr("等值线颜色"),tempColor,QVariant::Color,0,0,strColor);
	m_layerAttribute->insert(QString("cColor"),cColor);

	QStringList strWidth;
	for (int i=1; i<=10; i++)
	{
		strWidth.append(QString::number(i));
	}
	ILayerAttribute *cWidth = new ILayerAttribute("cWidth","cWidth",tr("等值线线宽"),pAttribute->GetLineWidth(),QVariant::Int,1,10,strWidth);
	m_layerAttribute->insert(QString("cWidth"), cWidth);

	QStringList strFill;
	strFill.append(tr("否"));
	strFill.append(tr("是"));
	ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill",tr("等值线是否填充"),pAttribute->GetFillType(),QVariant::Bool,0,0,strFill);
	m_layerAttribute->insert(QString("cFill"),cFill);
}

/************************************************************************/
/* 函数功能： 选中符号						                            */
/* 参数说明:  QRenderable												*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 06 月 21 日										*/
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
// 		//----------udno机制-----------------
// 		m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
// 		m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
// 		m_pUndoLineMoveEdit->SetRenderUnit(pTroughLine);
// 		//--------------end------------------
// 	}

	SymbolPoint* pSp = dynamic_cast<SymbolPoint*>(pPickObj);
	if(pSp)
	{
		pSp->SetSelected(true);
		//----------udno机制-----------------
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

	//20110510添加
	NormalSymbol* pSymNormal   = dynamic_cast<NormalSymbol*>(pPickObj);
	if(pSymNormal)
	{
		pSymNormal->SetSelected(true);
		//----------udno机制-----------------
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

	//符号多线
	CSymbolMultiLine *pSymMultiLine = dynamic_cast<CSymbolMultiLine *>(pPickObj);
	if(pSymMultiLine)
	{
		pSymMultiLine->SetSelected(true);
		//----------udno机制-----------------
		//m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
		//m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
		//m_pUndoLineMoveEdit->SetRenderUnit(pSymMultiLine);
		//--------------end------------------
		return;		
	}
	
	CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine *>(pPickObj);//等值线
	if(pContourLine)
	{
		pContourLine->SetSelected(true);
		pContourLine->ReDraw();
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
		pMultiContourLine->ReDraw();
		//----------udno机制-----------------
		//m_pUndoLineMoveEdit = new CUndoSymbolLineMoveCommand();
		//m_pUndoLineMoveEdit->SetUndoGLWidget(m_pOpenglView);
		//m_pUndoLineMoveEdit->SetRenderUnit(pMultiContourLine);
		//--------------end------------------
		return;		
	}

	//文字20110621添加
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
/* 函数功能： 释放选中的符号						                    */
/* 参数说明:  QRenderable												*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 05 月 10 日										*/
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
// 		//----------udno机制-----------------
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

	//符号线
	CSymbolMultiLine* pSymMultiLine = dynamic_cast<CSymbolMultiLine*>(pPickObj);
	if(pSymMultiLine)
	{
		pSymMultiLine->SetSelected(false);
		m_pPickObj = NULL;							//重新初始化拾取对象
		return ;
	}
	//等值线
	CSymbolContourLine* pContourLine = dynamic_cast<CSymbolContourLine*>(pPickObj);//等值线
	if(pContourLine)
	{
		pContourLine->SetSelected(false);
		pContourLine->ReDraw();
		m_pPickObj = NULL;							//重新初始化拾取对象
		return ;
	}

	//多等值线
	CSymbolMultiContourLine* pMultiContourLine = dynamic_cast<CSymbolMultiContourLine*>(pPickObj);//等值线
	if(pMultiContourLine)
	{
		pMultiContourLine->SetSelected(false);
		pMultiContourLine->ReDraw();
		m_pPickObj = NULL;							//重新初始化拾取对象
		return ;
	}

	//文字20110621添加
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
/* 函数功能： 删除符号						                            */
/* 参数说明:  QRenderable												*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 06 月 22 日										*/
/************************************************************************/
void CModifySymbol::DeleteSymbol(QRenderable *pPickObj)
{	
}

/************************************************************************/
/* 函数功能： 移动符号						                            */
/* 参数说明:  QRenderable												*/
/*            scPoint是鼠标左键点击的屏幕坐标							*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 26 日										*/
/************************************************************************/
void CModifySymbol::MoveSymbol(QRenderable *pPickObj,QVector3D &scPoint)
{
	QVector3D glPoint;
	int nIndex = -1;			//选择点对象索引
	if(m_iFlag == 0)
	{
		m_pOpenglView->setCursor(Qt::ClosedHandCursor);
		m_iFlag  =1;
	}
	//符号点对象移动
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

	//20110510添加--通用符号对象移动
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

	//符号线移动
	CSymbolLine *pSymLine = dynamic_cast<CSymbolLine*>(pPickObj);
	if(pSymLine)
	{
		QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pSymLine->VertexArray());
		QVector3DArray *pOrgData = dynamic_cast<QVector3DArray*>(pSymLine->OriginalArray());
		ScreenToGL(scPoint, glPoint);
		int nPos = pSymLine->GetPosition();
		if(nPos < 0) return;//没有获得拾取到的点的位置

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

	//多符号线移动
	CSymbolMultiLine *pSymMultiLine = dynamic_cast<CSymbolMultiLine*>(pPickObj);
	if(pSymMultiLine)
	{
		QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pSymMultiLine->VertexArray());
		QVector3DArray *pOrgData = dynamic_cast<QVector3DArray*>(pSymMultiLine->OriginalArray());
		ScreenToGL(scPoint, glPoint);
		int nPos = pSymMultiLine->GetPosition();
		if(nPos < 0) return;//没有获得拾取到的点的位置

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
	
	//等值线
	CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine*>(pPickObj);
	if(pContourLine)
	{
		QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pContourLine->VertexArray());
		QVector3DArray *pOrgData = dynamic_cast<QVector3DArray*>(pContourLine->OriginalArray());
		ScreenToGL(scPoint, glPoint);
		int nPos = pContourLine->GetPosition();
		if(nPos < 0) return;//没有获得拾取到的点的位置

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

	//多等值线
	CSymbolMultiContourLine *pMultiContourLine = dynamic_cast<CSymbolMultiContourLine*>(pPickObj);
	if(pMultiContourLine)
	{
		QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pMultiContourLine->VertexArray());
		QVector3DArray *pOrgData = dynamic_cast<QVector3DArray*>(pMultiContourLine->OriginalArray());
		ScreenToGL(scPoint, glPoint);
		int nPos = pMultiContourLine->GetPosition();
		if(nPos < 0) return;//没有获得拾取到的点的位置

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

	//文字移动--20110621添加
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

	//文字移动--20110621添加
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
	//槽线--20110629
// 	Line *pTroughLine = dynamic_cast<Line*>(pPickObj);
// 	if(pTroughLine)
// 	{
// 		QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pTroughLine->VertexArray());
// 		ScreenToGL(scPoint, glPoint);
// 		int nPos = pTroughLine->GetPosition();
// 		if(nPos < 0) return;//没有获得拾取到的点的位置
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
/* 函数功能： 编辑符号---目前只适用于等值线						        */
/* 参数说明:  QRenderable												*/
/*            scPoint是鼠标左键点击的屏幕坐标							*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 26 日										*/
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

	//等值线
	CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine*>(pPickObj);
	if(!pContourLine) return;

	QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pContourLine->VertexArray());
	QVector3DArray *pOriginalData = dynamic_cast<QVector3DArray*>(pContourLine->OriginalArray());
	if(!pData) return ;

	//编辑顶点
	pTempArray = new QVector3DArray;
	pTempOriginalArray = new QVector3DArray;
	int i;

	for(i=pData->size()-1; i>nPosStart; i--)	
	{
		if(i >= nPosEnd)
		{
			pTempArray->push_back(pData->at(i));//逆序存到临时数组中
			pTempOriginalArray->push_back(pOriginalData->at(i));
		}
		pData->pop_back();
		pOriginalData->pop_back();
	}
	pData->pop_back();
	pOriginalData->pop_back();

	//加入新编辑的点
	for(i=0; i<pPoints->size(); i++)
	{
		QVector3D temp = pPoints->at(i);
		pData->push_back(temp);		
		pOriginalData->push_back(TransGLtoWGS84(temp));
	}
	//加入原等值线后半部分的点
	for(i=pTempArray->size()-1; i>=0; i--)
	{
		pData->push_back(pTempArray->at(i));
		pOriginalData->push_back(pTempOriginalArray->at(i));
	}
	return;
}

