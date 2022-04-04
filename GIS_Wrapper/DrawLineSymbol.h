/*============================================================ 
文件名：DrawLineSymbol.h DrawLineSymbol.cpp 
类 名： CDrawLineSymbol
父 类： ManualDraw
子 类： 无 
功能说明：画矢量符号线的算法
调用说明：
1、获取用户的PING命令鼠标坐标点，调用下层算法，返回计算得到的点集；
2、将屏幕点转换成OPENGL的坐标，并传给相应的绘图管道；

--------------------------版本更新----------------------------
V 1.2 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：任雪
完成日期：2011年7月27日
============================================================*/


#pragma once
#include "manualdraw.h"
#include "Line.h"
#include "AlgorithmInterface.h"
#include "SymbolLine.h"
#include "CardinalSpline.h"
//#include "UndoAddDrawCommand.h"
#include "SymbolLayer.h"

class GIS_WRAPPER_EXPORT_IMPORT CDrawLineSymbol :	public ManualDraw
{
public:
	CDrawLineSymbol(int nType);
	CDrawLineSymbol(int nType,CoordinateTransform* proj);
	~CDrawLineSymbol(void);

	void MouseEvent(QMouseEvent* event);
	void MousePress(QMouseEvent *event);
	void MouseRelease(QMouseEvent *event);
	void LeaveEvent();
	void SetSymbol(Symbol *pSymbol) {m_Symbol = pSymbol;}
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void InitAttribute(int nType);
	void SetParentLayer(Layer * Layer){m_ParentLayer = Layer;}
	void GetSymbolAttribute();
	void SetCoordinateTransform(CoordinateTransform* proj){m_proj = dynamic_cast<RadProjTransform*> (proj);}
private:
	int    m_nType;	
	Line   *m_pDisplayLine;	
	bool   m_bFinished, m_bFirst;			//判断是否结束画线
	QVector3DArray  *m_pArray ;
	QVector3DArray  m_InputData, m_OutputData;
	CSymbolLine     *m_pSymbolLine;
	SymbolData      *m_pSymboldata;	
	CCardinalSpline *m_pSpLine;
	RadProjTransform* m_proj;
	bool cardline;
	//int m_OutLineNum;//记录输出数组的线的个数
	//QPoint lastPos;	
	//bool m_bFirst;
	//QVector<QVector3DArray> *pOutputData;
};

