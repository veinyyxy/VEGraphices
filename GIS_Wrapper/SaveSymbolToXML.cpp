#include "SaveSymbolToXML.h"
#include "Layer.h"
//#include <QTextCodec>

SaveSymbolToXML::SaveSymbolToXML(void)
{
	m_pFile = new QFile;
	m_pXmlWriter = new QXmlStreamWriter;
}

SaveSymbolToXML::~SaveSymbolToXML(void)
{
	delete m_pFile;
	delete m_pXmlWriter;
	m_pFile = NULL;
	m_pXmlWriter = NULL;
}

void SaveSymbolToXML::SaveSymbolToXMLFile(SymbolData *symbolData,QString filename)
{
	m_fileName = filename;
	beginWrite();
	SaveNormalSymbolToXML(symbolData);
	SavePointSymbolToXML(symbolData);
	SaveContourLineToXML(symbolData);	
	SaveLineSymbolToXML(symbolData);
	SaveMultiLineSymbolToXML(symbolData);
	SaveNormalSymbolToXML(symbolData);
	SaveRegionSymbolToXML(symbolData);
	SaveMarkSymbolToXML(symbolData);
	endWrite();
}
/************************************************************************
 功能说明：开始写XML
 作      者：杨东
 完成日期：2011年4月28日
************************************************************************/
bool SaveSymbolToXML::beginWrite()
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
			m_pXmlWriter->writeStartElement("SymbolsData");		
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
void SaveSymbolToXML::writeFileHead()
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

/************************************************************************
 功能说明：结束写XML
 作      者：杨东
 完成日期：2011年4月28日
************************************************************************/
bool SaveSymbolToXML::endWrite()
{
	m_pXmlWriter->writeEndElement(); //End of Data
	m_pXmlWriter->writeEndElement(); //End of Symbol data
	m_pXmlWriter->writeEndDocument();
	m_pFile->close();
	if (m_pFile->error())
	{
		return false;
	}
	return true;
}


/************************************************************************/
/* 点符号的保存
   zhanglei 2011.7.28*/
/************************************************************************/
void SaveSymbolToXML::SavePointSymbolToXML(SymbolData *symbolData)
{
	QVector<SymbolPoint*> *symbol = symbolData->getSymbolPoint();
	
	if (symbol->size() > 0)
	{
		m_pXmlWriter->writeStartElement("PointSymbols");
		int nType;
	
		for (int i=0; i<symbol->size(); i++)
		{
			//Get the information of symbolID
			SymbolPoint *pPointSymbol = symbol->at(i);
			Symbol* pSymbol = pPointSymbol->getSymbol();
			QVector<GLfloat> *angleArray = pPointSymbol->AngleArray();
			if (pSymbol != NULL)
			{
				nType = pSymbol->getType();
			}	
			//Get the pointer of data
			QVector3DArray *v3d = pPointSymbol->OriginalArray();	

			for (int j=0, k=0; j<v3d->size(); k++, j++)
			{
				//Write SymbolID
				m_pXmlWriter->writeStartElement("SymbolID");
				m_pXmlWriter->writeAttribute("id", QString::number(nType,10));
				
				//Get the information of symbolItem
			//	m_pXmlWriter->writeStartElement("PointItem");
				m_pXmlWriter->writeStartElement("Position");

				//Write Position
				QVector3D v3 = v3d->at(j);
				m_pXmlWriter->writeTextElement("x",QString::number(v3.x()));
				m_pXmlWriter->writeTextElement("y",QString::number(v3.y()));
				m_pXmlWriter->writeTextElement("z",QString::number(v3.z()));

				m_pXmlWriter->writeEndElement(); //End of position

				//Write Angle
				GLfloat angle_ = angleArray->at(k);
				m_pXmlWriter->writeTextElement("Angle",QString::number(angle_)); 

			//	m_pXmlWriter->writeEndElement();//End of PointItem	
				m_pXmlWriter->writeEndElement();	//End of SymbolID
			}		
		}

		m_pXmlWriter->writeEndElement();	//End of PointSymbols	
	}
}

