/*============================================================ 
�ļ�����SaveSymbolToM14File.h  
��  ���� CSaveSymbolToM14File
��  �ࣺ QWidget
��  �ࣺ �� 
����˵�������ֻ���ű����Micaps14�������ļ��ĸ�ʽ
����˵����

----------------------------�汾����----------------------------
V 1.0 
ԭ���� ��renxue
������ڣ�2011��08��08��
============================================================*/


#pragma once
#include "GIS_WrapperConfig.h"
#include <Qtcore/QFile>
#include <QtCore/QTextStream>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>

#include "SymbolData.h"


class GIS_WRAPPER_EXPORT_IMPORT CSaveSymbolToM14 : public QWidget
{
public:
	CSaveSymbolToM14(void);
	~CSaveSymbolToM14(void);

	/*************************************************************
	����˵�������ֻ���ű����Micapsʮ�������ݸ�ʽ���ļ�
	����˵����fileName�ǰ���·�����ļ������ַ���
	          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
	**************************************************************/
	bool SaveSymbolToM14File(QString fileName, SymbolData *pSymbolData);

private:
	void WriteFileHead();
	bool SavePointSymbol(SymbolData *pSymbolData); 
	bool SaveLineSymbol(SymbolData *pSymbolData);
	bool SaveContourSymbol(SymbolData *pSymbolData, bool bFlag);
	bool SavePolygonSymbol(SymbolData *pSymbolData);
	bool SaveMarkSymbol(SymbolData *pSymbolData);
	int  ChangeCodeToM14(int nType);

private:
	QFile        *m_pFile;
	QString      m_FileName;
	QTextStream  *m_pTextStream;
};
