#pragma once
#include "Config.h"
#include <QtOpenGL/QOpenGLBuffer>
#include "DrawSurface.h"

class QE_LIB_EXPORT_IMPORT PixelBufferSurface : public DrawSurface
{
public:
	PixelBufferSurface(void);
	~PixelBufferSurface(void);
	bool MakeGLContext();
	void DoneGLContext();
	void Resize(int iW, int iH);
	void FlushSurface();
    const QOpenGLContext* Context();
	QImage ToImage(bool br = false);
	QPixmap ToPixmap(int iW, int iH, bool bs = false);
	inline int Width(){return m_iWidth;}
	inline int Height(){return m_iHeight;}

private:
    QOpenGLBuffer* m_pGLPixelBuffer;
};

