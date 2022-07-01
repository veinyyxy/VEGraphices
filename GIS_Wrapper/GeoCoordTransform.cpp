#include <QtCore/QSettings>
//#include <QtCore/QTextCodec>
#include "GeoCoordTransform.h"

GeoCoordTransform::GeoCoordTransform()
{
	  m_Proj4Trans = new RadProjTransform;
	  m_coordtrans = new RadProjTransform;
}

/************************************************************************/
/* Îö¹¹º¯ÊýÉ¾³ý¶àÓà                                                     */
/************************************************************************/
GeoCoordTransform::~GeoCoordTransform()
{
	if(m_Proj4Trans)
	{
		delete m_Proj4Trans;
		m_Proj4Trans = NULL;
	}
	if (m_coordtrans)
	{
		delete m_coordtrans;
		m_coordtrans = NULL;
	}
}

void GeoCoordTransform::SetProj4Trans(RadProjTransform* proj4) 
{
	m_coordtrans = proj4;
}


QVector3D GeoCoordTransform::TransGLtoWGS84(QVector3D & glPostion)
{
	if (m_coordtrans)
	{
		m_Proj4Trans->setSourceCRS(m_coordtrans->getDesCRS());
		if ("RADARPROJ" == m_coordtrans->getDesCRS())
		{
			m_Proj4Trans->Set((m_coordtrans->m_central).x(),(m_coordtrans->m_central).y(),(m_coordtrans->m_central).z());
		}
	}
	m_Proj4Trans->setDestCRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
	return ( m_Proj4Trans->Transform(glPostion));
}

QVector3D GeoCoordTransform::TransWGS84toGL(QVector3D & oriPostion)
{
	m_Proj4Trans->setSourceCRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
	m_Proj4Trans->setDestCRS(m_coordtrans->getDesCRS());
	if ("RADARPROJ" == m_coordtrans->getDesCRS())
	{
		m_Proj4Trans->Set((m_coordtrans->m_central).x(),(m_coordtrans->m_central).y(),(m_coordtrans->m_central).z());
	}
	return ( m_Proj4Trans->Transform(oriPostion));
}
