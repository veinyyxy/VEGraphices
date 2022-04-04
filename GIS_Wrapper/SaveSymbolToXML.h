/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� �
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����SaveSymbolToXML.h SaveSymbolToXML.cpp 
�� ���� SaveSymbolToXML
�� �ࣺ QWidget
�� �ࣺ �� 
����˵�������༭����д��XML
����˵����

-------------------------------------�汾����------------------------------------------------ 
V 1.0 
ԭ���� ���
������ڣ�2011��5��3��

V 1.1 
�� �ߣ�
������ڣ�
����˵����
============================================================*/
#pragma once
#include "GIS_WrapperConfig.h"

#include <QtGui/QFileDialog>
#include <QtCore/QXmlStreamWriter>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>


#include "SymbolData.h"
class GIS_WRAPPER_EXPORT_IMPORT SaveSymbolToXML : public QWidget
{
public:
	SaveSymbolToXML(void);
	~SaveSymbolToXML(void);
	void SaveSymbolToXMLFile(SymbolData *symbolData,QString fileName);
private:
	bool beginWrite();
	void writeFileHead();
	bool endWrite();
	void SavePointSymbolToXML(SymbolData *symbolData); 
	void SaveLineSymbolToXML(SymbolData *symbolData);
	void SaveMultiLineSymbolToXML(SymbolData *symbolData);
	void SaveNormalSymbolToXML(SymbolData *symbolData);
	void SaveContourLineToXML(SymbolData *symbolData);
	void SaveRegionSymbolToXML(SymbolData *symbolData);
	void SaveMarkSymbolToXML(SymbolData *symbolData);

private:
	QFile *m_pFile;
	QXmlStreamWriter *m_pXmlWriter;
	QString m_fileName;
};

