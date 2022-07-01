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
#include <QtCore/QXmlStreamWriter>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include "SymbolLayer.h"

class GIS_WRAPPER_EXPORT_IMPORT SaveColorTableToXML : public QWidget
{
public:
	SaveColorTableToXML(void);
	~SaveColorTableToXML(void);
	void SaveColorTableToXMLFile(QString fileName,COLORTABLELIST &colortablelist);
private:
	bool beginWrite();
	void writeFileHead();
	void SaveColorTable(QString strname,COLORTABLE& colortable);
	bool endWrite();
private:
	QFile *m_pFile;
	QXmlStreamWriter *m_pXmlWriter;
	QString m_fileName;
//	COLORTABLELIST m_colortablelist;
};

