#include "ViewModelTransform.h"


ViewModelTransform::ViewModelTransform(void)
{
}


ViewModelTransform::~ViewModelTransform(void)
{
}

void ViewModelTransform::StartApplyTransform()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void ViewModelTransform::Transform( QMatrix4x4& tr )
{
	
}

void ViewModelTransform::Transform()
{
	m_RenderableTransform = m_TranslateMatirx * m_RotateMatirx * m_ScaleMatirx;
	glLoadMatrixf(m_RenderableTransform.data());
}

void ViewModelTransform::EndApplyTransform()
{
	glPopMatrix();
}
