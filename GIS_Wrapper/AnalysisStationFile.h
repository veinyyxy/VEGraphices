/*============================================================
文件名：CAnalysisStationFile.h
类 名： CAnalysisStationFile
父 类： 无
子 类： 无
功能说明：读CIPAS站点数据文件，解析文件，将数据存放在哈希表中
调用说明：
1、输入要解析站点文件、站点数据文件的路径和文件名；
2、输出以要素名称为Key值的哈希表，要素值以类的形式存储
---------------------------版本更新---------------------------
V 1.0
原 作 者：renxue
完成日期：2011年10月17日
============================================================*/
#pragma once
#include "GIS_WrapperConfig.h"
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtCore/QTextStream>
#include <QtCore/QHash>
#include <QtCore/QCoreApplication>

#include "../GIS_Wrapper/Const.h"

class  CStationElement
{
public:
	CStationElement(){pElement = NULL;}

	CStationElement(int nRow, int nCol)
	{
		pElement = new int[nRow*nCol];
	}

	~CStationElement()
	{
		delete []pElement;
		pElement = NULL;
	}

public:
	unsigned int nCode;  //站点编号
	double   dLatitude;  //站点纬度
	double   dLongitude; //站点经度
	int *    pElement;
};

class GIS_WRAPPER_EXPORT_IMPORT CAnalysisStationFile
{
public:
	CAnalysisStationFile(void);
	~CAnalysisStationFile(void);
	/************************************************************************
	功能说明：加载CIPAS格式的站点文件和数据文件
	参数说明：StationFileName是站点文件的路径和文件名；
	          DataFileName是数据文件的路径和文件名；
	          正确执行返回true，否则返回false；
	作    者：renxue
	完成日期：2011年10月16日
	************************************************************************/
	bool LoadAllFile(QString DataFileName, QString StationFileName = QString(""));

	/************************************************************************
	功能说明：加载CIPAS格式的站点文件
	参数说明：StationFileName是站点文件的路径和文件名；
	          正确执行返回true，否则返回false；
	作    者：renxue
	完成日期：2011年10月16日
	************************************************************************/
	bool LoadStationFile(QString StationFileName=QString(""));

	int StartYear(){return m_nStartYear;}
	int EndYear(){return m_nEndYear;}
	QHash<QString,CStationElement*>* HashData(){return &m_DataHash;}
	void ClearHashData(){m_DataHash.clear();}
	QVector<QString>* StationName(){return &m_StationName;}

private:
	/************************************************************************
	功能说明：加载数据文件
	参数说明：DataFileName是数据文件的路径和文件名；
	          正确执行返回true，否则返回false；
	作    者：renxue
	完成日期：2011年10月16日
	************************************************************************/
	bool LoadDataFile( QString DataFileName, int nStationNum);

	/************************************************************************
    功能说明：读站点文件，并分别存放文件的数据
    参数说明：txtStream是读取文件的流指针
              正确执行返回true，否则返回false；
    作    者：renxue
    完成日期：2011年10月15日
    ************************************************************************/
	bool ReadStationFile(QTextStream &txtStream);

	/************************************************************************
	功能说明：读取站点的数据文件（R1601、r1602等）
	参数说明：txtStream是读取文件的流指针;
	          pData是缓冲数组的首地址, nLength是pData的长度；
	          正确执行返回true，否则返回false;
	作    者：renxue
	完成日期：2011年10月15日
	************************************************************************/
	bool ReadDataFile(QTextStream &txtStream, int *pData, int nLength);

	/************************************************************************
    功能说明：创建哈希表中存放某一元素数据的二维数组,并全部置零
    参数说明：正确执行返回true，否则返回false；
    作    者：renxue
    完成日期：2011年10月15日
    ************************************************************************/
	bool CreateHashTable();

	/************************************************************************
	功能说明：将某月的文件数据保存到哈希表中的pElement所指的相应空间中
	参数说明：nMonth是月份，pData是存放站点所有数据的起始地址
	          pData是一个二维数据，有m_nYearSpan行，nStationNum列；
	          正确执行返回true，否则返回false；
	作    者：renxue
	完成日期：2011年10月15日
	************************************************************************/
	bool SaveElementData( int nMonth, int *pData);

	void PrintHashTable(QString strName);//for debug

private:
	QTextStream m_txtStream;
	int  m_nStartYear, m_nEndYear; //文件数据的起始年、终止年
	int  m_nYearSpan;
	int* m_pStationDataPerM;       //所有站点历史数据的同月值
	QVector<QString> m_StationName;
	QVector<CStationElement*>  m_StationElement;
	QHash<QString,CStationElement*> m_DataHash;
};
