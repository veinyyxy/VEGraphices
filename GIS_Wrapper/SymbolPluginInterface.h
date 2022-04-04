#include <QtCore\QObject>
#include "GIS_WrapperConfig.h"
/*=================================================================================== 
项目名： 人机交互平台
作 者： 卢高昇
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年4月18日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：CSymbolPluginInterface.h 
类 名： CSymbolPluginInterface
父 类： Object
子 类： 无 
功能说明：传递符号线算法数据数据，收集符号线算法输出数据
调用说明：
------------------------------------------------------------------------------------*/
#pragma once
#include "Symbol.h"
class GIS_WRAPPER_EXPORT_IMPORT CSymbolPluginInterface :public QObject
{
public:
	virtual Symbol* GetSymbol(int type,float fSpeed =0) = 0;
	virtual bool DeleteSymbol(int type) = 0;
	virtual bool CreateSymbol(int type) = 0;
	virtual bool DeleteAllSymbol() = 0 ;
};