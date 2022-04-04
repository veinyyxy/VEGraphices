/*============================================================
�ļ����� ConvertExponentToCipasFormat.cpp
��  ���� CConvertFormat
��  �ࣺ QWidget
��  �ࣺ ��
����˵������ָ���ļ������CIPAS��ʽ���ļ�
����˵����

----------------------------�汾����----------------------------
V 1.0
ԭ���� �� renxue
������ڣ�2011��10��25��
============================================================*/
#include <fstream>
#include <iomanip>
#include <QtCore/QTime>
#include <QtCore/QDate>
#include <Qtgui/QMessageBox>
#include <Qtcore/QTextCodec>
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
����˵����������Ҫ������ļ����ļ�·�����ļ���
����˵����fileName�ǰ���·�����ļ������ַ���
          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
��    �ߣ�renxue
������ڣ�2011��10��10��
************************************************************************/
bool CConvertFormat::SetSourceFileName(QString fileName)
{
	if(fileName.isEmpty()) return false;
	ReadExponentFile(fileName);
	return true;
}

/************************************************************************
 ����˵������ȡָ���ļ�
 ����˵����fileName�ǰ���·�����ļ������ַ���
           ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
 ��    �ߣ�renxue
 ������ڣ�2011��10��09��
************************************************************************/
bool CConvertFormat::ReadExponentFile( QString fileName )
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) 	return false;

	m_txtStream.setDevice(&file);

	QString name(fileName);
	int nLength = name.length();
	name.remove(0, nLength-5);
	//����ת�����ļ�
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
	m_pDestTxtStream->setCodec("UTF-8");

	//����ת��
	if(fileName.contains("HC074"))      //��HC074����ָ�����ݿ�
	{
		ConvertHC074(m_txtStream);
	}
	else if(fileName.contains("SC009")) //��SC009����ָ�����ݿ�
	{
		ConvertSC009(m_txtStream);
	}
	else if(fileName.contains("TC008")) //��TC008�¶ȵȼ����ݿ�
	{
		ConvertTC008(m_txtStream);
	}
	else if(fileName.contains("RC015")) //��RC015��ˮָ�����ݿ�
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
 ����˵����дCIPASָ���ļ�:û������ǰ���ꡢ��ֵ
 ����˵����nYear�Ǹ���Ԫ�ص������ݣ�nMonth�Ǹ���Ԫ�ص������ݣ�
           ptxtStream��д���ļ�����ָ�룻
           �޷���ֵ��
 ��    �ߣ�renxue
 ������ڣ�2011��10��10��
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
 ����˵����дCIPAS��ˮָ���ļ���RC015����ͷ��Ϣ
 ����˵����ptxtStream��д���ļ�����ָ��
           �޷���ֵ��
 ��    �ߣ�renxue
 ������ڣ�2011��10��10��
************************************************************************/
void CConvertFormat::WriteRC015FileHead( QTextStream *ptxtStream )
{
	*ptxtStream<<"CIPAS 4 2 8"<<endl;
	*ptxtStream<<"#HEADINFO"<<endl;
	*ptxtStream<<"#DESCRIPTION "<<'"'<<"precipitation index"<<'"'<<endl;
	*ptxtStream<<"#DATA_STORAGE_FORMAT TXT"<<endl;
	QDate curDate = QDate::currentDate();
	*ptxtStream<<"#TIME MMON 1951 01 -999 "<<curDate.year()<<" "<<curDate.month()<<" -999"<<endl;
	int nyear = curDate.year()-1951+1;
	*ptxtStream<<"#ATTRIBUTES "<<nyear<<" 17 Years int Months int "
		<<QString::fromLocal8Bit("�˰�������ˮָ�� int ����ƽԭ����ˮָ�� int ���ɹŽ�ˮָ�� int ��������ˮָ�� int ��������ˮָ�� int ")
		<<QString::fromLocal8Bit("��������������ˮָ�� int ��������ˮָ�� int ��������ˮָ�� int ��������ˮָ�� int ��������ˮָ�� int ")
		<<QString::fromLocal8Bit("��������ˮָ�� int ��������ˮָ�� int ��������ˮָ�� int �Ͻ�����ˮָ�� int ��ԭ����ˮָ�� int")<<endl;
	*ptxtStream<<"#FORMATDEFINE d4 d3 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7"<<endl;
}

