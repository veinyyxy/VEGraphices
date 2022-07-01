///*============================================================
//文件名：AnalysisStationFile.cpp
//类 名： CAnalysisStationFile
//父 类： QWidget
//子 类： 无
//功能说明：读站点文件，解析文件，将数据存放在哈希表中
//调用说明：
//1、输入要解析指数文件的路径和文件名；
//2、输出以要素名称为Key值的哈希表，要素值以二维数组（年份为行，月份为列）的形式存储
//
//---------------------------版本更新---------------------------
//V 1.0
//原 作 者：renxue
//完成日期：2011年10月17日
//============================================================*/
#include "AnalysisStationFile.h"
#include <QtCore/QDate>

CAnalysisStationFile::CAnalysisStationFile( void )
{
	m_nStartYear = 1951;
//	m_nStartYear = 1971;

	QDate curDate = QDate::currentDate();
	m_nEndYear = curDate.year();

	m_nYearSpan = m_nEndYear - m_nStartYear + 1;
	m_pStationDataPerM = NULL;
}

CAnalysisStationFile::~CAnalysisStationFile( void )
{
	delete m_pStationDataPerM;
	m_pStationDataPerM = NULL;
}

/************************************************************************
功能说明：加载CIPAS格式的站点文件和数据文件
参数说明：StationFileName是站点文件的路径和文件名；
          DataFileName是数据文件的路径和文件名；
          正确执行返回true，否则返回false；
作    者：renxue
完成日期：2011年10月16日
************************************************************************/
bool CAnalysisStationFile::LoadAllFile( QString DataFileName, QString StationFileName)
{
	if( DataFileName.isEmpty())
	{
		QMessageBox msgBox;
		msgBox.setText("There is something wrong with name of file!");
		msgBox.exec();
		return false;
	}

	int nStationNum = m_StationName.size();
	if(nStationNum == 0)
	{
		if(!LoadStationFile(StationFileName)) return false;
		nStationNum = m_StationName.size();
	}

	if(!CreateHashTable()) return false;

	QString TempFileName(DataFileName);
	int nLenth = DataFileName.size();
	QString leftName = DataFileName.left(nLenth-2);
	QVector<QString> rightName;
	rightName<<"01"<<"02"<<"03"<<"04"<<"05"<<"06"<<"07"<<"08"<<"09"<<"10"<<"11"<<"12";
	int i = 0;
	while(i<12)     //按照月份读数据文件,共需要读12个文件
	{
		TempFileName = leftName + rightName.at(i);
		if(!LoadDataFile(TempFileName, nStationNum)) return false;
		if(!SaveElementData(i+1, m_pStationDataPerM)) return false;
		i++;
	}
	return true;
}

/************************************************************************
功能说明：加载CIPAS格式的站点文件
参数说明：StationFileName是站点文件的路径和文件名；
          正确执行返回true，否则返回false；
作    者：renxue
完成日期：2011年10月16日
************************************************************************/
bool CAnalysisStationFile::LoadStationFile( QString StationFileName )
{
	if(StationFileName.isEmpty())
	{
		StationFileName = CIPASApplication::DataPath()+ "/HistoryCurveData/160sta_chn.TXT";
		//StationFileName = "G:\\Station\\160sta_chn.TXT";
	}

	QFile StationFile(StationFileName);
	if (!StationFile.open(QFile::ReadOnly | QFile::Text)) 	return false;
	m_txtStream.setDevice(&StationFile);

	ReadStationFile(m_txtStream);

	StationFile.close();
	if(StationFile.error() != QFile::NoError)
	{
		QMessageBox msgBox;
		msgBox.setText("Error occured when the station file was closed!");
		msgBox.exec();
		return false;
	}

	return true;
}

/************************************************************************
功能说明：加载数据文件
参数说明：DataFileName是一月份的数据文件路径、文件名；
          正确执行返回true，否则返回false；
作    者：renxue
完成日期：2011年10月16日
************************************************************************/
bool CAnalysisStationFile::LoadDataFile( QString DataFileName, int nStationNum)
{
	if( DataFileName.isEmpty())
	{
		QMessageBox msgBox;
		msgBox.setText("There is something wrong with name of file!");
		msgBox.exec();
		return false;
	}

	QFile DataFile(DataFileName);
	if(!DataFile.open(QFile::ReadOnly | QFile::Text))  return false;
	m_txtStream.setDevice(&DataFile);

	if(m_pStationDataPerM)
	{
		delete []m_pStationDataPerM;
		m_pStationDataPerM = NULL;
	}
	int nLength = nStationNum * m_nYearSpan;
	m_pStationDataPerM = new int[nLength];
	memset(m_pStationDataPerM, 0, nLength*sizeof(int));
	ReadDataFile(m_txtStream, m_pStationDataPerM, nLength);

	DataFile.close();
	if(DataFile.error() != QFile::NoError)
	{
		QMessageBox msgBox;
		msgBox.setText("Error occured when the station file was closed!");
		msgBox.exec();
		return false;
	}

	return true;
}

