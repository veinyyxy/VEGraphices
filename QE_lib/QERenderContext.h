#pragma once
#include "Config.h"
#include "QT_Include.h"
#include "QObserver.h"
#include "QTransform3D.h"

class QE_LIB_EXPORT_IMPORT QERenderContext
{
public:
	QERenderContext(void);
	virtual ~QERenderContext(void);
	inline void RenderWindow(QGLWidget* pW){m_pCurrentWindows = pW;}
	inline QGLWidget* RenderWindow(){return m_pCurrentWindows;}
	inline void Observer(QObserver* pOb){m_pCurrentObserver = pOb;}
	inline QObserver* Observer(){return m_pCurrentObserver;}
	inline void Transform(QTransform3D* pT){m_pCurrentTransform = pT;}
	inline QTransform3D* Transform(){return m_pCurrentTransform;}

private:
	QGLWidget* m_pCurrentWindows;
	QObserver* m_pCurrentObserver;
	QTransform3D* m_pCurrentTransform;
};

