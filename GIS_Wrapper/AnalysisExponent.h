/*============================================================ 
文件名：AnalysisExponent.h 
类 名： CAnalysisExponent
父 类： QWidget
子 类： 无 
功能说明：读CIPAS指数文件，解析文件，将数据存放在哈希表中
调用说明：
1、输入要解析指数文件的路径和文件名；
2、输出以要素名称为Key值的哈希表，要素值以二维数组（年份为行，月份为列）的形式存储
---------------------------版本更新---------------------------
V 1.0 
原 作 者：renxue
完成日期：2011年10月13日
============================================================*/
#pragma once
#include "GIS_WrapperConfig.h"
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QWidget>
#include <QTextStream>
#include <QHash>

class  GIS_WRAPPER_EXPORT_IMPORT CAnalysisExponent 
{
public:
	CAnalysisExponent(void);
	~CAnalysisExponent(void);
	/************************************************************************
	功能说明：加载CIPAS格式的指数文件，根据文件名进行相应操作
	参数说明：FileName是路径和文件名
	          正确执行返回true，否则返回false；
	作    者：renxue
	完成日期：2011年10月11日
	************************************************************************/
	bool LoadFile(QString FileName);

	QString  FileName(){return m_FileName;}
	QStringList ElementName(){return m_ElementName;}
	int StartYear(){return m_nStartYear;}
	int EndYear(){return m_nEndYear;}

private:
	/************************************************************************
    功能说明：创建哈希表中存放某一元素数据的二维数组,并全部置零
    参数说明：nNum是元素的个数
              正确执行返回true，否则返回false；
    作    者：renxue
    完成日期：2011年10月11日
    ************************************************************************/
	bool CreateHashTable(int nNum);

	/************************************************************************
	功能说明：读取CIPAS格式的指数文件头信息，目前只处理了TIME和ATTRIBUTE信息
	参数说明：txtStream是读取文件的流指针
	          正确执行返回true，否则返回false；
	作    者：renxue
	完成日期：2011年10月11日
	************************************************************************/
	bool ReadFileHead(QTextStream &txtStream);

	/************************************************************************
	功能说明：将指数元素值保存到m_nElementAddress所指的空间中
	参数说明：nYear是元素的年，nMonth是元素的月
	          nElementPos是元素位置
	          正确执行返回true，否则返回false；
	作    者：renxue
	完成日期：2011年10月11日
	************************************************************************/
	inline bool SaveElementData(int nYear, int nMonth, int nElementPos, int nData);

	/************************************************************************
	功能说明：将指数元素名和存储位置保存到哈希表中
	参数说明：目前使用的参数都为成员变量，故在此未写参数
	          正确执行返回true，否则返回false；
	作    者：renxue
	完成日期：2011年10月11日
	************************************************************************/
	bool FillHashTable();

	/************************************************************************
	功能说明：读取CIPAS格式的指数文件（RC015、TC008、HC074、SC009）
	参数说明：txtStream是读取文件的流指针
	          nElementSize是不同文件的元素个数
	          正确执行返回true，否则返回false;
	作    者：renxue
	完成日期：2011年10月13日
	************************************************************************/
	bool ReadData(QTextStream &txtStream, int nElementSize);

public:
	QHash<QString,int*> m_DataHash;

private:
	QTextStream m_txtStream;
	int  m_nStartYear, m_nStartMonth; //文件数据的起始年、月
	int  m_nEndYear, m_nEndMonth;     //文件数据的终止年、月
	int  m_nYearSpan;
	QString       m_FileName;
	QStringList   m_ElementName;
	QVector<int*> m_ElementAddress; 
};

