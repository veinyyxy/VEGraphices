/*============================================================
文件名： ConvertExponentToCipasFormat.cpp
类  名： CConvertFormat
父  类： QWidget
子  类： 无
功能说明：将指数文件保存成CIPAS格式的文件
调用说明：

----------------------------版本更新----------------------------
V 1.0
原作者 ： renxue
完成日期：2011年10月25日
============================================================*/
#include <fstream>
#include <iomanip>
#include <QtCore/QTime>
#include <QtCore/QDate>
#include <QtWidgets/QMessageBox>
#include "ConvertExponentToCipasFormat.h"

CConvertFormat::CConvertFormat( void )
{
	m_pFile = NULL;
	m_pDestTxtStream = NULL;
}

CConvertFormat::~CConvertFormat( void )
{
	delete m_pFile;
	delete m_pDestTxtStream;
	m_pFile = NULL;
	m_pDestTxtStream = NULL;
}

/************************************************************************
功能说明：设置需要读入的文件的文件路径和文件名
参数说明：fileName是包含路径和文件名的字符串
          返回值为bool型，正确执行返回true，错误或异常返回false
作    者：renxue
完成日期：2011年10月10日
************************************************************************/
bool CConvertFormat::SetSourceFileName(QString fileName)
{
	if(fileName.isEmpty()) return false;
	ReadExponentFile(fileName);
	return true;
}

/************************************************************************
 功能说明：读取指数文件
 参数说明：fileName是包含路径和文件名的字符串
           返回值为bool型，正确执行返回true，错误或异常返回false
 作    者：renxue
 完成日期：2011年10月09日
************************************************************************/
bool CConvertFormat::ReadExponentFile( QString fileName )
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) 	return false;

	m_txtStream.setDevice(&file);

	QString name(fileName);
	int nLength = name.length();
	name.remove(0, nLength-5);
	//建立转存后的文件
	m_pFile = new QFile;
	m_pDestTxtStream = new QTextStream;
	if(!m_pFile || !m_pDestTxtStream) return false;

	QDate curDate = QDate::currentDate();
	m_FileName = CIPASApplication::DataPath()+"/HistoryCurveData/Exponent/"+curDate.toString(Qt::ISODate)+ "_" +name + ".cps";
	m_pFile->setFileName(m_FileName);
	if(!m_pFile->open(QFile::WriteOnly | QFile::Text))
	{
		QMessageBox msgBox;
		msgBox.setText( "Cannot open file for writing!");
		msgBox.exec();
		return false;
	}

	m_pDestTxtStream->setDevice(m_pFile);
    //m_pDestTxtStream->setCodec("UTF-8");

	//分类转存
	if(fileName.contains("HC074"))      //读HC074环流指数数据块
	{
		ConvertHC074(m_txtStream);
	}
	else if(fileName.contains("SC009")) //读SC009海温指数数据块
	{
		ConvertSC009(m_txtStream);
	}
	else if(fileName.contains("TC008")) //读TC008温度等级数据块
	{
		ConvertTC008(m_txtStream);
	}
	else if(fileName.contains("RC015")) //读RC015降水指数数据块
	{
		ConvertRC015(m_txtStream);
	}

	file.close();

	if(file.error() != QFile::NoError)
	{
		return false;
	}
	return true;
}

/************************************************************************
 功能说明：写CIPAS指数文件:没行数据前的年、月值
 参数说明：nYear是该行元素的年数据，nMonth是该行元素的月数据，
           ptxtStream是写入文件的流指针；
           无返回值；
 作    者：renxue
 完成日期：2011年10月10日
************************************************************************/
void CConvertFormat::WriteDate(int &nYear, int &nMonth, QTextStream *ptxtStream)
{
	if(nMonth>12)
	{
		nMonth = 1;
		nYear++;
	}
	ptxtStream->setFieldAlignment(QTextStream::AlignRight);
	ptxtStream->setFieldWidth(4);
	*ptxtStream<<nYear;

	ptxtStream->setFieldAlignment(QTextStream::AlignRight);
	ptxtStream->setFieldWidth(3);
	*ptxtStream<<nMonth;
	nMonth++;
}

