#pragma once
/*============================================================ 
项目名：CIPAS 
作 者：  赵高攀	
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
文件名： DrawWindSymbol.h DrawWindSymbol.cpp 
类 名： CDrawWindSymbol 
父 类：  ManualDraw(基类) 
子 类： 无 
功能说明： 实现风标绘制功能
调用说明： 类调用说明，风标贴图旋转
-------------------------------------版本更新------------------------------------------------ 
V 0.1 
作 者：赵高攀
完成日期：
更新说明： 
============================================================*/ 
#include "ManualDraw.h"
//#include "QTextureFont.h"
#include <stdlib.h>
#include "SymbolPoint.h"
#include "ColorAttribute.h"
//#include "UndoAddPointCommand.h"
#include "SymbolData.h"
#include "SymbolLayer.h"

class GIS_WRAPPER_EXPORT_IMPORT CDrawWindSymbol: public ManualDraw
{
public:
	CDrawWindSymbol(void);
	~CDrawWindSymbol(void);
	//鼠标事件响应
	virtual void MouseEvent(QMouseEvent* event);	//鼠标移动，计算旋转角度，画线，贴出旋转角度
	virtual void MousePress(QMouseEvent *event);	//左键开始绘制小圆圈，右键按角度贴风标
	virtual void MouseRelease(QMouseEvent *event);	//
	void LeaveEvent();
	void SetSymbol(Symbol *pSymbol);
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void SetTextOnLine(double dAngle);									//画线时贴出文字
	double CalculateAngle();
private :
	Line *m_Circular;						//圆
	Line m_line;							//画线
	//QTextureFont *m_text;					//文本
	bool m_bStart;							//是否开始绘制
	QVector3D m_CircularCenter;				//the center of cricle，存储线段起点
	QVector3D m_LineEndPoint;				//存储线段终点
	SymbolPoint *m_symbolpoint;				//贴图类指针
	GLfloat m_fAngle;						//贴图旋转角度
	bool m_bFirst;							//决断是否第一次
	ColorAttribute * m_pColorAttri;			//颜色属性
	SymbolData *m_nSymbolData;
	SymbolLayer *m_nSymbolLayer;
};