/************************************************************************
功能说明：读站点数据文件，并分别存放文件的数据
参数说明：txtStream是读取文件的流指针
          正确执行返回true，否则返回false；
作    者：renxue
完成日期：2011年10月15日
************************************************************************/
bool CAnalysisStationFile::ReadStationFile( QTextStream &txtStream )
{
	QString strLine, strTemp;
	QStringList strList;

	strLine = txtStream.readLine();  //文件的第一行说明
	int nLength = 0;
	while(!txtStream.atEnd())
	{
		strLine = txtStream.readLine();
        strList = strLine.split("\\s+", Qt::SkipEmptyParts);
		nLength = strList.size();

		CStationElement *pStation = new CStationElement;
		pStation->nCode = strList.at(nLength-3).toUInt();
		pStation->dLatitude = strList.at(nLength-2).toDouble();
		pStation->dLongitude = strList.at(nLength-1).toDouble();
		m_StationElement<<pStation;

		strTemp.clear();
		for(int i=1; i<nLength-3; i++)
		{
			strTemp += strList.at(i);
		}
		m_StationName<<strTemp;
	}
	return true;
}

/************************************************************************
功能说明：读取站点的数据文件（R1601、r1602等）
参数说明：txtStream是读取文件的流指针；
          nStationNum是站点的个数, nLength是pData的长度；
          正确执行返回true，否则返回false；
作    者：renxue
完成日期：2011年10月15日
************************************************************************/
bool CAnalysisStationFile::ReadDataFile( QTextStream &txtStream, int *pData, int nLength )
{
	if(!pData ) return false;

	QStringList strList;
	int nListSize, i, j;
	QString strLine;
	j = 0;
	while(!m_txtStream.atEnd())
	{
		//按照行读数据文件的数据
		strLine = m_txtStream.readLine();
        strList = strLine.split("\\s+", Qt::SkipEmptyParts);
		nListSize = strList.size();
		for(i=0; i<nListSize; i++ )
		{
			*pData++ = strList.at(i).toInt();
		}
		strList.clear();
		j += nListSize;
	}
	while(j<nLength)
	{
		*pData++ = 999999;
		j++;
	}
	return true;
}

/************************************************************************
功能说明：创建哈希表，其中动态申请了存放站点数据的二维数组,并全部置999999
参数说明：正确执行返回true，否则返回false；
作    者：renxue
完成日期：2011年10月15日
************************************************************************/
bool CAnalysisStationFile::CreateHashTable()
{
	if(m_StationName.size() != m_StationElement.size()) return false;
	if(m_nYearSpan == 0) return false;

	int *pAddress, nSize;
	CStationElement *pStation;
	nSize = m_StationName.size();
	for(int i=0; i<nSize; i++)
	{
		pAddress = new int[12*m_nYearSpan];
		memset(pAddress, 0, 12*m_nYearSpan*sizeof(int));
		pStation = m_StationElement.at(i);
		pStation->pElement = pAddress;
		m_DataHash.insert(m_StationName.at(i), pStation);
	}
	return true;
}

/************************************************************************
功能说明：将某月的文件数据保存到哈希表中的pElement所指的相应空间中
参数说明：nMonth是月份，pData是存放站点所有数据的起始地址
          pData是一个二维数据，有m_nYearSpan行，nStationNum列；
          正确执行返回true，否则返回false；
作    者：renxue
完成日期：2011年10月15日
************************************************************************/
bool CAnalysisStationFile::SaveElementData(int nMonth, int *pData)
{
	if(nMonth<1 || !pData) 	return false;

	//转存数据
	int nStationNum = m_StationElement.size();
	int *pAddress, *pTemp, j;
	for(int i=0; i<nStationNum; i++)
	{
		pAddress = m_StationElement.at(i)->pElement;
		pAddress += (nMonth-1)*m_nYearSpan;
		pTemp = pData + i;
		for(j=0; j<m_nYearSpan; j++)
		{
			*pAddress++ = *pTemp;
			pTemp += nStationNum;
		}
	}

	return true;
}

/************************************************************************
功能说明：为了调试，将哈希表中的内容写入一个文件

作    者：renxue
完成日期：2011年10月11日
************************************************************************/
void CAnalysisStationFile::PrintHashTable(QString strName)
{
	QFile file;
	QTextStream txtStream;

	file.setFileName(strName);
	if(!file.open(QFile::WriteOnly | QFile::Text))
	{
		QMessageBox msgBox;
		msgBox.setText("Debug: Open file is wrong!");
		msgBox.exec();
		return ;
	}
	txtStream.setDevice(&file);
    //txtStream.setCodec("UTF-8");

	QHash<QString, CStationElement*>::Iterator i = m_DataHash.begin();

	while(i != m_DataHash.end())
	{
        txtStream<<i.key()<<Qt::endl;
		int *p = i.value()->pElement;
		for(int k=0; k<12; k++)
		{
			for(int j=0; j<61; j++)
			{
				txtStream.setFieldWidth(7);
				txtStream.setFieldAlignment(QTextStream::AlignRight);
				txtStream<<*p++;
			}
            txtStream<<Qt::endl;
		}
		i++;
	}

	file.close();
	if(file.error() != QFile::NoError)
	{
		QMessageBox msgBox;
		msgBox.setText("Debug: error occured when file was closed!");
		msgBox.exec();
	}
	return ;
}
