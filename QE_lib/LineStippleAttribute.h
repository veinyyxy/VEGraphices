#pragma once
#include "QAttribute.h"

class QE_LIB_EXPORT_IMPORT LineStippleAttribute : public QAttribute
{
public:
	LineStippleAttribute(void);
	~LineStippleAttribute(void);
	inline void Factor(GLint f);
	inline GLint Factor();
	inline void LinePattern(const GLshort lp);
	inline GLshort LinePattern();
	int Applay();
	int Recover();
private:
	GLint factor;
	GLint reFactor;
	GLshort line_pattern;
	GLint reLine_pattern;
	ATTRI_TYPE type;
};

