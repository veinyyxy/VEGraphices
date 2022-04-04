/*============================================================ 
项目名：CIPAS 
作 者： 杨东	
单 位： css
最后修改时间： 2011年3月19日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许
可不得擅自传播。
2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名： DrawNormalSymbol.h DrawNormalSymbol.cpp 
类 名：  DrawNormalSymbol 
父 类：  ManualDraw(基类) 
子 类： 无 
功能说明： 实现槽线的手绘功能
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
#include "Line.h"
#include "AlgorithmInterface.h"
#include "SymbolLine.h"
#include "CardinalSpline.h"
#include "SymbolPoint.h"
#include "NormalSymbol.h"
#include "SymbolLayer.h"
#include "SymbolAttribute.h"
//#include "UndoAddPointCommand.h"

class GIS_WRAPPER_EXPORT_IMPORT DrawNormalSymbol : public ManualDraw
{
public:
	DrawNormalSymbol(void);
	~DrawNormalSymbol(void);

	void MouseEvent(QMouseEvent* event);
	void MousePress(QMouseEvent *event);
	void MouseRelease(QMouseEvent *event);
	void LeaveEvent();
	void SetSymbol(Symbol *pSymbol) {m_Symbol = pSymbol;}
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void initAttribute(int nType);
	void SetParentLayer(Layer * Layer){m_ParentLayer = Layer;}
	void GetAttribute();

private:
	QPoint lastPos;
	Line  *plineSymbol, *line;
	bool  bFinished ;			//判断是否结束画线
	QVector3DArray *pArray ;
	QVector3DArray m_InputData, m_OutputData;
	Line *m_symbolLine;
	QVector<QVector3DArray> *pOutputData;
	int m_OutLineNum;//记录输出数组的线的个数
	bool m_bFirst;
	CCardinalSpline * m_pSpLine;
	SymbolPoint *symbolpoint0;
	SymbolPoint *symbolpoint1;
	SymbolPoint *symbolpoint2;
	SymbolPoint *symbolpoint3;
	SymbolPoint *symbolpoint4;
	SymbolPoint *symbolpoint5;
	SymbolPoint *symbolpoint6;
	LAYER_ATTRIBUTE *m_CSlayerAttribute;
	ILayerAttribute *m_nLayerAttr;
	bool b1;
	bool b2;
	NormalSymbol *symbolpoint;
	SymbolData * symboldata;
	SymbolLayer *nsLayer;
	QVector<NormalSymbol*> ns;
	CSymbolAttribute *pAttri;
};