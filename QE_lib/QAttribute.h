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
文件名：QAttribute.h QAttribute.cpp 
类 名： QAttribute
父 类： QObject
子 类： 有
功能说明： 图形引擎中所有绘图属性类的父类
调用说明： 
================================================================*/ 
#pragma once
#include "QT_Include.h"
#include "GL_Include.h"

typedef enum _ATTRI_TYPE
{
	QE_ATTRI_UNKONW = 0,
	QE_ATTRI_POINT_SIZE = 1,
	QE_ATTRI_LINE_STIPPLE,
	QE_ATTRI_LINE_WIDTH,
	QE_ATTRI_COLOR,
	QE_ATTRI_TEXTURE0
} ATTRI_TYPE, *P_ATTRI_TYPE;

class QE_LIB_EXPORT_IMPORT QAttribute
{
public:
	QAttribute(ATTRI_TYPE t = QE_ATTRI_UNKONW);
	virtual ~QAttribute(void);
	ATTRI_TYPE Type();
	/************************************************************************/
	/* 函数功能： 将属性设置应用至OpenGL中	     	                        */
	/* 参数说明:  NULL														*/
	/* 作    者:  lugsh														*/
	/* 日    期:  2011 年 04 月 15 日										*/
	/************************************************************************/
	virtual int Applay() = 0;
	virtual int Recover() = 0;
protected:
	ATTRI_TYPE type;
};

typedef QVector<QAttribute*> ATTRIBUTE_VECTOR, *P_ATTRIBUTE_VECTOR;
typedef QVectorIterator<QAttribute*> ATTRIBUTE_VECTOR_ITERATOR, *P_ATTRIBUTE_VECTOR_ITERATOR;
typedef QMap<ATTRI_TYPE, QAttribute*> ATTRIBUTE_MAP, *P_ATTRIBUTE_MAP;
typedef QMapIterator<ATTRI_TYPE, QAttribute*>  ATTRIBUTE_MAP_ITERATOR, *P_ATTRIBUTE_MAP_ITERATOR;