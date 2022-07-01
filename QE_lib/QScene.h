#pragma once
class DrawSurface;

#include "QT_Include.h"
#include "GL_Include.h"
#include "QNode.h"
#include "QCamera.h"
#include "QRenderable.h"
#include "QAccessor.h"
#include "QViewport.h"
#include "QObserver.h"
#include "QSceneCallBack.h"
#include "QNodeCallBack.h"
#include "QPickup.h"
#include "QTransform3D.h"
#include "QScene.h"
#include "QMouseManipulator.h"
#include "QDefaultMouseManipulator.h"
//#include "QZoomInMouseManipulator.h"
//#include "QZoomOutMouseManipulator.h"
#include "QERenderContext.h"
#include "QRenderWindow.h"
#include <math.h>

class QE_LIB_EXPORT_IMPORT QScene : public QRunnable
{
public:
	typedef enum _surface_type{WINDOW_TYPE = 1, OFF_SCREEN} SURFACE_TYPE;
	typedef QMap<QString, QObserver*> OBSERVER_MAP, *P_OVSERVER_MAP;
	QScene(void);
    QScene(QOpenGLContext* glContext);
	virtual ~QScene(void);
	virtual void InitScene();
	virtual void Render();
	virtual void RenderFrame();
	void SetCamera(const QCamera* cam);
	void SetNode(const QNode* n);
	void SetErgodic(QNode* n);
	inline QNode* GetNode();
	QVector3D SceneColor();
	void SceneColor(const QVector3D& color);
	DrawSurface* GetDrawSurface();
	void GetDrawSurface(QRenderWindow* glwin);
	void PickUp(int x, int y);
	void PickRay(const QPoint& screenPoint);
	bool IsInTrg(QVector3D vtxA, QVector3D vtxB, QVector3D vtxC, TriEqaResult& pHit);
	bool Intersect(QVector3D& vtxA, QVector3D& vtxB, QVector3D& vtxC, QVector3D* pos);
	void CalcPlaneNormal(const QVector3D& P1, const QVector3D& P2, const QVector3D& P3, QVector3D& res);
	double PointDistance(const QVector3D& a, const QVector3D& b);
	void ClearSceneTransform();
	virtual bool SetViewBackgroundColor(QColor color);
	void SwitchGLState(bool ss, GLenum stateid);
	void ScreenToGL(const QVector3D& screenPoint, QVector3D& glPoint);
	void GLToScreen(const QVector3D& glPoint, QVector3D& screenPoint);
	inline QObserver* GetObserver(const QString& name){return m_ObserverMap[name];}
	inline void InsertObserver(const QString& name, QObserver* observer){m_ObserverMap.insert(name, observer);}
	inline void DeleteObserver(const QString& name){m_ObserverMap.remove(name);}
	inline void SetCallBack(QSceneCallBack* sc){m_SceneCallBack = sc;}
	inline QSceneCallBack* GetCallBack(){return m_SceneCallBack;}
	inline void IsRun(bool bIs){m_bIsRun = bIs;}
	inline bool IsRun(){return m_bIsRun;}
	virtual void run();

	inline QTransform3D* GetSceneTransform(){return &m_SceneTransform;}
	bool close();
	void FlushScene();
	inline void MouseManipulator(QMouseManipulator* pMouseMani){
		m_pMouseManipulator = pMouseMani;
		if(m_pMouseManipulator)
			m_pMouseManipulator->UpDataManipulator(root, m_ObserverMap.begin().value(), &m_SceneTransform);
	}
	inline QMouseManipulator* MouseManipulator(){return m_pMouseManipulator;}
	virtual void ResizeMultViewport(int MainWindowWidth, int MainWindowHeight);
	virtual void RenderToPixmap(QPixmap* pPixmap);

	inline int QSceneID(){return m_iQSceneID;}
	inline DrawSurface* CreateDrawSurface(SURFACE_TYPE surType = WINDOW_TYPE);
	virtual void AdjustObserver(int width, int height);
	void SetBackgroundColor(const QColor& c);
	inline QColor GetBackgroundColor(){return m_backgroundColor;}
protected:
	DrawSurface* m_pDrawSurface;
	QVector3D backgroundColor;
	OBSERVER_MAP m_ObserverMap;
	QObserver* currentObserver;
	QNode* root;
	QVector3D rayStart;
	QVector3D rayEnd;
	QTransform3D m_SceneTransform;
	QSceneCallBack* m_SceneCallBack;
	QNodeCallBack* m_NodeCallBack;
    QOpenGLContext* m_glContext;
	QThreadPool m_ThreadPool;
	QMouseManipulator* m_pMouseManipulator;
	bool m_EnableThread;
	QERenderContext m_RenderContext;
	int m_iQSceneID;
	QColor m_backgroundColor;
	bool m_bIsRun;
};
