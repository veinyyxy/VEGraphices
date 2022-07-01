/*============================================================
项目名： 人机交互平台
作 者： 陈志学
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年4月12日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
-------------------------------------------------------------------------------------
文件名：SymbleLayer.h SymbleLayer.cpp
类 名： SymbleLayer
父 类： 无
子 类： 无
功能说明：站点层
调用说明：

-------------------------------------版本更新------------------------------------------------
V 1.3
作 者：卢高昇
完成日期：4月14
更新说明：修改符号存储方式
V 1.2
作 者：陈志学
完成日期：4月14
更新说明：
V 1.1
作 者：输入作者（或修改者）名字
完成日期：
更新说明：
V 1.0
原作者 ：陈志学
完成日期：2011年4月14日
============================================================*/
#pragma once
#include "vectorlayer.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QPluginLoader>
#include "Nccstation.h"
#include "Nccclosedcontour.h"
#include "Point.h"
#include "ContourData.h"
#include "FunPluginInterface.h"
//#include "StationInterpretation.h"
#include "SymbolData.h"
//#include "Nccstation.cpp"

#include "../GIS_Wrapper/Const.h"

using namespace NCC;

class Nccstation;

class COLOR_PROPERTY
{
public:
	COLOR_PROPERTY(QString Name,QColor color,int Zcount):_Name(Name),_color(color),_Zcount(Zcount){}
	COLOR_PROPERTY(){}
	~COLOR_PROPERTY() {}
public:
	QString _Name;
	QColor _color;
	int _Zcount;
};

typedef QMap<double,COLOR_PROPERTY> COLORTABLE;				//颜色表
//typedef QMap<double,QColor> COLORTABLE;				//颜色表
typedef QMap<QString,COLORTABLE> COLORTABLELIST;    //颜色表列表，存储多个颜色表的数据结构
static COLORTABLELIST colortablelist;

class GIS_WRAPPER_EXPORT_IMPORT SymbolLayer :
	public VectorLayer
{
	Q_OBJECT
public:
	SymbolLayer(void);
	~SymbolLayer(void);
	//void setMapTree(MapTree* mapTree);
	//QVector<CSymbolContourLine*>  m_contArray;								//被框选的等值线
	/*StationDataArray  *m_stArray;*/
	//StationInterpretation * GetStation();
	QList<NccStation> stationList;//站点List（吴）
	QList<NccClosedContour> closedContourList;//等值线List（吴）
public:
	bool addData(const QString filepath,const int type);					// 添加数据
	void  DrawStation();													// 绘制站点
	void  StartProfile(QVector3D & point);									//开始绘制选框
	void  ProcessProfile(QVector3D & point);								//绘制选框
	void  EndProfile(QVector3D & point);									//结束选框
	//void UpdateLayer(QGLWidget* widget);
	void UpdateLayer(ILayerAttribute* iLA);
	//StationDataArray*  GlobalStationPoints(const int staType=4);
	void  Inversion(bool isGlobal);											//反演
	SymbolData* getSymbolData();					//获得存点容器
	inline void setSymbolData(SymbolData* sd){m_SymobolData = sd;}
	void  SelectContourLines(QVector<CSymbolContourLine *> &m_contArray);
	QList<NccStation>& GetStationList(bool isGlobal,const int staType=4);
	QList<NccClosedContour>  GetClosedContour(bool isGlobal);
	void SetStationType(int staType=4);
	void SetIsInterpol(bool isInterpol);
	void DisplayInverRestult(QString);// 返回框选的等值线
	inline void RemoveSymbolContourLine();
	inline void ReloadSymbolContourLine(bool isfill);

public:
	virtual void InitAttribute();
	
private:
	void  DrawID();															// 绘制ID
	int   getAttribut();													// 绘制那些站点
#if 0
	StationDataArray*  SelectStationPoints(const int staType=4);								// 返回框选的站点坐标
#endif
	int initMutualLayerAttribute();
private:
	int  bDraw;
	//StationInterpretation  *stationData;									//站点数据源
	Point  *stPoint;
	GISPolygon  *m_pRect;													//用于绘制选框
	bool  m_bStart;															//标识为开始绘制选框
	QVector3D  m_IniPoint;													//选框左上角点
	QVector3D  m_EndniPoint;												//选框右下角点
	Proj4Transform  *m_Proj4Trans;											//用于将投影坐标转换成经纬坐标
	bool isGlobal;
	SymbolData*   m_SymobolData;
//	MapTree* m_MapTree;
	Layer* pLayer;
	bool bFirst;															//第一次绘制
	int m_StaType;															//站点类型 基本 一般等等
	bool m_IsInterpol;														//是否插值
signals:
	void KXsingals(QList<NccClosedContour> *closedContourList,QList<NccStation> *stationList, bool isInterpol);
};
