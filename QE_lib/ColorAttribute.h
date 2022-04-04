#pragma once
#include "QAttribute.h"
class QE_LIB_EXPORT_IMPORT ColorAttribute : public QAttribute
{
public:
	ColorAttribute();
	inline void Color(const QVector4D& c);
	inline QVector4D Color();
	inline int Applay();
	inline int Recover(){glColor4d(oldColor.x(), oldColor.y(), oldColor.z(),oldColor.w());return 1;}
private:
	QVector4D oldColor;
	QVector4D color;
	ATTRI_TYPE type;
};