/************************************************************************
 ����˵����дCIPAS�¶ȵȼ�ָ���ļ���TC008����ͷ��Ϣ
 ����˵����ptxtStream��д���ļ�����ָ��
           �޷���ֵ��
 ��    �ߣ�renxue
 ������ڣ�2011��10��10��
************************************************************************/
void CConvertFormat::WriteTC008FileHead( QTextStream *ptxtStream )
{
	*ptxtStream<<"CIPAS 4 2 8"<<endl;
	*ptxtStream<<"#HEADINFO"<<endl;
	*ptxtStream<<"#DESCRIPTION "<<'"'<<"temperature index"<<'"'<<endl;
	*ptxtStream<<"#DATA_STORAGE_FORMAT TXT"<<endl;
	QDate curDate = QDate::currentDate();
	*ptxtStream<<"#TIME MMON 1951 01 -999 "<<curDate.year()<<" "<<curDate.month()<<" -999"<<endl;
	int nyear = curDate.year()-1951+1;
	*ptxtStream<<"#ATTRIBUTES "<<nyear<<" 10 Years int Months int "
		       <<QString::fromLocal8Bit("�������¶ȵȼ� int �������¶ȵȼ� int �������������¶ȵȼ� int �������¶ȵȼ� int ")
		       <<QString::fromLocal8Bit("�������¶ȵȼ� int �������¶ȵȼ� int �½����¶ȵȼ� int ȫ���¶ȵȼ� int")<<endl;
	*ptxtStream<<"#FORMATDEFINE d4 d3 d7 d7 d7 d7 d7 d7 d7 d7"<<endl;
}

/************************************************************************
 ����˵����д74����CIPASָ���ļ���ͷ��Ϣ
 ����˵����ptxtStream��д���ļ�����ָ��
           �޷���ֵ��
 ��    �ߣ�renxue
 ������ڣ�2011��10��10��
************************************************************************/
void CConvertFormat::WriteHC074FileHead(QTextStream *ptxtStream)
{
	*ptxtStream<<"CIPAS 4 2 8"<<endl;
	*ptxtStream<<"#HEADINFO"<<endl;
	*ptxtStream<<"#DESCRIPTION "<<'"'<<"sstidx key SST index"<<'"'<<endl;
	*ptxtStream<<"#DATA_STORAGE_FORMAT TXT"<<endl;
	QDate curDate = QDate::currentDate();
	*ptxtStream<<"#TIME MMON 1951 01 -999 "<<curDate.year()<<" "<<curDate.month()<<" -999"<<endl;
	int nyear = curDate.year()-1951+1;
	*ptxtStream<<"#ATTRIBUTES "<<nyear<<QString::fromLocal8Bit(" 76 Years int Months int �����򸱸����ָ�� int ���Ǹ������ָ�� int ���Ǵ��������������ָ�� int ӡ�ȸ������ָ�� int ��̫ƽ�󸱸����ָ�� int ��̫ƽ�󸱸����ָ�� int �����������ָ�� int �����󸱸����ָ�� int ")
		<<QString::fromLocal8Bit("�Ϻ��������ָ�� int ���������󸱸����ָ�� int ̫ƽ�󸱸����ָ�� int �����򸱸�ǿ��ָ�� int ���Ǹ���ǿ��ָ�� int ���Ǵ�����������ǿ��ָ�� int ӡ�ȸ������ǿ��ָ�� int ��̫ƽ�󸱸�ǿ��ָ�� int ��̫ƽ�󸱸�ǿ��ָ�� int ��������ǿ��ָ�� int �����󸱸�ǿ��ָ�� int �Ϻ�����ǿ��ָ�� int ���������󸱸�ǿ��ָ�� int ")
		<<QString::fromLocal8Bit("̫ƽ�󸱸�ǿ��ָ�� int �����򸱸߼��� int ���Ǹ��߼��� int ���Ǵ����������߼��� int ӡ�ȸ��߼��� int ��̫ƽ�󸱸߼��� int ��̫ƽ�󸱸߼��� int �������߼��� int �����󸱸߼��� int �Ϻ����߼��� int ���������󸱸߼��� int ̫ƽ�󸱸߼��� int �����򸱸߱��� int ���Ǹ��߱��� int ���Ǵ����������߱��� int ӡ�ȸ��߱��� int ��̫ƽ�󸱸߱��� int ")
		<<QString::fromLocal8Bit("��̫ƽ�󸱸߱��� int �������߱��� int �����󸱸߱��� int �Ϻ����߱��� int ���������󸱸߱��� int ̫ƽ�󸱸߱��� int ��̫ƽ�󸱸����켹�� int �������������ָ�� int ̫ƽ�����������ָ�� int �������������ָ�� int ������-ŷ�����������ָ�� int �����������ָ�� int ����������ǿ��ָ�� int ̫ƽ������ǿ��ָ�� int ����������ǿ��ָ�� int ������-ŷ��������ǿ��ָ�� int ��������ǿ��ָ�� int ������������λ�� int ")
		<<QString::fromLocal8Bit("������������ǿ�� int W������-ŷ�޻����� int C������-ŷ�޻����� int E������-ŷ�޻����� int ŷ��γ��(IZ,0-150E)����ָ�� int ŷ�Ǿ���(IM,0-150E)����ָ�� int ����γ��(IZ,60-150E)����ָ�� int ���޾���(IM,60-150E)����ָ�� int λ�ö��ǲ� int ǿ�ȶ��ǲ� int (25N-35N,80E-100E)���ظ�ԭ int (30N-40N,75E-105E)���ظ�ԭ int ӡ��� int ����� int ���̨�� int ��½̨�� int ̫������ int �Ϸ��ζ�ָ��SOI int")<<endl;
	*ptxtStream<<"#FORMATDEFINE d4 d3 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7 d7"<<endl;
}

