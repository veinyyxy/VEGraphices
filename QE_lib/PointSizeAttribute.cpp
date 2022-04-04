#include "PointSizeAttribute.h"


int PointSizeAttribute::Applay()
{
	glPointSize(size);
	return true;
}

PointSizeAttribute::PointSizeAttribute() : type(QE_ATTRI_POINT_SIZE)
{

}

PointSizeAttribute::~PointSizeAttribute()
{
	glPointSize(1.0f);
}

void PointSizeAttribute::Size( GLfloat s )
{
	size = s;
}

GLfloat PointSizeAttribute::Size()
{
	return size;
}
