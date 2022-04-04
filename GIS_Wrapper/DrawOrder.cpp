/*============================================================ 
项目名：CIPAS 
作 者： 董健	
单 位： css
最后修改时间： 2011年7月18日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许
可不得擅自传播。
2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名： DrawOrder.h DrawOrder.cpp 
类 名：  CDrawOrder 
父 类：  无
子 类： 无 
功能说明： 判断弧段绘制方向为顺时针方向还是逆时针方向
调用说明： 类调用说明：一共两个参数，其一为待判断的弧段本身，另一个参数为弧段是否闭合，true为闭合，false为不闭合。
-------------------------------------版本更新------------------------------------------------ 
============================================================*/  
#include "DrawOrder.h"


CDrawOrder::CDrawOrder(void)
{
}


CDrawOrder::~CDrawOrder(void)
{
}
/************************************************************************/
/* 函数功能： 判断弧段绘制方向						                        */
/* 参数说明: 参数1为绘制的弧段本身，参数2为弧段是否闭合，返回值为true，则弧段为逆时针，否则为顺时针														*/
/* 作    者:  董健														*/
/* 日    期:  2011 年 07 月 18 日										*/
/************************************************************************/
bool CDrawOrder::isConvert(QVector3DArray *pData,bool bisClosed)
{

	int nCount = pData->count();
	QVector3D p1, p2, p3,pMax,pMaxTemp; 
	int i;
	pMax = pData->at(0);
	int pMaxIndex =0;
	for(i=0; i<nCount; i++)	
	{   
		pMaxTemp =  pData->at(i);	  
		if(pMaxTemp.y()>pMax.y())
		{  
			pMax = pMaxTemp;
			pMaxIndex = i;
		}
	}
	if(bisClosed = true)   //如果isClosed = true，即曲线闭合，则取曲线最大点即周围两点
	{
		if(pMaxIndex==nCount-1)
		{
			p1 = pData->at(pMaxIndex-1);
			p2 = pData->at(pMaxIndex);
			p3 = pData->at(0);
		}
		else if (pMaxIndex == 0 )
		{
			p1 = pData->at(nCount-2);
			p2 = pData->at(pMaxIndex);
			p3 = pData->at(pMaxIndex+1);
		}
		else
		{
			p1 = pData->at(pMaxIndex-1);
			p2 = pData->at(pMaxIndex);
			p3 = pData->at(pMaxIndex+1);
		}// 遍历获取要编辑的等高线的y坐标最大的点
	}
	else    //如果曲线不闭合，则取0点，中点，终点
	{
		p1 = pData->at(0);
		p2 = pData->at(pData->count() / 2);
		p3 = pData->at(pData->count() - 1);
	}

	

	//判断原来是逆时针还是顺时针
	float fVector =( p2.x() - p1.x()) * (p3.y() - p2.y()) - (p2.y() - p1.y()) *(p3.x() - p2.x());
	bool bIsConvert ; 
	if(fVector > 0 )
	{
		bIsConvert = true;
	}
	else
	{
		bIsConvert = false;
	}
	return bIsConvert;
}