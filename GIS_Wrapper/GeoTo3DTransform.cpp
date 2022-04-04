#include "GeoTo3DTransform.h"

#define PI 3.141592653590
#define ANGLE_TO_RADIAN(A1) A1*PI/180
GeoTo3DTransform::GeoTo3DTransform(void) : m_fEarthR(6400.1f)
{
}


GeoTo3DTransform::~GeoTo3DTransform(void)
{
}

QVector3D GeoTo3DTransform::Transform( const QVector3D vertex, TRANSFORM_DIRECTION direction /*= FORWARD_DIRECTION*/ )
{
	float x, y, z;
	GeoTo3D(vertex.x(), vertex.y(), vertex.z(), &x, &y, &z);
	QVector3D transResult(x, y, z);
	return transResult;
}

QVector3D GeoTo3DTransform::Transform( GLdouble gLx, GLdouble gLy, GLdouble gLz, TRANSFORM_DIRECTION direction /*= FORWARD_DIRECTION*/ )
{
	float x, y, z;
	GeoTo3D(gLx, gLy, gLz, &x, &y, &z);
	QVector3D transResult(x, y, z);
	return transResult;
}

int GeoTo3DTransform::Transform( const QVector3DArray& src, QVector3DArray& dst, TRANSFORM_DIRECTION direction /*= FORWARD_DIRECTION*/ )
{
	return -1;
}

int GeoTo3DTransform::Transform( const QVector3D& src, QVector3D& dst )
{
	float x, y, z;
	GeoTo3D(src.x(), src.y(), src.z(), &x, &y, &z);
	dst.setX(x);dst.setY(y);dst.setZ(z);
	return 0;
}

QRectF GeoTo3DTransform::TransformBoundingBox( const QRectF rectangle, TRANSFORM_DIRECTION direction /*= FORWARD_DIRECTION*/ )
{
	QRectF planeRect(0, 0, m_fEarthR, m_fEarthR);
	return planeRect;
}

QString GeoTo3DTransform::ImportFromESRI( const char * pszFname )
{
	QString notThis("Do not need this.");
	return notThis;
}

void GeoTo3DTransform::setDestCRS( QString ProjString )
{

}

QString GeoTo3DTransform::getDesCRS()
{
	QString notThis("Do not need this.");
	return notThis;
}

void GeoTo3DTransform::setSourceCRS( QString ProjString )
{

}

QString GeoTo3DTransform::getSourceCRS()
{
	QString notThis("Do not need this.");
	return notThis;
}

QString GeoTo3DTransform::getDesProjName()
{
	QString notThis("Do not need this.");
	return notThis;
}

QString GeoTo3DTransform::getSourceProjName()
{
	QString notThis("Do not need this.");
	return notThis;
}

void GeoTo3DTransform::setSourceProjName( QString ProjString )
{

}

void GeoTo3DTransform::setDesProjName( QString ProjString )
{

}

void GeoTo3DTransform::initProMap()
{

}

void GeoTo3DTransform::initGeoMap()
{

}

void GeoTo3DTransform::GeoTo3D( float fIX, float fIY, float fIZ, float* fOX, float* fOY, float* fOZ )
{
	float fNewR =  m_fEarthR + fIZ;
	/**fOX = m_fEarthR * sin(ANGLE_TO_RADIAN(fIX)) * cos(ANGLE_TO_RADIAN(fIY));
	*fOY = m_fEarthR * sin(ANGLE_TO_RADIAN(fIX)) * sin(ANGLE_TO_RADIAN(fIY));
	*fOZ = m_fEarthR* cos(ANGLE_TO_RADIAN(fIX));*/
	*fOX = - fNewR * sin(fIX / 180 * 3.14159264) * cos(fIY / 180 * 3.14159264);
	*fOY = fNewR * cos(fIX / 180 * 3.14159264) * cos(fIY / 180 * 3.14159264);
	*fOZ = fNewR * sin(fIY / 180 * 3.14159264);
}
