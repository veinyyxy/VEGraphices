#include "ColorAttribute.h"

int ColorAttribute::Applay()
{
	GLfloat c[4] = {0.0f};
	glGetFloatv(GL_CURRENT_COLOR, c);
	oldColor.setX(c[0]);
	oldColor.setY(c[1]);
	oldColor.setZ(c[2]);
	oldColor.setW(c[3]);
	glColor4f(color.x(), color.y(), color.z(), color.w());
	return true;
}

ColorAttribute::ColorAttribute() : type(QE_ATTRI_COLOR)
{

}

void ColorAttribute::Color( const QVector4D& c )
{
	color = c;
}

QVector4D ColorAttribute::Color()
{
	return color;
}