/************************************************************************
 功能说明：写CIPAS降水指数文件（RC015）的头信息
 参数说明：ptxtStream是写入文件的流指针
           无返回值；
 作    者：renxue
 完成日期：2011年10月10日
************************************************************************/
void CConvertFormat::WriteRC015FileHead( QTextStream *ptxtStream )
{
    *ptxtStream<<"CIPAS 4 2 8"<<Qt::endl;
    *ptxtStream<<"#HEADINFO"<<Qt::endl;
    *ptxtStream<<"#DESCRIPTION "<<'"'<<"precipitation index"<<'"'<<Qt::endl;
    *ptxtStream<<"#DATA_STORAGE_FORMAT TXT"<<Qt::endl;
	QDate curDate = QDate::currentDate();
    *ptxtStream<<"#TIME MMON 1951 01 -999 "<<curDate.year()<<" "<<curDate.month()<<" -999"<<Qt::endl;
	int nyear = curDate.year()-1951+1;
	*ptxtStream<<"#ATTRIBUTES "<<nyear<<" 17 Years int Months int "
		<<QString::fromLocal8Bit("兴安岭区降水指数 int 松辽平原区降水指数 int 内蒙古降水指数 int 华北区降水指数 int 淮河区降水指数 int ")
		<<QString::fromLocal8Bit("长江中下游区降水指数 int 江南区降水指数 int 华南区降水指数 int 云南区降水指数 int 川贵区降水指数 int ")
        <<QString::fromLocal8Bit("河套区降水指数 int 河西区降水指数 int 北疆区降水指数 int 南疆区降水指数 int 高原区降水指数 int")<<Qt::endl;
    *ptxtStream<<"#FORMATDEFINE d4 d3 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7"<<Qt::endl;
}

/************************************************************************
 功能说明：写CIPAS温度等级指数文件（TC008）的头信息
 参数说明：ptxtStream是写入文件的流指针
           无返回值；
 作    者：renxue
 完成日期：2011年10月10日
************************************************************************/
void CConvertFormat::WriteTC008FileHead( QTextStream *ptxtStream )
{
    *ptxtStream<<"CIPAS 4 2 8"<<Qt::endl;
    *ptxtStream<<"#HEADINFO"<<Qt::endl;
    *ptxtStream<<"#DESCRIPTION "<<'"'<<"temperature index"<<'"'<<Qt::endl;
    *ptxtStream<<"#DATA_STORAGE_FORMAT TXT"<<Qt::endl;
	QDate curDate = QDate::currentDate();
    *ptxtStream<<"#TIME MMON 1951 01 -999 "<<curDate.year()<<" "<<curDate.month()<<" -999"<<Qt::endl;
	int nyear = curDate.year()-1951+1;
	*ptxtStream<<"#ATTRIBUTES "<<nyear<<" 10 Years int Months int "
		       <<QString::fromLocal8Bit("东北区温度等级 int 华北区温度等级 int 长江中下游区温度等级 int 华南区温度等级 int ")
               <<QString::fromLocal8Bit("西南区温度等级 int 西北区温度等级 int 新疆区温度等级 int 全国温度等级 int")<<Qt::endl;
    *ptxtStream<<"#FORMATDEFINE d4 d3 d7 d7 d7 d7 d7 d7 d7 d7"<<Qt::endl;
}

