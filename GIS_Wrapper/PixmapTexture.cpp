#include "QTextrueInterface.h"
#include "PixmapTexture.h"


CPixmapTexture::CPixmapTexture(QGLWidget* context) : m_pGLWiget(context)
{
}


CPixmapTexture::~CPixmapTexture(void)
{
}

int CPixmapTexture::LoadTextrueFromFile( const QString& strFileName )
{
	return -1;
}

int CPixmapTexture::LoadTextrueFromPixelBuffer( const QGLPixelBuffer* pPixelBuffer )
{
	return -1;
}

int CPixmapTexture::LoadTextrueFromPixmap( const QPixmap* pPixmap )
{
	QGLPixelBuffer* pPixbuffer = new QGLPixelBuffer(pPixmap->width(), pPixmap->height(), m_pGLWiget->format(), m_pGLWiget);
	bool bRes = pPixbuffer->makeCurrent();
	//pPixmap->save("c:/aaa.bmp");
	m_gliTexture =  pPixbuffer->bindTexture(*pPixmap);
	delete pPixbuffer;
	return 1;
}

//int CPixmapTexture::LoadTextrueFromPixmap( const QPixmap* pPixmap, QGLWidget* pGLContext )
//{
//	QGLPixelBuffer* pPixbuffer = new QGLPixelBuffer(pPixmap->width(), pPixmap->height());
//	m_gliTexture =  pPixbuffer->bindTexture(*pPixmap);
//	return 1;
//}

int CPixmapTexture::BindTextrue()
{
	glBindTexture(m_Target, m_gliTexture);
	return 1;
}

int CPixmapTexture::ApplyTextrueParam()
{
	TEXTURE_PARAM_SET::iterator index = m_TextrueParamSet.begin();
	TEXTURE_PARAM_SET::iterator end = m_TextrueParamSet.end();
	for(;index != end; index++)
	{
		glTexParameterf(m_Target, (*index).first, (*index).second);
	}
	return 1;
}
