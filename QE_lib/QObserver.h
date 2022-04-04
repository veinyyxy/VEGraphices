#pragma once
#include "Config.h"
#include "QViewport.h"
#include "QCamera.h"

class QE_LIB_EXPORT_IMPORT QObserver : public QObject
{
public:
	QObserver(void);
	~QObserver(void);
        void Viewport(const QViewport& vp);
        QViewport* Viewport();
        void Camera(const QCamera& c);
        QCamera* Camera();
	void ApplyObserver();
	void SetViewport(GLfloat x, GLfloat y, GLfloat w, GLfloat h);
	void SetViewport(const QVector4D& vpv);
	void SetCamera(GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble zN, GLdouble zF);
        QMatrix4x4& ObserverTransform();
        void ObserverTransform(const QMatrix4x4& ot);
	const QObserver& operator=( const QObserver& it );
protected:
	QViewport viewport;
	QCamera camera;
	QMatrix4x4 obTransform;
};

