#include "QScene.h"
#include "QRenderWindow.h"


QRenderWindow::QRenderWindow(void)
{
	
}

QRenderWindow::QRenderWindow( const QGLFormat & format, QWidget * parent /*= 0*/
	, const QGLWidget * shareWidget /*= 0*/, Qt::WindowFlags f /*= 0 */ )
	: QGLWidget(format, parent, shareWidget, f)
{

}

QRenderWindow::~QRenderWindow(void)
{
}

void QRenderWindow::initializeGL()
{
	if(m_pAssociatedScene)
		m_pAssociatedScene->InitScene();
}

void QRenderWindow::paintGL()
{
	if(m_pAssociatedScene)
		m_pAssociatedScene->RenderFrame();
}

void QRenderWindow::resizeGL( int width, int height )
{
	m_pAssociatedScene->AdjustObserver(width, height);
}

void QRenderWindow::mousePressEvent( QMouseEvent *event )
{
	QMouseManipulator* pMouse = 0;
	if(m_pAssociatedScene)
		pMouse = m_pAssociatedScene->MouseManipulator();
	else
		return;
	if(pMouse)
	{
		pMouse->SetGLContext((QGLContext*)QGLWidget::context());
		pMouse->SetAssociScene(m_pAssociatedScene);
		pMouse->mousePressEvent(event);
	}
	FlushSurface();
}

void QRenderWindow::mouseReleaseEvent( QMouseEvent *event )
{
	QMouseManipulator* pMouse = m_pAssociatedScene->MouseManipulator();
	if(pMouse)
	{
		pMouse->SetGLContext((QGLContext*)QGLWidget::context());
		pMouse->SetAssociScene(m_pAssociatedScene);
		pMouse->mouseReleaseEvent(event);
	}
	FlushSurface();
}

void QRenderWindow::mouseMoveEvent( QMouseEvent *event )
{
	QMouseManipulator* pMouse = m_pAssociatedScene->MouseManipulator();
	if(pMouse)
	{
		pMouse->SetGLContext((QGLContext*)QGLWidget::context());
		pMouse->SetAssociScene(m_pAssociatedScene);
		pMouse->mouseMoveEvent(event);
	}
	FlushSurface();
}

void QRenderWindow::wheelEvent( QWheelEvent *event )
{
	QMouseManipulator* pMouse = m_pAssociatedScene->MouseManipulator();
	if(pMouse)
	{
		pMouse->SetGLContext((QGLContext*)QGLWidget::context());
		pMouse->SetAssociScene(m_pAssociatedScene);
		pMouse->wheelEvent(event);
	}
	FlushSurface();
}

void QRenderWindow::keyPressEvent( QKeyEvent *event )
{
	
}

void QRenderWindow::keyReleaseEvent( QKeyEvent *event )
{
	
}

bool QRenderWindow::MakeGLContext()
{
	QGLWidget::makeCurrent();
	return true;
}

void QRenderWindow::DoneGLContext()
{
	QGLWidget::doneCurrent();
}

void QRenderWindow::Resize( int iW, int iH )
{

}

void QRenderWindow::FlushSurface()
{
	QGLWidget::update();
}

const QGLContext* QRenderWindow::Context()
{
	return QGLWidget::context();
}

QImage QRenderWindow::ToImage(bool br)
{
	return QGLWidget::grabFrameBuffer(br);
}

int QRenderWindow::Width()
{
	return QGLWidget::width();
}

int QRenderWindow::Height()
{
	return QGLWidget::height();
}

QPixmap QRenderWindow::ToPixmap( int iW, int iH, bool bs /*= false*/ )
{
	return QGLWidget::renderPixmap(iW, iH, bs);
}