/************************************************************************
 ����˵����дCIPAS����ָ���ļ���SC009����ͷ��Ϣ
 ����˵����ptxtStream��д���ļ�����ָ��
           �޷���ֵ��
 ��    �ߣ�renxue
 ������ڣ�2011��10��10��
************************************************************************/
void CConvertFormat::WriteSC009FileHead( QTextStream *ptxtStream )
{
	*ptxtStream<<"CIPAS 4 2 8"<<endl;
	*ptxtStream<<"#HEADINFO"<<endl;
	*ptxtStream<<"#DESCRIPTION "<<'"'<<"SST index"<<'"'<<endl;
	*ptxtStream<<"#DATA_STORAGE_FORMAT TXT"<<endl;
	QDate curDate = QDate::currentDate();
	*ptxtStream<<"#TIME MMON 1951 01 -999 "<<curDate.year()<<" "<<curDate.month()<<" -999"<<endl;
	int nyear = curDate.year()-1951+1;
	*ptxtStream<<"#ATTRIBUTES "<<nyear<<" 11 Years int Months int "
		       <<QString::fromLocal8Bit("�׳���ָ�� int ����Ư����ָ�� int �ڳ���ָ�� int NINO1��ָ�� int NINO2��ָ�� int ")
		       <<QString::fromLocal8Bit("NINO1+2��ָ�� int NINO3��ָ�� int NINO4��ָ�� int ���С�����ָ�� int")<<endl;
	*ptxtStream<<QString::fromLocal8Bit("#FORMATDEFINE d4 d3 d7 d7 d7 d7 d7 d7 d7 d7 d7")<<endl;
}

