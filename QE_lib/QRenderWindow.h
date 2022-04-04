#pragma once
#include "Config.h"
#include "QT_Include.h"
#include "DrawSurface.h"

class QE_LIB_EXPORT_IMPORT QRenderWindow : public QGLWidget, public DrawSurface
{
public:
	QRenderWindow(void);
	~QRenderWindow(void);
	QRenderWindow( const QGLFormat & format, QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
	/*void glInit();
	void glDraw();
	void StartContext();
	void EndContext();*/
	bool MakeGLContext();
	void DoneGLContext();
	void Resize(int iW, int iH);
	void FlushSurface();
	const QGLContext* Context();
	QImage ToImage(bool br = false);
	QPixmap ToPixmap(int iW, int iH, bool bs = false);
	int Width();
	int Height();
protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
private:
	QMatrix4x4 m_CurrentTranslate;
	QMatrix4x4 m_CurrentRotation;
	QMatrix4x4 m_CurrentScale;
};

