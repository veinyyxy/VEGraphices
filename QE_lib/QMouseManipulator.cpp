#include "QMouseManipulator.h"


QMouseManipulator::QMouseManipulator(void) : 
	m_pGLContext(0),
	m_pSceneRootNode(0),
	m_pObserver(0),
	m_pSceneTransform(0)
{
	
}


QMouseManipulator::~QMouseManipulator(void)
{
}

/************************************************************************/
/* 函数功能： 屏幕点转换成opengl投影坐标		                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void QMouseManipulator::ScreenToGL( const QVector3D& screenPoint, QVector3D& glPoint )
{
    //m_pGLContext->makeCurrent();
	GLint    viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ = 0.0f;
	GLdouble posX, posY, posZ;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	winX = screenPoint.x();
	winY = viewport[3] - screenPoint.y();
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	glPoint.setX(posX);
	glPoint.setY(posY);
	glPoint.setZ(0.6);
}

void QMouseManipulator::GLToScreen( const QVector3D& src, QVector3D& des )
{
    //m_pGLContext->makeCurrent();
	qreal x(0), y(0), z(0), x1(0), y1(0), z1(0);
	GLint    viewport[4] = {0};
	GLdouble modelview[16] = {0};
	GLdouble projection[16] = {0};
	GLfloat winX = 0.0f, winY = 0.0f, winZ = 0.0f;
	GLdouble posX = 0.0, posY = 0.0, posZ = 0.0;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	//glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluProject(src.x(), src.y(), src.z(), modelview, projection, viewport, &x, &y, &z);
	des.setX(x);
	des.setY(y);
	des.setZ(z);
}

