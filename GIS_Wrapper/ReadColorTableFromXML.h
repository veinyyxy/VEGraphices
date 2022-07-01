/*============================================================ 

-------------------------------------版本更新------------------------------------------------ 
V 1.0 
原作者 ：zhanglei
完成日期：2011年11月23日

V 1.1 
作 者：
完成日期：
更新说明：
============================================================*/
#pragma once
#include "GIS_WrapperConfig.h"

#include <QtWidgets/QFileDialog>
#include <QtCore/QXmlStreamReader>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtCore/QDebug>
#include <QtGui/QVector3D>
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
	void ReadColorElement(QColor &color); //读颜色
private:
//	QTreeWidget *treeWidget;
	QXmlStreamReader m_XMLreader;
//	COLORTABLELIST m_colortablelist;
	QString m_DataType;
	QString m_Description;
	QString m_Date;
};

