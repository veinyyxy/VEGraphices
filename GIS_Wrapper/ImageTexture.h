#pragma once
#define CORE_EXPORT _declspec(dllimport)
#define GUI_EXPORT _declspec(dllimport)
/*============================================================ 
��Ŀ���� �˻�����ƽ̨ 
�� �ߣ� ��־ѧ
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����ImageTexture.h ImageTexture.cpp 
�� ���� ImageTexture
�� �ࣺ QTextrueInterface
�� �ࣺ �� 
����˵����Ӱ����������
����˵����

-------------------------------------�汾����------------------------------------------------ 
V 1.0 
ԭ���� ����־ѧ
������ڣ�2011��09��20��
============================================================*/

#include "QTextrueInterface.h"
#include <QtCore/qglobal.h>

typedef struct
{
	float m_fImageBoundBoxMax_X;
	float m_fImageBoundBoxMax_Y;
	float m_fImageBoundBoxMin_X;
	float m_fImageBoundBoxMin_Y;

}GeoBoundary;
class Q_DECL_EXPORT ImageTexture :public QTextrueInterface
{
public:
	ImageTexture(void);
	~ImageTexture(void);
public:
	typedef QPair<GLenum, GLint> TEXTURE_PARAM, *P_TEXTURE_PARAM;
	typedef QSet<TEXTURE_PARAM> TEXTURE_PARAM_SET, *P_TEXTURE_PARAM_SET;

	virtual int LoadTextrueFromFile(const QString& strFileName) ;
    virtual int LoadTextrueFromPixelBuffer(const QOpenGLBuffer* pPixelBuffer) ;
	virtual int LoadTextrueFromPixmap(const QPixmap* pPixmap);
	virtual int LoadTextrueFromQimage(const QImage* pQimage);
	virtual int BindTextrue();
	virtual int ApplyTextrueParam();
	//GLuint GetTextrueID(){return texture[0];};
    bool SetGLGLWidget(QOpenGLWindow * sharewidget);
    void SetGLContext(QOpenGLContext* pglContext){m_pGLContext = pglContext;}
	void SetImage(const QImage* pImage){m_pImage = const_cast<QImage*>(pImage);}
private:
	GLuint texture[1];
    QOpenGLWindow *currentWidget;
    QOpenGLContext* m_pGLContext;
	QImage* m_pImage;
};
