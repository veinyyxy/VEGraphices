/*============================================================ 
文件名：AnalysisExponent.cpp
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
#include "AnalysisExponent.h"

CAnalysisExponent::CAnalysisExponent( void )
{
	m_nStartYear = m_nStartMonth = 0;
	m_nEndYear = m_nEndMonth =0;
	m_nYearSpan = 0;
}

CAnalysisExponent::~CAnalysisExponent( void )
{
	for(int i=0; i<m_ElementAddress.size(); i++)
	{
		delete m_ElementAddress.at(i);
	}
	m_ElementAddress.clear();
}

/************************************************************************
功能说明：加载CIPAS格式的指数文件，根据文件名进行相应操作
参数说明：FileName是路径和文件名
          正确执行返回true，否则返回false；
作    者：renxue
完成日期：2011年10月26日
************************************************************************/
bool CAnalysisExponent::LoadFile( QString FileName )
{
	if(FileName.isEmpty() || !FileName.contains(".cps")) 
	{
		QMessageBox msgBox;
		msgBox.setText("There is something wrong with name of file!");
		msgBox.exec();
		return false;
	}

	QString name(FileName);
	int nLenth = name.size();
	name.remove(0, nLenth-6);
	name.remove(2,4);
	int nElementSize = name.toInt(); //怎么能从文件名得到ElementSize? 

	QFile file(FileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) 	return false;

	m_txtStream.setDevice(&file);
    //m_txtStream.setCodec("UTF-8");

	ReadFileHead(m_txtStream);

	if(m_ElementName.size()!=nElementSize)
	{
// 		QMessageBox msgBox;
// 		msgBox.setText("Size of Element is wrong!");
// 		msgBox.exec();
// 		return false;
		nElementSize = m_ElementName.size();
	}

	switch(nElementSize)
	{
	case 74:
		m_FileName = QObject::tr("环流特征量");
		break;
	case 15:
		m_FileName = QObject::tr("降水指数");
		break;
	case 9:
		m_FileName = QObject::tr("海温指数");
		break;
	case 8:
		m_FileName = QObject::tr("温度等级");
		break;
	default:
		break;
	}

	CreateHashTable(nElementSize);
	ReadData(m_txtStream, nElementSize);
	file.close();
	if(file.error() != QFile::NoError) 
	{
		QMessageBox msgBox;
		msgBox.setText("Error occured when the file was closed!");
		msgBox.exec();
		return false;
	}
}

/************************************************************************
功能说明：创建哈希表中存放某一元素数据的二维数组,并全部置零
参数说明：nNum是元素的个数
          正确执行返回true，否则返回false；
作    者：renxue
完成日期：2011年10月11日
************************************************************************/
bool CAnalysisExponent::CreateHashTable( int nNum )
{
	int *pAddress;
	for(int i=0; i<nNum; i++)
	{
		pAddress = new int[12*m_nYearSpan];
		memset(pAddress, 0, 12*m_nYearSpan*sizeof(int));
		m_ElementAddress.push_back(pAddress);
	}
	return true;
}

/************************************************************************
功能说明：读取CIPAS格式的指数文件头信息，目前只处理了TIME和ATTRIBUTE信息
参数说明：txtStream是读取文件的流指针
          正确执行返回true，否则返回false；
作    者：renxue
完成日期：2011年10月11日
************************************************************************/
bool CAnalysisExponent::ReadFileHead( QTextStream &txtStream )
{
	int nEnd, nStart, nNum;
	QString strLine, strTemp;
	QStringList strList;
	int nLineNum = 0;
	while(!m_txtStream.atEnd() && nLineNum<8)
	{
		strLine = m_txtStream.readLine();
				
		if(strLine.contains("TIME"))
		{
            strList = strLine.split("\\s+",Qt::SkipEmptyParts);
			m_nStartYear = strList.at(2).toInt();
			m_nStartMonth = strList.at(3).toInt();
			m_nEndYear = strList.at(5).toInt();					
			m_nEndMonth = strList.at(6).toInt();
		}
		else if(strLine.contains("ATTRIBUTES"))
		{
            strList = strLine.split("\\s+",Qt::SkipEmptyParts);
			int nSize = strList.size();
			for(int i=7; i<nSize; i+=2)
			{
				m_ElementName<<strList.at(i);
			}			
		}
		nLineNum++;
	}
	m_nYearSpan = m_nEndYear - m_nStartYear+1;
	return true;
}

