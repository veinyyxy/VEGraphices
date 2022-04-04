/*============================================================ 
�ļ�����AnalysisExponent.h 
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
	����˵��������CIPAS��ʽ��ָ���ļ��������ļ���������Ӧ����
	����˵����FileName��·�����ļ���
	          ��ȷִ�з���true�����򷵻�false��
	��    �ߣ�renxue
	������ڣ�2011��10��11��
	************************************************************************/
	bool LoadFile(QString FileName);

	QString  FileName(){return m_FileName;}
	QStringList ElementName(){return m_ElementName;}
	int StartYear(){return m_nStartYear;}
	int EndYear(){return m_nEndYear;}

private:
	/************************************************************************
    ����˵����������ϣ���д��ĳһԪ�����ݵĶ�ά����,��ȫ������
    ����˵����nNum��Ԫ�صĸ���
              ��ȷִ�з���true�����򷵻�false��
    ��    �ߣ�renxue
    ������ڣ�2011��10��11��
    ************************************************************************/
	bool CreateHashTable(int nNum);

	/************************************************************************
	����˵������ȡCIPAS��ʽ��ָ���ļ�ͷ��Ϣ��Ŀǰֻ������TIME��ATTRIBUTE��Ϣ
	����˵����txtStream�Ƕ�ȡ�ļ�����ָ��
	          ��ȷִ�з���true�����򷵻�false��
	��    �ߣ�renxue
	������ڣ�2011��10��11��
	************************************************************************/
	bool ReadFileHead(QTextStream &txtStream);

	/************************************************************************
	����˵������ָ��Ԫ��ֵ���浽m_nElementAddress��ָ�Ŀռ���
	����˵����nYear��Ԫ�ص��꣬nMonth��Ԫ�ص���
	          nElementPos��Ԫ��λ��
	          ��ȷִ�з���true�����򷵻�false��
	��    �ߣ�renxue
	������ڣ�2011��10��11��
	************************************************************************/
	inline bool SaveElementData(int nYear, int nMonth, int nElementPos, int nData);

	/************************************************************************
	����˵������ָ��Ԫ�����ʹ洢λ�ñ��浽��ϣ����
	����˵����Ŀǰʹ�õĲ�����Ϊ��Ա���������ڴ�δд����
	          ��ȷִ�з���true�����򷵻�false��
	��    �ߣ�renxue
	������ڣ�2011��10��11��
	************************************************************************/
	bool FillHashTable();

	/************************************************************************
	����˵������ȡCIPAS��ʽ��ָ���ļ���RC015��TC008��HC074��SC009��
	����˵����txtStream�Ƕ�ȡ�ļ�����ָ��
	          nElementSize�ǲ�ͬ�ļ���Ԫ�ظ���
	          ��ȷִ�з���true�����򷵻�false;
	��    �ߣ�renxue
	������ڣ�2011��10��13��
	************************************************************************/
	bool ReadData(QTextStream &txtStream, int nElementSize);

public:
	QHash<QString,int*> m_DataHash;

private:
	QTextStream m_txtStream;
	int  m_nStartYear, m_nStartMonth; //�ļ����ݵ���ʼ�ꡢ��
	int  m_nEndYear, m_nEndMonth;     //�ļ����ݵ���ֹ�ꡢ��
	int  m_nYearSpan;
	QString       m_FileName;
	QStringList   m_ElementName;
	QVector<int*> m_ElementAddress; 
};

