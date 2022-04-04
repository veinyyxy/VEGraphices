#pragma once
#include "Config.h"
#include "QT_Include.h"
#include "QNode.h"
#include "QObserver.h"
#include "QTransform3D.h"

class QScene;

class QE_LIB_EXPORT_IMPORT QMouseManipulator : public QObject
{
public:
	QMouseManipulator(void);

	typedef enum MouseManipulatorType
	{
		MOUSE_EXPAND = 0,
		MOUSE_DEFAULT = 1,
		MOUSE_ZOOMIN = 2,
		MOUSE_ZOOMOUT =3,
		MOUSE_MANUALDRAW =4,
		
	};
	virtual ~QMouseManipulator(void);
	virtual void mousePressEvent(QMouseEvent *event) = 0;
	virtual void mouseReleaseEvent(QMouseEvent *event) = 0;
	virtual void mouseMoveEvent(QMouseEvent *event) = 0;
	virtual void wheelEvent(QWheelEvent *event) = 0;
	virtual void mouseDoubleClickEvent (QMouseEvent * event) = 0;
	/*virtual void keyPressEvent(QKeyEvent *event) = 0;
	virtual void keyReleaseEvent(QKeyEvent *event) = 0;*/
	virtual void leaveEvent(QEvent * even) = 0;
	inline void UpDataManipulator(/*QGLWidget* pgl, */QNode* pn, QObserver* pob, QTransform3D* ptr){
		//m_pGLWidget = pgl;
		m_pSceneRootNode = pn;
		m_pObserver = pob;
		m_pSceneTransform = ptr;
	}
	inline void SetGLContext(QGLContext* pglc){m_pGLContext = pglc;}
	inline QGLContext* GLContext(){return m_pGLContext;}
	inline QNode* Node(){return m_pSceneRootNode;}
	inline QObserver* Observer(){return m_pObserver;}
	inline QTransform3D* SceneTransform(){return m_pSceneTransform;}
	void ScreenToGL(const QVector3D& screenPoint, QVector3D& glPoint);
	void GLToScreen( const QVector3D& src, QVector3D& des );
	inline void SetAssociScene(QScene* pS){m_pAssociatedScene = pS;}
	inline QScene* GetAssociScene(){return m_pAssociatedScene;}
protected:
	QGLContext* m_pGLContext;
	QNode* m_pSceneRootNode;
	QObserver* m_pObserver;
	QTransform3D* m_pSceneTransform;
	QScene* m_pAssociatedScene;
};

