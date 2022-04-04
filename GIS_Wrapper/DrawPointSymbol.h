#pragma once
#include "manualdraw.h"
#include "Line.h"
//#include "Algorithm.h"
#include "SymbolPoint.h"
//#include "UndoAddPointCommand.h"
#include "SymbolLayer.h"
#include "SymbolData.h"

class GIS_WRAPPER_EXPORT_IMPORT CDrawPointSymbol :public ManualDraw
{
public:
	CDrawPointSymbol(void);
	~CDrawPointSymbol(void);

	void MouseEvent(QMouseEvent* event)  ;
	void MousePress(QMouseEvent *event) ;
	void MouseRelease(QMouseEvent *event);
	void LeaveEvent();
	void SetSymbol(Symbol *pSymbol) {m_Symbol = pSymbol;} ;
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void SetParentLayer(Layer * Layer){m_ParentLayer = Layer;}
private:
	QPoint lastPos;
	QRenderVolume* drawVol;
	Line* line;
	bool  bFinished ;			//判断是否结束画线
	QVector3DArray *pArray ;
	QVector3DArray InputData;
	SymbolPoint *symbolpoint;
	//bool m_bFirst;				//判断是否第一次绘制
	QVector3DArray * pOriginalArray;
	SymbolData *m_pSymbolData;
	QVector<SymbolPoint*> m_pSymbolPoint;
};
