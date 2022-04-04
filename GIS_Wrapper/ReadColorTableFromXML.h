/*============================================================ 

-------------------------------------�汾����------------------------------------------------ 
V 1.0 
ԭ���� ��zhanglei
������ڣ�2011��11��23��

V 1.1 
�� �ߣ�
������ڣ�
����˵����
============================================================*/
#pragma once
#include "GIS_WrapperConfig.h"

#include <QtGui/QFileDialog>
#include <QtCore/QXmlStreamReader>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtCore/QDebug>
#include <QVector3D>
#include "SymbolLayer.h"

class GIS_WRAPPER_EXPORT_IMPORT ReadColorTableFromXML : public QWidget
{
public:
	ReadColorTableFromXML(void);
	~ReadColorTableFromXML(void);

	bool readFile(const QString &fileName,COLORTABLELIST &colortablelist);		
	void skipUnknowElement();
private:	
	void ReadFilehead();
	void ReadColorTableList(COLORTABLELIST &colortablelist);
	void ReadData(COLORTABLELIST &colortablelist);
	void ReadColorTable(COLORTABLE &colortable);
	void ReadItem(double &vlaue,QString &valuestring,QColor &color,int &Zcount);
	void ReadColorElement(QColor &color); //����ɫ
private:
//	QTreeWidget *treeWidget;
	QXmlStreamReader m_XMLreader;
//	COLORTABLELIST m_colortablelist;
	QString m_DataType;
	QString m_Description;
	QString m_Date;
};