/************************************************************************
 功能说明：写74环流CIPAS指数文件的头信息
 参数说明：ptxtStream是写入文件的流指针
           无返回值；
 作    者：renxue
 完成日期：2011年10月10日
************************************************************************/
void CConvertFormat::WriteHC074FileHead(QTextStream *ptxtStream)
{
    *ptxtStream<<"CIPAS 4 2 8"<<Qt::endl;
    *ptxtStream<<"#HEADINFO"<<Qt::endl;
    *ptxtStream<<"#DESCRIPTION "<<'"'<<"sstidx key SST index"<<'"'<<Qt::endl;
    *ptxtStream<<"#DATA_STORAGE_FORMAT TXT"<<Qt::endl;
	QDate curDate = QDate::currentDate();
    *ptxtStream<<"#TIME MMON 1951 01 -999 "<<curDate.year()<<" "<<curDate.month()<<" -999"<<Qt::endl;
	int nyear = curDate.year()-1951+1;
	*ptxtStream<<"#ATTRIBUTES "<<nyear<<QString::fromLocal8Bit(" 76 Years int Months int 北半球副高面积指数 int 北非副高面积指数 int 北非大西洋北美副高面积指数 int 印度副高面积指数 int 西太平洋副高面积指数 int 东太平洋副高面积指数 int 北美副高面积指数 int 大西洋副高面积指数 int ")
		<<QString::fromLocal8Bit("南海副高面积指数 int 北美大西洋副高面积指数 int 太平洋副高面积指数 int 北半球副高强度指数 int 北非副高强度指数 int 北非大西洋北美副高强度指数 int 印度副高面积强度指数 int 西太平洋副高强度指数 int 东太平洋副高强度指数 int 北美副高强度指数 int 大西洋副高强度指数 int 南海副高强度指数 int 北美大西洋副高强度指数 int ")
		<<QString::fromLocal8Bit("太平洋副高强度指数 int 北半球副高脊线 int 北非副高脊线 int 北非大西洋北美副高脊线 int 印度副高脊线 int 西太平洋副高脊线 int 东太平洋副高脊线 int 北美副高脊线 int 大西洋副高脊线 int 南海副高脊线 int 北美大西洋副高脊线 int 太平洋副高脊线 int 北半球副高北界 int 北非副高北界 int 北非大西洋北美副高北界 int 印度副高北界 int 西太平洋副高北界 int ")
		<<QString::fromLocal8Bit("东太平洋副高北界 int 北美副高北界 int 大西洋副高北界 int 南海副高北界 int 北美大西洋副高北界 int 太平洋副高北界 int 西太平洋副高西伸脊点 int 亚洲区极涡面积指数 int 太平洋区极涡面积指数 int 北美区极涡面积指数 int 大西洋-欧洲区极涡面积指数 int 北半球极涡面积指数 int 亚洲区极涡强度指数 int 太平洋区涡强度指数 int 北美区极涡强度指数 int 大西洋-欧洲区极涡强度指数 int 北半球极涡强度指数 int 北半球极涡中心位置 int ")
        <<QString::fromLocal8Bit("北半球极涡中心强度 int W大西洋-欧洲环流型 int C大西洋-欧洲环流型 int E大西洋-欧洲环流型 int 欧亚纬向(IZ,0-150E)环流指数 int 欧亚经向(IM,0-150E)环流指数 int 亚洲纬向(IZ,60-150E)环流指数 int 亚洲经向(IM,60-150E)环流指数 int 位置东亚槽 int 强度东亚槽 int (25N-35N,80E-100E)西藏高原 int (30N-40N,75E-105E)西藏高原 int 印缅槽 int 冷空气 int 编号台风 int 登陆台风 int 太阳黑子 int 南方涛动指数SOI int")<<Qt::endl;
    *ptxtStream<<"#FORMATDEFINE d4 d3 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7"<<Qt::endl;
}

/************************************************************************
 功能说明：写CIPAS海温指数文件（SC009）的头信息
 参数说明：ptxtStream是写入文件的流指针
           无返回值；
 作    者：renxue
 完成日期：2011年10月10日
************************************************************************/
void CConvertFormat::WriteSC009FileHead( QTextStream *ptxtStream )
{
    *ptxtStream<<"CIPAS 4 2 8"<<Qt::endl;
    *ptxtStream<<"#HEADINFO"<<Qt::endl;
    *ptxtStream<<"#DESCRIPTION "<<'"'<<"SST index"<<'"'<<Qt::endl;
    *ptxtStream<<"#DATA_STORAGE_FORMAT TXT"<<Qt::endl;
	QDate curDate = QDate::currentDate();
    *ptxtStream<<"#TIME MMON 1951 01 -999 "<<curDate.year()<<" "<<curDate.month()<<" -999"<<Qt::endl;
	int nyear = curDate.year()-1951+1;
	*ptxtStream<<"#ATTRIBUTES "<<nyear<<" 11 Years int Months int "
		       <<QString::fromLocal8Bit("亲潮区指数 int 西风漂流区指数 int 黑潮区指数 int NINO1区指数 int NINO2区指数 int ")
               <<QString::fromLocal8Bit("NINO1+2区指数 int NINO3区指数 int NINO4区指数 int 监测小组分区指数 int")<<Qt::endl;
    *ptxtStream<<QString::fromLocal8Bit("#FORMATDEFINE d4 d3 d7 d7 d7 d7 d7 d7 d7 d7 d7")<<Qt::endl;
}

