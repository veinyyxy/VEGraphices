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
文件名：ResizeSymbol.h ResizeSymbol.cpp 
类 名： CResizeSymbol
父 类： NULL
子 类： 无 
功能说明： 拾取修改类
调用说明： 

-------------------------------------版本更新------------------------------------------------ 
V 1.0 
原作者 ： 
完成日期：2011年？月？日
============================================================*/

#pragma once
#include "manualdraw.h"
//#include "UndoSymbolPointMoveCommand.h"
//#include "UndoSymbolMarkMoveCommand.h"
#include "CardinalSpline.h"
#include "SymbolAttribute.h"
//#include "UndoSymbolLineMoveCommand.h"
//#include "UndoDeleteDrawCommand.h"
//#include "UndoDelPointCommand.h"


#define BUFSIZE 1024						//设定选择模式下缓冲区大小
class GIS_WRAPPER_EXPORT_IMPORT CScaleSymbolLine :
	public ManualDraw
{
	Q_OBJECT
public:
	CScaleSymbolLine(void);
	virtual ~CScaleSymbolLine(void);
	void MouseEvent(QMouseEvent* event);				//鼠标移动事件
	void MousePress(QMouseEvent *event);				//鼠标按下事件
	void MouseRelease(QMouseEvent *event);				//鼠标释放事件
	void MouseDoubleClick(QMouseEvent *event);			//鼠标双击事件
	void LeaveEvent();
    void keyPressEvent(QKeyEvent * event );				//按下键盘事件
	void SetSymbol(Symbol *pSymbol);
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void SetContourAttrib(CSymbolAttribute *pAttribute);//设置属性值
	//void GetContourAttribute();                         //获得等值线的属性值
	void SelectSymbol(QRenderable *pPickObj);                         //选中符号
	void ReleaseSymbol(QRenderable *pPickObj);                        //释放选中的符号
	void DeleteSymbol(QRenderable *pPickObj);                         //删除符号 	
private:
	inline void DrawRect(Line* rect,QVector3D& glpos,int srcD,QVector4D color);
	inline void DrawSymbolLineBound(QRenderable* m_pPickObj,QVector4D color);
	
private:
	bool SelectSymbolBound(QRenderable *pPickObj);
	void ResizeSymbol(QRenderable *pPickObj,float scale);
	inline QVector3D GetScalePoint(QVector3D glCenter,QVector3D glPosOrigin,float scale);
signals:
	void UpdateAttribute(ManualDraw *);                 //鼠标在图层上释放事件

private:
	Line *m_pShowLine;
	QVector3DArray *pArray;                             //Line的顶点指针
	QVector4DArray *pColor;                             //Line的颜色指针
	QRenderable* m_pPickObj, *m_pEditObj;				//得到当前拾取对象，编辑对象
	QRenderable* m_pPickObjShade;
	bool m_bCanMove;									//设置是否移动点
	int  m_iFlag;										//设置鼠标样式 1 为可以拖动
	bool m_bEdit;										//可以编辑符号
	
	QVector3DArray m_PressPoints, m_SmoothPoints;       //收集的编辑点
	CCardinalSpline m_SpLine;                           //平滑算法
	int nPosStart, nPosEnd;
	QVector3DArray *pTempArray, *pTempOriginalArray;
	CSymbolAttribute *m_pAttribute;		                //符号属性值
	QVector4D  SourceColor;

	//CUndoSymbolPointMoveCommand* m_pUndoSymbolPointEdit;
	//CUndoSymbolLineMoveCommand* m_pUndoLineMoveEdit;
	//CUndoSymbolMarkMoveCommand* m_pundoSymbolMarkEdit;
	float m_fXMoveDistance, m_fYMoveDistance;

private:
	QVector3D m_ScOldPoint,m_ScCurrentPoint;
	bool isLmousePressing;
	Line *m_pRectTemp;	
	Line *m_rect1;
	Line *m_rect2;
	Line *m_rect3;
	Line *m_rect4;
	Line *m_rect5;
	Line *m_rect6;
	Line *m_rect7;
	Line *m_rect8;

	
};

