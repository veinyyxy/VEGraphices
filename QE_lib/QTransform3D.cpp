#include "QTransform3D.h"


QTransform3D::QTransform3D(void)
{
	m_Translate.setToIdentity();
	m_Rotate.setToIdentity();
	m_Scale.setToIdentity();
	m_AllInTransform.setToIdentity();
	m_temp.setToIdentity();
	m_TransforMode = GL_MODELVIEW_MATRIX;
}

QTransform3D::~QTransform3D(void)
{
}

void QTransform3D::EnableTransforms()
{
	glMatrixMode(m_TransforMode);
	//glPushMatrix();
	glLoadIdentity();
	/*m_temp = m_Rotate * m_Translate * m_Scale;
	m_AllInTransform *= m_temp;*/
	m_AllInTransform  = m_Translate * m_Rotate * m_Scale;
	glMultMatrixd(m_AllInTransform.data());
	//glLoadIdentity();
	/*m_Translate.setToIdentity();
	m_Rotate.setToIdentity();
	m_Scale.setToIdentity();*/
	
}

void QTransform3D::ToIdentity()
{
	m_AllInTransform.setToIdentity();
}

void QTransform3D::UnableTransforms()
{
	//glPopMatrix();
}

QTransform3D& QTransform3D::operator=( const QTransform3D& other )
{
	this->m_Translate = other.m_Translate;
	this->m_Rotate = other.m_Rotate;
	this->m_Scale = other.m_Scale;
	this->m_AllInTransform = other.m_AllInTransform;
	this->m_temp = other.m_temp;
	this->m_TransforMode = other.m_TransforMode;
	return *this;
}
