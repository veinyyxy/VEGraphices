#include "QObserver.h"


QObserver::QObserver(void)
{
	obTransform.setToIdentity();
}

QObserver::~QObserver(void)
{
}

void QObserver::Viewport( const QViewport& vp )
{
	viewport = vp;
}

QViewport* QObserver::Viewport()
{
	return &viewport;
}

void QObserver::ApplyObserver()
{
	viewport.ApplyViewport();
	camera.ApplyCamera();
	glMultMatrixf(obTransform.data());
}

void QObserver::Camera( const QCamera& c )
{
	camera = c;
}

QCamera* QObserver::Camera()
{
	return &camera;
}

void QObserver::SetViewport( GLfloat x, GLfloat y, GLfloat w, GLfloat h )
{
	viewport.SetViewport(x, y, w, h);
}

void QObserver::SetViewport( const QVector4D& vpv )
{
	viewport.SetViewport(vpv.x(), vpv.y(), vpv.z(), vpv.w());
}

void QObserver::SetCamera( GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble zN, GLdouble zF )
{
	camera.Ortho(l,r,b,t,zN,zF);
}

QMatrix4x4& QObserver::ObserverTransform()
{
	return obTransform;
}

void QObserver::ObserverTransform( const QMatrix4x4& ot )
{
	obTransform = ot;
}

const QObserver& QObserver::operator=( const QObserver& it )
{
	this->camera = it.camera;
	this->viewport = it.viewport;
	this->obTransform = it.obTransform;
	return *this;
}