#pragma once
#include "QAttribute.h"
class QE_LIB_EXPORT_IMPORT LineWidthAttribute :
	public QAttribute
{
public:
	LineWidthAttribute(void);
	~LineWidthAttribute(void);
	inline void Width(GLfloat w);
	inline GLfloat Width();
	int Applay();
	int Recover();
private:
	GLfloat width;
	GLfloat reWidth;
	ATTRI_TYPE type;
};

