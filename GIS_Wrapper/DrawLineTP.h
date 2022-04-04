#pragma once
#include "manualdraw.h"
#include "Line.h"
//#include "..\PluginSymbol\AlgorithmInterface.h"
#include "SymbolLine.h"
//#include "UndoAddDrawCommand.h"
#include "SymbolLayer.h"
#include "CardinalSpline.h"
#include "RadProjTransform.h"
class GIS_WRAPPER_EXPORT_IMPORT DrawLineTP :public ManualDraw
{
	Q_OBJECT

signals:
	void DrawProAction();
public:
	DrawLineTP(int nType);
	DrawLineTP(int nType,CoordinateTransform* proj,bool otp);
	~DrawLineTP(void);

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
	QVector3DArray* getPoint(){return p;}
private:

	int    m_nType;	
	Line   *m_pDisplayLine;	
	bool   m_bFinished, m_bFirst;			//≈–∂œ «∑ÒΩ· ¯ª≠œﬂ
	QVector3DArray  *m_pArray ;
	QVector3DArray  m_InputData, m_OutputData;
	CSymbolLine     *m_pSymbolLine;
	SymbolData      *m_pSymboldata;	
	CCardinalSpline *m_pSpLine;
	int m_pressNum;
	RadProjTransform* m_proj;
	bool OnlydrawTP;
	QVector3D p1;
	QVector3D p2;
	QVector3D central;
	QVector3DArray *p;
};