/************************************************************************
功能说明：将指数元素值保存到m_nElementAddress所指的空间中
参数说明：nYear是元素的年，nMonth是元素的月;
          nElementPos是元素位置,从零开始;
          正确执行返回true，否则返回false；
作    者：renxue
完成日期：2011年10月11日
************************************************************************/
bool CAnalysisExponent::SaveElementData(int nYear, int nMonth, int nElementPos, int nData)
{
	if(m_ElementAddress.size() == 0) 	return false;
	
	int *pAddress = m_ElementAddress.at(nElementPos);
	int i = nYear - m_nStartYear;
	*(pAddress+ (nMonth-1)*m_nYearSpan + i) = nData;
	return true;
}

/************************************************************************
功能说明：将指数元素名和存储位置保存到哈希表中
参数说明：目前使用的参数都为成员变量，故在此未写参数
          正确执行返回true，否则返回false；
作    者：renxue
完成日期：2011年10月11日
************************************************************************/
bool CAnalysisExponent::FillHashTable()
{
	//debug-------renxue--------			
	//QFile file;
	//QTextStream txtStream;

	//file.setFileName("G:\\test----TC008.cipas");			
	//if(!file.open(QFile::WriteOnly | QFile::Text))
	//{
	//	QMessageBox msgBox;
	//	msgBox.setText("Debug: Open file is wrong!");
	//	msgBox.exec();
	//	return false;
	//}
	//txtStream.setDevice(&file);
	//txtStream.setCodec("UTF-8");
	//end------debug------------

	if(m_ElementName.size()==m_ElementAddress.size() && m_ElementName.size()!=0)
	{
		int nSize = m_ElementName.size();
		int i=0;
		while(i<nSize)
		{
			m_DataHash.insert(m_ElementName.at(i), m_ElementAddress.at(i));

			//debug-------renxue--------			
			//txtStream<<m_ElementName.at(i)<<endl;
			//int *p = m_ElementAddress.at(i);
			//for(int i=0; i<12; i++)
			//{
			//for(int j=0; j<61; j++)
			//{
			//txtStream.setFieldWidth(7);
			//txtStream.setFieldAlignment(QTextStream::AlignRight);
			//txtStream<<*p++;
			//}
			//txtStream<<endl;
			//}
			//end------debug------------

			i++;
		}
		return true;
	}
	
	QMessageBox msgBox;
	msgBox.setText("There is something wrong about filling hash table！");
	msgBox.exec();
	return false;
}

/************************************************************************
功能说明：读取CIPAS格式的指数文件（RC015、TC008、HC074、SC009）
参数说明：txtStream是读取文件的流指针
          nElementSize是不同文件的元素个数
          正确执行返回true，否则返回false；
作    者：renxue
完成日期：2011年10月13日
************************************************************************/
bool CAnalysisExponent::ReadData( QTextStream &txtStream, int nElementSize )
{
	QStringList strList;
	int nYear, nMonth, nPos, nDataNum;
	QString strLine;
	int nLineNum = 0;
	int nSumLineNum = (m_nYearSpan-1) * 12 + m_nEndMonth;
	nYear = nMonth = nPos = 0;
	int valuetemp;

	if(nElementSize == 74)
	{
		nDataNum = 2 + nElementSize/2;
		while(!m_txtStream.atEnd() && nLineNum<nSumLineNum)
		{
			//指数文件的数据行第一行
			strLine = m_txtStream.readLine();
            strList = strLine.split("\\s+", Qt::SkipEmptyParts);
			nYear = strList.at(0).toInt();
			nMonth = strList.at(1).toInt();
			for(int i=2; i<nDataNum; i++ )
			{
				nPos = i-2;
				valuetemp = strList.at(i).toInt();
				SaveElementData(nYear, nMonth, nPos,valuetemp);
			}
			strList.clear();

			//指数文件的数据行第二行
			strLine = m_txtStream.readLine();
            strList = strLine.split("\\s+", Qt::SkipEmptyParts);
			nDataNum = strList.size(); 
			for(int i=0; i<nDataNum; i++ )
			{
				nPos = i + 37;
				valuetemp = strList.at(i).toInt();
				SaveElementData(nYear, nMonth, nPos, valuetemp);
			}
			strList.clear();
			nLineNum++;
		}
	}
	else
	{
		while(!m_txtStream.atEnd() && nLineNum<nSumLineNum)
		{
			//指数文件的数据行
			strLine = m_txtStream.readLine();
            strList = strLine.split("\\s+", Qt::SkipEmptyParts);
			nYear = strList.at(0).toInt();
			nMonth = strList.at(1).toInt();
			nDataNum = strList.size();
			for(int i=2; i<nDataNum; i++ )
			{
				nPos = i-2;
				valuetemp = strList.at(i).toInt();
				SaveElementData(nYear, nMonth, nPos,valuetemp/3);
			}
			strList.clear();
			nLineNum++;
		}
	}
	FillHashTable();
	return true;
}
