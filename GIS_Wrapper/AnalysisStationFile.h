/*============================================================
�ļ�����CAnalysisStationFile.h
�� ���� CAnalysisStationFile
�� �ࣺ ��
�� �ࣺ ��
����˵������CIPASվ�������ļ��������ļ��������ݴ���ڹ�ϣ����
����˵����
1������Ҫ����վ���ļ���վ�������ļ���·�����ļ�����
2�������Ҫ������ΪKeyֵ�Ĺ�ϣ��Ҫ��ֵ�������ʽ�洢
---------------------------�汾����---------------------------
V 1.0
ԭ �� �ߣ�renxue
������ڣ�2011��10��17��
============================================================*/
#pragma once
#include "GIS_WrapperConfig.h"
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QWidget>
#include <QTextStream>
#include <QHash>
#include <QCoreApplication>

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
	unsigned int nCode;  //վ����
	double   dLatitude;  //վ��γ��
	double   dLongitude; //վ�㾭��
	int *    pElement;
};

class GIS_WRAPPER_EXPORT_IMPORT CAnalysisStationFile
{
public:
	CAnalysisStationFile(void);
	~CAnalysisStationFile(void);
	/************************************************************************
	����˵��������CIPAS��ʽ��վ���ļ��������ļ�
	����˵����StationFileName��վ���ļ���·�����ļ�����
	          DataFileName�������ļ���·�����ļ�����
	          ��ȷִ�з���true�����򷵻�false��
	��    �ߣ�renxue
	������ڣ�2011��10��16��
	************************************************************************/
	bool LoadAllFile(QString DataFileName, QString StationFileName = QString(""));

	/************************************************************************
	����˵��������CIPAS��ʽ��վ���ļ�
	����˵����StationFileName��վ���ļ���·�����ļ�����
	          ��ȷִ�з���true�����򷵻�false��
	��    �ߣ�renxue
	������ڣ�2011��10��16��
	************************************************************************/
	bool LoadStationFile(QString StationFileName=QString(""));

	int StartYear(){return m_nStartYear;}
	int EndYear(){return m_nEndYear;}
	QHash<QString,CStationElement*>* HashData(){return &m_DataHash;}
	void ClearHashData(){m_DataHash.clear();}
	QVector<QString>* StationName(){return &m_StationName;}

private:
	/************************************************************************
	����˵�������������ļ�
	����˵����DataFileName�������ļ���·�����ļ�����
	          ��ȷִ�з���true�����򷵻�false��
	��    �ߣ�renxue
	������ڣ�2011��10��16��
	************************************************************************/
	bool LoadDataFile( QString DataFileName, int nStationNum);

	/************************************************************************
    ����˵������վ���ļ������ֱ����ļ�������
    ����˵����txtStream�Ƕ�ȡ�ļ�����ָ��
              ��ȷִ�з���true�����򷵻�false��
    ��    �ߣ�renxue
    ������ڣ�2011��10��15��
    ************************************************************************/
	bool ReadStationFile(QTextStream &txtStream);

	/************************************************************************
	����˵������ȡվ��������ļ���R1601��r1602�ȣ�
	����˵����txtStream�Ƕ�ȡ�ļ�����ָ��;
	          pData�ǻ���������׵�ַ, nLength��pData�ĳ��ȣ�
	          ��ȷִ�з���true�����򷵻�false;
	��    �ߣ�renxue
	������ڣ�2011��10��15��
	************************************************************************/
	bool ReadDataFile(QTextStream &txtStream, int *pData, int nLength);

	/************************************************************************
    ����˵����������ϣ���д��ĳһԪ�����ݵĶ�ά����,��ȫ������
    ����˵������ȷִ�з���true�����򷵻�false��
    ��    �ߣ�renxue
    ������ڣ�2011��10��15��
    ************************************************************************/
	bool CreateHashTable();

	/************************************************************************
	����˵������ĳ�µ��ļ����ݱ��浽��ϣ���е�pElement��ָ����Ӧ�ռ���
	����˵����nMonth���·ݣ�pData�Ǵ��վ���������ݵ���ʼ��ַ
	          pData��һ����ά���ݣ���m_nYearSpan�У�nStationNum�У�
	          ��ȷִ�з���true�����򷵻�false��
	��    �ߣ�renxue
	������ڣ�2011��10��15��
	************************************************************************/
	bool SaveElementData( int nMonth, int *pData);

	void PrintHashTable(QString strName);//for debug

private:
	QTextStream m_txtStream;
	int  m_nStartYear, m_nEndYear; //�ļ����ݵ���ʼ�ꡢ��ֹ��
	int  m_nYearSpan;
	int* m_pStationDataPerM;       //����վ����ʷ���ݵ�ͬ��ֵ
	QVector<QString> m_StationName;
	QVector<CStationElement*>  m_StationElement;
	QHash<QString,CStationElement*> m_DataHash;
};