void SaveSymbolToXML::SaveMultiLineSymbolToXML(SymbolData *symbolData)
{
	QVector<CSymbolMultiLine*> *symbol = symbolData->getSymbolMultiLine();

	if (!(symbol->isEmpty()))
	{		
		m_pXmlWriter->writeStartElement("MultiLineSymbols");

		for (int i=0; i<symbol->size(); i++)
		{
			//Get the information of symbolID				
			CSymbolMultiLine *symbolLine = symbol->at(i);
			QVector3DArray *v3d = symbolLine->OriginalArray();
			QVector4D symbolColor = symbolLine->m_LineAttribute.GetColor();

			int nType = symbolLine->m_LineAttribute.GetSymbolType(); //获取线型ID 0,1,2 
			int width = symbolLine->m_LineAttribute.GetLineWidth(); //线宽
			int nSymbolId = symbolLine->GetLineStyle()->getType(); //获取 SymbolID,601
			
		//	if (nSymbolId != 1308)
			{
				//Write SymbolID
				m_pXmlWriter->writeStartElement("SymbolID");
				m_pXmlWriter->writeAttribute("id", QString::number(nSymbolId,10));
				//    m_pXmlWriter->writeStartElement("LineItem");


				for (int j=0, k=0; j<v3d->size(); k++, j++)
				{
					//Get the information of symbolItem			
					m_pXmlWriter->writeStartElement("Position");
					//Write Position
					QVector3D v3 = v3d->at(j);
					m_pXmlWriter->writeTextElement("x",QString::number(v3.x()));
					m_pXmlWriter->writeTextElement("y",QString::number(v3.y()));
					m_pXmlWriter->writeTextElement("z",QString::number(v3.z()));
					m_pXmlWriter->writeEndElement(); //End of position				
				}
				//	m_pXmlWriter->writeEndElement();//End of LineItem		

				m_pXmlWriter->writeStartElement("Attribute");				
				//		m_pXmlWriter->writeTextElement("Color",ColorString);
				m_pXmlWriter->writeStartElement("Color");
				m_pXmlWriter->writeTextElement("r",QString::number(symbolColor.x()));
				m_pXmlWriter->writeTextElement("g",QString::number(symbolColor.y()));
				m_pXmlWriter->writeTextElement("b",QString::number(symbolColor.z()));
				m_pXmlWriter->writeTextElement("a",QString::number(symbolColor.w()));
				m_pXmlWriter->writeEndElement(); //End of Color	

				m_pXmlWriter->writeTextElement("LineStyle",QString::number(nType)); 
				m_pXmlWriter->writeTextElement("LineWidth",QString::number(width));				
				m_pXmlWriter->writeEndElement(); //End of Attribute	

				m_pXmlWriter->writeEndElement();	//End of SymbolID
			}
		}
		m_pXmlWriter->writeEndElement();	//End of MulitiLineSymbols					
	}	
}
/************************************************************************/
/*线符号的保存
   zhanglei 2011.7.28*/
