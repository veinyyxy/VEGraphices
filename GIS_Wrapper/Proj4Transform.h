/*=================================================================================== 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ������
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011��4��18��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����Proj4Transform.h Proj4Transform.cpp 
�� ���� Proj4Transform
�� �ࣺ CoordinateTransform
�� �ࣺ �� 
����˵����GISͶӰ����任������
����˵����
------------------------------------------------------------------------------------*/

#pragma once

#include "GIS_WrapperConfig.h"
extern "C"
{
#include <proj.h>
}
#include "CoordinateTransform.h"
//#include "cpl_string.h"
//#include <ogr_spatialref.h>

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
    PJ_CONTEXT* m_pContext;
    PJ *mSourceProjection, *mDestinationProjection;
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



