#pragma once
#include "GIS_WrapperConfig.h"
#include "QT_Include.h"
#include "QDataArray.h"
#include "QRenderUnit.h"
#include "QRenderVolume.h"
#include "QCamera.h"
#include "QViewport.h"
#include "QNode.h"
#include "QLeaf.h"

typedef enum _Direction
{
	POSITIVE_DIRECTION = 1,
	FORWARD_DIRECTION
} TRANSFORM_DIRECTION;



class GIS_WRAPPER_EXPORT_IMPORT CoordinateTransform
{
public:

	CoordinateTransform(void);
	virtual ~CoordinateTransform(void);
	virtual QVector3D Transform(const QVector3D vertex, TRANSFORM_DIRECTION direction = FORWARD_DIRECTION) = 0;
	virtual QVector3D Transform(GLdouble gLx, GLdouble gLy, GLdouble gLz, TRANSFORM_DIRECTION direction = FORWARD_DIRECTION) = 0;
	virtual int Transform(const QVector3DArray& src, QVector3DArray& dst, TRANSFORM_DIRECTION direction = FORWARD_DIRECTION) = 0;
	virtual int Transform(const QVector3D& src, QVector3D& dst) = 0;
	virtual QRectF TransformBoundingBox(const QRectF rectangle, TRANSFORM_DIRECTION direction = FORWARD_DIRECTION) = 0;
	virtual QString ImportFromESRI(const char *  pszFname ) = 0;
	virtual void setDestCRS (QString ProjString) = 0;
	virtual QString getDesCRS() = 0;
	virtual void setSourceCRS (QString ProjString) = 0;
	virtual QString getSourceCRS() = 0;

 	virtual QString getDesProjName() = 0;
 	virtual QString getSourceProjName() = 0;
	virtual void setSourceProjName(QString ProjString) = 0;
	virtual void setDesProjName(QString ProjString) = 0;
 	virtual void initProMap() = 0;
 	virtual void initGeoMap() = 0;
};

