#include "QPixmapTextrue.h"


QPixmapTextrue::QPixmapTextrue(void) : m_pGlContext(0), m_Texture(0)
{
}


QPixmapTextrue::~QPixmapTextrue(void)
{
}

int QPixmapTextrue::LoadTextrueFromFile( const QString& strFileName )
{
    QOpenGLContext* pGLContext = const_cast<QOpenGLContext*>(QOpenGLContext::currentContext());
	if(!pGLContext) return 1;//opengl�����Ĳ��ܻ��
	if(m_pGlContext != pGLContext)
	{
		m_pGlContext = pGLContext;
	}
	m_Pixmap.load(strFileName);
    //m_Texture = glBindTexture(m_Pixmap, m_Target);
	if(!m_Target) return 2;//�����޷�����
	return 0;
}

int QPixmapTextrue::LoadTextrueFromPixelBuffer( const QOpenGLBuffer* pPixelBuffer )
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
	
    QOpenGLContext* pGLContext = const_cast<QOpenGLContext*>(QOpenGLContext::currentContext());
	if(!pGLContext) return 1;//opengl�����Ĳ��ܻ��
	if(m_pGlContext != pGLContext)
	{
        //if(m_Texture) pGLContext->deleteTexture(m_Texture);
        //m_Texture = pGLContext->bindTexture(m_Pixmap, m_Target);
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
