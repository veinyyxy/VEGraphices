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

#include "ModifyContour.h"
//#include "UndoContourEditCommand.h"
#include <QtGui/QColor>
#include <QtWidgets/QMessageBox>
#include "CIPASMessageBox.h"

/************************************************************************/
/* 函数功能： 默认构造函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
CModifyContour::CModifyContour(void)
{
	m_pPickObj = NULL;
	//m_pUndoSymbolPointEdit = NULL;
	//m_pUndoLineMoveEdit = NULL;
	//m_pUndoContourEdit = NULL;
	//m_pUndoContourEdit2 = NULL;
	//m_pUndoMultiContourEdit = NULL;
	m_bEditStart = false;				//默认没有开启编辑
	m_pShowLine = new Line;				//修改等值线时进行显示
	pArray = NULL;
	pColor = NULL;
	m_pAttribute=NULL;
	m_bClose = true;
	m_currentZ = 0 ;					//初始化为零
}

/************************************************************************/
/* 函数功能： 默认析构函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
CModifyContour::~CModifyContour(void)
{
}

/************************************************************************/
/* 函数功能： 默认构造函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void CModifyContour::MouseEvent(QMouseEvent* event)
{

	//释放前次拾取的对象
	if(!m_bEditStart)
	{
		ReleaseSymbol(m_pPickObj);
		//重新拾取对象
		QVector3D screenPoint(event->x(),event->y(), 0);	
		screenPoint.setY(m_pOpenglView->height() - screenPoint.y());
		m_pPickObj = m_ParentLayer->PickUp(screenPoint);

		QVector3D scPoint(event->x(),event->y(), m_ParentLayer->LayerZ()), glPoint;
		ScreenToGL(scPoint, glPoint);
		SelectSymbol(m_pPickObj);
	}else   //收集移动过程中的点
	{
		QVector3D  glPoint, scPoint(event->x(), event->y(), 1.0f);
		ScreenToGL(scPoint, glPoint);

		m_PressPoints.push_back(glPoint);
		DrawControlLine();
		m_PressPoints.pop_back();
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
void CModifyContour::MouseDoubleClick(QMouseEvent *event)
{
	//重新拾取
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

		//弹出等值线的属性框
		CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine *>(m_pPickObj);
		CSymbolMultiContourLine *pMultiContourLine = dynamic_cast<CSymbolMultiContourLine *>(m_pPickObj);
		if(pContourLine)
		{
			InitAttribute(pContourLine->GetClosedLine());
			m_bClose = pContourLine->GetClosedLine();

			//设置拾取到的线的属性
			m_pAttribute = pContourLine->GetAttribute();
			SetContourAttrib(m_pAttribute);
			emit UpdateAttribute(this);
		}
		if (pMultiContourLine)
		{
			InitAttribute(pMultiContourLine->GetClosedLine());
			m_bClose = pMultiContourLine->GetClosedLine();

			//设置拾取到的线的属性
			m_pAttribute = pMultiContourLine->GetAttribute();
			SetContourAttrib(m_pAttribute);
			emit UpdateAttribute(this);

		}
	}
}

/************************************************************************/
/* 函数功能： 默认构造函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void CModifyContour::MousePress(QMouseEvent *event)
{
	if(NULL !=m_pPickObj)
	{
		QVector3D scPoint(event->x(),event->y(), m_ParentLayer->LayerZ()), glPoint;
		ScreenToGL(scPoint, glPoint);		
		m_lastScreen = scPoint;
		if(event->buttons() & Qt::LeftButton)				//按下左键编辑状态
		{
			if(!m_bEditStart)
			{
				m_PressPoints.Clear();
				CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine*>(m_pPickObj);
				CSymbolMultiContourLine *pMultiContourLine = dynamic_cast<CSymbolMultiContourLine*>(m_pPickObj);

				QVector3DArray *pData = NULL;

				//基本的等值线功能
				if(pContourLine)
				{
					pData = dynamic_cast<QVector3DArray*>(pContourLine->VertexArray());
					int nPos = pContourLine->GetPosition();
					if(nPos<0 || nPos>(pData->size()-1)) return;//没有拾取到点
					nPosStart = nPos;
					m_bEditStart = true;
				}

				//绘制灾害的带立体边框的图形
				if(pMultiContourLine)
				{
					pData = dynamic_cast<QVector3DArray*>(pMultiContourLine->VertexArray());
					int nPos = pMultiContourLine->GetPosition();
					if(nPos<0 || nPos>(pData->size()-1)) return;//没有拾取到点
					nPosStart = nPos;
					m_bEditStart = true;
				}

				//增加第一次的控制点
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
						strName2 = tr("逆时针");
					}else
					{
						strName2 = tr("顺时针");
					}

					if(m_bPositive)
					{
						strName1 = tr("逆时针");
					}else
					{
						strName1 = tr("顺时针");
					}

					QString strContext;
					strContext.append(tr("等值线为"));
					strContext.append(strName1);
					strContext.append(",");
					strContext.append("编辑线为");
					strContext.append(strName2);
					strContext.append(",");
					strContext.append(tr("请保持方向相同!"));
					CCIPASMessageBox::CIPASInformation(0, CCIPASMessageBox::CIPAS_OK,tr("错误"),strContext);
					return;
				}
			}
			//绘制控制线
			DrawControlLine();

		}
		else if(event->buttons() & Qt::RightButton)				//按下右键进行编辑
		{
			if(NULL != pArray && NULL != pColor)
			{
				pArray->Clear();
				pColor->Clear();
			}
			
			//移除控制线
			ClearControlLine();

			if(!CheckDirection())
			{
				return;
			}
			//重新拾取，为了得到结束点的位置
			QVector3D screenPoint(event->x(),event->y(), 0);	
			screenPoint.setY(m_pOpenglView->height() - screenPoint.y());
			m_lastScreen.setY(m_pOpenglView->height() - m_lastScreen.y());
			//重新拾取前，先释放以前的pickobj
			ReleaseSymbol(m_pPickObj);	
			m_pPickObj = m_ParentLayer->PickUp(m_lastScreen);
			m_bEditStart = false;				//无论是否成功拾取，都设为false

			//弹出最后一个控制点
			m_PressPoints.pop_back();
			//绘制编辑功能
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
					//没有拾取到点 ,释放原来的拾取的对象
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
				}//没有拾取到点
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
/* 函数功能： 检查绘制的编辑线是否与等值线方向相同				        */
/* 参数说明:  NULL														*/
/*																		*/
/* 作    者:  lugsh														*/
/* 日    期:  2012 年 01 月 11 日										*/
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
			strName2 = tr("逆时针");
		}else
		{
			strName2 = tr("顺时针");
		}

		if(m_bPositive)
		{
			strName1 = tr("逆时针");
		}else
		{
			strName1 = tr("顺时针");
		}

		QString strContext;
		strContext.append(tr("等值线为"));
		strContext.append(strName1);
		strContext.append(",");
		strContext.append("编辑线为");
		strContext.append(strName2);
		strContext.append(",");
		strContext.append(tr("请保持方向相同!"));
		CCIPASMessageBox::CIPASInformation(0, CCIPASMessageBox::CIPAS_OK,tr("提示"),strContext);

		return false;
	}
	return true;
}