/************************************************************************/
void SaveSymbolToXML::SaveLineSymbolToXML(SymbolData *symbolData)
{
	
	QVector<CSymbolLine*> *symbol = symbolData->getSymbolLine();

	if (!(symbol->isEmpty()))
	{		
			m_pXmlWriter->writeStartElement("LineSymbols");

			for (int i=0; i<symbol->size(); i++)
			{
				//Get the information of symbolID				
				CSymbolLine *symbolLine = symbol->at(i);
				QVector3DArray *v3d = symbolLine->OriginalArray();
				QVector4D symbolColor = symbolLine->m_LineAttribute.GetColor();

				int nType = symbolLine->m_LineAttribute.GetSymbolType(); //获取线型ID 0,1,2 
				int width = symbolLine->m_LineAttribute.GetLineWidth(); //线宽
				int nSymbolId = symbolLine->GetLineStyle()->getType(); //获取 SymbolID,601
// 				bool isFill = symbolLine->m_LineAttribute.GetFillType();
// 
// 				QString isFillString = (isFill == true) ? "true":"false";
				//color to QString
				//QString ColorString = QString("(")+ QString::number(color.x()) + QString(",")+ QString::number(color.y()) + QString(",") + QString::number(color.z())+ QString(",") + QString::number(color.w())+ QString(")");
				if (nSymbolId != 1308)
				{
					//Write SymbolID
					m_pXmlWriter->writeStartElement("SymbolID");
					m_pXmlWriter->writeAttribute("id", QString::number(nSymbolId,10));
					//    m_pXmlWriter->writeStartElement("LineItem");


					for (int j=0, k=0; j<v3d->size(); k++, j++)
					{
						//Get the information of symbolItem			
						m_pXmlWriter->writeStartElement("Position");
						//Write Position
						QVector3D v3 = v3d->at(j);
						m_pXmlWriter->writeTextElement("x",QString::number(v3.x()));
						m_pXmlWriter->writeTextElement("y",QString::number(v3.y()));
						m_pXmlWriter->writeTextElement("z",QString::number(v3.z()));
						m_pXmlWriter->writeEndElement(); //End of position				
					}
					//	m_pXmlWriter->writeEndElement();//End of LineItem		

					m_pXmlWriter->writeStartElement("Attribute");				
					//		m_pXmlWriter->writeTextElement("Color",ColorString);
					m_pXmlWriter->writeStartElement("Color");
					m_pXmlWriter->writeTextElement("r",QString::number(symbolColor.x()));
					m_pXmlWriter->writeTextElement("g",QString::number(symbolColor.y()));
					m_pXmlWriter->writeTextElement("b",QString::number(symbolColor.z()));
					m_pXmlWriter->writeTextElement("a",QString::number(symbolColor.w()));
					m_pXmlWriter->writeEndElement(); //End of Color	

					m_pXmlWriter->writeTextElement("LineStyle",QString::number(nType)); 
					m_pXmlWriter->writeTextElement("LineWidth",QString::number(width));				
					m_pXmlWriter->writeEndElement(); //End of Attribute	

					m_pXmlWriter->writeEndElement();	//End of SymbolID
				}
							}
			m_pXmlWriter->writeEndElement();	//End of LineSymbols					
	}	
}

/************************************************************************/
/* 通用符号的保存
   zhanglei 2011.7.28*/
/************************************************************************/
void SaveSymbolToXML::SaveNormalSymbolToXML(SymbolData *symbolData)
{
	QVector<NormalSymbol*> *normalSymbol = symbolData->getNormalSymbol();

	if (normalSymbol->size() > 0)
	{
		m_pXmlWriter->writeStartElement("NormalSymbols");

		int type; //SymbolID
		
		for (int i=0; i<normalSymbol->size(); i++)
		{
			NormalSymbol *ns = normalSymbol->at(i);

			Symbol* pSymbol = ns->SetPointStyle();

			if (pSymbol != NULL)
			{
				type = pSymbol->getType();
			}		

			QVector<CSymbolAttribute*> * sa = ns->getIndex();
			QVector3DArray *v3DA = ns->OriginalArray();		

			for (int k=0,j=0; k<v3DA->size(), j<sa->size(); k++,j++)
			{
				QVector3D v3d = v3DA->at(k);
				CSymbolAttribute *CAtype = sa->at(j);
				//type = CAtype->GetSymbolType();
				QVector4D symbolColor = CAtype->GetColor();
				int symbolSize = CAtype->GetValue();	

				m_pXmlWriter->writeStartElement("SymbolID");
				m_pXmlWriter->writeAttribute("id", QString::number(type,10)); //SymbolID
					
				m_pXmlWriter->writeStartElement("Position");
				//Write Position				
				m_pXmlWriter->writeTextElement("x",QString::number(v3d.x()));
				m_pXmlWriter->writeTextElement("y",QString::number(v3d.y()));
				m_pXmlWriter->writeTextElement("z",QString::number(v3d.z()));
				m_pXmlWriter->writeEndElement(); //End of position		
				
			//	QString ColorString = "(" + QString::number(symbolColor.x())+","+ QString::number(symbolColor.y())+","+ QString::number(symbolColor.z())+")";

				m_pXmlWriter->writeStartElement("Attribute");
		//		m_pXmlWriter->writeTextElement("Color",ColorString);
				m_pXmlWriter->writeStartElement("Color");
				m_pXmlWriter->writeTextElement("r",QString::number(symbolColor.x()));
				m_pXmlWriter->writeTextElement("g",QString::number(symbolColor.y()));
				m_pXmlWriter->writeTextElement("b",QString::number(symbolColor.z()));
				m_pXmlWriter->writeTextElement("a",QString::number(symbolColor.w()));
				m_pXmlWriter->writeEndElement(); //End of Color		

				m_pXmlWriter->writeTextElement("Size",QString::number(symbolSize));
				m_pXmlWriter->writeEndElement(); //End of Attribute		

				m_pXmlWriter->writeEndElement();			// End of SymbolID	
			}	
		}	
		m_pXmlWriter->writeEndElement();			// End of NormalSymbols	
	}		
}
/************************************************************************/
/* 等值线的保存
   zhanglei 2011.7.28*/
