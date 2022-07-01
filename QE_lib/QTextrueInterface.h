#pragma once
#include "QT_Include.h"
#include "GL_Include.h"

class QE_LIB_EXPORT_IMPORT QTextrueInterface
{
public:
	typedef QPair<GLenum, GLint> TEXTURE_PARAM, *P_TEXTURE_PARAM;
	typedef QSet<TEXTURE_PARAM> TEXTURE_PARAM_SET, *P_TEXTURE_PARAM_SET;
	QTextrueInterface(void);
	virtual ~QTextrueInterface(void);
	virtual int LoadTextrueFromFile(const QString& strFileName) = 0;
    virtual int LoadTextrueFromPixelBuffer(const QOpenGLBuffer* pPixelBuffer) = 0;
	virtual int LoadTextrueFromPixmap(const QPixmap* pPixmap) = 0;
	virtual int BindTextrue() = 0;
	virtual int ApplyTextrueParam() = 0;
	virtual int UnloadTexture();
	inline void TargetTextrue(GLenum target){m_Target = target;}
	inline GLenum TargetTextrue(){return m_Target;}
	inline void InsertTextrueParam(GLenum pname, GLint param){m_TextrueParamSet.insert(TEXTURE_PARAM(param, param));}
	inline P_TEXTURE_PARAM_SET GetTextrueParam(){return &m_TextrueParamSet;}

protected:
	//GLuint* m_pGLTextre;
	GLenum m_Target;
	TEXTURE_PARAM_SET m_TextrueParamSet;
};

