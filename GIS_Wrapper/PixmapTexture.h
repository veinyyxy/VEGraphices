#pragma once
class CPixmapTexture : public QTextrueInterface
{
public:
	CPixmapTexture(QGLWidget* context);
	virtual ~CPixmapTexture(void);
	virtual int LoadTextrueFromFile(const QString& strFileName);
	virtual int LoadTextrueFromPixelBuffer(const QGLPixelBuffer* pPixelBuffer);
	virtual int LoadTextrueFromPixmap(const QPixmap* pPixmap);
	//virtual int LoadTextrueFromPixmap(const QPixmap* pPixmap, QGLWidget* pGLContext);
	virtual int BindTextrue();
	virtual int ApplyTextrueParam();

private:
	GLint m_gliTexture;
	QGLWidget* m_pGLWiget;
};

