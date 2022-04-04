/*============================================================ 
项目名：CIPAS 
作 者： 卢高昇	
单 位： css
最后修改时间： 2011年3月17日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许
可不得擅自传播。
2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名： ModifyContour.h ModifyContour.cpp 
类 名：  CModifyContour 
父 类：  ManualDraw(基类) 
子 类： 无 
功能说明： 修改等值线
调用说明： 类调用说明，可设置等值线的属性成员。
-------------------------------------版本更新------------------------------------------------ 
V 0.1
作 者: 
完成日期: 
更新说明: 
----------------------------------------------------------------------------------------
V 0.0 
作 者：
完成日期：
更新说明： 
============================================================*/ 

#pragma once
#include "manualdraw.h"
//#include "UndoSymbolLineMoveCommand.h"
//#include "UndoContourEditCommand.h"
//#include "UndoMultiContourEditCommand.h"
//#include "UndoDeleteDrawCommand.h"
//#include "UndoDelPointCommand.h"
//#include "UndoSymbolPointMoveCommand.h"
#include "CardinalSpline.h"
#include "DrawOrder.h"
#include "CLineAnalysis.h"
#include "SymbolMultiContourLine.h"

class GIS_WRAPPER_EXPORT_IMPORT CModifyContour :public ManualDraw
{
	Q_OBJECT
public:
	CModifyContour(void);
	~CModifyContour(void);
	void MouseEvent(QMouseEvent* event);
	void MousePress(QMouseEvent *event);
	void MouseRelease(QMouseEvent *event);
	void LeaveEvent();
	void MouseDoubleClick(QMouseEvent *event);			//鼠标双击事件

	void SetSymbol(Symbol *pSymbol) {m_Symbol = pSymbol;}
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void SetContourAttrib(CSymbolAttribute *pAttribute);//设置属性值
	bool EditSymbol(QRenderable *pPickObj,QVector3DArray *pPoint, int nPosStart, int nPosEnd); //编辑等值线
	//bool EditSymbol(QRenderable *pPickObj,QVector3DArray *pPoint, int nPosStart, int nPosEnd); //编辑等值线
private:
	void SelectSymbol(QRenderable *pPickObj);
	void ReleaseSymbol(QRenderable *pPickObj);
	void DrawControlLine();								//绘制控制线
	void ClearControlLine();							//移除控制线
	void DrawCrossArray(QVector3DArray&  InputData);
	void ClearCrossArray(QVector3DArray&  InputData);
	bool CheckDirection();								//检索编辑线与等值线是否
signals:
	void UpdateAttribute(ManualDraw *);                 //
private:
	QRenderable* m_pPickObj;							//得到当前拾取对象，编辑对象
	//QRenderable* m_pPickObj, *m_pEditObj;				//得到当前拾取对象，编辑对象
	//CUndoSymbolPointMoveCommand* m_pUndoSymbolPointEdit;
	//CUndoSymbolLineMoveCommand* m_pUndoLineMoveEdit;
	//CUndoContourEditCommand*    m_pUndoContourEdit;
	//CUndoContourEditCommand* m_pUndoContourEdit2;
	//CUndoMultiContourEditCommand* m_pUndoMultiContourEdit;
	QVector3DArray m_PressPoints, m_SmoothPoints,m_SmoothPointsTemp;       //收集的编辑点
	CCardinalSpline m_SpLine;                           //平滑算法
	bool m_bEditStart;
	int nPosStart, nPosEnd;
	Line *m_pShowLine;
	QVector3DArray *pArray;                             //Line的顶点指针
	QVector4DArray *pColor;                             //Line的颜色指针
	CSymbolAttribute *m_pAttribute;		                //符号属性值
	bool m_bClose;										//区分封合线
	float m_currentZ;									//当前拾取对象的Z值
	CLineAnalysis m_lineanalysis;
	QVector<Line*> m_CrossArray;
	QVector3D m_lastScreen;								//最后的控制点
	bool m_bPositive;
	CDrawOrder m_DrawOrder;								//判断顺时针对象
};