/************************************************************************
 功能说明：转存74环流指数文件（HC074）为CIPAS格式的指数文件
 参数说明：ptxtStream是写入文件的流指针
           正确执行返回true，否则返回false；
 作    者：renxue
 完成日期：2011年10月13日
************************************************************************/
bool CConvertFormat::ConvertHC074( QTextStream &txtStream )
{
	WriteHC074FileHead(m_pDestTxtStream);

	//写数据
    *m_pDestTxtStream<<"#DATASET"<<Qt::endl;
	int nYear = 1951;
	int nMonth = 1;
	int nListSize = 0;
	int nTemp = 0;
	QString strLine;
	QStringList strList;

	while(!m_txtStream.atEnd())
	{
		WriteDate(nYear, nMonth, m_pDestTxtStream);
		//读第一行数据
		strLine = m_txtStream.readLine();
        strList = strLine.split("\\s+", Qt::SkipEmptyParts);
		nListSize = strList.size();

	    for(int i=0; i<nListSize; i++)
		{
			if(strList.at(i).isEmpty()) continue;
			m_pDestTxtStream->setFieldWidth(7);
			m_pDestTxtStream->setFieldAlignment(QTextStream::AlignRight);
			nTemp = strList.at(i).toInt();
			if(nTemp == 999)
			{
				*m_pDestTxtStream<<999999;
			}
			else
			{
				*m_pDestTxtStream<<nTemp;
			}
		}
        *m_pDestTxtStream<<Qt::endl;

		//读第二行数据
		*m_pDestTxtStream<<QString::fromLocal8Bit("       ");//为与上一行对齐，写入7个空格；
		strLine = m_txtStream.readLine();
        strList = strLine.split("\\s+", Qt::SkipEmptyParts);
		nListSize = strList.size();

		for(int i=0; i<nListSize; i++)
		{
			if(strList.at(i).isEmpty()) continue;
			m_pDestTxtStream->setFieldWidth(7);
			m_pDestTxtStream->setFieldAlignment(QTextStream::AlignRight);
			nTemp = strList.at(i).toInt();
			if(nTemp == 999)
			{
				*m_pDestTxtStream<<999999;
			}
			else
			{
				*m_pDestTxtStream<<nTemp;
			}
		}
        *m_pDestTxtStream<<Qt::endl;
	}

	m_pFile->close();
	if(m_pFile->error() != QFile::NoError)
	{
		return false;
	}
	return true;
}

/************************************************************************
 功能说明：转存海温指数文件（SC009）为CIPAS格式的指数文件
 参数说明：ptxtStream是写入文件的流指针
           正确执行返回true，否则返回false；
 作    者：renxue
 完成日期：2011年10月13日
************************************************************************/
bool CConvertFormat::ConvertSC009( QTextStream &txtStream )
{
	WriteSC009FileHead(m_pDestTxtStream);//写文件头信息

	//写数据
    *m_pDestTxtStream<<"#DATASET"<<Qt::endl;
	int nYear = 1951;
	int nMonth = 1;
	int nListSize = 0;
	int nTemp = 0;
	QString strLine, strTemp;
	QStringList strList;

	while(!m_txtStream.atEnd())
	{
		WriteDate(nYear, nMonth, m_pDestTxtStream);

		strLine = m_txtStream.readLine();
        strList = strLine.split("\\s+", Qt::SkipEmptyParts);
		nListSize = strList.size();
		for(int i=0; i<nListSize; i++)
		{
			if(strList.at(i).isEmpty())
			{
				continue;
			}

			m_pDestTxtStream->setFieldWidth(7);
			m_pDestTxtStream->setFieldAlignment(QTextStream::AlignRight);
			nTemp = strList.at(i).toInt();
			if(nTemp == 999)
			{
				*m_pDestTxtStream<<999999;
			}
			else
			{
				*m_pDestTxtStream<<nTemp;
			}
		}
        *m_pDestTxtStream<<Qt::endl;
	}

	m_pFile->close();
	if(m_pFile->error() != QFile::NoError)
	{
		return false;
	}
	return true;
}

