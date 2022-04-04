/*============================================================
��Ŀ����CIPAS
�� �ߣ� �Ը���
�� λ�� css
����޸�ʱ�䣺 2011��4��20��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved
��Ҫ������
-------------------------------------------------------------------------------------
�ļ����� OptMicapsDataXml.h OptMicapsDataXml.cpp
�� ����  OptMicapsDataXml
�� �ࣺ
�� �ࣺ ��
����˵���� ��micapsdataxml�ļ��Ĳ�������д��,���ڳ�ʼ�����水ť��Ϊ��ť�ṩ��������
����˵���� ���������Vector����
-------------------------------------�汾����------------------------------------------------
V 0.1
�� ��: �Ը���
�������: 2011-04-29
����˵��: ��ӻ����㷨�ͺ���������Ӧ��ע��
----------------------------------------------------------------------------------------
V 0.0
�� �ߣ�
������ڣ�
����˵����
============================================================*/
#pragma once
#include "qglobal.h"
#include "QtXml/QXmlStreamReader"
#include <QtGui/QTreeWidget>
#include <QDebug>
#include "qvector.h"

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
	bool WriteMicapsBtnData(QTreeWidget *treeWidget);						//д��MicapsDataXml�а�ť�������
	bool WriteMicapsToolData(QTreeWidget *treeWidget,QString xmlFileName);						//д��MicapsToolXml�а�ť�������
	bool WriteMapPathData(QTreeWidget *treeWidget,QString xmlFileName);
	bool WriteMapColorConfigData(QObjectList *objectList,QString xmlFileName);//��ͼ������ǰ��ɫ����

	bool ReadMicapsBtnData();						//��ȡMicapsDataXml�а�ť�������
	bool ReadMeteToolBoxData();                     //��ȡMeteToolBoxXml�а�ť�������--renxue

	bool ReadMapColorConfigFile(QColor& bcolor,QColor& fcolor,bool& AlasCheck,QString& sysname,QString& projectionname);

	void SetMicapsDataFileName(const QString &filename);				//����Xml�ļ���
	QVector<MicapsDataA> GetMicapsBtnDataArray();						//��ȡXml�а�ť����
private:
	void ReadMapColorConfig(QColor& bcolor,QColor& fcolor,bool& AlasCheck,QString& sysname,QString& projectionname);
	void ReadColor(QString name,QColor& bcolor);
	void Readbool(QString name,bool& AlasCheck);
	void ReadString(QString name,QString& sysname);
//	void ReadProjectionName(QString& projectionname);
private:
	void SkipUnKnownElement();
	void ReadItemBtnData();														//��ȡMicapsDataXml�а�ťItem����
	void WriteMicapsIndexEntry(QXmlStreamWriter *xmlWriter,QTreeWidgetItem *item);	//д��MicapsDataXml�а�ťItem����
//data
private:
	QXmlStreamReader   m_XmlReader;
	QVector<MicapsDataA> m_MicapsBtnDataArray;
	QString m_FileName;
};