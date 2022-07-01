#pragma once
#include <QtCore/QtPlugin>
#include <Qtcore/QString>
#include "Nccclosedcontour.h"
#include "dem.h"
#include "tin.h"
#include "NccStation.h"
using namespace NCC;
class FunPluginInterface
{
	
public:
	//FunPluginInterface(void);
	//~FunPluginInterface(void);
	virtual QString GetString()const=0;
	
	//控制点未处理
	virtual bool GetSortedContour(QList<NccClosedContour> *closedContourList,QList<NccClosedContour> *sortedClosedContourList)=0;
	//不插值计算多边形内的站点值
	virtual bool ProcessPointsNonInterpol(QList<NccStation> *stationList,QList<NccClosedContour> *sortedClosedContourList)=0;
		
	virtual bool GetContourFromQList(QList<NccClosedContour> *closedContourList, CBlockContour *pBlockContour)=0;

	//处理控制点
	/*************************************************
	Function:       GetContourFromQListPoint
	Description:    获取控制点
	Input:          控制点集合
					等值线集合地址
	Output:         
	Return:         true
	Others:         
	*************************************************/
	virtual bool GetContourFromQListPoint(QList<NccPoint> pointList, CBlockContour *pBlockContour)=0;
	
	//支持带控制点的情况
	virtual bool CreateTin(CBlockContour* pBlockContour, CBlockTIN* pBlockTIN)=0;
	//返回处理后的站点集合
	virtual bool TriangleInterpol(QList<NccStation> *stationList,CBlockTIN* pBlockTIN)=0;
	//判断点是否在多边形内部
	virtual bool bIsPointInPolygon(QList<NccPoint> pointList, NccPoint* pt)=0;
	//计算多边形面积 2d
	virtual double ProfileArea2D(QList<NccPoint> pointList)=0;

};

Q_DECLARE_INTERFACE(FunPluginInterface, "CSS.NCC.FunPluginInterface") 
