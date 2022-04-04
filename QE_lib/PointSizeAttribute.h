#pragma once
#include "QAttribute.h"
class QE_LIB_EXPORT_IMPORT PointSizeAttribute :
	public QAttribute
{
public:
	PointSizeAttribute();
	~PointSizeAttribute();
	inline void Size(GLfloat s);
	inline GLfloat Size();
	inline int Applay();

private:
	GLfloat size;
	ATTRI_TYPE type;
};

