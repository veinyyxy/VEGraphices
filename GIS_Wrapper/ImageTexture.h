#pragma once
#define CORE_EXPORT _declspec(dllimport)
#define GUI_EXPORT _declspec(dllimport)
/*============================================================ 
项目名： 人机交互平台 
作 者： 陈志学
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：ImageTexture.h ImageTexture.cpp 
类 名： ImageTexture
父 类： QTextrueInterface
子 类： 无 
功能说明：影像数据纹理化
调用说明：

-------------------------------------版本更新------------------------------------------------ 
V 1.0 
原作者 ：陈志学
完成日期：2011年09月20日
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
