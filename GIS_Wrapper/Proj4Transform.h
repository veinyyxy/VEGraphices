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
文件名：Proj4Transform.h Proj4Transform.cpp 
类 名： Proj4Transform
父 类： CoordinateTransform
子 类： 无 
功能说明：GIS投影坐标变换抽象类
调用说明：
------------------------------------------------------------------------------------*/

#pragma once

#include "GIS_WrapperConfig.h"
extern "C"
{
#include <proj_api.h>
}
#include "CoordinateTransform.h"
#include "cpl_string.h"
#include <ogr_spatialref.h>

typedef QString PROJCRS;
typedef QString PROJNAME;

//typedef QMap<PROJCRS,PROJNAME>PRO_MAP,*PRO_MAP;
//typedef QMap<PROJCRS,PROJNAME>GEOCOORD_MAP,*GEOCOORD_MAP;

class GIS_WRAPPER_EXPORT_IMPORT Proj4Transform : public CoordinateTransform
{
public:
	Proj4Transform(void);
	~Proj4Transform(void);
	virtual QVector3D Transform(const QVector3D vertex, TRANSFORM_DIRECTION direction = FORWARD_DIRECTION);
	virtual QVector3D Transform(GLdouble gLx, GLdouble gLy, GLdouble gLz, TRANSFORM_DIRECTION direction = FORWARD_DIRECTION);
	virtual int Transform(const QVector3DArray& src, QVector3DArray& dst, TRANSFORM_DIRECTION direction = FORWARD_DIRECTION);
	virtual int Transform(const QVector3D& src, QVector3D& dst);
	virtual QRectF TransformBoundingBox(const QRectF rectangle, TRANSFORM_DIRECTION direction = FORWARD_DIRECTION);
	virtual void setDestCRS (QString ProjString);
	QString getDesCRS();
	virtual void setSourceCRS (QString ProjString);
	QString getSourceCRS();
	QString ImportFromESRI(const char *  pszFname );

 	QString getDesProjName();
 	QString getSourceProjName();
	void setSourceProjName(QString ProjString);
	void setDesProjName(QString ProjString);
	void initProMap();
	void initGeoMap();

private:
	projPJ mSourceProjection,mDestinationProjection;
	QString strSourceProjection, strDestinationProjection;
	QString strSourceProName,strDestinationProName;
	int i,projResult;
	QString msg,points;
	qreal x,y,z;
	qreal xMAX,yMAX,xMIN,yMIN;
	void initialise();
	int transformCoords( const int &numPoint, double *x, double *y, double *z, TRANSFORM_DIRECTION direction = FORWARD_DIRECTION );

private:	
	QMap<PROJCRS,PROJNAME> Pro_map;
	QMap<PROJCRS,PROJNAME> Geo_map;
//	PRO_MAP Pro_map;
//	GEOCOORD_MAP Geo_map;
};



