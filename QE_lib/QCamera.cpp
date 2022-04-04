#include "QCamera.h"


QCamera::QCamera(CAMERA_TYPE t) : 
position(0, 0, 0), 
	center(0, 0, 0),
	up(0, 0, 0),
	left(0), 
	right(0),
	bottom(0),
	top(0),
	zNear(0),
	zFar(0),
	m_cameraScal(1.0)
	/*viewPointMatrix((qreal)0.0),
	lensMatrix((qreal)0.0)*/
{
	QObject::setObjectName(QString("QCamera"));
}

QCamera::QCamera( GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble zN, GLdouble zF )
{
	QObject::setObjectName(QString("QCamera"));
	left = l; 
	right = r;
	bottom = b;
	top = t;
	zNear = zN;
	zFar = zF;
}

QCamera::QCamera( GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
	QObject::setObjectName(QString("QCamera"));
}


QCamera::~QCamera(void)
{
}

void QCamera::SetLookAt( const QVector3D& pos, const QVector3D& cen, const QVector3D& up )
{
	position = pos;
	center = cen;
	this->up = up;
}

void QCamera::SetPerspectiveMatrix( const QMatrix4x4& maix )
{
	
}

void QCamera::SetPerspectiveMatrix( GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
	
}

void QCamera::Frustum( GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble zN, GLdouble zF )
{
	left = l;
	right = r;
	bottom = t;
	top = t;
	zNear = zN;
	zFar = zFar = zF;
}

void QCamera::Ortho( GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble zN, GLdouble zF )
{
	left = l;
	right = r;
	bottom = b;
	top = t;
	zNear = zN;
	zFar = zFar = zF;
}

void QCamera::Move( TRANSFORM_MODE mode, const QVector3D& pos )
{
	QMatrix4x4 transMatrix;
	transMatrix.translate(pos);

	left += pos.x();
	right += pos.x();
	top += pos.y();
	bottom += pos.y();
	zNear += pos.z();
	zFar += pos.z();
	/*QVector4D v3l(left, 0, 0, 0), v3r(right, 0, 0, 0), v3t(top, 0, 0, 0), v3b(bottom, 0, 0, 0);
	v3l *= transMatrix;*/
}

void QCamera::Move( const QMatrix4x4& maix )
{
	
}

void QCamera::Rotate( TRANSFORM_MODE mode, GLfloat degree, const QVector3D& aix )
{

}

void QCamera::Rotate( const QMatrix4x4& maix )
{
	
}

void QCamera::Scale( const GLfloat s )
{
	m_cameraScal *= s;
	/*left *= m_cameraScal;
	right *= m_cameraScal;
	top *= m_cameraScal;
	bottom *= m_cameraScal;*/
}

void QCamera::Scale( const QMatrix4x4& maix )
{
	
}

void QCamera::ApplyCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch(cameraType)
	{
	case ORTHO:
		glOrtho(left * m_cameraScal, right * m_cameraScal, bottom * m_cameraScal, top * m_cameraScal, zNear, zFar);
		break;
	case FRUSTRUM:
		glFrustum(left * m_cameraScal, right * m_cameraScal, bottom * m_cameraScal, top * m_cameraScal, zNear, zFar);
		break;
	}
	glMatrixMode(GL_MODELVIEW);
}

QMatrix4x4 QCamera::GetProjectionMatix()
{
	QMatrix4x4 projMatiix;
	glGetDoublev(GL_PROJECTION_MATRIX, projMatiix.data());
	return projMatiix;
}

qreal QCamera::GetCameraScale()
{
	return m_cameraScal;
}

void QCamera::SetCameraScale( qreal sc )
{
	m_cameraScal = sc;
}

const QCamera& QCamera::operator=( const QCamera& it )
{
	this->position = it.position;
	this->center = it.center;
	this->up = it.up;

	//
	this->left = it.left;
	this->right = it.right;
	this->bottom = it.bottom;
	this->top = it.top;
	this->zNear = it.zNear;
	this->zFar = it.zFar;

	//
	this->viewPointMatrix = it.viewPointMatrix;
	this->lensMatrix = it.lensMatrix;

	this->cameraType = it.cameraType;
	this->m_cameraScal = it.m_cameraScal;

	return *this;
}

