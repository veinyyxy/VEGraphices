#pragma once
#include "GIS_WrapperConfig.h"
#include "geometry.h"
class GIS_WRAPPER_EXPORT_IMPORT Plane :
	public Geometry
{
public:
	Plane(void);
	~Plane(void);
	inline QVector4DArray* getColorArray();
	inline QVector2DArray* getTextureAttray();
	
	inline QVector3DArray* getNormalAttray(){return 0;}
	Line* AsLine(){return 0;}
	Plane* AsPlane(){return this;}
	GISPolygon* AsPolygon(){return 0;}
	Point* AsPoint(){return 0;}
private:
	//QVector4DArray colorAttr;

	QVector2DArray texture;
	//QVector3DArray normal;
};

