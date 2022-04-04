
/*============================================================ 
�ļ����� DrawArea.h DrawrArea.cpp 
�� ����  DrawArea 
�� �ࣺ  ManualDraw(����) 
�� �ࣺ �� 
����˵����  
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
#include <QMap>
#include "manualdraw.h"
#include "Line.h"
#include "SymbolContourLine.h"
#include "SymbolMultiContourLine.h"
#include "CardinalSpline.h"
//#include "UndoAddDrawCommand.h"
//#include "UndoAddDrawMultiCommand.h"
#include "SymbolLayer.h"
#include "SymbolAttribute.h"
#include "CLineAnalysis.h"

// typedef QMap<QString,QColor> COLORLIST;
// static COLORLIST m_colorlist;

class GIS_WRAPPER_EXPORT_IMPORT DrawArea :public ManualDraw
{
public:
	DrawArea(QString str);
	virtual ~DrawArea(void);	

	void MouseEvent(QMouseEvent* event) ;
	void MousePress(QMouseEvent *event) ;
	void MouseRelease(QMouseEvent *event);
	void LeaveEvent();
	void SetSymbol(Symbol *pSymbol) {m_Symbol = pSymbol;} ;	
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void SetParentLayer(Layer * Layer){m_ParentLayer = Layer;}
//	bool GetContourAttribute(double index);
	void GetCrossLineData(QVector3D scPoint, int i);
	void GetAttribute(CSymbolMultiContourLine *pContourLine);
	void SetLayerType(Layer::ELayerType type);
private:
	void InitColorlist();
	void SetCurrentColorTable(QString str);
//	void InitCoutourArray();
private:
	Line  *pline, *pCrossLine;
	bool  m_bCloseLine;			//��ֵ�߱պ����ı�ֵ
	QVector3DArray  *pArray ;
	QVector3DArray  m_InputData, m_OutputData,m_OutputData2,m_mergeData;
	QVector3DArray  *pOriginalArray;	
//	CSymbolContourLine *m_pContourLine;
	CSymbolMultiContourLine *m_pMultiContourLine;
	QVector<QVector3DArray> *pOutputData;
	QVector<QVector3DArray>  m_CrossPoints;
	CCardinalSpline  *m_pSpLine;
	CLineAnalysis m_lineanalysis;
	SymbolData       *m_pSymbolData;
	int   m_iLastCount;
	static int  m_nCount;
	float  m_fStep;
	QString m_strname;	
	QMap<QString,QColor> m_colorlist;
	QMap<QString,double> m_strkeymap;
//	QMap<double,QColor> m_currentcolorindex;
	COLORITEM_TABLE m_currentcolorindex;
	COLORTABLE m_curcolortable;
	Layer::ELayerType m_layertype;
};