/************************************************************************/
/* 函数功能： 绘制编辑时的控制线功能							        */
/* 参数说明:  NULL														*/
/*																		*/
/* 作    者:  lugsh														*/
/* 日    期:  2012 年 01 月 11 日										*/
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
/* 函数功能： 移除编辑时的控制线功能							        */
/* 参数说明:  NULL														*/
/*																		*/
/* 作    者:  lugsh														*/
/* 日    期:  2012 年 01 月 11 日										*/
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
/* 函数功能： 绘制控制点										        */
/* 参数说明:  NULL														*/
/*																		*/
/* 作    者:  lugsh														*/
/* 日    期:  2012 年 01 月 11 日										*/
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
/* 函数功能： 清除绘制的控制点										        */
/* 参数说明:  NULL														*/
/*																		*/
/* 作    者:  lugsh														*/
/* 日    期:  2012 年 01 月 11 日										*/
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
//	//等值线
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
//	//设置undo的数组;
//
//	
//	if(!pData || pData->count() < 3 || pPoints->count() <3) return false ;     //当绘制的等值线少于三个点时，退出
//	
//	//编辑顶点
//	QVector3DArray *pTempArray, *pTempOriginalArray;
//	pTempArray = new QVector3DArray;
//	pTempOriginalArray = new QVector3DArray;
//	int i;
//	
//	//如果等高线是逆时针绘制
//	if(m_bPositive == true) 
//	{
//		if(nPosStart < nPosEnd  )//同方向
//		{   
//			bool bModify = m_DrawOrder.isConvert(pPoints,false);
//			if(bModify == m_bPositive)  
//			{
//				for(i=pData->size()-1; i>nPosStart; i--)	
//				{
//					if(i >= nPosEnd)
//					{
//						pTempArray->push_back(pData->at(i));//逆序存到临时数组中
//						pTempOriginalArray->push_back(pOriginalData->at(i));
//					}
//					pData->pop_back();
//					pOriginalData->pop_back();
//				}
//				pData->pop_back();
//				pOriginalData->pop_back();
//
//				//加入新编辑的点
//				for(i=0; i<pPoints->size(); i++)
//				{
//					QVector3D temp = pPoints->at(i);
//					temp.setZ(pDataZ);
//					pData->push_back(temp);		
//					pOriginalData->push_back(TransGLtoWGS84(temp));
//				}
//
//				//加入原等值线后半部分的点
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
//				//弹出从0点到nposEnd的点
//				for(i = pData->count()-1 ; i>nPosEnd; i--)
//				{
//					pData->pop_back();
//					pOriginalData->pop_back();
//				}
//
//				//弹出从nposstart之后的点
//				for(i = 0 ; i <nPosStart; i++)
//				{
//					pData->pop_front();
//					pOriginalData->pop_front();
//				}
//
//				//重新生成线
//				for(i=0; i< pPoints->count()-1; i++)
//				{
//					pData ->push_front(pPoints->at(i));
//					QVector3D temp  = pPoints->at(i);
//					temp.setZ(pDataZ);
//					pOriginalData->push_front(TransGLtoWGS84(temp));					
//				}
//			}
//		}
//		else if (nPosStart > nPosEnd)    //方向相反
//		{
//		  bool bModify = m_DrawOrder.isConvert(pPoints,false);
//
//			if(bModify == m_bPositive)     //开始点在结束点前面
//			{
//				//弹出从0点到nposEnd的点
//				for(i = 0 ; i< nPosEnd; i++)
//				{
//					pData->pop_front();
//					pOriginalData->pop_front();
//				}
//
//				//弹出从nposstart之后的点
//				for(i = pData->count()-1 ; i >(nPosStart - nPosEnd); i--)
//				{
//					pData->pop_back();
//					pOriginalData->pop_back();
//				}
//
//				//重新生成线
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
//						pTempArray->push_back(pData->at(i));//逆序存到临时数组中
//						pTempOriginalArray->push_back(pOriginalData->at(i));
//					}
//					pData->pop_back();
//					pOriginalData->pop_back();
//				}
//				pData->pop_back();
//				pOriginalData->pop_back();
//				
//				//加入新编辑的点
//				for(i=pPoints->size()-1; i>0; i--)
//				{
//					QVector3D temp = pPoints->at(i);
//					temp.setZ(pDataZ);
//					pData->push_back(temp);		
//					pOriginalData->push_back(TransGLtoWGS84(temp));
//				}
//				
//				//加入原等值线后半部分的点
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
//    else if(m_bPositive == false)  //如果等值线是顺时针绘制
//	{
//		if(nPosStart < nPosEnd  )//同方向
//		{   
//			bool bModify = m_DrawOrder.isConvert(pPoints,false);
//
//			if(bModify == m_bPositive)  
//			{
//				for(i=pData->size()-1; i>nPosStart; i--)	
//				{
//					if(i >= nPosEnd)
//					{
//						pTempArray->push_back(pData->at(i));//逆序存到临时数组中
//						pTempOriginalArray->push_back(pOriginalData->at(i));
//					}
//					pData->pop_back();
//					pOriginalData->pop_back();
//				}
//				pData->pop_back();
//				pOriginalData->pop_back();
//				
//				//加入新编辑的点
//				for(i=0; i<pPoints->size(); i++)
//				{
//					QVector3D temp = pPoints->at(i);
//					temp.setZ(pDataZ);
//					pData->push_back(temp);		
//					pOriginalData->push_back(TransGLtoWGS84(temp));
//				}
//
//				//加入原等值线后半部分的点
//				for(i=pTempArray->size()-2; i>=0; i--)
//				{
//					QVector3D temp = pTempOriginalArray->at(i);
//					pData->push_back(pTempArray->at(i));
//					pOriginalData->push_back(temp);
//				}
//			}
//			else
//			{
//				//弹出从0点到nposEnd的点
//				for(i = pData->count()-1 ; i>nPosEnd; i--)
//				{
//					pData->pop_back();
//					pOriginalData->pop_back();
//				}
//
//				//弹出从nposstart之后的点
//				for(i = 0 ; i <nPosStart; i++)
//				{
//					pData->pop_front();
//					pOriginalData->pop_front();
//				}
//
//				//重新生成线
//				for(i=0; i< pPoints->count()-1; i++)
//				{
//					pData ->push_front(pPoints->at(i));
//					QVector3D temp  = pPoints->at(i);
//					temp.setZ(pDataZ);
//					pOriginalData->push_front(TransGLtoWGS84(temp));
//				}
//			}
//		}
//		else if (nPosStart > nPosEnd)    //方向相反
//		{
//			bool bModify = m_DrawOrder.isConvert(pPoints,false);
//
//			if(bModify == m_bPositive)     //开始点在结束点前面
//			{
//				//弹出从0点到nposEnd的点
//				for(i = 0 ; i< nPosEnd; i++)
//				{
//					pData->pop_front();
//					pOriginalData->pop_front();
//				}
//
//				//弹出从nposstart之后的点
//				for(i = pData->count()-1 ; i >(nPosStart - nPosEnd); i--)
//				{
//					pData->pop_back();
//					pOriginalData->pop_back();
//				}
//
//				//重新生成线
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
//						pTempArray->push_back(pData->at(i));//逆序存到临时数组中
//						pTempOriginalArray->push_back(pOriginalData->at(i));
//					}
//					pData->pop_back();
//					pOriginalData->pop_back();
//				}
//				pData->pop_back();
//				pOriginalData->pop_back();
//
//				//加入新编辑的点
//				for(i=pPoints->size()-1; i>0; i--)
//				{
//					QVector3D temp = pPoints->at(i);
//					temp.setZ(pDataZ);
//					pData->push_back(temp);		
//					pOriginalData->push_back(TransGLtoWGS84(temp));
//				}
//				
//				//加入原等值线后半部分的点
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
//		CCIPASMessageBox::CIPASInformation(0, CCIPASMessageBox::CIPAS_OK,tr("提示"),tr("裁减后多边形中出现线段相交,请重新裁剪！"));
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
/* 函数功能： 编辑符号---目前只适用于等值线						        */
/* 参数说明:  QRenderable												*/
/*            scPoint是鼠标左键点击的屏幕坐标							*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 26 日										*/
/************************************************************************/
bool CModifyContour::EditSymbol(QRenderable *pPickObj, QVector3DArray *pPoints, int nPosStart, int nPosEnd)
{
	if (!pPickObj && !pPoints) return false ;
	if(nPosStart == nPosEnd) return false;
	int nTemp;

	//等值线
	CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine*>(pPickObj);
	if(!pContourLine) return false;

	QVector3DArray *pData = dynamic_cast<QVector3DArray*>(pContourLine->VertexArray());
	QVector3DArray *pOriginalData = dynamic_cast<QVector3DArray*>(pContourLine->OriginalArray());
	
	//设置undo的数组;
	//if(pUndoContourEdit)
	//{
	//	pUndoContourEdit->SetLineVertex(pData, CUndoContourEditCommand::OldLine);
	//	pUndoContourEdit->SetLineVertex(pOriginalData, CUndoContourEditCommand::OldOriginalLine);
	//}
	//
	if(!pData || pData->count() < 3 || pPoints->count() <3) return false ;     //当绘制的等值线少于三个点时，退出

	//编辑顶点
	QVector3DArray *pTempArray, *pTempOriginalArray;
	pTempArray = new QVector3DArray;
	pTempOriginalArray = new QVector3DArray;
	int i;

	//如果等高线是逆时针绘制
	if(m_bPositive == true) 
	{
		if(nPosStart < nPosEnd  )//同方向
		{   
			bool bModify = m_DrawOrder.isConvert(pPoints,false);
			if(bModify == m_bPositive)  
			{
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
				//弹出从0点到nposEnd的点
				for(i = pData->count()-1 ; i>nPosEnd; i--)
				{
					pData->pop_back();
					pOriginalData->pop_back();
				}

				//弹出从nposstart之后的点
				for(i = 0 ; i <nPosStart; i++)
				{
					pData->pop_front();
					pOriginalData->pop_front();
				}

				//重新生成线
				for(i=0; i< pPoints->count()-1; i++)
				{
					pData ->push_front(pPoints->at(i));
					QVector3D temp  = pPoints->at(i);
					pOriginalData->push_front(TransGLtoWGS84(temp));
				}
			}
		}
		else if (nPosStart > nPosEnd)    //方向相反
		{
		  bool bModify = m_DrawOrder.isConvert(pPoints,false);

			if(bModify == m_bPositive)     //开始点在结束点前面
			{
				//弹出从0点到nposEnd的点
				for(i = 0 ; i< nPosEnd; i++)
				{
					pData->pop_front();
					pOriginalData->pop_front();
				}

				//弹出从nposstart之后的点
				for(i = pData->count()-1 ; i >(nPosStart - nPosEnd); i--)
				{
					pData->pop_back();
					pOriginalData->pop_back();
				}

				//重新生成线
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
						pTempArray->push_back(pData->at(i));//逆序存到临时数组中
						pTempOriginalArray->push_back(pOriginalData->at(i));
					}
					pData->pop_back();
					pOriginalData->pop_back();
				}
				pData->pop_back();
				pOriginalData->pop_back();
				
				//加入新编辑的点
				for(i=pPoints->size()-1; i>0; i--)
				{
					QVector3D temp = pPoints->at(i);
					pData->push_back(temp);		
					pOriginalData->push_back(TransGLtoWGS84(temp));
				}
				
				//加入原等值线后半部分的点
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
    else if(m_bPositive == false)  //如果等值线是顺时针绘制
	{
		if(nPosStart < nPosEnd  )//同方向
		{   
			bool bModify = m_DrawOrder.isConvert(pPoints,false);

			if(bModify == m_bPositive)  
			{
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
				for(i=pTempArray->size()-2; i>=0; i--)
				{
					QVector3D temp = pTempOriginalArray->at(i);
					pData->push_back(pTempArray->at(i));
					pOriginalData->push_back(temp);
				}
			}
			else
			{
				//弹出从0点到nposEnd的点
				for(i = pData->count()-1 ; i>nPosEnd; i--)
				{
					pData->pop_back();
					pOriginalData->pop_back();
				}

				//弹出从nposstart之后的点
				for(i = 0 ; i <nPosStart; i++)
				{
					pData->pop_front();
					pOriginalData->pop_front();
				}

				//重新生成线
				for(i=0; i< pPoints->count()-1; i++)
				{
					pData ->push_front(pPoints->at(i));
					QVector3D temp  = pPoints->at(i);
					pOriginalData->push_front(TransGLtoWGS84(temp));
				}
			}
		}
		else if (nPosStart > nPosEnd)    //方向相反
		{
			bool bModify = m_DrawOrder.isConvert(pPoints,false);

			if(bModify == m_bPositive)     //开始点在结束点前面
			{
				//弹出从0点到nposEnd的点
				for(i = 0 ; i< nPosEnd; i++)
				{
					pData->pop_front();
					pOriginalData->pop_front();
				}

				//弹出从nposstart之后的点
				for(i = pData->count()-1 ; i >(nPosStart - nPosEnd); i--)
				{
					pData->pop_back();
					pOriginalData->pop_back();
				}

				//重新生成线
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
						pTempArray->push_back(pData->at(i));//逆序存到临时数组中
						pTempOriginalArray->push_back(pOriginalData->at(i));
					}
					pData->pop_back();
					pOriginalData->pop_back();
				}
				pData->pop_back();
				pOriginalData->pop_back();

				//加入新编辑的点
				for(i=pPoints->size()-1; i>0; i--)
				{
					QVector3D temp = pPoints->at(i);
					pData->push_back(temp);		
					pOriginalData->push_back(TransGLtoWGS84(temp));
				}
				
				//加入原等值线后半部分的点
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
		msg.setText(tr("裁减后多边形中出现线段相交,请重新裁剪！"));
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
/* 函数功能： 默认构造函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void CModifyContour::MouseRelease(QMouseEvent *event)
{
	return;
}

/************************************************************************/
/* 函数功能： 鼠标离开事件						                        */
/* 参数说明:  NULL														*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 06 月 29 日										*/
/************************************************************************/
void CModifyContour::LeaveEvent()
{
	return;
}

/************************************************************************/
/* 函数功能： 默认构造函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void CModifyContour::setILayerAttrib(ILayerAttribute *Ilayer)
{
	if(NULL != m_pPickObj&&m_bEditStart)
	{
		CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine *>(m_pPickObj);
		CSymbolMultiContourLine *pMultiContourLine = dynamic_cast<CSymbolMultiContourLine *>(m_pPickObj);
		
		//这里有问题啊，连续修改值时候 得到的line 是空的
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
/* 函数功能： 设置属性值                    						    */
/* 参数说明:  NULL														*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 05 月 10 日										*/
/************************************************************************/
void CModifyContour::SetContourAttrib(CSymbolAttribute *pAttribute)
{
	m_layerAttribute->clear();
	QStringList strValue;

	const int nShowValue =(int) pAttribute->GetValue();
	ILayerAttribute *cValue = new ILayerAttribute("cValue","cValue",tr("等值线数值"),nShowValue,QVariant::Double,-10000.00000,10000.00000,strValue);
	m_layerAttribute->insert(QString("cValue"),cValue);

	QStringList strKinds;
	strKinds.append(QString("实线"));
	strKinds.append(QString("短划线"));
	strKinds.append(QString("点线"));
	QString strLineType = m_pAttribute->GetLineType();
	int nLineType = strLineType.toInt();

	ILayerAttribute *cKinds = new ILayerAttribute("cKinds","cKinds",tr("等值线类型"),nLineType,QVariant::StringList,0,0,strKinds);
	m_layerAttribute->insert(QString("cKinds"),cKinds);

	QVector4D tempColor = pAttribute->GetColor();
	tempColor.setX(tempColor.x()*255);
	tempColor.setY(tempColor.y()*255);
	tempColor.setZ(tempColor.z()*255);
	tempColor.setW(tempColor.w());
	QColor temp(tempColor.x(),tempColor.y(),tempColor.z(),tempColor.w() );
	QStringList strColor;
	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor",tr("等值线颜色"),temp,QVariant::Color,0,0,strColor);
	m_layerAttribute->insert(QString("cColor"),cColor);

	QStringList strWidth;
	for (int i=1; i<=10; i++)
	{
		strWidth.append(QString::number(i));
	}
	ILayerAttribute *cWidth = new ILayerAttribute("cWidth","cWidth",tr("等值线线宽"),pAttribute->GetLineWidth(),QVariant::Int,1,10,strWidth);
	m_layerAttribute->insert(QString("cWidth"), cWidth);

	if(m_bClose)
	{
		QStringList strFill;
		strFill.append(tr("否"));
		strFill.append(tr("是"));
		ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill",tr("等值线是否填充"),pAttribute->GetFillType(),QVariant::Bool,0,0,strFill);
		m_layerAttribute->insert(QString("cFill"),cFill);

		QVector4D tempColor = pAttribute->GetFillColor();
		tempColor.setX(tempColor.x()*255);
		tempColor.setY(tempColor.y()*255);
		tempColor.setZ(tempColor.z()*255);
		tempColor.setW(tempColor.w()*255);
		QColor temp(tempColor.x(),tempColor.y(),tempColor.z(),tempColor.w() );
		QStringList strColor;
		ILayerAttribute *cFillColor = new ILayerAttribute("cFillColor","cFillColor",tr("填充颜色"),temp,QVariant::Color,0,0,strColor);
		m_layerAttribute->insert(QString("cFillColor"), cFillColor);
	}
	

}

/************************************************************************/
/* 函数功能： 选中符号						                            */
/* 参数说明:  QRenderable												*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 26 日										*/
/************************************************************************/
void CModifyContour::SelectSymbol(QRenderable *pPickObj)
{
	CSymbolContourLine *pContourLine = dynamic_cast<CSymbolContourLine *>(pPickObj);//等值线
	CSymbolMultiContourLine *pContourMultiLine = dynamic_cast<CSymbolMultiContourLine *>(pPickObj);//等值线
	if(pContourLine)
	{
           pContourLine->SetSelected(true);
		   pContourLine->ReDraw();

		  //----------udno机制-----------------
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

		  //----------udno机制-----------------
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
/* 函数功能： 释放选中的符号						                    */
/* 参数说明:  QRenderable												*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 05 月 10 日										*/
/************************************************************************/
void CModifyContour::ReleaseSymbol(QRenderable *pPickObj)
{
	CSymbolContourLine* pContourLine = dynamic_cast<CSymbolContourLine*>(pPickObj);
	CSymbolMultiContourLine* pMultiContourLine = dynamic_cast<CSymbolMultiContourLine*>(pPickObj);
	if(pContourLine)
	{
		pContourLine->SetSelected(false);
		pContourLine->ReDraw();
		m_pPickObj = NULL;				  //重新初始化拾取对象
		return ;
	}
	if(pMultiContourLine)
	{
		pMultiContourLine->SetSelected(false);
		pMultiContourLine->ReDraw();
		m_pPickObj = NULL;				  //重新初始化拾取对象
		return ;
	}	
}
