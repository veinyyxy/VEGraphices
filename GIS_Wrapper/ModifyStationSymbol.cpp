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

#include "ModifyStationSymbol.h"
#include "SymbolPoint.h"
#include "SymbolLine.h"
#include "SymbolContourLine.h"
#include "NormalSymbol.h"
#include <QtGui/QCursor>
#include <QDockWidget>
//#include "QTextureFont.h"

/************************************************************************/
/* 函数功能： 默认构造函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
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
/* 函数功能： 默认析构函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
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
/* 函数功能： 移动鼠标事件						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/* 修改作者:  renxue													*/
/* 修改日期:  2011 年 04 月 26 日										*/
/************************************************************************/
void CModifyStationSymbol::MouseEvent(QMouseEvent* event) 
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
void CModifyStationSymbol::MousePress(QMouseEvent *event)
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
	
}

/************************************************************************/
/* 函数功能： 抬起鼠标事件						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void CModifyStationSymbol::MouseRelease(QMouseEvent *event)
{
	/*	m_pOpenglView->setCursor(Qt::ArrowCursor);	//鼠标恢复原始状态
		m_bCanMove = false;							//移动标志置为false
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
/* 函数功能： 鼠标离开事件  					                        */
/* 参数说明:  NULL														*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 06 月 29 日										*/
/************************************************************************/
void CModifyStationSymbol::LeaveEvent()
{
}


/************************************************************************/
/* 函数功能： 鼠标双击事件						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void CModifyStationSymbol::keyPressEvent(QKeyEvent * event )
{

/*	if(NULL != m_pPickObj)			//表示有符号被选中
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
/* 函数功能： 鼠标双击事件						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/* 修改作者:  renxue													*/
/* 修改日期:  2011 年 04 月 26 日										*/
/************************************************************************/
void CModifyStationSymbol::MouseDoubleClick(QMouseEvent *event)
{
	/*QVector3D screenPoint(event->x(),event->y(), 0);
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
	*/
}

/************************************************************************/
/* 函数功能： 设置对应的符号					                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void CModifyStationSymbol::SetSymbol(Symbol *pSymbol)
{

}

/************************************************************************/
/* 函数功能： 设置属性值					                            */
/* 参数说明:  NULL														*/
/* 作    者:  renxue														*/
/* 日    期:  2011 年 04 月 29 日										*/
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
/* 函数功能： 设置属性值--目前只适用于等值线						    */
/* 参数说明:  NULL														*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 05 月 10 日										*/
/************************************************************************/
void CModifyStationSymbol::SetContourAttrib(CSymbolAttribute *pAttribute)
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
// 		//----------udno机制-----------------
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
		//----------udno机制-----------------
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



