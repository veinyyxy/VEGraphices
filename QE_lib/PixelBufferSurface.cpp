#include "QScene.h"
#include "PixelBufferSurface.h"


PixelBufferSurface::PixelBufferSurface(void) : m_pGLPixelBuffer(0)
{
}


PixelBufferSurface::~PixelBufferSurface(void)
{
}

bool PixelBufferSurface::MakeGLContext()
{
	return m_pGLPixelBuffer->makeCurrent();
}

void PixelBufferSurface::DoneGLContext()
{
	m_pGLPixelBuffer->doneCurrent();
}

void PixelBufferSurface::Resize( int iW, int iH )
{
	DrawSurface::Resize(iW, iH);
	//QGLPixelBuffer* pTempBuffer = new QGLPixelBuffer(iW, iH);
	if(m_pGLPixelBuffer) delete m_pGLPixelBuffer;
	m_pGLPixelBuffer = new QGLPixelBuffer(iW, iH);
}

void PixelBufferSurface::FlushSurface()
{
	if(m_pGLPixelBuffer)
		m_pGLPixelBuffer->makeCurrent();

	if(m_pAssociatedScene)
	{
		m_pAssociatedScene->InitScene();
		m_pAssociatedScene->AdjustObserver(m_iWidth, m_iHeight);
		m_pAssociatedScene->RenderFrame();
	}
	glFlush();
}

const QGLContext* PixelBufferSurface::Context()
{
	return 0;
}

QImage PixelBufferSurface::ToImage( bool br /*= false*/ )
{
	return m_pGLPixelBuffer->toImage();
}

QPixmap PixelBufferSurface::ToPixmap( int iW, int iH, bool bs /*= false*/ )
{
	QPixmap nullObject;
	return nullObject;
}