/************************************************************************/
void SaveSymbolToXML::SaveContourLineToXML(SymbolData *symbolData)
{
	QVector<CSymbolContourLine*> *contourSymbol = symbolData->getSymbolContourLine();
	if (contourSymbol->size() > 0)
	{
		m_pXmlWriter->writeStartElement("ContourLines");		
		
		for (int i=0; i<contourSymbol->size(); i++)
		{
			CSymbolContourLine* symbolContourLine = contourSymbol->at(i);			
			bool b = symbolContourLine->GetClosedLine();
			bool isMark = symbolContourLine->IsMark();
		 //   nType = symbolContourLine->GetAttribute()->GetSymbolType(); //符号类型
			double dValue = symbolContourLine->GetAttribute()->GetValue(); //线的标值
		//	CONTOUR_TYPE ContourType = symbolContourLine->GetContourType();
			int ContourType = symbolContourLine->GetContourType();

			QVector3DArray *v3da = symbolContourLine->OriginalArray();
			QVector4D symbolColor = symbolContourLine->GetAttribute()->GetColor();
			QString typeContour = symbolContourLine->GetAttribute()->GetLineType();//线的类型
			bool bFill = symbolContourLine->GetAttribute()->GetFillType();
			int widthCpntour = symbolContourLine->GetAttribute()->GetLineWidth(); //线宽

	//		QString ColorString = "(" + QString::number(symbolColor.x())+","+ QString::number(symbolColor.y())+","+ QString::number(symbolColor.z())+")";
			QString FillString = (bFill == true) ? QString("true"):QString("false");
			QString IsMarkString = (isMark == true) ? QString("true"):QString("false");			
			
			int nType = (b == true) ? 1101:1102;
			if (ContourType = (int)Layer::ELayerType::Temperature_Layer) nType = 1103;
			if (ContourType = (int)Layer::ELayerType::Rain_Layer) nType = 1104;
			if (ContourType = (int)Layer::ELayerType::Rain_Yubao_Layer) nType = 1105;
		
			 m_pXmlWriter->writeStartElement("SymbolID");
			 m_pXmlWriter->writeAttribute("id", QString::number(nType,10)); //SymbolID
			for (int j=0; j<v3da->size(); j++)
			{
			   //Position
				QVector3D v3d = v3da->at(j);
				m_pXmlWriter->writeStartElement("Position");
				//Write Position				
				m_pXmlWriter->writeTextElement("x",QString::number(v3d.x()));
				m_pXmlWriter->writeTextElement("y",QString::number(v3d.y()));
				m_pXmlWriter->writeTextElement("z",QString::number(v3d.z()));
				m_pXmlWriter->writeEndElement(); //End of position					
			}
			m_pXmlWriter->writeStartElement("Attribute");

			m_pXmlWriter->writeStartElement("Color");
			m_pXmlWriter->writeTextElement("r",QString::number(symbolColor.x()));
			m_pXmlWriter->writeTextElement("g",QString::number(symbolColor.y()));
			m_pXmlWriter->writeTextElement("b",QString::number(symbolColor.z()));
			m_pXmlWriter->writeTextElement("a",QString::number(symbolColor.w()));
			m_pXmlWriter->writeEndElement(); //End of Color	

			m_pXmlWriter->writeStartElement("FillColor");
			m_pXmlWriter->writeTextElement("r",QString::number(symbolColor.x()));
			m_pXmlWriter->writeTextElement("g",QString::number(symbolColor.y()));
			m_pXmlWriter->writeTextElement("b",QString::number(symbolColor.z()));
			m_pXmlWriter->writeTextElement("a",QString::number(symbolColor.w()));
			m_pXmlWriter->writeEndElement(); //End of FillColor	

			m_pXmlWriter->writeTextElement("LineStyle",typeContour);
			m_pXmlWriter->writeTextElement("LineWidth",QString::number(widthCpntour));
			m_pXmlWriter->writeTextElement("Fill",FillString);
			m_pXmlWriter->writeTextElement("Mark",IsMarkString);	//是否标值
			m_pXmlWriter->writeTextElement("Value",QString::number(dValue));		
			m_pXmlWriter->writeTextElement("ContourType",QString::number(ContourType));

			m_pXmlWriter->writeEndElement(); //End of Attribute	

			m_pXmlWriter->writeEndElement(); //End of SymbolID		
		}	
		m_pXmlWriter->writeEndElement();	// End of ContourLines	
	}
}

