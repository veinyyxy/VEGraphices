#include "QViewport.h"


QViewport::QViewport(void)
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

QViewport::QViewport( GLfloat x, GLfloat y, GLfloat w, GLfloat h )
{
	this->x = x;
	this->y = y;
	width = w;
	height = h;
}


QViewport::~QViewport(void)
{
}

void QViewport::SetViewport( GLfloat x, GLfloat y, GLfloat w, GLfloat h )
{
	this->x = x;
	this->y = y;
	width = w;
	height = h;
}

GLfloat QViewport::GetViewportPix()
{
	return width * height;
}

void QViewport::CreateViewport()
{
	glViewport(x, y, width, height);
}

void QViewport::ScreenToGL( const QVector3D& screenPoint, QVector3D& glPoint )
{
	GLint    viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ = 0.0f;
	GLdouble posX, posY, posZ;

	//glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	winX = screenPoint.x();
	winY = width - screenPoint.y();
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	glPoint.setX(posX);
	glPoint.setY(posY);
	glPoint.setZ(0.0);
}

GLfloat QViewport::X()
{
	{return y;}
}

GLfloat QViewport::Y()
{
	{return x;}
}

GLfloat QViewport::Width()
{
	{return width;}
}

GLfloat QViewport::Height()
{
	{return height;}
}

void QViewport::ApplyViewport()
{
	glViewport(x, y, width, height);
}

const QViewport& QViewport::operator=( const QViewport& it )
{
	this->x = it.x;
	this->y = it.y;
	this->width = it.width;
	this->height = it.height;
	return *this;
}
