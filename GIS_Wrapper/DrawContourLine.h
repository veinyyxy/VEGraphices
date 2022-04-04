
/*============================================================ 
文件名： DrawContourLine.h DrawContourLine.cpp 
类 名：  DrawContourLine 
父 类：  ManualDraw(基类) 
子 类： 无 
功能说明： 实现等值线的手绘功能
调用说明： 类调用说明，可设置等值线的属性成员。
---------------------------版本更新---------------------------
V 0.1
作 者: 卢高昇
完成日期: 2011-04-07 
更新说明: 删除多余方法、添加平滑算法、调整标值颜色
V 0.2
作 者: renxue
完成日期: 2011-04-26 
更新说明: 调整整个类，使之类似于CDrawLineSymbol
============================================================*/ 
#pragma once
#include "manualdraw.h"
#include "Line.h"
#include "SymbolContourLine.h"
#include "CardinalSpline.h"
//#include "UndoAddDrawCommand.h"
#include "SymbolLayer.h"
#include "SymbolAttribute.h"
#include "CLineAnalysis.h"
#include "CPolygonAnalysis.h"

class GIS_WRAPPER_EXPORT_IMPORT DrawContourLine :public ManualDraw
{
public:
	DrawContourLine(void);
	DrawContourLine(bool bclose);
	virtual ~DrawContourLine(void);	

	void MouseEvent(QMouseEvent* event)  ;
	void MousePress(QMouseEvent *event) ;
	void MouseRelease(QMouseEvent *event);
	void LeaveEvent();
	void SetSymbol(Symbol *pSymbol) {m_Symbol = pSymbol;} ;
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void SetParentLayer(Layer * Layer){m_ParentLayer = Layer;}
	void GetContourAttribute();
	void GetCrossLineData(QVector3D scPoint, int i);
private:
	void DrawCrossArray(QVector3DArray&  InputData);
	void ClearCrossArray(QVector3DArray&  InputData);
private:
	Line  *pline, *pCrossLine;
	QVector<Line*> m_CrossArray;
	bool  m_bCloseLine;			//等值线闭合与否的标值
	QVector3DArray  *pArray ;
	QVector3DArray  m_InputData, m_OutputData;
	QVector3DArray  *pOriginalArray;	
	CSymbolContourLine *m_pContourLine;
	QVector<QVector3DArray> *pOutputData;
	QVector<QVector3DArray>  m_CrossPoints;
	CCardinalSpline  *m_pSpLine;
	SymbolData       *m_pSymbolData;
	/*QVector<CSymbolContourLine*> symbolContourLine;*/
	int   m_iLastCount;
	static int  m_nCount;
	float  m_fStep;	
	CLineAnalysis m_lineanalysis;
	CPolygonAnalysis m_polygonalysis;
};

