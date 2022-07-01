///*============================================================
//�ļ�����AnalysisStationFile.cpp
//�� ���� CAnalysisStationFile
//�� �ࣺ QWidget
//�� �ࣺ ��
//����˵������վ���ļ��������ļ��������ݴ���ڹ�ϣ����
//����˵����
//1������Ҫ����ָ���ļ���·�����ļ�����
//2�������Ҫ������ΪKeyֵ�Ĺ�ϣ��Ҫ��ֵ�Զ�ά���飨���Ϊ�У��·�Ϊ�У�����ʽ�洢
//
//---------------------------�汾����---------------------------
//V 1.0
//ԭ �� �ߣ�renxue
//������ڣ�2011��10��17��
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
����˵��������CIPAS��ʽ��վ���ļ��������ļ�
����˵����StationFileName��վ���ļ���·�����ļ�����
          DataFileName�������ļ���·�����ļ�����
          ��ȷִ�з���true�����򷵻�false��
��    �ߣ�renxue
������ڣ�2011��10��16��
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
	while(i<12)     //�����·ݶ������ļ�,����Ҫ��12���ļ�
	{
		TempFileName = leftName + rightName.at(i);
		if(!LoadDataFile(TempFileName, nStationNum)) return false;
		if(!SaveElementData(i+1, m_pStationDataPerM)) return false;
		i++;
	}
	return true;
}

/************************************************************************
����˵��������CIPAS��ʽ��վ���ļ�
����˵����StationFileName��վ���ļ���·�����ļ�����
          ��ȷִ�з���true�����򷵻�false��
��    �ߣ�renxue
������ڣ�2011��10��16��
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
����˵�������������ļ�
����˵����DataFileName��һ�·ݵ������ļ�·�����ļ�����
          ��ȷִ�з���true�����򷵻�false��
��    �ߣ�renxue
������ڣ�2011��10��16��
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
����˵������վ�������ļ������ֱ����ļ�������
����˵����txtStream�Ƕ�ȡ�ļ�����ָ��
          ��ȷִ�з���true�����򷵻�false��
��    �ߣ�renxue
������ڣ�2011��10��15��
************************************************************************/
bool CAnalysisStationFile::ReadStationFile( QTextStream &txtStream )
{
	QString strLine, strTemp;
	QStringList strList;

	strLine = txtStream.readLine();  //�ļ��ĵ�һ��˵��
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
����˵������ȡվ��������ļ���R1601��r1602�ȣ�
����˵����txtStream�Ƕ�ȡ�ļ�����ָ�룻
          nStationNum��վ��ĸ���, nLength��pData�ĳ��ȣ�
          ��ȷִ�з���true�����򷵻�false��
��    �ߣ�renxue
������ڣ�2011��10��15��
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
		//�����ж������ļ�������
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
����˵����������ϣ�����ж�̬�����˴��վ�����ݵĶ�ά����,��ȫ����999999
����˵������ȷִ�з���true�����򷵻�false��
��    �ߣ�renxue
������ڣ�2011��10��15��
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
����˵������ĳ�µ��ļ����ݱ��浽��ϣ���е�pElement��ָ����Ӧ�ռ���
����˵����nMonth���·ݣ�pData�Ǵ��վ���������ݵ���ʼ��ַ
          pData��һ����ά���ݣ���m_nYearSpan�У�nStationNum�У�
          ��ȷִ�з���true�����򷵻�false��
��    �ߣ�renxue
������ڣ�2011��10��15��
************************************************************************/
bool CAnalysisStationFile::SaveElementData(int nMonth, int *pData)
{
	if(nMonth<1 || !pData) 	return false;

	//ת������
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
����˵����Ϊ�˵��ԣ�����ϣ���е�����д��һ���ļ�

��    �ߣ�renxue
������ڣ�2011��10��11��
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
