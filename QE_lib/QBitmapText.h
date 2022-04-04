/*============================================================ 
项目名： 人机交互平台
作 者： 杨晓宇
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年4月15日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：QCamera.h QCamera.cpp 
类 名： QCamera
父 类： QObject
子 类： 无
功能说明： 位图文字的实现，此类在OpenGL光栅化时将文字添加到渲染表面，
		   效率较低，尽量开启渲染列对其渲染，以提高效率
调用说明： 
================================================================*/ 
#pragma once
#include "QRenderUnit.h"
#include "StationValues.h"

class QE_LIB_EXPORT_IMPORT QBitmapText :
	public QRenderUnit
{
public:
	QBitmapText(void);
	virtual ~QBitmapText(void);
	void Render();
	inline int InsertRenderable(const QRenderable* unit);
	inline int RemoveRenderable(QRenderable* unit);
	inline QVector<QString>* StringArray();
	inline QVector3DArray* Pointion();
	inline void* FontSize();
	inline void FontSize(void* fs);
	inline void SetColor(const QAttribute* color);
	inline void SetXOffset(float);
	inline void SetYOffset(float);
	inline QVector<StationValues>* StationValueArray();
	bool PickUp(const QVector3D* pickPoint);//chenzhixue--20110622
	int GetPickIndex();                    //chenzhixue--20110622
	void SetPickIndex();                  //chenzhixue--20110622
	void CallDisplayList(int i);
protected:
	void TextRender();
	void TextRenderUseCallList();
	QVector<QString> stringArray;
	QVector3DArray postion;
	void* size;
	//QAttributeSet attribSet;
	float m_fXAlpha;    // OFFSET OF X
	float m_fYAlpha;    // OFFSET OF Y
	QVector<StationValues> m_SValuesArray;//站点数据数组
	int m_iPickIndex;
	GLsizei m_iListRang;
	QVector<int> m_ListVector;
	GLuint m_uiList;
	GLuint m_uiBitmapTextList;
};

