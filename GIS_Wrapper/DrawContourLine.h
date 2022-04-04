
/*============================================================ 
�ļ����� DrawContourLine.h DrawContourLine.cpp 
�� ����  DrawContourLine 
�� �ࣺ  ManualDraw(����) 
�� �ࣺ �� 
����˵���� ʵ�ֵ�ֵ�ߵ��ֻ湦��
����˵���� �����˵���������õ�ֵ�ߵ����Գ�Ա��
---------------------------�汾����---------------------------
V 0.1
�� ��: ¬�ߕN
�������: 2011-04-07 
����˵��: ɾ�����෽�������ƽ���㷨��������ֵ��ɫ
V 0.2
�� ��: renxue
�������: 2011-04-26 
����˵��: ���������࣬ʹ֮������CDrawLineSymbol
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
	bool  m_bCloseLine;			//��ֵ�߱պ����ı�ֵ
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

