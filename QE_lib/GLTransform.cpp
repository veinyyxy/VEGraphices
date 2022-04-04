#include "GLTransform.h"


GLTransform::GLTransform(void)
{
	m_TranslateMatirx.setToIdentity();
	m_RotateMatirx.setToIdentity();
	m_ScaleMatirx.setToIdentity();
}

GLTransform::GLTransform( QMatrix4x4& tr, QMatrix4x4 ro, QMatrix4x4 sc ) :
	m_TranslateMatirx(tr),
	m_RotateMatirx(ro),
	m_ScaleMatirx(sc)
{

}

GLTransform::~GLTransform(void)
{
}

void GLTransform::TranslateMatirx( const QMatrix4x4& tm4x4 )
{
	m_TranslateMatirx = tm4x4;
}

QMatrix4x4& GLTransform::TranslateMatirx()
{
	return m_TranslateMatirx;
}

void GLTransform::RotateMatirx( const QMatrix4x4& rm4x4 )
{
	m_RotateMatirx = rm4x4;
}

QMatrix4x4& GLTransform::RotateMatirx()
{
	return m_RotateMatirx;
}

void GLTransform::ScaleMatirx( const QMatrix4x4& sm4x4 )
{
	m_ScaleMatirx = sm4x4;
}

QMatrix4x4& GLTransform::ScaleMatirx()
{
	return m_ScaleMatirx;
}

QMatrix4x4& GLTransform::GetTransform()
{
	return m_RenderableTransform;
}

void GLTransform::SetTransform( QMatrix4x4& tr )
{
	m_RenderableTransform = tr;
}

