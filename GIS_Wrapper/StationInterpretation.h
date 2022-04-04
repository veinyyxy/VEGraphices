#pragma once
#include "GIS_WrapperConfig.h"
#include "QFile"
#include "../CDataElement/Station.h"
#include "QtXml/QXmlStreamReader"
#include <QPointer>
#include <QtSql/QSqlQuery>
#include "../QE_lib/QDataArray.h"


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
文件名：StationInterpretation.h StationInterpretation.cpp 
类 名： StationInterpretation
父 类： 无
子 类： 无 
功能说明：站点XML数据解析工具
调用说明： 

-------------------------------------版本更新------------------------------------------------ 
V 1.2 
作 者：陈志学
完成日期：4月12
更新说明： 
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：陈志学
完成日期：2011年4月11日
============================================================*/


typedef struct StationInfo                   //站点信息结构体
{
	double   dvalue ;                        //属性要素值
	QString  id;                             //Id;
	QString  staNumb;						//站号
	QString  staType;                        //站点类型
	QVector3D pSymbolPoints;   //点坐标数组    
}StationInfo;

typedef QVector  <StationInfo>  StationDataArray;       //站点信息结构体数组

class GIS_WRAPPER_EXPORT_IMPORT StationInterpretation // : public QXmlStreamReader
{
public:
	StationInterpretation(void);
	~StationInterpretation(void);
	   bool readFile(const QString &filename,const int type); //打开XML文件
	   bool isEmpty();              //判断是否有数
       StationDataArray * getStationPointArray(const int type);  
		//QList<CStation*>   stList; 
private:
	   void readRowDataElement(); // 解析一个含有0个或多个<ROW>元素的<ROWDATA>元素
	   void readRowElement();     // 解析一个含有0个或多个其它元素的<ROW>元素、
	QString readC0Element();      // 解析一个ID元素
	QString readCHNameElement();      // 解析一个Name元素
	QString readV01000Element();      // 解析一个V01000元素( 区号	数字（以下要素均为实型数）当为船舶站时此字段为马士顿号)
	 double readV05001Element();      // 解析一个V05001元素(纬度	单位:度 )
	 double readV06001Element();      // 解析一个V06001元素(经度	单位：度)
	 double readV07001Element();      // 解析一个V07001元素(测站高度	单位：M)
	 double readV07033Element();      // 解析一个V07033元素
	QString readV02001Element();      // 解析一个V02001元素(台站类型	见编码表02001)
	QString readCHAreaProvinceElement();      // 解析一个CH_AREA_PROVINCE元素
	QString readCHAreaCityElement();          // 解析一个CH_AREA_CITY元素
	QString readCHAreaCountyElement();        // 解析一个CH_AREA_COUNTY元素
	QString readBYStatusElement();            // 解析一个BY_STATUS元素
	QString readCHOBSTypeElement();           // 解析一个CH_OBS_TYPE元素
	QString readCHBusnessTypeElement();       // 解析一个CH_BUSINESS_TYPE元素
	QString readCHStartYearElement();         // 解析一个CH_START_YEAR元素
	QString readCHStartMonthElement();        // 解析一个CH_START_MONTH元素
	QString readCHEndYearElement();           // 解析一个CH_END_YEAR元素
	QString readCHEndMonthElement();          // 解析一个CH_END_MONTH元素
	QString readROWIDElement();			      // 解析一个ROWID元素

	   void skipUnknownElement();             // 忽略不能识别的元素
	  // bool addDataIntoDB();                  // 向内存数据库添加数据
	  // bool selectFromDB();                   // 从内存数据库提取站点数据
	  

private:
	       //CDataBase   db;
	QXmlStreamReader   reader;	 	  
	StationDataArray*  stationPointArray;       //所有站点
	StationDataArray*  stationPointArray1;      //一般站点
	StationDataArray*  stationPointArray2;      //基本站点
	StationDataArray*  stationPointArray3;      //基准站点
	int m_stationType;

};
