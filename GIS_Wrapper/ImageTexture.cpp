#include "ImageTexture.h"


ImageTexture::ImageTexture( void ) : m_pGLContext(0), m_pImage(0), currentWidget(0)
{
	texture[0] = 0;
}

ImageTexture::~ImageTexture( void )
{

}

int ImageTexture::LoadTextrueFromFile( const QString& strFileName )
{
	m_pImage = new QImage(strFileName);
	return 1;
}

int ImageTexture::LoadTextrueFromPixelBuffer( const QOpenGLBuffer* pPixelBuffer )
{
	return 1;
}

int ImageTexture::LoadTextrueFromQimage(const QImage* pQimage)
{
    const QOpenGLContext* pGLContext = QOpenGLContext::currentContext();
	if(!pGLContext)
	{
		if(currentWidget)
			pGLContext = currentWidget->context();
		else
			return -1;
	}
	
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    QOpenGLBuffer *pixBuff = new QOpenGLBuffer(QOpenGLBuffer::PixelPackBuffer/*pQimage->width(),pQimage->height(),pGLContext->format(), currentWidget*/);
    //bool x = pixBuff->makeCurrent();
    texture[0] = 0;//pixBuff->bindTexture(*pQimage);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	delete pixBuff;
	return 1;
}

int ImageTexture::LoadTextrueFromPixmap( const QPixmap* pPixmap )
{
    const QOpenGLContext* pGLContext = QOpenGLContext::currentContext();
	if(!pGLContext)
	{
		if(currentWidget)
			pGLContext = currentWidget->context();
		else
			return -1;
	}
	
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    texture[0] = 0;//(( QGLContext*)pGLContext)->bindTexture(*pPixmap);
	glBindTexture(GL_TEXTURE_2D,texture[0]);

	return 1;
}

int ImageTexture::BindTextrue()
{
	static bool bCreateTextrue = true;
	if(m_pImage && bCreateTextrue)
	{
		LoadTextrueFromQimage(m_pImage);
		bCreateTextrue = false;
	}
	glBindTexture(m_Target, texture[0]);
	return 1;
}

int ImageTexture::ApplyTextrueParam()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	return true;
}

bool ImageTexture::SetGLGLWidget( QOpenGLWindow * sharewidget )
{
	//currentWidget =new QGLWidget;
	currentWidget = sharewidget;
    m_pGLContext = const_cast<QOpenGLContext*>(sharewidget->context());/*const_cast<QGLContext*>(QGLContext::currentContext());*/
	return true;
}

