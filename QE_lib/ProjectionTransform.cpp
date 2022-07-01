#include "ProjectionTransform.h"


ProjectionTransform::ProjectionTransform(void)
{
}

ProjectionTransform::~ProjectionTransform(void)
{
}

void ProjectionTransform::StartApplyTransform()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
}

void ProjectionTransform::Transform( QMatrix4x4& tr )
{
	
}

void ProjectionTransform::Transform()
{
	m_RenderableTransform = m_TranslateMatirx * m_RotateMatirx * m_ScaleMatirx;
	glLoadMatrixf(m_RenderableTransform.data());
}

void ProjectionTransform::EndApplyTransform()
{
	glPopMatrix();
}
