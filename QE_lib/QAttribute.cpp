#include "QAttribute.h"

//int QAttribute::Applay()
//{
//	switch(type)
//	{
//	case QE_ATTRI_POINT:
//		break;
//		glPointSize(width);
//	case QE_ATTRI_LINES:
//		//glEnable(GL_LINE_STIPPLE);
//		glLineWidth(width);
//		glLineStipple(factor, line_pattern);
//		break;
//	case QE_ATTRI_POLYGON:
//		//glPolygonMode();
//		glPolygonStipple(polygon_pattern);
//		break;
//	case QE_ATTRI_COLOR:
//		glColor4d(color.x(), color.y(), color.z(), color.w());
//		break;
//	default:
//		return GL_FALSE;
//	}
//	return GL_TRUE;
//}

QAttribute::QAttribute( ATTRI_TYPE t /*= QE_ATTRI_UNKONW*/ )
{

}

QAttribute::~QAttribute( void )
{

}

ATTRI_TYPE QAttribute::Type()
{
	return type;
}
