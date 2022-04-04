/*============================================================ 
�ļ�����AnalysisExponent.cpp
�� ���� CAnalysisExponent
�� �ࣺ QWidget
�� �ࣺ �� 
����˵������CIPASָ���ļ��������ļ��������ݴ���ڹ�ϣ����
����˵����
1������Ҫ����ָ���ļ���·�����ļ�����
2�������Ҫ������ΪKeyֵ�Ĺ�ϣ��Ҫ��ֵ�Զ�ά���飨���Ϊ�У��·�Ϊ�У�����ʽ�洢

---------------------------�汾����---------------------------
V 1.0 
ԭ �� �ߣ�renxue
������ڣ�2011��10��13��
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
����˵��������CIPAS��ʽ��ָ���ļ��������ļ���������Ӧ����
����˵����FileName��·�����ļ���
          ��ȷִ�з���true�����򷵻�false��
��    �ߣ�renxue
������ڣ�2011��10��26��
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
	int nElementSize = name.toInt(); //��ô�ܴ��ļ����õ�ElementSize? 

	QFile file(FileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) 	return false;

	m_txtStream.setDevice(&file);
	m_txtStream.setCodec("UTF-8");

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
		m_FileName = QObject::tr("����������");
		break;
	case 15:
		m_FileName = QObject::tr("��ˮָ��");
		break;
	case 9:
		m_FileName = QObject::tr("����ָ��");
		break;
	case 8:
		m_FileName = QObject::tr("�¶ȵȼ�");
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
����˵����������ϣ���д��ĳһԪ�����ݵĶ�ά����,��ȫ������
����˵����nNum��Ԫ�صĸ���
          ��ȷִ�з���true�����򷵻�false��
��    �ߣ�renxue
������ڣ�2011��10��11��
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
����˵������ȡCIPAS��ʽ��ָ���ļ�ͷ��Ϣ��Ŀǰֻ������TIME��ATTRIBUTE��Ϣ
����˵����txtStream�Ƕ�ȡ�ļ�����ָ��
          ��ȷִ�з���true�����򷵻�false��
��    �ߣ�renxue
������ڣ�2011��10��11��
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
			strList = strLine.split(QRegExp("\\s+"),QString::SkipEmptyParts);
			m_nStartYear = strList.at(2).toInt();
			m_nStartMonth = strList.at(3).toInt();
			m_nEndYear = strList.at(5).toInt();					
			m_nEndMonth = strList.at(6).toInt();
		}
		else if(strLine.contains("ATTRIBUTES"))
		{
			strList = strLine.split(QRegExp("\\s+"),QString::SkipEmptyParts);
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
����˵������ָ��Ԫ��ֵ���浽m_nElementAddress��ָ�Ŀռ���
����˵����nYear��Ԫ�ص��꣬nMonth��Ԫ�ص���;
          nElementPos��Ԫ��λ��,���㿪ʼ;
          ��ȷִ�з���true�����򷵻�false��
��    �ߣ�renxue
������ڣ�2011��10��11��
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
����˵������ָ��Ԫ�����ʹ洢λ�ñ��浽��ϣ����
����˵����Ŀǰʹ�õĲ�����Ϊ��Ա���������ڴ�δд����
          ��ȷִ�з���true�����򷵻�false��
��    �ߣ�renxue
������ڣ�2011��10��11��
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
	msgBox.setText("There is something wrong about filling hash table��");
	msgBox.exec();
	return false;
}

/************************************************************************
����˵������ȡCIPAS��ʽ��ָ���ļ���RC015��TC008��HC074��SC009��
����˵����txtStream�Ƕ�ȡ�ļ�����ָ��
          nElementSize�ǲ�ͬ�ļ���Ԫ�ظ���
          ��ȷִ�з���true�����򷵻�false��
��    �ߣ�renxue
������ڣ�2011��10��13��
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
			//ָ���ļ��������е�һ��
			strLine = m_txtStream.readLine();
			strList = strLine.split(QRegExp("\\s+"),QString::SkipEmptyParts);
			nYear = strList.at(0).toInt();
			nMonth = strList.at(1).toInt();
			for(int i=2; i<nDataNum; i++ )
			{
				nPos = i-2;
				valuetemp = strList.at(i).toInt();
				SaveElementData(nYear, nMonth, nPos,valuetemp);
			}
			strList.clear();

			//ָ���ļ��������еڶ���
			strLine = m_txtStream.readLine();
			strList = strLine.split(QRegExp("\\s+"),QString::SkipEmptyParts);
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
			//ָ���ļ���������
			strLine = m_txtStream.readLine();
			strList = strLine.split(QRegExp("\\s+"),QString::SkipEmptyParts);
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
