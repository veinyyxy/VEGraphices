#include "OptMicapsDataXml.h"
#include "qfile.h"
#include "qcoreapplication.h"
#include "QObject.h"

 OptMicapsDataXml::OptMicapsDataXml()
{
	m_FileName = CIPASApplication::ConfigPath()+ "/BasicInfo/MicapsData.xml";
}

OptMicapsDataXml::~OptMicapsDataXml()
{
}
/************************************************************************
 功能说明：写入MicapsDataXml中按钮相关数据
 作      者：赵高攀
 完成日期：2011年5月23日
************************************************************************/
bool OptMicapsDataXml::WriteMapPathData(QTreeWidget *treeWidget,QString xmlFileName)
{
	QFile file(xmlFileName);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		return false;
	}
	QXmlStreamWriter xmlWriter(&file);
	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("ConfigMap");

	for (int i =0;i<treeWidget->topLevelItemCount();i++)
	{
		xmlWriter.writeStartElement("Info");
		xmlWriter.writeAttribute("mapname",treeWidget->topLevelItem(i)->text(0));
		xmlWriter.writeAttribute("shpname",treeWidget->topLevelItem(i)->text(1));
		QColor color = treeWidget->topLevelItem(i)->backgroundColor(2);
		xmlWriter.writeAttribute("red",QString::number(color.redF()));
		xmlWriter.writeAttribute("green",QString::number(color.greenF()));
		xmlWriter.writeAttribute("blue",QString::number(color.blueF()));
		xmlWriter.writeAttribute("wvalue",QString::number(color.alphaF()));
		if(treeWidget->topLevelItem(i)->text(3) == QObject::tr("面"))
		{
			xmlWriter.writeAttribute("plolygontype",QString::number(2));
			xmlWriter.writeAttribute("layerz",QString::number(-0.99));
			}
		else
			{
				xmlWriter.writeAttribute("plolygontype",QString::number(1));
				xmlWriter.writeAttribute("layerz",QString::number(0.5));
			}
		if (treeWidget->topLevelItem(i)->text(4)==QObject::tr("是"))
		{
			xmlWriter.writeAttribute("checked",QString::number(1));
		}else
			xmlWriter.writeAttribute("checked",QString::number(0));
		xmlWriter.writeAttribute("width",treeWidget->topLevelItem(i)->text(5));
		QString strtemp = QString::fromLocal8Bit("0xFFFF");
		if (treeWidget->topLevelItem(i)->text(6)==QObject::tr("点线"))
		{
			strtemp = QString::fromLocal8Bit("0xAAAA");
		}else
			if (treeWidget->topLevelItem(i)->text(6)==QObject::tr("短划线"))
			{
				strtemp = QString::fromLocal8Bit("0x0F0F");
			}
		xmlWriter.writeAttribute("ztipple",strtemp);

		if(treeWidget->topLevelItem(i)->text(7) == QObject::tr("否"))
			xmlWriter.writeAttribute("hascontroler",QString("0"));
		else
			xmlWriter.writeAttribute("hascontroler",QString("1"));
		xmlWriter.writeAttribute("isrender",treeWidget->topLevelItem(i)->text(8));
		xmlWriter.writeAttribute("notrender",treeWidget->topLevelItem(i)->text(9));
		xmlWriter.writeEndElement();
	}
	xmlWriter.writeEndDocument();

	file.close();
	if (file.error())
	{
		return false;
	}
	return true;
}
/************************************************************************
 功能说明：写入MicapsDataXml中按钮相关数据
 作      者：赵高攀
 完成日期：2011年5月23日
************************************************************************/
bool OptMicapsDataXml::WriteMicapsBtnData(QTreeWidget *treeWidget)
{
	QFile file(m_FileName);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		return false;
	}
	QXmlStreamWriter xmlWriter(&file);
	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("MicapsData");

	for (int i =0;i<treeWidget->topLevelItemCount();i++)
	{
		xmlWriter.writeStartElement("AItem");
		xmlWriter.writeAttribute("name",treeWidget->topLevelItem(i)->text(0));
		xmlWriter.writeAttribute("icon",treeWidget->topLevelItem(i)->text(5));
		for (int j = 0;j<treeWidget->topLevelItem(i)->childCount();j++)
		{
			xmlWriter.writeStartElement("BItem");
			xmlWriter.writeAttribute("name",treeWidget->topLevelItem(i)->child(j)->text(0));
			xmlWriter.writeAttribute("filetype",treeWidget->topLevelItem(i)->child(j)->text(1));
			xmlWriter.writeAttribute("path",treeWidget->topLevelItem(i)->child(j)->text(2));
			xmlWriter.writeAttribute("value",treeWidget->topLevelItem(i)->child(j)->text(3));
			xmlWriter.writeAttribute("res",treeWidget->topLevelItem(i)->child(j)->text(4));
			xmlWriter.writeAttribute("icon",treeWidget->topLevelItem(i)->child(j)->text(5));
			for (int m = 0;m<treeWidget->topLevelItem(i)->child(j)->childCount();m++)
			{
				WriteMicapsIndexEntry(&xmlWriter,treeWidget->topLevelItem(i)->child(j)->child(m));
			}
			xmlWriter.writeEndElement();
		}
		xmlWriter.writeEndElement();
	}
	xmlWriter.writeEndDocument();

	file.close();
	if (file.error())
	{
		return false;
	}
	return true;
}
bool OptMicapsDataXml::WriteMicapsToolData(QTreeWidget *treeWidget,QString xmlFileName)
{
	QFile file(xmlFileName);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		return false;
	}
	QXmlStreamWriter xmlWriter(&file);
	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("MeteToolBox");

	for (int i =0;i<treeWidget->topLevelItemCount();i++)
	{
		xmlWriter.writeStartElement("AItem");
		xmlWriter.writeAttribute("name",treeWidget->topLevelItem(i)->text(0));
		xmlWriter.writeAttribute("icon",treeWidget->topLevelItem(i)->text(1));
		for (int j = 0;j<treeWidget->topLevelItem(i)->childCount();j++)
		{
			xmlWriter.writeStartElement("BItem");
			xmlWriter.writeAttribute("name",treeWidget->topLevelItem(i)->child(j)->text(0));
			xmlWriter.writeAttribute("filetype",treeWidget->topLevelItem(i)->child(j)->text(1));
			xmlWriter.writeAttribute("path",treeWidget->topLevelItem(i)->child(j)->text(2));
			xmlWriter.writeAttribute("value",treeWidget->topLevelItem(i)->child(j)->text(3));
			QString sss=treeWidget->topLevelItem(i)->child(j)->text(4);
			xmlWriter.writeAttribute("icon",treeWidget->topLevelItem(i)->child(j)->text(4));
			for (int m = 0;m<treeWidget->topLevelItem(i)->child(j)->childCount();m++)
			{
				WriteMicapsIndexEntry(&xmlWriter,treeWidget->topLevelItem(i)->child(j)->child(m));
			}
			xmlWriter.writeEndElement();
		}
		xmlWriter.writeEndElement();
	}
	xmlWriter.writeEndDocument();

	file.close();
	if (file.error())
	{
		return false;
	}
	return true;
}
/************************************************************************
 功能说明：写入MicapsDataXml中按钮Item数据
 作      者：赵高攀
 完成日期：2011年5月23日
************************************************************************/
void OptMicapsDataXml::WriteMicapsIndexEntry(QXmlStreamWriter *xmlWriter,QTreeWidgetItem *item)
{
	xmlWriter->writeStartElement("CItem");
	xmlWriter->writeAttribute("name",item->text(0));
	xmlWriter->writeAttribute("filetype",item->text(1));
	xmlWriter->writeAttribute("path",item->text(2));
	xmlWriter->writeAttribute("value",item->text(3));
	xmlWriter->writeAttribute("res",item->text(4));
	xmlWriter->writeAttribute("icon",item->text(5));
	xmlWriter->writeEndElement();
}
/************************************************************************
功能说明：读取MicapsDataXml中按钮相关数据
作      者：赵高攀
完成日期：2011年4月29日
************************************************************************/
bool OptMicapsDataXml::ReadMicapsBtnData()
{
	QFile file(m_FileName);
	if(file.open(QFile::ReadOnly|QFile::Text))
	{
		m_XmlReader.setDevice(&file);
		m_XmlReader.readNext();
		while (!m_XmlReader.atEnd())
		{
			if (m_XmlReader.isStartElement())
			{
				if (m_XmlReader.name() == "MicapsData")
				{
					ReadItemBtnData();
				}
				else
				{
					m_XmlReader.raiseError(QObject::tr("Not a MicapsData file"));
				}
			}
			else
			{
				m_XmlReader.readNext();
			}
		}
	}
	else
	{
		return false;
	}
	file.close();
	return true;
}
/************************************************************************
功能说明：设置Xml文件名
作      者：赵高攀
完成日期：2011年4月29日
************************************************************************/
void OptMicapsDataXml::SetMicapsDataFileName(const QString &filename)
{
	m_FileName = filename;
}
/************************************************************************
功能说明：获取Xml中按钮数据
作      者：赵高攀
完成日期：2011年4月29日
************************************************************************/
QVector<MicapsDataA> OptMicapsDataXml::GetMicapsBtnDataArray()
{
	return m_MicapsBtnDataArray;
}
/************************************************************************
功能说明：读取MicapsDataXml中按钮Item数据
作      者：赵高攀
完成日期：2011年4月29日
************************************************************************/
void OptMicapsDataXml::ReadItemBtnData()
{
	m_XmlReader.readNext();
	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			//qDebug()<<m_XmlReader.name();
			if (m_XmlReader.name() == "AItem")
			{
				MicapsDataA temDataA;
				temDataA.Depth=1;
				temDataA.ABtnName = m_XmlReader.attributes().value("name").toString();
				temDataA.EnBtnName = m_XmlReader.attributes().value("alias").toString();
				temDataA.Aicon = m_XmlReader.attributes().value("icon").toString();
				m_MicapsBtnDataArray.push_back(temDataA);
				ReadItemBtnData();
			}
			else if (m_XmlReader.name() == "BItem")
			{
				MicapsDataB temDataB;
				temDataB.Depth=2;
				temDataB.BBtnName = m_XmlReader.attributes().value("name").toString();
				temDataB.EnBtnName = m_XmlReader.attributes().value("alias").toString();
				temDataB.BFileType = m_XmlReader.attributes().value("filetype").toString();
				temDataB.BDataPath= m_XmlReader.attributes().value("path").toString();
				temDataB.BDataValue = m_XmlReader.attributes().value("value").toString();
				temDataB.Bicon = m_XmlReader.attributes().value("icon").toString();
				temDataB.BRes = m_XmlReader.attributes().value("res").toString();
				if (m_MicapsBtnDataArray.count()>0)
				{
					m_MicapsBtnDataArray.last().BMicapsDataArray.push_back(temDataB);
				}
				ReadItemBtnData();
			}
			else if (m_XmlReader.name() == "CItem")
			{
				MicapsDataC temDataC;
				temDataC.CBtnName = m_XmlReader.attributes().value("name").toString();
				temDataC.EnBtnName = m_XmlReader.attributes().value("alias").toString();
				temDataC.CFileType = m_XmlReader.attributes().value("filetype").toString();
				temDataC.CDataPath= m_XmlReader.attributes().value("path").toString();
				temDataC.CDataValue = m_XmlReader.attributes().value("value").toString();
				temDataC.Cicon = m_XmlReader.attributes().value("icon").toString();
				temDataC.CRes = m_XmlReader.attributes().value("res").toString();
				if(m_MicapsBtnDataArray.last().BMicapsDataArray.count()>0)
				{
					m_MicapsBtnDataArray.last().BMicapsDataArray.last().CMicapsDataArray.push_back(temDataC);
				}
				ReadItemBtnData();
			}
			else
			{
				SkipUnKnownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/************************************************************************
功能说明：跳过不能识别的元素
作      者：赵高攀
完成日期：2011年4月29日
************************************************************************/
void OptMicapsDataXml::SkipUnKnownElement()
{
	m_XmlReader.readNext();
	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			SkipUnKnownElement();
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}

/************************************************************************
功能说明：读取MeteToolBoxXml中按钮相关数据
作    者：renxue
完成日期：2011年05月12日
************************************************************************/
bool OptMicapsDataXml::ReadMeteToolBoxData()
{
	QFile file(m_FileName);
	if(file.open(QFile::ReadOnly|QFile::Text))
	{
		m_XmlReader.setDevice(&file);
		m_XmlReader.readNext();
		while (!m_XmlReader.atEnd())
		{
			if (m_XmlReader.isStartElement())
			{
				if (m_XmlReader.name() == "MeteToolBox")
				{
					ReadItemBtnData();
				}
				else
				{
					m_XmlReader.raiseError(QObject::tr("Not a MeteToolBox file"));
				}
			}
			else
			{
				m_XmlReader.readNext();
			}
		}
	}
	else
	{
		return false;
	}
	file.close();
	return true;
}

/************************************************************************
 功能说明：写入地图前景色，背景色等相关数据
 作      者：易家祥
 完成日期：2011年10月31日
************************************************************************/
bool OptMicapsDataXml::WriteMapColorConfigData( QObjectList *objectList,QString xmlFileName )
{
	QFile file(xmlFileName);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		return false;
	}
	QXmlStreamWriter xmlWriter(&file);
	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("MapColorConfig");

	for (int i =0;i<objectList->count();i++)
	{
	//	xmlWriter.writeStartElement("Info");
		if (objectList->at(i)->property("bColor").toString() == QString("bColor"))
		{
			xmlWriter.writeStartElement("bColor");
		//	xmlWriter.writeAttribute("name","bColor");					
			xmlWriter.writeTextElement("r",objectList->at(i)->property("red").toString());
			xmlWriter.writeTextElement("g",objectList->at(i)->property("green").toString());
			xmlWriter.writeTextElement("b",objectList->at(i)->property("blue").toString());
			xmlWriter.writeTextElement("a",objectList->at(i)->property("alpha").toString());
			xmlWriter.writeEndElement();
		}
		else if (objectList->at(i)->property("fColor").toString() == QString("fColor"))
		{
			xmlWriter.writeStartElement("fColor");
		//	xmlWriter.writeAttribute("name","fColor");
			xmlWriter.writeTextElement("r",objectList->at(i)->property("red").toString());
			xmlWriter.writeTextElement("g",objectList->at(i)->property("green").toString());
			xmlWriter.writeTextElement("b",objectList->at(i)->property("blue").toString());
			xmlWriter.writeTextElement("a",objectList->at(i)->property("alpha").toString());
			xmlWriter.writeEndElement();
		}
		else if (objectList->at(i)->property("Aaliasing").toString() == QString("Aaliasing"))
		{
			xmlWriter.writeStartElement("Aaliasing");
		//	xmlWriter.writeAttribute("name","Aaliasing");		
			xmlWriter.writeTextElement("checked",objectList->at(i)->property("checked").toString());
			xmlWriter.writeEndElement();
		}
		else if (objectList->at(i)->property("objsysName").toString() == QString("objsysName"))
		{
			xmlWriter.writeStartElement("objsysName");
		//	xmlWriter.writeAttribute("name","objsysName");
			xmlWriter.writeTextElement("str",objectList->at(i)->property("str").toString());
			xmlWriter.writeEndElement();
		}
		else if (objectList->at(i)->property("Projection").toString() == QString("Projection"))
		{
			xmlWriter.writeStartElement("Projection");
		//	xmlWriter.writeAttribute("name","Projection");
			xmlWriter.writeTextElement("str",objectList->at(i)->property("str").toString());
			xmlWriter.writeEndElement();
		}
	/*	else if (objectList->at(i)->property("M").toString() == QString("M"))
		{
			xmlWriter.writeAttribute("name","M");
			xmlWriter.writeAttribute("index",objectList->at(i)->property("index").toString());
			xmlWriter.writeAttribute("checked",objectList->at(i)->property("checked").toString());
		}
		else if (objectList->at(i)->property("L").toString() == QString("L"))
		{
			xmlWriter.writeAttribute("name","L");
			xmlWriter.writeAttribute("index",objectList->at(i)->property("index").toString());
			xmlWriter.writeAttribute("checked",objectList->at(i)->property("checked").toString());
		}
		else if (objectList->at(i)->property("E").toString() == QString("E"))
			{
			xmlWriter.writeAttribute("name","E");
			xmlWriter.writeAttribute("index",objectList->at(i)->property("index").toString());
			xmlWriter.writeAttribute("checked",objectList->at(i)->property("checked").toString());
			}
		else if (objectList->at(i)->property("R").toString() == QString("R"))
			{
			xmlWriter.writeAttribute("name","R");
			xmlWriter.writeAttribute("index",objectList->at(i)->property("index").toString());
			xmlWriter.writeAttribute("checked",objectList->at(i)->property("checked").toString());
			}
		else if (objectList->at(i)->property("W").toString() == QString("W"))
			{
			xmlWriter.writeAttribute("name","W");
			xmlWriter.writeAttribute("index",objectList->at(i)->property("index").toString());
			xmlWriter.writeAttribute("checked",objectList->at(i)->property("checked").toString());
			}
		else if (objectList->at(i)->property("O").toString() == QString("O"))
			{
			xmlWriter.writeAttribute("name","O");
			xmlWriter.writeAttribute("index",objectList->at(i)->property("index").toString());
			xmlWriter.writeAttribute("checked",objectList->at(i)->property("checked").toString());
			}
		else if (objectList->at(i)->property("N").toString() == QString("N"))
			{
			xmlWriter.writeAttribute("name","N");
			xmlWriter.writeAttribute("index",objectList->at(i)->property("index").toString());
			xmlWriter.writeAttribute("checked",objectList->at(i)->property("checked").toString());
			}
		else if (objectList->at(i)->property("S").toString() == QString("S"))
			{
			xmlWriter.writeAttribute("name","S");
			xmlWriter.writeAttribute("index",objectList->at(i)->property("index").toString());
			xmlWriter.writeAttribute("checked",objectList->at(i)->property("checked").toString());
			}
          */
	//	xmlWriter.writeEndElement();
	}
	xmlWriter.writeEndElement();
	xmlWriter.writeEndDocument();
	file.close();
	if (file.error())
	{
		return false;
	}
	return true;
}

void OptMicapsDataXml::ReadColor(QString name,QColor& color)
{
	int r = 0;
	int g = 0;
	int b = 0;
	int a = 0;

	while(!m_XmlReader.atEnd())  
	{  
		m_XmlReader.readNext();  
		if(m_XmlReader.isStartElement())  
		{  
			if(m_XmlReader.name() == "r")  
			{  
				r = m_XmlReader.readElementText().toInt();
			}  
			if(m_XmlReader.name() == "g")  
			{  
				g =m_XmlReader.readElementText().toInt();
			}  
			if(m_XmlReader.name() == "b")  
			{  
				b = m_XmlReader.readElementText().toInt();
			}  
			if(m_XmlReader.name() == "a")  
			{  
				a = m_XmlReader.readElementText().toInt();
			}  	
		}  
		if(m_XmlReader.isEndElement())  
		{  
			if(m_XmlReader.name() == name)  
			{  			
				break;  
			}  
		}  
	} 
	color.setRed(r);
	color.setGreen(g);
	color.setBlue(b);
	color.setAlpha(a);
}


void OptMicapsDataXml::Readbool(QString name,bool& AlasCheck)
{
	QString strCheck;
	while (!m_XmlReader.atEnd())
	{
		m_XmlReader.readNext();

		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "checked")
			{
				strCheck = m_XmlReader.readElementText();
			}			
		}		 			

		if (m_XmlReader.isEndElement())
		{
			if(m_XmlReader.name() == name)  
			{  			
				break;  
			} 
		}
	}

	if (strCheck == "true")
	{
		AlasCheck = true;
	}
	else
	{
		AlasCheck = false;
	}
}

