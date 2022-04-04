
/*============================================================ 
文件名： DrawContourArea.h DrawContourArea.cpp 
类 名：  DrawContourArea 
父 类：  ManualDraw(基类) 
子 类： 无 
功能说明：  
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
#include <QMap>
#include "manualdraw.h"
#include "Line.h"
#include "SymbolContourLine.h"
#include "CardinalSpline.h"
//#include "UndoAddDrawCommand.h"
#include "SymbolLayer.h"
#include "SymbolAttribute.h"
#include "CLineAnalysis.h"


typedef QMap<int, float > CoutourZArray;			//Z值序列

class GIS_WRAPPER_EXPORT_IMPORT DrawContourArea :public ManualDraw
{
public:
//	DrawContourArea(CONTOUR_TYPE type);
	DrawContourArea(Layer::ELayerType type);
	virtual ~DrawContourArea(void);	

	void MouseEvent(QMouseEvent* event) ;
	void MousePress(QMouseEvent *event) ;
	void MouseRelease(QMouseEvent *event);
	void LeaveEvent();
	void SetSymbol(Symbol *pSymbol) {m_Symbol = pSymbol;} ;
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void SetParentLayer(Layer * Layer){m_ParentLayer = Layer;}
	bool GetContourAttribute(double index);
	void SetCurrentColorTable(QString str);
	void InitAttribute(bool b);
	void AddTempCountorLine();
	void AddRainCountorLine();
	void AddRainYUBAOCountorLine();

private:
	void InitColorTable();
	void InitCoutourArray();
	//绘制控制点程序
	void DrawCrossArray(QVector3DArray&  InputData);
	void ClearCrossArray(QVector3DArray&  InputData);
private:
	Line  *pline;
	bool  m_bCloseLine;			//等值线闭合与否的标值
	QVector3DArray  *pArray ;
	QVector3DArray  m_InputData, m_OutputData,m_OutputData2,m_mergeData;
	QVector3DArray  *pOriginalArray;	
	CSymbolContourLine *m_pContourLine;	
	QVector<QVector3DArray> *pOutputData;
	QVector<QVector3DArray>  m_CrossPoints;
	CCardinalSpline  *m_pSpLine;
	SymbolData       *m_pSymbolData;
	int   m_iLastCount;
	static int  m_nCount;
	float  m_fStep;

//	CONTOUR_TYPE m_areaType;
	Layer::ELayerType m_layertype;
	bool isCanDraw;
	COLORTABLE m_currentColorTable;	
	QStringList m_stringitems;
//	QMap<double,QColor> m_currentcolorindex;
	COLORITEM_TABLE m_currentcolorindex;
	QMap<QString,double> m_currentmap;
	CoutourZArray m_CoutourZArray;	
	CLineAnalysis m_lineanalysis;
	QColor m_LineColor;				//等值线的边界颜色
	QVector<Line*> m_CrossArray;    //绘制十字线功能
};

