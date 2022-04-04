#include "QPixmapTextrue.h"


QPixmapTextrue::QPixmapTextrue(void) : m_pGlContext(0), m_Texture(0)
{
}


QPixmapTextrue::~QPixmapTextrue(void)
{
}

int QPixmapTextrue::LoadTextrueFromFile( const QString& strFileName )
{
	QGLContext* pGLContext = const_cast<QGLContext*>(QGLContext::currentContext());
	if(!pGLContext) return 1;//opengl上下文不能获得
	if(m_pGlContext != pGLContext)
	{
		m_pGlContext = pGLContext;
	}
	m_Pixmap.load(strFileName);
	m_Texture = m_pGlContext->bindTexture(m_Pixmap, m_Target);
	if(!m_Target) return 2;//纹理无法生成
	return 0;
}

int QPixmapTextrue::LoadTextrueFromPixelBuffer( const QGLPixelBuffer* pPixelBuffer )
{
	return -1;
}

int QPixmapTextrue::LoadTextrueFromPixmap( const QPixmap* pPixmap )
{
	m_Pixmap = *pPixmap;
	return 0;
}

int QPixmapTextrue::BindTextrue()
{
	glEnable(m_Target);
	//glEnable(GL_BLEND);
	
	QGLContext* pGLContext = const_cast<QGLContext*>(QGLContext::currentContext());
	if(!pGLContext) return 1;//opengl上下文不能获得
	if(m_pGlContext != pGLContext)
	{
		if(m_Texture) pGLContext->deleteTexture(m_Texture);
		m_Texture = pGLContext->bindTexture(m_Pixmap, m_Target);
		m_pGlContext = pGLContext;
	}
	return 0;
}

int QPixmapTextrue::ApplyTextrueParam()
{
	glBindTexture(m_Target, m_Texture);
	return 0;
}

int QPixmapTextrue::UnloadTexture()
{
	//glDisable(GL_BLEND);
	glDisable(m_Target);
	return 0;
}
