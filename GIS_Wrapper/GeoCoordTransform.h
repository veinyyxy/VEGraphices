#pragma once
#include "GIS_WrapperConfig.h"
#include "RadProjTransform.h"

class GIS_WRAPPER_EXPORT_IMPORT GeoCoordTransform
{
public:
	GeoCoordTransform();
	GeoCoordTransform(RadProjTransform* proj4) { m_coordtrans = proj4;}
	~GeoCoordTransform();
	void SetProj4Trans(RadProjTransform* proj4);
	QVector3D TransGLtoWGS84(QVector3D & glPostion);	
	QVector3D TransWGS84toGL(QVector3D & oriPostion);
private:
	RadProjTransform* m_Proj4Trans;
	RadProjTransform* m_coordtrans;


};