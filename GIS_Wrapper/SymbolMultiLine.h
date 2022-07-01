/*============================================================ 
文件名：SymbolLine.h SymbolLine.cpp 
类 名： CSymbolLine
父 类： QRenderUnit
子 类： 无 
功能说明：传递符号线算法数据数据，收集符号线算法输出数据
调用说明：

---------------------------版本更新---------------------------
V 1.0 
原作者 ：任雪
完成日期：2011年03月13日

V 1.1 
作 者：杨晓宇
完成日期：2011年4月18日
更新说明：加入符号线拾取功能 
============================================================*/
#pragma once
#include <QtCore/QPair>
#include "GIS_WrapperConfig.h"
#include "Line.h"
#include "Symbol.h"
#include "SymbolAttribute.h"
#include "AlgorithmInterface.h"

// class CSymbolLine;
// typedef QPair<CSymbolLine*,CSymbolLine*> SymbolLinePair;
// typedef QList<SymbolLinePair>  SymbolLinePairList;

class GIS_WRAPPER_EXPORT_IMPORT CSymbolMultiLine : public Line //QRenderUnit
{
public:
	CSymbolMultiLine(void);
	~CSymbolMultiLine(void);
	void Render();                                      //调用算法进行图像显示绘制的函数
	void SetSymbolLineStyle(Symbol *pAlgo);                   //设置符号线的算法
	QVector3DArray * GetArray() { return &m_InputArray;} //返回符号线的输入点集数组
	Symbol*  GetLineStyle();                            //获得符号线的类型
	//QVector<QVector3DArray>* GetOutputData();           //返回符号线的输出点集数组
	virtual bool PickUp(const QVector3D* pickPoint);    //晓宇--拾取算法
	int  GetPickIndex();
	void setLineAttributeType(int nType);
	void SetSelected(bool bSelect){m_bHadPicked = bSelect;}//设置线的选中标志位
	void SetPosition(int nPos){m_nPos = nPos;}
	int  GetPosition(){return m_nPos;}
private:
	void DrawSymbolLine2();
public:
	CSymbolAttribute m_LineAttribute;

protected:
	Symbol * m_pSymLine;
	Line *pLine2;
	bool     m_bHadPicked;
	int      m_nPos;                  //拾取到的点的坐标位置
	QVector3DArray m_InputArray;

};


