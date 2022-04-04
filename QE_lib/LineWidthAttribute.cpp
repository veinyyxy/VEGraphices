#include "LineWidthAttribute.h"


LineWidthAttribute::LineWidthAttribute(void)
{
	type = QE_ATTRI_LINE_WIDTH;
	reWidth = 1;
}


LineWidthAttribute::~LineWidthAttribute(void)
{
	glLineWidth(1.0f);
}

int LineWidthAttribute::Applay()
{
	glGetFloatv(GL_LINE_WIDTH, &reWidth);
	glLineWidth(width);
	return true;
}

void LineWidthAttribute::Width( GLfloat w )
{
	width = w;
}

GLfloat LineWidthAttribute::Width()
{
	return width;
}

int LineWidthAttribute::Recover()
{
	glLineWidth(reWidth);
	return true;
}
