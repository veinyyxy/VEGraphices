#pragma once
#include "Config.h"
#include "QTextrueInterface.h"
#include <QtGui/QOpenGLContext>
#include <QtOpenGL/QOpenGLBuffer>

class QPixmapTextrue :
	public QTextrueInterface
{
public:
	QPixmapTextrue(void);
	virtual ~QPixmapTextrue(void);
	int LoadTextrueFromFile(const QString& strFileName);
    int LoadTextrueFromPixelBuffer(const QOpenGLBuffer* pPixelBuffer);
	int LoadTextrueFromPixmap(const QPixmap* pPixmap);
	int BindTextrue();
	int ApplyTextrueParam();
	int UnloadTexture();

private:
    QOpenGLContext* m_pGlContext;
	QPixmap m_Pixmap;
	GLuint m_Texture;
};

