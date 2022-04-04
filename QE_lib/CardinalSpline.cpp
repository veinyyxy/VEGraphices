/*============================================================ 
项目名： 人机交互平台
作 者： lugsh
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：CardinalSpline.h CardinalSpline.cpp 
类 名： CCardinalSpline
父 类： NULL
子 类： 无 
功能说明： 基数样条插值算法
调用说明： 

-------------------------------------版本更新------------------------------------------------ 
V 1.2 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：lugsh
完成日期：2011年？月？日
============================================================*/

#include "CardinalSpline.h"
#include <math.h>
#define BOUNDS(pp) { if (pp < 0) pp = 0; else if (pp >= (int)m_ControlPoints.size()-1) pp = m_ControlPoints.size() - 1; }
const  int  CONTROL_NUM = 300 ;

/************************************************************************/
/* 函数功能： 默认构造函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
CCardinalSpline::CCardinalSpline(void) 
	:m_ControlPoints(),m_delta(0)
{
}

/************************************************************************/
/* 函数功能： 析构函数							                        */
/* 参数说明:  NULL														*/
/* 返 回 值:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
CCardinalSpline::~CCardinalSpline(void)
{
}

/************************************************************************/
/* 函数功能： 构造函数							                        */
/* 参数说明:  NULL														*/
/* 返 回 值:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
CCardinalSpline::CCardinalSpline(const CCardinalSpline& s)
{
	for (int i = 0; i < (int)s.m_ControlPoints.size(); i++)
		m_ControlPoints.push_back(s.m_ControlPoints[i]);
	m_delta = s.m_delta;
}

/************************************************************************/
/* 函数功能： 根据次序取出平滑点				                        */
/* 参数说明:  NULL														*/
/* 返 回 值:  QVector3D   返回平滑算法									*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
QVector3D CCardinalSpline::GetInterpolatedSplinePoint(float t)
{
	int p = (int)(t / m_delta);
	int p0 = p - 1;     BOUNDS(p0);
	int p1 = p;         BOUNDS(p1);
	int p2 = p + 1;     BOUNDS(p2);
	int p3 = p + 2;     BOUNDS(p3);
	float lt = (t - m_delta*(float)p) / m_delta;
	return CCardinalSpline::Eq(lt, m_ControlPoints[p0], m_ControlPoints[p1], m_ControlPoints[p2], m_ControlPoints[p3]);
}

/************************************************************************/
/* 函数功能： 返回控制点个数					                        */
/* 参数说明:  NULL														*/
/* 返 回 值:  int   返回控制点数据										*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
int CCardinalSpline::GetNumPoints()
{
	return m_ControlPoints.size();
}

/************************************************************************/
/* 函数功能： 取出指定的控制点					                        */
/* 参数说明:  int n   ：控制点顺序										*/
/* 返 回 值:  QVector3D  返回指定控制点									*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
QVector3D& CCardinalSpline::GetNthPoint(int n)
{
	return m_ControlPoints[n];
}

/************************************************************************/
/* 函数功能： 按照次序取出平滑数据点			                        */
/* 参数说明:  float t       ：次序数据									*/
/*			  QVector3D& p1 ：分隔点1									*/
/*			  QVector3D& p2 ：分隔点2									*/
/*			  QVector3D& p3 ：分隔点3									*/
/*			  QVector3D& p4 ：分隔点4									*/
/* 返 回 值:   QVector3D   插值点数据   								*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
QVector3D CCardinalSpline::Eq(float t, const QVector3D& p1, const QVector3D& p2, const QVector3D& p3, const QVector3D& p4)
{
	float t2 = t * t;
	float t3 = t2 * t;

	float b1 = .5 * (  -t3 + 2*t2 - t);
	float b2 = .5 * ( 3*t3 - 5*t2 + 2);
	float b3 = .5 * (-3*t3 + 4*t2 + t);
	float b4 = .5 * (   t3 -   t2    );

	return (p1*b1 + p2*b2 + p3*b3 + p4*b4); 
}

/************************************************************************/
/* 函数功能： 添加基数样条平滑控制点			                        */
/* 参数说明:   NULL														*/
/* 返 回 值:   NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void CCardinalSpline::AddSplinePoint(const QVector3D& v)
{
	m_ControlPoints.push_back(v);
	m_delta = (float)1 / (float)m_ControlPoints.size();
}

/************************************************************************/
/* 函数功能： 清除基数样条对象控制点			                        */
/* 参数说明:  NULL														*/
/* 返 回 值:   NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void CCardinalSpline::ClearSplinePoint()
{
	m_ControlPoints.Clear(); 
}

/************************************************************************/
/* 函数功能： 通过输入控制点数据，输出平滑点数据                        */
/* 参数说明:  QVector3DArray  inputData  : 控制点数据					*/
/*			  QVector3DArray  inputData  : 输出平滑点数据				*/	
/* 返 回 值:   NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void CCardinalSpline::GetSmoothPoints(QVector3DArray &inputData, QVector3DArray &OutputData)
{
	int  nSum = 0;
	
	ClearSplinePoint();										//清除原有控制点数据
	if(OutputData.count() > 0 ) OutputData.Clear();			//清除输出平滑点数据

	if(inputData.count() <= 0 )  return;

	//设置控制点
	for(int i = 0 ; i < inputData.count(); i++)
	{
		AddSplinePoint(inputData[i]);
	}
	QVector3D lastPoint = inputData[inputData.count() -1];

	nSum = inputData.count()*10;

	//取得平滑点
	for(int j = 0 ; j < nSum; j++)//CONTROL_NUM
	{
		float t = (float)j / (float)nSum;//CONTROL_NUM;
		QVector3D point = GetInterpolatedSplinePoint(t);
		OutputData.push_back(point);

		float x = point.x() - lastPoint.x();
		float y = point.y() - lastPoint.y();

		if(abs(x) <1 && abs(y) < 1 &&j >=20) 
			break;
	}
}

//融合两条平滑线
void CCardinalSpline::MergeSmoothLines(QVector3DArray &OutPutData, QVector3DArray &OutputData2,QVector3DArray &OutPutDataSum)
{
	if(OutPutData.count() <= 0 )  return;
	if(OutputData2.count() <= 0 )  return;

	int index_begin = 0;
	int index_end = OutPutData.count()-1;
	int nCount2 = OutputData2.count();
	OutPutDataSum.clear();

	for(int i=OutPutData.count()-1;i>0;i--)
	{
		if (OutPutData[i] == OutputData2[0]) index_end = i;		
	}
	for (int i=0;i<OutPutData.count()-1;i++)
	{
		if (OutPutData[i] == OutputData2[nCount2-1]) index_begin = i;		
	}
	
	//merge data
	for (int i=index_begin;i<index_end;i++)
	{
		OutPutDataSum.push_back(OutPutData[i]);
	}
	for (int i=0;i<nCount2;i++)
	{
		OutPutDataSum.push_back(OutputData2[i]);
	}	
}