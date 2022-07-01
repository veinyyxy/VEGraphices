/*============================================================
项目名：CIPAS
作 者： 赵高攀
单 位： css
最后修改时间： 2011年4月20日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved
重要声明：
-------------------------------------------------------------------------------------
文件名： OptMicapsDataXml.h OptMicapsDataXml.cpp
类 名：  OptMicapsDataXml
父 类：
子 类： 无
功能说明： 对micapsdataxml文件的操作（读写）,用于初始化界面按钮，为按钮提供操作属性
调用说明： 返回所需的Vector容器
-------------------------------------版本更新------------------------------------------------
V 0.1
作 者: 赵高攀
完成日期: 2011-04-29
更新说明: 添加绘制算法和函数。做相应的注释
----------------------------------------------------------------------------------------
V 0.0
作 者：
完成日期：
更新说明：
============================================================*/
#pragma once
#include <QtCore/QXmlStreamReader>
#include <QtWidgets/QTreeWidget>
#include <QtCore/QDebug>
#include <QtCore/QVector>

#include "../GIS_Wrapper/Const.h"

struct MicapsDataC
{
	QString CBtnName;
	QString EnBtnName;
	QString CFileType;
	QString CDataPath;
	QString CDataValue;
	QString Cicon;
	QString CRes;
};
struct MicapsDataB
{
	int Depth;
	QString BBtnName;
	QString EnBtnName;
	QString BFileType;
	QString BDataPath;
	QString BDataValue;
	QString Bicon;
	QString BRes;
	QVector<MicapsDataC> CMicapsDataArray;
};

struct MicapsDataA
{
	int Depth;
	QString ABtnName;
	QString EnBtnName;
	QString Aicon;
	QVector<MicapsDataB> BMicapsDataArray;
};

class  Q_DECL_EXPORT OptMicapsDataXml
{
public:
	OptMicapsDataXml();
	~OptMicapsDataXml();
//func
public:
	bool WriteMicapsBtnData(QTreeWidget *treeWidget);						//写入MicapsDataXml中按钮相关数据
	bool WriteMicapsToolData(QTreeWidget *treeWidget,QString xmlFileName);						//写入MicapsToolXml中按钮相关数据
	bool WriteMapPathData(QTreeWidget *treeWidget,QString xmlFileName);
	bool WriteMapColorConfigData(QObjectList *objectList,QString xmlFileName);//地图背景，前景色保存

	bool ReadMicapsBtnData();						//读取MicapsDataXml中按钮相关数据
	bool ReadMeteToolBoxData();                     //读取MeteToolBoxXml中按钮相关数据--renxue

	bool ReadMapColorConfigFile(QColor& bcolor,QColor& fcolor,bool& AlasCheck,QString& sysname,QString& projectionname);

	void SetMicapsDataFileName(const QString &filename);				//设置Xml文件名
	QVector<MicapsDataA> GetMicapsBtnDataArray();						//获取Xml中按钮数据
private:
	void ReadMapColorConfig(QColor& bcolor,QColor& fcolor,bool& AlasCheck,QString& sysname,QString& projectionname);
	void ReadColor(QString name,QColor& bcolor);
	void Readbool(QString name,bool& AlasCheck);
	void ReadString(QString name,QString& sysname);
//	void ReadProjectionName(QString& projectionname);
private:
	void SkipUnKnownElement();
	void ReadItemBtnData();														//读取MicapsDataXml中按钮Item数据
	void WriteMicapsIndexEntry(QXmlStreamWriter *xmlWriter,QTreeWidgetItem *item);	//写入MicapsDataXml中按钮Item数据
//data
private:
	QXmlStreamReader   m_XmlReader;
	QVector<MicapsDataA> m_MicapsBtnDataArray;
	QString m_FileName;
};
