#include "QScene.h"
#include "QRenderWindow.h"


QRenderWindow::QRenderWindow(void)
{
	
}

QRenderWindow::QRenderWindow( const QSurfaceFormat & format, QWidget * parent,
                              const QOpenGLWidget * shareWidget, Qt::WindowFlags f)
    : QOpenGLWindow()
{

}

QRenderWindow::~QRenderWindow(void)
{
}

void QRenderWindow::initializeGL()
{

}

void QRenderWindow::paintGL()
{

}

void QRenderWindow::resizeGL( int width, int height )
{

}

void QRenderWindow::mousePressEvent( QMouseEvent *event )
{
    QMouseManipulator* pMouse = 0;
	if(pMouse)
	{
        pMouse->SetGLContext(QOpenGLWindow::context());
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
        pMouse->SetGLContext(QOpenGLWindow::context());
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
        pMouse->SetGLContext(QOpenGLWindow::context());
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
        pMouse->SetGLContext(QOpenGLWindow::context());
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
    QOpenGLWindow::makeCurrent();
	return true;
}

void QRenderWindow::DoneGLContext()
{
    QOpenGLWindow::doneCurrent();
}

void QRenderWindow::Resize( int iW, int iH )
{

}

void QRenderWindow::FlushSurface()
{
    update();
}

const QOpenGLContext* QRenderWindow::Context()
{
    return QOpenGLWindow::context();
}

QImage QRenderWindow::ToImage(bool br)
{
    return QOpenGLWindow::grabFramebuffer();
}

int QRenderWindow::Width()
{
    return QOpenGLWindow::width();
}

int QRenderWindow::Height()
{
    return QOpenGLWindow::height();
}

QPixmap QRenderWindow::ToPixmap( int iW, int iH, bool bs /*= false*/ )
{
    //return QOpenGLWidget::renderPixmap(iW, iH, bs);
    QPixmap nullPixmap;
    return nullPixmap;
}

