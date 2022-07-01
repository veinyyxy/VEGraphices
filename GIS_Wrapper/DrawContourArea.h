
/*============================================================ 
�ļ����� DrawContourArea.h DrawContourArea.cpp 
�� ����  DrawContourArea 
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
#include <QtCore/QMap>
#include "manualdraw.h"
#include "Line.h"
#include "SymbolContourLine.h"
#include "CardinalSpline.h"
//#include "UndoAddDrawCommand.h"
#include "SymbolLayer.h"
#include "SymbolAttribute.h"
#include "CLineAnalysis.h"


typedef QMap<int, float > CoutourZArray;			//Zֵ����

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
	//���ƿ��Ƶ����
	void DrawCrossArray(QVector3DArray&  InputData);
	void ClearCrossArray(QVector3DArray&  InputData);
private:
	Line  *pline;
	bool  m_bCloseLine;			//��ֵ�߱պ����ı�ֵ
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
	QColor m_LineColor;				//��ֵ�ߵı߽���ɫ
	QVector<Line*> m_CrossArray;    //����ʮ���߹���
};

