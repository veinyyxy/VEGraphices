#include "SaveColorTableToXML.h"

SaveColorTableToXML::SaveColorTableToXML(void)
{
	m_pFile = new QFile;
	m_pXmlWriter = new QXmlStreamWriter;
}

SaveColorTableToXML::~SaveColorTableToXML(void)
{
	delete m_pFile;
	delete m_pXmlWriter;
	m_pFile = NULL;
	m_pXmlWriter = NULL;
}

void SaveColorTableToXML::SaveColorTableToXMLFile(QString filename,COLORTABLELIST &colortablelist)
{
	m_fileName = filename;
	beginWrite();	
	COLORTABLELIST::iterator it;
	QString colorTableName;
	for (it=colortablelist.begin();it!= colortablelist.end();++it)
	{
		colorTableName = it.key();
		SaveColorTable(colorTableName,it.value());
	}
	endWrite();
}

/************************************************************************
 功能说明：开始写XML
 作      者：杨东
 完成日期：2011年4月28日
************************************************************************/
bool SaveColorTableToXML::beginWrite()
{
//	QString fileName = QFileDialog::getSaveFileName(this,tr("Save Symbol"),  "/home/Symbol.xml", tr("*.XML"));
	if (m_fileName.isEmpty()) 
	{
		return false;
	}
	else
	{
		QFileInfo temDir(m_fileName);
		QString fileRex = temDir.suffix();//后缀

		if (fileRex.toLower() == "xml")
		{
			m_pFile->setFileName(m_fileName);
			if (!m_pFile->open(QFile::WriteOnly | QFile::Text))
			{
				return false;
			}	

			m_pXmlWriter->setDevice(m_pFile);
			m_pXmlWriter->setAutoFormatting(true);
			m_pXmlWriter->writeStartDocument();
			// 	QTextCodec *gbk = QTextCodec::codecForName("GB2312"); 
			// 	m_pXmlWriter->setCodec(gbk);							
			m_pXmlWriter->writeStartElement("ColorTableList");
			writeFileHead();
			m_pXmlWriter->writeStartElement("Data");
			return true;
		}	
		else
			return false;
	}	
}

/************************************************************************
**** 写文件头
**** zhanglei
************************************************************************/
void SaveColorTableToXML::writeFileHead()
{
	m_pXmlWriter->writeStartElement("FileHead");
	//Write DataType
	m_pXmlWriter->writeStartElement("DataType");
	m_pXmlWriter->writeAttribute("Type", "CIPAS");
	m_pXmlWriter->writeEndElement();
	//Write Description
	m_pXmlWriter->writeStartElement("Description");
	m_pXmlWriter->writeEndElement();
	//Write Date
	m_pXmlWriter->writeStartElement("Date");
	m_pXmlWriter->writeEndElement();

	m_pXmlWriter->writeEndElement(); //End of FileHead
}

void SaveColorTableToXML::SaveColorTable(QString strname,COLORTABLE& colortable)
{	
	m_pXmlWriter->writeStartElement("ColorTableID");
	m_pXmlWriter->writeAttribute("id", strname);

	COLORTABLE::iterator it;

	double tempValue;
	QColor TempColor;
	QString tempString;
	int Zcount;
	for(it = colortable.begin();it!= colortable.end();++it)
	{
		tempValue = it.key();
		TempColor = it.value()._color;
		tempString = it.value()._Name;
		Zcount = it.value()._Zcount;
		m_pXmlWriter->writeStartElement("ColorItem");
		m_pXmlWriter->writeTextElement("value",QString::number(tempValue)); 
		m_pXmlWriter->writeTextElement("ValueString",tempString); 
		m_pXmlWriter->writeStartElement("Color");
		m_pXmlWriter->writeTextElement("r",QString::number(TempColor.redF()));
		m_pXmlWriter->writeTextElement("g",QString::number(TempColor.greenF()));
		m_pXmlWriter->writeTextElement("b",QString::number(TempColor.blueF()));
		m_pXmlWriter->writeTextElement("a",QString::number(TempColor.alphaF()));
		m_pXmlWriter->writeEndElement(); //End of Color		
		m_pXmlWriter->writeTextElement("ZValue",QString::number(Zcount)); 
		m_pXmlWriter->writeEndElement(); //End of ColorItem		
		
	}
	m_pXmlWriter->writeEndElement();	//End of ColorTableID
}

/************************************************************************
 功能说明：结束写XML
 作      者：zhanglei
 完成日期：2011年11月23日
************************************************************************/
bool SaveColorTableToXML::endWrite()
{	
	m_pXmlWriter->writeEndElement(); //End of Data
	m_pXmlWriter->writeEndElement(); //End of ColorTableList
	m_pXmlWriter->writeEndDocument();
	m_pFile->close();
	if (m_pFile->error())
	{
		return false;
	}
	return true;
}
