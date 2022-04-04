#include "Point.h"

Point::Point()
{
    Mode(GL_POINTS);
}

Line* Point::AsLine()
{
	return 0;
}

Plane* Point::AsPlane()
{
	return 0;
}

GISPolygon* Point::AsPolygon()
{
	return 0;
}

Point* Point::AsPoint()
{
	return this;
}

// QVector4DArray* Point::getColorArray()
// {
// 	return 0;
// }

QVector2DArray* Point::getTextureAttray()
{
	return 0;
}

QVector3DArray* Point::getNormalAttray()
{
	return 0;
}

QVector<StationValues>* Point::StationValueArray()
{
	return &m_SValuesArray;
}