void OptMicapsDataXml::ReadString(QString name,QString& sysname)
{
	while (!m_XmlReader.atEnd())
	{
		m_XmlReader.readNext();

		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "str")
			{
				sysname = m_XmlReader.readElementText();
			}			
		}		 			

		if (m_XmlReader.isEndElement())
		{
			if(m_XmlReader.name() == name)  
			{  			
				break;  
			} 
		}
	}
}

void OptMicapsDataXml::ReadMapColorConfig(QColor& bcolor,QColor& fcolor,bool& AlasCheck,QString& sysname,QString& projectionname)
{
	while (!m_XmlReader.atEnd())
	{
		m_XmlReader.readNext();

		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "bColor")
			{
				ReadColor(QString("bColor"),bcolor);
			}
			if (m_XmlReader.name() == "fColor")
			{
				ReadColor(QString("fColor"),fcolor);
			}
			if (m_XmlReader.name()== "Aaliasing")
			{
				Readbool(QString("Aaliasing"),AlasCheck);
			}
			if (m_XmlReader.name()== "objsysName")
			{
				ReadString(QString("objsysName"),sysname);
			}
			if (m_XmlReader.name()== "Projection")
			{
				ReadString(QString("Projection"),projectionname);
			}
		}		 			

		if (m_XmlReader.isEndElement())
		{
			if(m_XmlReader.name() == "MapColorConfig")  
			{  			
				break;  
			} 
		}
	}
}

bool OptMicapsDataXml::ReadMapColorConfigFile(QColor& bcolor,QColor& fcolor,bool& AlasCheck,QString& sysname,QString& projectionname)
{
	QString fileName = CIPASApplication::ConfigPath()+ "/BasicInfo/MapColorConfig.xml";
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		return false;
	}
	m_XmlReader.setDevice(&file);

	while (!m_XmlReader.atEnd())
	{
		m_XmlReader.readNext();

		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "MapColorConfig")
			{
				ReadMapColorConfig(bcolor,fcolor,AlasCheck,sysname,projectionname);				
			}			
		}		
	}
	file.close();
	if (m_XmlReader.hasError())
	{
		return false;
	}else if(file.error() != QFile::NoError)
	{
		return false;
	}
	return true;
}