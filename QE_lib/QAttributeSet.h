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
文件名：QAttributeSet.h QAttributeSet.cpp 
类 名： QAttributeSet
父 类： ATTRIBUTE_MAP
子 类： 有
功能说明： 图形引擎中所有绘图属性的集合容器
调用说明： 
================================================================*/ 
#pragma once
#include "QT_Include.h"
#include "GL_Include.h"
#include "QAttribute.h"

class QE_LIB_EXPORT_IMPORT QAttributeSet : public ATTRIBUTE_MAP
{
public:
	QAttributeSet(void);
	~QAttributeSet(void);
	void Applay();
	void Recover();
};

