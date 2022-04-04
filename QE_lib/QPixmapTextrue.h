#pragma once
#include "Config.h"
#include "QTextrueInterface.h"
#include <QtOpenGL/QGLContext>
class QPixmapTextrue :
	public QTextrueInterface
{
public:
	QPixmapTextrue(void);
	virtual ~QPixmapTextrue(void);
	int LoadTextrueFromFile(const QString& strFileName);
	int LoadTextrueFromPixelBuffer(const QGLPixelBuffer* pPixelBuffer);
	int LoadTextrueFromPixmap(const QPixmap* pPixmap);
	int BindTextrue();
	int ApplyTextrueParam();
	int UnloadTexture();
private:
	QGLContext* m_pGlContext;
	QPixmap m_Pixmap;
	GLuint m_Texture;
};

