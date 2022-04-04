#pragma once
#include "SymbolLine.h"
#include "SymbolMultiLine.h"
#include "SymbolPoint.h"
#include "SymbolContourLine.h"
#include "SymbolMultiContourLine.h"
#include "NormalSymbol.h"
//#include "QTextureFont.h"



class GIS_WRAPPER_EXPORT_IMPORT SymbolData
{
public:
	SymbolData(void);
	~SymbolData(void);
	
	inline void setNormalSymbol(QVector<NormalSymbol*> ns){vectorNormalSymbol = ns;}
	inline QVector<NormalSymbol*> *getNormalSymbol(){return &vectorNormalSymbol;}
	inline void setSymbolPoint(QVector<SymbolPoint*> sp){vectorSymbolPoint = sp;}
	inline QVector<SymbolPoint*> *getSymbolPoint(){return &vectorSymbolPoint;}
	inline void setSymbolLine(QVector<CSymbolLine*> sl){vectorSymbolLine = sl;}
	inline QVector<CSymbolLine*> *getSymbolLine(){return &vectorSymbolLine;}
	inline void setSymbolMultiLine(QVector<CSymbolMultiLine*> sl){vectorSymbolMultiLine = sl;}
	inline QVector<CSymbolMultiLine*> *getSymbolMultiLine(){return &vectorSymbolMultiLine;}

	inline void setSymbolContourLine(QVector<CSymbolContourLine*> scl){vectorSymbolContourLine = scl;}
	inline QVector<CSymbolContourLine*> *getSymbolContourLine(){return &vectorSymbolContourLine;}
	inline void setSymbolMultiContourLine(QVector<CSymbolMultiContourLine*> scl){vectorSymbolMultiContourLine = scl;}
	inline QVector<CSymbolMultiContourLine*> *getSymbolMultiContourLine(){return &vectorSymbolMultiContourLine;}

	//inline void setMarkSymbol(QVector<QTextureFont*> sm) {vectorFontSymbol = sm;}
	//inline QVector<QTextureFont*> *getMarkSymbol() {return &vectorFontSymbol;}

	inline void setTroughLine(QVector<Line*> line){vectorTroughLine = line;}
	inline QVector<Line*> *getTroughLine(){return &vectorTroughLine;}

private:
	QVector<NormalSymbol*> vectorNormalSymbol;
	QVector<SymbolPoint*> vectorSymbolPoint;
	QVector<CSymbolLine*> vectorSymbolLine;
	QVector<CSymbolMultiLine*> vectorSymbolMultiLine;
	QVector<CSymbolContourLine*> vectorSymbolContourLine;
	QVector<CSymbolMultiContourLine*> vectorSymbolMultiContourLine;
	QVector<Line*> vectorTroughLine;
	//QVector<QTextureFont*> vectorFontSymbol;
};

