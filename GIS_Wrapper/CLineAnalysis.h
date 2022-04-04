#pragma once
#include "GIS_WrapperConfig.h"
#include "Line.h"

class GIS_WRAPPER_EXPORT_IMPORT CLineAnalysis
{
public:
	CLineAnalysis(){}
	~CLineAnalysis(){}
public:
	bool isPolygonintersect(QVector3DArray &dataArray);
	inline bool isLineintersect(float x0,float x1,float x2,float x3,float y0,float y1,float y2,float y3);

};