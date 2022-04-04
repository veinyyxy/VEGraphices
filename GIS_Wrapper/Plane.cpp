#include "Plane.h"


Plane::Plane(void)
{
	Mode(GL_TRIANGLE_STRIP);
	texture.DataType(GL_TEXTURE_COORD_ARRAY);
}


Plane::~Plane(void)
{
}

QVector4DArray* Plane::getColorArray()
{
	return 0;
}

QVector2DArray* Plane::getTextureAttray()
{
	SetArrayData(&texture);
	return &texture;
}
