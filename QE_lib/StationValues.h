
/*============================================================ 
项目名：CIPAS 
作 者： 赵高攀	
单 位： css
最后修改时间： 2011年6月15日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许
可不得擅自传播。
2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名： StationValues.h StationValues.cpp 
类 名：  StationValues 
父 类：  
子 类： 无 
功能说明： 存储某个站点所有要素的值
调用说明： 
-------------------------------------版本更新------------------------------------------------ 
V 0.1
作 者: 赵高攀
完成日期: 2011-6-15
更新说明: 
----------------------------------------------------------------------------------------
V 0.0 
作 者：
完成日期：
更新说明： 
============================================================*/ 
#pragma once
#include "QRenderUnit.h"
struct ElementValue
{
	QString strName;
	float fvalue;
};
class QE_LIB_EXPORT_IMPORT StationValues
{
public:
	StationValues(void);
	~StationValues(void);
	void SetStationValue(QString strKey,float fvalue);	
	QMap<int,ElementValue>* GetStationValue();
	int GetIndex() {return m_nindex;}

private:
	QMap<int,ElementValue> m_AllElementValue;
	int m_nindex;
};

