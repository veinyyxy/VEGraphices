#pragma once
#include <QtOpenGL/QOpenGLWindow>
#include <QtOpenGL/QOpenGLBuffer>

class CPixmapTexture : public QTextrueInterface
{
public:
    CPixmapTexture(QOpenGLWindow* context);
	virtual ~CPixmapTexture(void);
	virtual int LoadTextrueFromFile(const QString& strFileName);
    virtual int LoadTextrueFromPixelBuffer(const QOpenGLBuffer* pPixelBuffer);
	virtual int LoadTextrueFromPixmap(const QPixmap* pPixmap);
	//virtual int LoadTextrueFromPixmap(const QPixmap* pPixmap, QGLWidget* pGLContext);
	virtual int BindTextrue();
	virtual int ApplyTextrueParam();

private:
	GLint m_gliTexture;
    QOpenGLWindow* m_pGLWiget;
};

