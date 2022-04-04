/*=================================================================================== 
项目名： 人机交互平台
作 者： 杨晓宇
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年4月18日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：Point.h Point.cpp 
类 名： Point
父 类： Geometry
子 类： 无 
功能说明：封装GIS点功能的绘制
调用说明：
------------------------------------------------------------------------------------*/
#ifndef POINT_H
#define POINT_H
#include "GIS_WrapperConfig.h"
#include "Geometry.h"
#include "Symbol.h"
#include "StationValues.h"

class GIS_WRAPPER_EXPORT_IMPORT Point : public Geometry
{
public:
    Point();
	inline Line* AsLine();
	inline Plane* AsPlane();
	inline GISPolygon* AsPolygon();
	inline Point* AsPoint();
	inline Symbol* PointSymbol();
	/*inline QVector4DArray* getColorArray();*/
	inline QVector2DArray* getTextureAttray();
	inline QVector3DArray* getNormalAttray();
	inline void setPointSize(float fsize){glPointSize(fsize);};//点的大小

	inline QVector<StationValues>* StationValueArray();//站点数据zhaogaopan
private:
	Symbol* symbol;
	QVector<StationValues> m_SValuesArray;//站点数据数组
};

#endif