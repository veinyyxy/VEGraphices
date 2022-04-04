#include "StationInterpretation.h"




StationInterpretation::StationInterpretation(void)
{	
	stationPointArray = new StationDataArray;
	stationPointArray1 = new StationDataArray;;      //一般站点
	stationPointArray2 = new StationDataArray;;      //基本站点
    stationPointArray3 = new StationDataArray;;      //基准站点
	m_stationType = 0;

}

StationInterpretation::~StationInterpretation(void)
{
	delete stationPointArray;
	stationPointArray = NULL;
	delete stationPointArray1;
	stationPointArray1 = NULL;
	delete stationPointArray2;
	stationPointArray2 = NULL;
	delete stationPointArray3;
	stationPointArray3 = NULL;
}
/************************************************************************/
// 函数功能： 读取站点数据
// 返 回 值:  bool
// 作    者:  陈志学		
// 日    期:  2011 年 4 月 14 日
/************************************************************************/
bool StationInterpretation::readFile( const QString &filename,const int type )
{
	QFile file(filename);
	if(!file.open(QFile::ReadOnly|QFile::Text))
	{
		//std::cerr <<"Error: Cannot read file"<<qPrintable(filename)
		//		  <<":" <<qPrintable(file.errorString())
		//		  << std::endl;
		return false;

	}
	else
	{
		m_stationType = type;
		reader.setDevice(&file);
		reader.readNext();
		while (!reader.atEnd())
		{
			if (reader.isStartElement())
			{		
				if (reader.name() == "ROWDATA")
				{
					readRowElement();
				}
				else
				{
					reader.raiseError(QObject::tr("Not a station file"));
				}
			}
			else
			{
				reader.readNext();
			}
		}
		file.close();
		return true;
	}

}

void StationInterpretation::readRowDataElement()
{
	reader.readNext();
	while(!reader.atEnd())
	{
		if (reader.isEndElement())
		{
			reader .readNext();
			break;
		}
		if (reader.isStartElement())
		{
			if (reader.name() == "ROW")
			{
				readRowElement();
			} 
			else
			{
				skipUnknownElement();
			}
		}
		else
		{
			reader.readNext();
		}
	}
}

void StationInterpretation::readRowElement()
{
	CStation* stationElement = new CStation();
	reader.readNext();
	while (!reader.atEnd())
	{
		
		if (reader.isEndElement())
		{
			reader.readNext();
			break;
		}
		if (reader.isStartElement())
		{
			if (reader.name() == "ROW")
			{
				readRowElement();
			}
			else if (reader.name() == "C0")
			{
				stationElement->c0 = readC0Element();
			} 
			else if (reader.name() == "CH_NAME")
			{
				stationElement->ch_name = readCHNameElement();
			}
			else if (reader.name() == "V01000")
			{
				stationElement->v01000 = readV01000Element();
			}
			else if (reader.name() == "V05001")
			{
				stationElement->v05001 = readV05001Element();
			}
			else if (reader.name() == "V06001")
			{
				stationElement->v06001 = readV06001Element();
			}
			else if (reader.name() == "V07001")
			{
				stationElement->v07001 = readV07001Element();
			}
			else if (reader.name() == "V07033")
			{
				stationElement->v07033 = readV07033Element();
			}
			else if (reader.name() == "V02001")
			{
				stationElement->v02001 = readV02001Element();
			}
			else if (reader.name() == "CH_AREA_PROVINCE")
			{
				stationElement->ch_area_province = readCHAreaProvinceElement();
			}
			else if (reader.name() == "CH_AREA_CITY")
			{
				stationElement->ch_area_city = readCHAreaCityElement();
			}
			else if (reader.name() == "CH_AREA_COUNTY")
			{
				stationElement->ch_area_county = readCHAreaCountyElement();
			}
			else if (reader.name() == "BY_STATUS")
			{
				stationElement->by_status = readBYStatusElement();
			}
			else if (reader.name() == "CH_OBS_TYPE")
			{
				stationElement->ch_obs_type = readCHOBSTypeElement();
			}

			else if (reader.name() == "CH_BUSINESS_TYPE")
			{
				stationElement->ch_business_type = readCHBusnessTypeElement();
			}
			else if (reader.name() == "CH_START_YEAR")
			{
				stationElement->ch_start_year = readCHStartYearElement();
			}
			else if (reader.name() == "CH_START_MONTH")
			{
				stationElement->ch_start_month = readCHStartMonthElement();
			}
			else if (reader.name() == "CH_END_YEAR")
			{
				stationElement->ch_end_year = readCHEndYearElement();
			}
			else if (reader.name() == "CH_END_MONTH")
			{
				stationElement->ch_end_month = readCHEndMonthElement();

				StationInfo staInfo ;
				staInfo.id = stationElement->c0;
				staInfo.staNumb=stationElement->v01000;
				staInfo.pSymbolPoints = QVector3D(stationElement->v06001,stationElement->v05001,0/*stationElement->v07001*/);
				staInfo.staType = stationElement->ch_obs_type;
				if (staInfo.staType == QObject::tr("一般站"))
				{
					stationPointArray1->push_back(staInfo);//只添加一般站
				}
				else if (staInfo.staType ==QObject::tr("基本站"))
				{
					stationPointArray2->push_back(staInfo);//只添加基本站
				}

				else if (staInfo.staType ==QObject::tr("基准站"))
				{
					stationPointArray3->push_back(staInfo);//只添加基准站
				}
				stationPointArray->push_back(staInfo); //添加所有站点				
			}
			else 
			{
				skipUnknownElement();
			}

		}
		else
		{
			reader.readNext();
		}
		
	}
//	int size = stList.size();
}

