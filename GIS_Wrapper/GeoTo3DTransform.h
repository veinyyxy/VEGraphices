#pragma once
#include "GIS_WrapperConfig.h"
#include "CoordinateTransform.h"
class GIS_WRAPPER_EXPORT_IMPORT GeoTo3DTransform :
	public CoordinateTransform
{
public:
	GeoTo3DTransform(void);
	virtual ~GeoTo3DTransform(void);
	virtual QVector3D Transform(const QVector3D vertex, TRANSFORM_DIRECTION direction = FORWARD_DIRECTION);
	virtual QVector3D Transform(GLdouble gLx, GLdouble gLy, GLdouble gLz, TRANSFORM_DIRECTION direction = FORWARD_DIRECTION);
	virtual int Transform(const QVector3DArray& src, QVector3DArray& dst, TRANSFORM_DIRECTION direction = FORWARD_DIRECTION);
	virtual int Transform(const QVector3D& src, QVector3D& dst);
	virtual QRectF TransformBoundingBox(const QRectF rectangle, TRANSFORM_DIRECTION direction = FORWARD_DIRECTION);
	virtual QString ImportFromESRI(const char *  pszFname );
	virtual void setDestCRS (QString ProjString);
	virtual QString getDesCRS();
	virtual void setSourceCRS (QString ProjString);
	virtual QString getSourceCRS();

	virtual QString getDesProjName();
	virtual QString getSourceProjName();
	virtual void setSourceProjName(QString ProjString);
	virtual void setDesProjName(QString ProjString);
	virtual void initProMap();
	virtual void initGeoMap();
private:
	void GeoTo3D(float fIX, float fIY, float fIZ, float* fOX, float* fOY, float* fOZ);
	float m_fEarthR;
};

