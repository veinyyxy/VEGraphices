/*============================================================ 
项目名： 人机交互平台
作 者： 任雪
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：AlgorithmInterface.h AlgorithmInterface.cpp 
类 名： CObject
父 类： 无
子 类： 无 
功能说明： 双线、箭头、四种锋线、霜冻线算法的算法接口
调用说明： 虚类

-------------------------------------版本更新------------------------------------------
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：任雪
完成日期：2011年04月02日
============================================================*/

#include "AlgorithmInterface.h"

CAlgorithmInterface::CAlgorithmInterface(void)
{
}

CAlgorithmInterface::~CAlgorithmInterface(void)
{
}

bool CAlgorithmInterface::WindAlgorithm(float fSpeed, QVector<QVector3DArray> &outputLine )
{
	return false;
}

