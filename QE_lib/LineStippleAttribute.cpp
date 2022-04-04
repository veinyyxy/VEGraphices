#include "LineStippleAttribute.h"


LineStippleAttribute::LineStippleAttribute(void)
{
	type = QE_ATTRI_LINE_STIPPLE;
	reFactor = 1;
	line_pattern = -1;
}


LineStippleAttribute::~LineStippleAttribute(void)
{
	//glDisable(GL_LINE_STIPPLE);
}

int LineStippleAttribute::Applay()
{
	//glEnable(GL_LINE_STIPPLE);
	glGetIntegerv(GL_LINE_STIPPLE_PATTERN, &reLine_pattern);
	glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &reFactor);
	glLineStipple(factor, line_pattern);
	return true;
}

void LineStippleAttribute::Factor( GLint f )
{
	factor = f;
}

GLint LineStippleAttribute::Factor()
{
	return factor;
}

void LineStippleAttribute::LinePattern( const GLshort lp )
{
	line_pattern = lp;
}

GLshort LineStippleAttribute::LinePattern()
{
	return line_pattern;
}

int LineStippleAttribute::Recover()
{
	glLineStipple(reFactor, (GLushort)reLine_pattern);
	return true;
}