QString StationInterpretation::readC0Element()
{
	QString id = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (id.isEmpty())
	{
		return "";
	}
	else
	{
	return id;
	}
}

QString StationInterpretation::readCHNameElement()
{
	QString name = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (name.isEmpty())
	{
		return "";
	}
	else
	{
		return name;
	}
	
}

QString StationInterpretation::readV01000Element()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return 0;
	}
	else
	{
		return str;
	}
}

double StationInterpretation::readV05001Element()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return 0;
	}
	else
	{
		return str.toDouble();
	}
}

double StationInterpretation::readV06001Element()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return 0;
	}
	else
	{
		return str.toDouble();
	}
}

double StationInterpretation::readV07001Element()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return 0;
	}
	else
	{
		return str.toDouble();
	}
}

double StationInterpretation::readV07033Element()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return 0;
	}
	else
	{
		return str.toDouble();
	}
}

QString StationInterpretation::readV02001Element()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return "";
	}
	else
	{
		return str;
	}
}

QString StationInterpretation::readCHAreaProvinceElement()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return "";
	}
	else
	{
		return str;
	}
}

QString StationInterpretation::readCHAreaCityElement()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return "";
	}
	else
	{
		return str;
	}
}

QString StationInterpretation::readCHAreaCountyElement()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return "";
	}
	else
	{
		return str;
	}
}

QString StationInterpretation::readBYStatusElement()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return "";
	}
	else
	{
		return str;
	}
}

QString StationInterpretation::readCHOBSTypeElement()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return "";
	}
	else
	{
		return str;
	}
}

QString StationInterpretation::readCHBusnessTypeElement()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return "";
	}
	else
	{
		return str;
	}
}

QString StationInterpretation::readCHStartYearElement()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return "";
	}
	else
	{
		return str;
	}
}

QString StationInterpretation::readCHStartMonthElement()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return "";
	}
	else
	{
		return str;
	}
}

QString StationInterpretation::readCHEndYearElement()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return "";
	}
	else
	{
		return str;
	}
}

QString StationInterpretation::readCHEndMonthElement()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return "";
	}
	else
	{
		return str;
	}
}

QString StationInterpretation::readROWIDElement()
{
	QString str = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	if (str.isEmpty())
	{
		return "";
	}
	else
	{
		return str;
	}
}

void StationInterpretation::skipUnknownElement()
{
	reader.readNext();
	while (!reader.atEnd())
	{
		if(reader.isEndElement())
		{
			reader.readNext();
			break;
		}
		if (reader.isStartElement())
		{
			skipUnknownElement();
		}
		else
		{
			reader.readNext();
		}
	}
}

/************************************************************************/
// 函数功能： 建立表并将站点数据输入数据库

// 返 回 值:  bool
// 作    者:  陈志学		
// 日    期:  2011 年 4 月 18 日
/************************************************************************/
//bool StationInterpretation::addDataIntoDB()
//{
//////	int tt = stList.size();
////	if (db.CreateStationTable() &&  db.InsertStations(stList))
////	{
////		return true;
////	}
////	else
////	{
////		return false;
////	}
//	return true;
//	
//}

/************************************************************************/
// 函数功能： 从内存数据库中查询站点数据
// 返 回 值:  bool
// 作    者:  陈志学		
// 日    期:  2011 年 4 月 18 日
/************************************************************************/
//bool StationInterpretation::selectFromDB()
//{
//
//	QString tablename  = db.StationTableName();
//	QStringList sqllist;
//	sqllist.push_back( QString("SELECT"));
//	sqllist.push_back( QString("c0"));
//	sqllist.push_back( QString("V06001,"));
//	sqllist.push_back( QString("V05001"));
//	sqllist.push_back( QString("V07001"));	
//	sqllist.push_back( QString("FROM"));
//	sqllist.push_back( tablename);
//	QString sql = (sqllist.join(" "));
//	QSqlDatabase database = db.GetDatabase();
//	QSqlQuery query(database);
//	query.exec( sql );
//
//	while (query.next())
//	{
//		StationInfo station;
//		station.pSymbolPoints = QVector3D(query.value(1).toDouble(),query.value(2).toDouble(),query.value(3).toDouble());
//		station.id =  query.value(0).toString();
//		stationPointArray->push_back(station);
//	}
//	if (!stationPointArray->isEmpty())
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}

/************************************************************************/
// 函数功能： 获得站点数组
// 返 回 值:  QVector3DArray*
// 作    者:  陈志学		
// 日    期:  2011 年 4 月 15 日
/************************************************************************/
StationDataArray * StationInterpretation::getStationPointArray(const int type)
{
	if (0 == type )
	{
		return stationPointArray;
	}
	else if (1 == type)
	{
		return stationPointArray1;
	}
	else if (2 == type)
	{
		return stationPointArray2;
	}
	else if (3 == type)
	{
		return stationPointArray3;

	}
	else if (4 == type)
	{
		return stationPointArray;
	}
//	else if (12== type)
//	{
//		return &(*stationPointArray1+*stationPointArray2);
//	}
//	else if (13== type)
//	{
//		return &(*stationPointArray1+*stationPointArray3);
//	}
//	else if (23== type)
//	{
//		return &(*stationPointArray2+*stationPointArray3);
//	}
}

/************************************************************************/
// 函数功能： 判断站点是否有数据
// 返 回 值:  bool
// 作    者:  陈志学		
// 日    期:  2011 年 4 月 15 日
/************************************************************************/
bool StationInterpretation::isEmpty()
{
	if (stationPointArray->isEmpty()){return true; }else{return false;}
}
