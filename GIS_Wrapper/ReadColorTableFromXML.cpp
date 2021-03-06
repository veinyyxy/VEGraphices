#include "ReadColorTableFromXML.h"

ReadColorTableFromXML::ReadColorTableFromXML(void)
{	
	
}

ReadColorTableFromXML::~ReadColorTableFromXML(void)
{
}
/************************************************************************
 功能说明：读xml
 作      者：杨东
 完成日期：2011年5月12日

 zhanglei 修改 2011.7.28
************************************************************************/
bool ReadColorTableFromXML::readFile( const QString &fileName,COLORTABLELIST &colortablelist )
{
	QFile file(fileName);	
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		return false;
	}
	m_XMLreader.setDevice(&file);

	while(!m_XMLreader.atEnd())
	{
		m_XMLreader.readNext();
		if (m_XMLreader.isStartElement())
        {
            if (m_XMLreader.name() == QStringView(QString::fromLocal8Bit("ColorTableList")))
			{
				colortablelist.clear();
				ReadColorTableList(colortablelist);
			}
		}		
	}
	file.close();
	if (m_XMLreader.hasError())
	{
		return false;
	}else if(file.error() != QFile::NoError)
	{
		return false;
	}
	return true;
}

/************************************************************************
 功能说明：处理未知元素
 作      者：杨东
 完成日期：2011年5月12日
************************************************************************/
void ReadColorTableFromXML::skipUnknowElement()
{
	m_XMLreader.readNext();
	while(!m_XMLreader.atEnd())
	{
		if (m_XMLreader.isEndElement())
		{
			m_XMLreader.readNext();
			break;
		}
		if (m_XMLreader.isStartElement())
		{
			skipUnknowElement();
		}
		else
		{
			m_XMLreader.readNext();
		}
	}
}

//读文件头
void ReadColorTableFromXML::ReadFilehead()
{
	//qDebug()<<"Start to read fileHead...";  

	while (!m_XMLreader.atEnd())
	{
		m_XMLreader.readNext();

		if (m_XMLreader.isStartElement())
		{
            if (m_XMLreader.name() == QObject::tr("DataType"))
			{
				m_DataType = m_XMLreader.readElementText();
			}	
            if (m_XMLreader.name() == QObject::tr("Description"))
			{
				m_Description = m_XMLreader.readElementText();
			}
            if (m_XMLreader.name() == QObject::tr("Date"))
			{
				m_Date = m_XMLreader.readElementText();
			}			
		}	

		if (m_XMLreader.isEndElement())
		{
            if(m_XMLreader.name() == QObject::tr("FileHead"))
			{  
				//qDebug()<<"Now at the end of FileHead...";  
				break;  
			} 
		}
	}
}
void ReadColorTableFromXML::ReadData(COLORTABLELIST &colortablelist)
{
	QString name;
	while (!m_XMLreader.atEnd())
	{
		m_XMLreader.readNext();

		if (m_XMLreader.isStartElement())
		{
            if (m_XMLreader.name() == QObject::tr("ColorTableID"))
 			{
// 				ReadPointSymbols();
				name = m_XMLreader.attributes().value("id").toString();
				COLORTABLE colorTableTemp;
				ReadColorTable(colorTableTemp);
				colortablelist.insert(name,colorTableTemp);
			}				
		}	

		if (m_XMLreader.isEndElement())
		{
            if(m_XMLreader.name() == QObject::tr("Data"))
			{  
				//qDebug()<<"Now at the end of Data...";  
				break;  
			} 
		}
	}
}

void ReadColorTableFromXML::ReadColorTable(COLORTABLE &colortable)
{
	double vauleTemp;
	QString valuestringtemp;
	QColor colorTemp;
	int Zcount;
//	COLOR_PROPERTY temppor;
	while (!m_XMLreader.atEnd())
	{
		m_XMLreader.readNext();

		if (m_XMLreader.isStartElement())
		{
            if (m_XMLreader.name() == QObject::tr("ColorItem"))
			{
			   ReadItem(vauleTemp,valuestringtemp,colorTemp,Zcount);
			   colortable.insert(vauleTemp,COLOR_PROPERTY(valuestringtemp,colorTemp,Zcount));
			}				
		}	

		if (m_XMLreader.isEndElement())
		{
            if(m_XMLreader.name() == QObject::tr("ColorTableID"))
			{  
				break;  
			} 
		}
	}
}

void ReadColorTableFromXML::ReadItem(double &vlaue,QString &valuestring,QColor &color,int &Zcount)
{
	while (!m_XMLreader.atEnd())
	{
		m_XMLreader.readNext();  
		if(m_XMLreader.isStartElement())  
		{  
            if(m_XMLreader.name() == QObject::tr("value"))
			{  
				vlaue = m_XMLreader.readElementText().toDouble();			
			}  	
            if(m_XMLreader.name() == QObject::tr("ValueString"))
			{  
				valuestring = m_XMLreader.readElementText();			
			}  
            if (m_XMLreader.name() == QObject::tr("Color"))
			{
				ReadColorElement(color);								
			}
            if (m_XMLreader.name() == QObject::tr("ZValue"))
			{
				Zcount = m_XMLreader.readElementText().toInt();									
			}
			
		}  
		if(m_XMLreader.isEndElement())  
		{  
            if(m_XMLreader.name() == QObject::tr("ColorItem"))
			{  
				//qDebug()<<"Now at the end of SymbolID...";  
				break;  
			}  
		}  									  
	}
}

void ReadColorTableFromXML::ReadColorElement(QColor &color) //读颜色
{
	QString r,g,b,a;
	while(!m_XMLreader.atEnd())  
	{  
		m_XMLreader.readNext();  
		if(m_XMLreader.isStartElement())  
		{  
            if(m_XMLreader.name() == QObject::tr("r"))
			{  
				r = m_XMLreader.readElementText();
				color.setRedF(r.toFloat());
			}  
            if(m_XMLreader.name() == QObject::tr("g"))
			{  
				g =m_XMLreader.readElementText();
				color.setGreenF(g.toFloat());
			}  
            if(m_XMLreader.name() == QObject::tr("b"))
			{  
				b = m_XMLreader.readElementText();
				color.setBlueF(b.toFloat());
			}  		
            if(m_XMLreader.name() == QObject::tr("a"))
			{  
				a = m_XMLreader.readElementText();
				color.setAlphaF(a.toFloat());
			} 
		}  
		if(m_XMLreader.isEndElement())  
		{  
            if(m_XMLreader.name() == QObject::tr("Color"))
			{  
				//qDebug()<<"Now at the end of Color..";  
				break;  
			}  
		}  
	} 
}

void ReadColorTableFromXML::ReadColorTableList(COLORTABLELIST &colortablelist)
{
	while(!m_XMLreader.atEnd())  
	{  
		m_XMLreader.readNext();  
		if(m_XMLreader.isStartElement())  
		{  
            if (m_XMLreader.name() == QObject::tr("FileHead"))
			{
				ReadFilehead();
			}
            if (m_XMLreader.name() == QObject::tr("Data"))
 			{
 			    ReadData(colortablelist);
 			}			  
		}  
		if(m_XMLreader.isEndElement())  
		{  
            if(m_XMLreader.name() == QObject::tr("ColorTableList"))
			{    
				break;  
			}  
		}  
	} 	
}