void SaveSymbolToXML::SaveRegionSymbolToXML(SymbolData *symbolData)
{
	
	QVector<CSymbolLine*> *symbol = symbolData->getSymbolLine();
	bool isHasRegionSymbol = false;

	if (symbol->size() > 0)
	{		
			for (int i=0; i<symbol->size(); i++)
			{
				int nSymbolId = symbol->at(i)->GetLineStyle()->getType(); //获取 SymbolID,601	
				if (nSymbolId == 1308)
				{					
					isHasRegionSymbol = true;
					break;
				}
			}
			if (isHasRegionSymbol)
			{
				m_pXmlWriter->writeStartElement("RegionSymbols");
				for (int i=0; i<symbol->size(); i++)
				{
					//Get the information of symbolID				
					CSymbolLine *symbolLine = symbol->at(i);

					QVector3DArray *v3d = symbolLine->OriginalArray();
					QVector4D symbolColor = symbolLine->m_LineAttribute.GetColor();
					int nType = symbolLine->m_LineAttribute.GetSymbolType(); //获取线型ID 0,1,2 
					//	int width = symbolLine->m_LineAttribute.GetLineWidth(); //线宽
					int nSymbolId = symbolLine->GetLineStyle()->getType(); //获取 SymbolID,601				
					QString isFillString = (symbolLine->m_LineAttribute.GetFillType() == true)? "true":"false";
					QString StringValue = symbolLine->m_LineAttribute.GetStringValue(); 			

					if (nSymbolId == 1308)
					{
						//Write SymbolID
						m_pXmlWriter->writeStartElement("SymbolID");
						m_pXmlWriter->writeAttribute("id", QString::number(nSymbolId,10));
						//    m_pXmlWriter->writeStartElement("LineItem");

						for (int j=0, k=0; j<v3d->size(); k++, j++)
						{
							//Get the information of symbolItem			
							m_pXmlWriter->writeStartElement("Position");
							//Write Position
							QVector3D v3 = v3d->at(j);
							m_pXmlWriter->writeTextElement("x",QString::number(v3.x()));
							m_pXmlWriter->writeTextElement("y",QString::number(v3.y()));
							m_pXmlWriter->writeTextElement("z",QString::number(v3.z()));
							m_pXmlWriter->writeEndElement(); //End of position				
						}
						//	m_pXmlWriter->writeEndElement();//End of LineItem		

						m_pXmlWriter->writeStartElement("Attribute");				
						//		m_pXmlWriter->writeTextElement("Color",ColorString);				

						m_pXmlWriter->writeTextElement("LineStyle",QString::number(nType)); 
						m_pXmlWriter->writeTextElement("Fill",isFillString); 

						m_pXmlWriter->writeStartElement("Color");
						m_pXmlWriter->writeTextElement("r",QString::number(symbolColor.x()));
						m_pXmlWriter->writeTextElement("g",QString::number(symbolColor.y()));
						m_pXmlWriter->writeTextElement("b",QString::number(symbolColor.z()));
						m_pXmlWriter->writeTextElement("a",QString::number(symbolColor.w()));
						m_pXmlWriter->writeEndElement(); //End of Color	

						m_pXmlWriter->writeTextElement("Value",StringValue); 	

						m_pXmlWriter->writeEndElement(); //End of Attribute	
						m_pXmlWriter->writeEndElement();	//End of SymbolID						
					}				
				}
				m_pXmlWriter->writeEndElement();	//End of RigionSymbols		
			}				
	}		
}

