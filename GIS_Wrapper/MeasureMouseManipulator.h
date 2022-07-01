
/*============================================================ 
文件名： MeasureMouseManipulator.h MeasureMouseManipulator.cpp 
类 名：  MeasureMouseManipulator 
父 类：  ManualDraw(基类) 
子 类： 无 

---------------------------版本更新---------------------------

============================================================*/ 
#pragma once
#include <QtWidgets/QMessageBox>
#include "manualdraw.h"
#include "Line.h"
#include "SymbolLayer.h"
#include "SymbolAttribute.h"

class GIS_WRAPPER_EXPORT_IMPORT MeasureMouseManipulator :public ManualDraw
{
public:
	MeasureMouseManipulator(void);
	MeasureMouseManipulator(bool bclose);
	virtual ~MeasureMouseManipulator(void);	

	void MouseEvent(QMouseEvent* event)  ;
	void MousePress(QMouseEvent *event) ;
	void MouseRelease(QMouseEvent *event);
	void LeaveEvent();
	void SetSymbol(Symbol *pSymbol) {m_Symbol = pSymbol;} ;
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void SetParentLayer(Layer * Layer){m_ParentLayer = Layer;}
	void GetContourAttribute();	
	inline double CalGeoDistance(QVector3D &pos1,QVector3D &pos2);

private:
	Line  *pline, *pCrossLine;
	bool  m_bCloseLine;	//等值线闭合与否的标值
	QVector3DArray  *pArray ;
	QVector3DArray  *pOriginalArray;	
	QVector<QVector3DArray> *pOutputData;
	int   m_iLastCount;

	QVector3D glPosOld,glPosCurrent;
	QVector3D GeoPosOld,GeoPosCurrent;
	double m_distance;
	QMessageBox msgBox;
};

