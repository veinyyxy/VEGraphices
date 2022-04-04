#pragma once
#include "Config.h"
#include "QT_Include.h"
class QScene;

class QE_LIB_EXPORT_IMPORT DrawSurface
{
public:
	DrawSurface(void);
	virtual ~DrawSurface(void);
	virtual bool MakeGLContext() = 0;
	virtual void DoneGLContext() = 0;
	virtual void FlushSurface() = 0;
	virtual const QGLContext* Context() = 0;
	virtual QImage ToImage(bool br = false) = 0;
	virtual QPixmap ToPixmap(int iW, int iH, bool bs) = 0;
	virtual void Resize(int iW, int iH){m_iWidth = iW; m_iHeight = iH;}
	virtual int Width() = 0;
	virtual int Height() = 0;
	inline void SetAssociatedScene(QScene* pScene){m_pAssociatedScene = pScene;}
	inline QScene* GetAssociatedScene(){return m_pAssociatedScene;}

protected:
	QScene* m_pAssociatedScene;
	QGLContext* m_pContext;
	int m_iWidth;
	int m_iHeight;
};