void SaveSymbolToXML::SaveMarkSymbolToXML(SymbolData *symbolData)
{
	//QVector<QTextureFont*> *symbol = symbolData->getMarkSymbol();

	//if (symbol->size() > 0)
	//{		
	//	m_pXmlWriter->writeStartElement("MarkSymbols");

	//	for (int i=0; i<symbol->size(); i++)
	//	{
	//		QTextureFont* symbolFont = symbol->at(i);
	//		int nSymbolId = symbolFont->GetStyle(); //获取 SymbolID,601	

	//		m_pXmlWriter->writeStartElement("SymbolID");
	//		m_pXmlWriter->writeAttribute("id", QString::number(nSymbolId,10));			

	//		m_pXmlWriter->writeStartElement("Position");
	//		
	//		//QVector3D pos = symbolFont->GetPosition();
	//		QVector3DArray *pOriginalArray = symbolFont->OriginalArray();
	//		QVector3D pos = pOriginalArray->at(0);
	//		//Write Position			
	//		m_pXmlWriter->writeTextElement("x",QString::number(pos.x()));
	//		m_pXmlWriter->writeTextElement("y",QString::number(pos.y()));
	//		m_pXmlWriter->writeTextElement("z",QString::number(pos.z()));
	//		m_pXmlWriter->writeEndElement(); //End of position	

	//		m_pXmlWriter->writeStartElement("Attribute");				
	//		//		m_pXmlWriter->writeTextElement("Color",ColorString);				
	//		QString fontname = symbolFont->GetFontName();		

	//		m_pXmlWriter->writeTextElement("Font",fontname); 
	//		m_pXmlWriter->writeStartElement("Color");
	//		m_pXmlWriter->writeTextElement("r",QString::number(symbolFont->GetColor().x()));
	//		m_pXmlWriter->writeTextElement("g",QString::number(symbolFont->GetColor().y()));
	//		m_pXmlWriter->writeTextElement("b",QString::number(symbolFont->GetColor().z()));
	//		m_pXmlWriter->writeTextElement("a",QString::number(symbolFont->GetColor().w()));
	//		m_pXmlWriter->writeEndElement(); //End of Color	

	//		m_pXmlWriter->writeTextElement("Size",QString::number(symbolFont->PixelWidth())); 
	//		m_pXmlWriter->writeTextElement("Angle",QString::number(symbolFont->GetAngle())); 
	//		m_pXmlWriter->writeEndElement(); //End of Attribute	

	//		m_pXmlWriter->writeTextElement("Value",symbolFont->StringArray());
	//		m_pXmlWriter->writeEndElement();	//End of SymbolID		

	//	}
	//	m_pXmlWriter->writeEndElement();	//End of MarkSymbols				
	//}		
}