/************************************************************************
 功能说明：转存温度等级指数文件（TC008）为CIPAS格式的指数文件
 参数说明：ptxtStream是写入文件的流指针
           正确执行返回true，否则返回false；
 作    者：renxue
 完成日期：2011年10月13日
************************************************************************/
bool CConvertFormat::ConvertTC008( QTextStream &txtStream )
{
	WriteTC008FileHead(m_pDestTxtStream);//写文件头信息

	//写数据
    *m_pDestTxtStream<<"#DATASET"<<Qt::endl;
	int nYear = 1951;
	int nMonth = 1;
	int nListSize = 0;
	int nTemp = 0;
	QString strLine;
	QStringList strList;

	while(!m_txtStream.atEnd())
	{
		WriteDate(nYear, nMonth, m_pDestTxtStream);

		strLine = m_txtStream.readLine();
        strList = strLine.split("\\s+", Qt::SkipEmptyParts);
		nListSize = strList.size();

		//读每行数据信息，并写到新文件中
		for(int i=0; i<nListSize; i++)
		{
			if(strList.at(i).isEmpty()) continue;
			if(i == 8)
			{
                *m_pDestTxtStream<<Qt::endl;
				WriteDate(nYear, nMonth, m_pDestTxtStream);
			}

			m_pDestTxtStream->setFieldWidth(7);
			m_pDestTxtStream->setFieldAlignment(QTextStream::AlignRight);
			nTemp = strList.at(i).toInt();
			if(nTemp == 999)
			{
				*m_pDestTxtStream<<999999;
			}
			else
			{
				*m_pDestTxtStream<<nTemp;
			}
		}//处理完一行数据
        *m_pDestTxtStream<<Qt::endl;
	}

	m_pFile->close();
	if(m_pFile->error() != QFile::NoError)
	{
		return false;
	}
	return true;
}

/************************************************************************
 功能说明：转存降水指数文件（RC015）为CIPAS格式的指数文件
 参数说明：ptxtStream是写入文件的流指针
           正确执行返回true，否则返回false；
 作    者：renxue
 完成日期：2011年10月13日
************************************************************************/
bool CConvertFormat::ConvertRC015( QTextStream &txtStream )
{
	WriteRC015FileHead(m_pDestTxtStream);//写文件头信息

	//写数据
    *m_pDestTxtStream<<"#DATASET"<<Qt::endl;
	int nYear = 1951;
	int nMonth = 1;
	int nListSize = 0;
	int nTemp = 0;
	QString strLine;
	QStringList strList;

	while(!m_txtStream.atEnd())
	{
		WriteDate(nYear, nMonth, m_pDestTxtStream);

		strLine = m_txtStream.readLine();
        strList = strLine.split("\\s+", Qt::SkipEmptyParts);
		nListSize = strList.size();

		//读每行数据信息，并写到新文件中
		for(int i=0; i<nListSize; i++)
		{
			if(strList.at(i).isEmpty()) continue;
			if(i == 15)
			{
                *m_pDestTxtStream<<Qt::endl;
				WriteDate(nYear, nMonth, m_pDestTxtStream);
			}

			m_pDestTxtStream->setFieldWidth(7);
			m_pDestTxtStream->setFieldAlignment(QTextStream::AlignRight);
			nTemp = strList.at(i).toInt();
			if(nTemp == 999)
			{
				*m_pDestTxtStream<<999999;
			}
			else
			{
				*m_pDestTxtStream<<nTemp;
			}
		}//处理完一行数据
        *m_pDestTxtStream<<Qt::endl;
	}

	m_pFile->close();
	if(m_pFile->error() != QFile::NoError)
	{
		return false;
	}
	return true;
}