/************************************************************************
 ����˵����ת��74����ָ���ļ���HC074��ΪCIPAS��ʽ��ָ���ļ�
 ����˵����ptxtStream��д���ļ�����ָ��
           ��ȷִ�з���true�����򷵻�false��
 ��    �ߣ�renxue
 ������ڣ�2011��10��13��
************************************************************************/
bool CConvertFormat::ConvertHC074( QTextStream &txtStream )
{
	WriteHC074FileHead(m_pDestTxtStream);

	//д����
	*m_pDestTxtStream<<"#DATASET"<<endl;
	int nYear = 1951;
	int nMonth = 1;
	int nListSize = 0;
	int nTemp = 0;
	QString strLine;
	QStringList strList;

	while(!m_txtStream.atEnd())
	{
		WriteDate(nYear, nMonth, m_pDestTxtStream);
		//����һ������
		strLine = m_txtStream.readLine();
		strList = strLine.split(QRegExp("\\s+"),QString::SkipEmptyParts);
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
		*m_pDestTxtStream<<endl;

		//���ڶ�������
		*m_pDestTxtStream<<QString::fromLocal8Bit("       ");//Ϊ����һ�ж��룬д��7���ո�
		strLine = m_txtStream.readLine();
		strList = strLine.split(QRegExp("\\s+"),QString::SkipEmptyParts);
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
		*m_pDestTxtStream<<endl;
	}

	m_pFile->close();
	if(m_pFile->error() != QFile::NoError)
	{
		return false;
	}
	return true;
}

/************************************************************************
 ����˵����ת�溣��ָ���ļ���SC009��ΪCIPAS��ʽ��ָ���ļ�
 ����˵����ptxtStream��д���ļ�����ָ��
           ��ȷִ�з���true�����򷵻�false��
 ��    �ߣ�renxue
 ������ڣ�2011��10��13��
************************************************************************/
bool CConvertFormat::ConvertSC009( QTextStream &txtStream )
{
	WriteSC009FileHead(m_pDestTxtStream);//д�ļ�ͷ��Ϣ

	//д����
	*m_pDestTxtStream<<"#DATASET"<<endl;
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
		strList = strLine.split(QRegExp("\\s+"),QString::SkipEmptyParts);
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
		*m_pDestTxtStream<<endl;
	}

	m_pFile->close();
	if(m_pFile->error() != QFile::NoError)
	{
		return false;
	}
	return true;
}

/************************************************************************
 ����˵����ת���¶ȵȼ�ָ���ļ���TC008��ΪCIPAS��ʽ��ָ���ļ�
 ����˵����ptxtStream��д���ļ�����ָ��
           ��ȷִ�з���true�����򷵻�false��
 ��    �ߣ�renxue
 ������ڣ�2011��10��13��
************************************************************************/
bool CConvertFormat::ConvertTC008( QTextStream &txtStream )
{
	WriteTC008FileHead(m_pDestTxtStream);//д�ļ�ͷ��Ϣ

	//д����
	*m_pDestTxtStream<<"#DATASET"<<endl;
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
		strList = strLine.split(QRegExp("\\s+"),QString::SkipEmptyParts);
		nListSize = strList.size();

		//��ÿ��������Ϣ����д�����ļ���
		for(int i=0; i<nListSize; i++)
		{
			if(strList.at(i).isEmpty()) continue;
			if(i == 8)
			{
				*m_pDestTxtStream<<endl;
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
		}//������һ������
		*m_pDestTxtStream<<endl;
	}

	m_pFile->close();
	if(m_pFile->error() != QFile::NoError)
	{
		return false;
	}
	return true;
}

/************************************************************************
 ����˵����ת�潵ˮָ���ļ���RC015��ΪCIPAS��ʽ��ָ���ļ�
 ����˵����ptxtStream��д���ļ�����ָ��
           ��ȷִ�з���true�����򷵻�false��
 ��    �ߣ�renxue
 ������ڣ�2011��10��13��
************************************************************************/
bool CConvertFormat::ConvertRC015( QTextStream &txtStream )
{
	WriteRC015FileHead(m_pDestTxtStream);//д�ļ�ͷ��Ϣ

	//д����
	*m_pDestTxtStream<<"#DATASET"<<endl;
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
		strList = strLine.split(QRegExp("\\s+"),QString::SkipEmptyParts);
		nListSize = strList.size();

		//��ÿ��������Ϣ����д�����ļ���
		for(int i=0; i<nListSize; i++)
		{
			if(strList.at(i).isEmpty()) continue;
			if(i == 15)
			{
				*m_pDestTxtStream<<endl;
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
		}//������һ������
		*m_pDestTxtStream<<endl;
	}

	m_pFile->close();
	if(m_pFile->error() != QFile::NoError)
	{
		return false;
	}
	return true;
}