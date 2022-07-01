/*============================================================
�ļ����� ConvertExponentToCipasFormat.h
��  ���� CConvertFormat
��  �ࣺ QWidget
��  �ࣺ ��
����˵������ָ���ļ������CIPAS��ʽ���ļ�
����˵����
1��ͷ��Ϣ�е�ȱʡֵʹ�á�-999����
2�����ݼ���ȱʡֵʹ�á�999999.00����999999��

----------------------------�汾����----------------------------
V 1.0
ԭ���� �� renxue
������ڣ�2011��10��09��
============================================================*/

#pragma once
#include "GIS_WrapperConfig.h"
#include <Qtcore/QFile>
#include <QtCore/QTextStream>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include "../GIS_Wrapper/Const.h"

class GIS_WRAPPER_EXPORT_IMPORT CConvertFormat
{
public:
	CConvertFormat(void);
	~CConvertFormat(void);

	/************************************************************************
     ����˵����������Ҫ������ļ����ļ�·�����ļ���
     ����˵����fileName�ǰ���·�����ļ������ַ���
              ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
     ��    �ߣ�renxue
     ������ڣ�2011��10��10��
    ************************************************************************/
	bool SetSourceFileName(QString fileName);

private:

	/************************************************************************
     ����˵������ȡָ���ļ�
     ����˵����fileName�ǰ���·�����ļ������ַ���
              ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
     ��    �ߣ�renxue
     ������ڣ�2011��10��09��
    ************************************************************************/
	bool ReadExponentFile(QString fileName);

	/************************************************************************
     ����˵����д��ָ���ļ���������Ϣ
     ����˵����nYear���ļ�ͷ��Ϣ�е�������
              ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
     ��    �ߣ�renxue
     ������ڣ�2011��10��09��
    ************************************************************************/
	void WriteDate(int &nYear, int &nMonth, QTextStream *ptxtStream);
	void WriteRC015FileHead(QTextStream *ptxtStream);
	void WriteRS015FileHead(QTextStream *ptxtStream);
	void WriteTC008FileHead(QTextStream *ptxtStream);
	void WriteHC074FileHead(QTextStream *ptxtStream);
	void WriteSC009FileHead(QTextStream *ptxtStream);
	bool ConvertRC015(QTextStream &txtStream);
	bool ConvertRS015(QTextStream &txtStream);
	bool ConvertTC008(QTextStream &txtStream);
	bool ConvertHC074(QTextStream &txtStream);
	bool ConvertSC009(QTextStream &txtStream);

private:
	int          *m_pIntData;
	QFile        *m_pFile;
	QString      m_FileName;
	QTextStream  m_txtStream;
	QTextStream  *m_pDestTxtStream;
};
