#pragma once
#include "manualdraw.h"
#include "Line.h"
//#include "..\PluginSymbol\AlgorithmInterface.h"
#include "SymbolLine.h"
//#include "UndoAddDrawCommand.h"
#include "SymbolLayer.h"
#include "CardinalSpline.h"
#include "RadProjTransform.h"
#include "QEText.h"
class GIS_WRAPPER_EXPORT_IMPORT DrawLineDistance : public ManualDraw
{
public:
	DrawLineDistance(int nType,CoordinateTransform* proj);
	~DrawLineDistance(void);

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
	RadProjTransform* m_proj;
	int    m_nType;	
	Line   *m_pDisplayLine;	
	bool   m_bFinished, m_bFirst;			//≈–∂œ «∑ÒΩ· ¯ª≠œﬂ
	QVector3DArray  *m_pArray ;
	QVector3DArray  m_InputData, m_OutputData;
	CSymbolLine     *m_pSymbolLine;
	SymbolData      *m_pSymboldata;	
	CCardinalSpline *m_pSpLine;
	QEText* pText;
};

