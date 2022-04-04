/*============================================================ 
文件名：ReadSymbolFromM14File.cpp 
类 名： CReadSymbolFromM14File
父 类： QWidget
子 类： 无 
功能说明：从Micaps14类数据文件中读取编辑符号
调用说明：

----------------------------版本更新----------------------------
V 1.0 
原作者 ：renxue
完成日期：2011年08月01日
============================================================*/
#include <QDebug>
#include <QMessageBox>
#include "ReadSymbolFromM14File.h"
#include "ManualDraw.h"

CReadSymbolFromM14File::CReadSymbolFromM14File( QTreeWidget *tree, CSymbolPluginInterface *symbolPlugin, SymbolLayer *sl)
	:treeWidget(tree), m_pSymbolPlugin(symbolPlugin), m_pSymbolLayer(sl),m_pSymbol(NULL)
{	
	m_pSymbolAttri = new CSymbolAttribute;
	m_pSymbolData  = new SymbolData;
	m_pSymPoint = NULL;
	m_pSymLine = NULL;
	m_pSymNormal = NULL;
	m_pSymContour = NULL;
	//设置七种颜色：青、橙、黄、绿、蓝、紫、红
	m_ColorTable4D<<QVector4D(0, 1.0f, 1.0f, 1.0f)<<QVector4D(1.0f, 0.6f, 0, 1.0f)<<QVector4D(1.0f, 1.0f, 0, 1.0f);
	m_ColorTable4D<<QVector4D(0, 1.0f, 0.5f, 1.0f)<<QVector4D(0, 0, 1.0f, 1.0f)<<QVector4D(0.9f, 0.5f, 0.9f, 1.0f)<<QVector4D(1.0f, 0, 0, 1.0f);
}

CReadSymbolFromM14File::~CReadSymbolFromM14File(void)
{
}

/************************************************************************
 功能说明：读十四类数据文件
 参数说明：fileName是包含路径和文件名的字符串
           返回值为bool型，正确执行返回true，错误或异常返回false
 作    者：renxue
 完成日期：2011年11月21日
************************************************************************/
bool CReadSymbolFromM14File::ReadFile( const QString &fileName )
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) 	return false;
	
	m_txtStream.setDevice(&file);
	QString strLine = m_txtStream.readLine();
	if(strLine.isNull() || !strLine.contains("diamond 14")) return false;

	while(!m_txtStream.atEnd())
	{
		strLine = m_txtStream.readLine();//读时间信息，便于后续操作
		while(strLine.isEmpty() && !m_txtStream.atEnd())
		{
			strLine = m_txtStream.readLine();
		}

		if(strLine.indexOf("LINES_SYMBOL") == 0)        //读LINES_SYMBOL数据块		
		{
			ReadLinesSymbol(strLine);
		}
		else if(strLine.indexOf("LINES") == 0)          //读LINES数据块
		{
			ReadLines(strLine);			
		}
		else if(strLine.indexOf("SYMBOLS") == 0)        //读SYMBOLS数据块
		{
			ReadSymbol(strLine);
		}
		else if(strLine.indexOf("CLOSED_CONTOURS") == 0)//读CLOSED_CONTOURS数据块
		{
			ReadContour(strLine);
		}
		else if(strLine.indexOf("STATION_SITUATION") == 0)//读STATION_SITUATION数据块
		{
			ReadStationSituation(strLine);//没有实现，原因：吴焕平不需要实现
		}
		else if(strLine.indexOf("WEATHER_REGION") == 0)//读WEATHER_REGION数据块
		{
			ReadWeatherRegion(strLine);
		}
		else if(strLine.indexOf("FILLAREA") == 0)      //读FILLAREA数据块
		{
			ReadFillArea(strLine);
		}
		else if(strLine.indexOf("NOTES_SYMBOL") == 0)  //读NOTES_SYMBOL数据块
		{
			ReadNotesSymbol(strLine);
		}
	}
	file.close();
	
	if(file.error() != QFile::NoError)
	{
		return false;
	}
	return true;
}

/************************************************************************
 功能说明：读十四类数据文件中的LINES数据块,对应CIPAS的非闭合等值线
 参数说明：strLine是有标识符“LINES”的数据行
           返回值为bool型，正确执行返回true，错误或异常返回false
 作    者：renxue
 完成日期：2011年11月21日
************************************************************************/
bool CReadSymbolFromM14File::ReadLines(QString strLineHead)
{
	strLineHead.remove("LINES: ");
	int nLineNum = strLineHead.toInt();
	if(0 == nLineNum) return false;

	QString strLine;
	QStringList strList;
	int nLineWidth, nPointNum;
	int nValue, nValueNum;
	while(nLineNum>0 && !m_txtStream.atEnd())
	{
		strLine = m_txtStream.readLine();
		while(strLine.isEmpty() && !m_txtStream.atEnd())
		{
			strLine = m_txtStream.readLine();
		}
		
		//读每条线的控制信息：编号、线宽、点的个数
		strList = strLine.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		if(strList.length() == 2)
		{
			nLineWidth = strList.at(0).toInt();
			nPointNum = strList.at(1).toInt();
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText("File: Wrong Data in LINES!");
			msgBox.exec();
			return false;
		}

		//读经纬坐标点
		m_PointArray.Clear();
		if(nPointNum>0) ReadPositionArray(m_PointArray, nPointNum);
		
		//读控制信息：标号、个数
		QString  strValue;
		strLine = m_txtStream.readLine();
		while(strLine.isEmpty() && !m_txtStream.atEnd())
		{
			strLine = m_txtStream.readLine();
		}

		if(strLine.contains("NoLabel 0"))//无标值
		{
			CreatContourLine(nLineWidth, 0, nPointNum, m_PointArray, m_PointArray, false);
		}
		else //有标值
		{
			nPointNum = 0;
			strList = strLine.split(QRegExp("\\s+"), QString::SkipEmptyParts);
			if(strList.length() == 2)
			{
				strValue = strList.at(0);
				nPointNum = strList.at(1).toInt();
			}
			else
			{
				QMessageBox msgBox;
				msgBox.setText("File: Wrong Data in LINES!");
				msgBox.exec();
				return false;
			}

			//读标号的坐标位置
			QVector3DArray tArray;
			ReadPositionArray(tArray, nPointNum);

			//创建非闭合等值线符号
			CreatContourLine(nLineWidth, strValue, nPointNum, m_PointArray, tArray, false);
		}
		nLineNum--;
	}

	return true;
}

/************************************************************************
功能说明：读十四类数据文件中的LINES_SYMBOL数据块
参数说明：strLine是有标识符“LINES_SYMBOL”的数据行
          返回值为bool型，正确执行返回true，错误或异常返回false
 作    者：renxue
 完成日期：2011年11月21日
************************************************************************/
bool CReadSymbolFromM14File::ReadLinesSymbol(QString strLineHead)
{
	strLineHead.remove("LINES_SYMBOL: ");
	int nLineNum = strLineHead.toInt();
	if(0 == nLineNum) return false;

	int nType, nLineWidth, nPointNum;
	int nValue, nValueNum;
	QString strLine;
	QStringList strList;
	while(nLineNum>0)
	{
		strLine = m_txtStream.readLine();
		while(strLine.isEmpty() && !m_txtStream.atEnd())
		{
			strLine = m_txtStream.readLine();
		}
		
		//读每条线的控制信息：编号、线宽、点的个数
		strList = strLine.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		if(strList.length() == 3)
		{
			nType = strList.at(0).toInt();
			nLineWidth = strList.at(1).toInt();
			nPointNum = strList.at(2).toInt();
		}//处理完一行数据
		else
		{
			QMessageBox msgBox;
			msgBox.setText("File: Wrong Data in  LINES_SYMBOL!");
			msgBox.exec();
			return false;
		}
		
		//读经纬坐标点	
		m_PointArray.Clear();                   
		ReadPositionArray(m_PointArray, nPointNum);	
		
		strLine = m_txtStream.readLine();       //处理带有“NoLabel 0”的一行
		while((strLine.isEmpty() && !m_txtStream.atEnd()) || !strLine.contains("NoLabel 0"))
		{
			strLine = m_txtStream.readLine();
		}

		nLineNum--;

		CreatLinesSymbol(nType, nLineWidth, m_PointArray);//创建相应的线符号
	}

	return true;
}


/************************************************************************
功能说明：读十四类数据文件中的SYMBOLS数据块
参数说明：strLine是有标识符“SYMBOLS”的数据行
          返回值为bool型，正确执行返回true，错误或异常返回false
 作    者：renxue
 完成日期：2011年11月21日
************************************************************************/
bool CReadSymbolFromM14File::ReadSymbol(QString strLineHead)
{
	strLineHead.remove("SYMBOLS: ");
	int nLineNum = strLineHead.toInt();
	if(0 == nLineNum) return false;
	
	int  nType;
	QString   strLine, strAngle;
	QStringList strList;
	QVector3D vetPosition;
	while(nLineNum>0 && !m_txtStream.atEnd())
	{
		strLine = m_txtStream.readLine();
		while(strLine.isEmpty() && !m_txtStream.atEnd())
		{
			strLine = m_txtStream.readLine();
		}
		
		//读每个符号的信息：编号、坐标、风向角度
		strList = strLine.split(QRegExp("\\s+"), QString::SkipEmptyParts);		
		if(strList.length() == 5)
		{
			nType = strList.at(0).toInt();
			vetPosition.setX(strList.at(1).toDouble());
			vetPosition.setY(strList.at(2).toDouble());
			vetPosition.setZ(strList.at(3).toDouble());
			strAngle = strList.at(4);
		}//读完一行数据
		else
		{
			QMessageBox msgBox;
			msgBox.setText("File：Wrong Datas in SYMBOLS!");
			msgBox.exec();
		}
		nLineNum--;

		CreatSymbols(nType, vetPosition, strAngle);    //根据读取的信息创建相应的符号
	}

	if(nLineNum != 0 && m_txtStream.atEnd()) return false;
	return true;
}

/************************************************************************
 功能说明：读十四类数据文件中的CLOSED_CONTOURS数据块
 参数说明：strLine是有标识符“CLOSED_CONTOURS”的数据行
           返回值为bool型，正确执行返回true，错误或异常返回false
 作    者：renxue
 完成日期：2011年11月21日
************************************************************************/
bool CReadSymbolFromM14File::ReadContour(QString strLineHead)
{
	strLineHead.remove("CLOSED_CONTOURS: ");
	int nLineNum = strLineHead.toInt();
	if(0 == nLineNum) return false;
	m_ClosedContourArray.clear();

	int nType, nLineWidth, nPointNum;		
	QString strLine;
	QStringList strList;
	
	while(nLineNum>0 && !m_txtStream.atEnd())
	{		
		//读每条线的控制信息：线宽、点的个数		
		strLine = m_txtStream.readLine();
		while(strLine.isEmpty() && !m_txtStream.atEnd())    //遇到空行，重复读
		{
			strLine = m_txtStream.readLine();
		}
		strList = strLine.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		
		if(strList.length()==2)
		{
			nLineWidth = strList.at(0).toInt();
			nPointNum = strList.at(1).toInt();
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText("File: Wrong Data in CLOSED_CONTOURS!");
			msgBox.exec();
			return false;
		}

		//读经纬坐标点
		//m_PointArray.Clear();		
		//if(nPointNum>0) ReadPositionArray(m_PointArray, nPointNum);
		QVector3DArray  PointsArray;
		if(nPointNum>0) ReadPositionArray(PointsArray, nPointNum);
		m_ClosedContourArray.push_back(PointsArray);
		
		//读控制信息：标号、个数
		QString strValue;	
		strLine = m_txtStream.readLine();
		while(strLine.isEmpty() && !m_txtStream.atEnd())    //遇到空行，重复读
		{
			strLine = m_txtStream.readLine();
		}
		if(strLine.contains("NoLabel 0"))
		{
			//根据读取的信息创建等值线
			//CreatContourLine(nLineWidth, 0, nPointNum, m_PointArray, m_PointArray, true);
			strValue = "0";
		}
		else
		{
			strList = strLine.split(QRegExp("\\s+"), QString::SkipEmptyParts);
			if(strList.length() == 2)
			{
				strValue = strList.at(0);
				nPointNum = strList.at(1).toInt();	
			}
			else
			{
				QMessageBox msgBox;
				msgBox.setText("File: Wrong Data in  CLOSED_CONTOURS!");
				msgBox.exec();
				return false;
			}
			//读标号的坐标位置
			QVector3DArray valuePointArray;
			ReadPositionArray(valuePointArray, nPointNum);

			//根据读取的信息创建等值线
			//CreatContourLine(nLineWidth, strValue, nPointNum, m_PointArray, valuePointArray, true);
		}
		CSymbolAttribute *pContourAttri = new CSymbolAttribute();
		pContourAttri->SetLineWidth(nLineWidth);
		pContourAttri->SetFillType(true);	
		pContourAttri->SetLineType(QString(tr("实线")));
		pContourAttri->SetValue(strValue.toFloat());
		m_ClosedContourAttributs.push_back(pContourAttri);
		nLineNum--;
	}
	CreatClosedContourArea(m_ClosedContourArray, m_ClosedContourAttributs);
	return true;
}


/************************************************************************
功能说明：读十四类数据文件中的STATION_SITUATION数据块
参数说明：strLine是有标识符“STATION_SITUATION”的数据行
          返回值为bool型，正确执行返回true，错误或异常返回false
作    者：renxue
完成日期：2011年08月02日
************************************************************************/
bool CReadSymbolFromM14File::ReadStationSituation(QString strLineHead)
{
	return false;
}


/************************************************************************
 功能说明：读十四类数据文件中的WEATHER_REGION数据块
 参数说明：strLine是有标识符“WEATHER_REGION”的数据行
           返回值为bool型，正确执行返回true，错误或异常返回false
 作    者：renxue
 完成日期：2011年11月21日
 ///////////////////////meiyou验证////////////////////////////////////////
************************************************************************/
bool CReadSymbolFromM14File::ReadWeatherRegion(QString strLineHead)
{
	strLineHead.remove("WEATHER_REGION: ");
	int nLineNum = strLineHead.toInt();
	if(0 == nLineNum) return false;

	int  nCode, nPointNum;
	QString strLine;
	QStringList strList;
	while(nLineNum>0 && !m_txtStream.atEnd())
	{
		strLine = m_txtStream.readLine();
		while(strLine.isEmpty() && !m_txtStream.atEnd())    //遇到空行，重复读
		{
			strLine = m_txtStream.readLine();
		}

		//读每条线的控制信息：编号、点的个数
		strList = strLine.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		if(strList.length() == 2)
		{
			nCode = strList.at(0).toInt();
			nPointNum = strList.at(1).toInt();
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText("File: Wrong Data in WEATHER_REGION!");
			msgBox.exec();
			return false;
		}
	
		m_PointArray.Clear();                   //读经纬坐标点	
		ReadPositionArray(m_PointArray, nPointNum);	

		nLineNum--;
		QVector4D color(0, 0, 0, 1);
		CreatPolygon(nCode, m_PointArray, color, true);
	}

	return true;
}

/************************************************************************
功能说明：读十四类数据文件中的FILLAREA数据块
参数说明：strLine是有标识符“FILLAREA”的数据行
          返回值为bool型，正确执行返回true，错误或异常返回false
作    者：renxue
完成日期：2011年11月21日
************************************************************************/
bool CReadSymbolFromM14File::ReadFillArea(QString strLineHead)
{
	strLineHead.remove("FILLAREA: ");
	int nLineNum = strLineHead.toInt();
	if(0 == nLineNum) return false;

	int nPointNum, nType;
	QString strLine;
	QStringList strList;
	while(nLineNum>0 && !m_txtStream.atEnd())
	{
		strLine = m_txtStream.readLine();
		while(strLine.isEmpty() && !m_txtStream.atEnd())    //遇到空行，重复读
		{
			strLine = m_txtStream.readLine();
		}

		//读每个区域的控制信息：编号、点的个数
		strList = strLine.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		if(strList.length() == 2)
		{
			nType = strList.at(0).toInt();
			nPointNum = strList.at(1).toInt();
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText("File: Wrong Data in FILLAREA!");
			msgBox.exec();
			return false;
		}

		//读经纬坐标点
		m_PointArray.Clear();
		if(nPointNum>0) ReadPositionArray(m_PointArray, nPointNum);

		//读控制信息：填充类型、线色、前景色、背景色、渐变色角度、图案代码、是否画边框
		QString  strLineType, strColorAngle, strImageCode, strBorder;
		QVector4D  LineColor, ForgroundColor, BackgroundColor;
		strLine = m_txtStream.readLine();
		while(strLine.isEmpty() && !m_txtStream.atEnd())    //遇到空行，重复读
		{
			strLine = m_txtStream.readLine();
		}
		strList = strLine.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		if(strList.length() == 16)
		{
			strLineType = strList.at(0);				
			LineColor.setX(strList.at(1).toInt());				
			LineColor.setY(strList.at(2).toInt());					
			LineColor.setZ(strList.at(3).toInt());					
			LineColor.setW(strList.at(4).toInt());

			ForgroundColor.setX(strList.at(5).toInt());				
			ForgroundColor.setY(strList.at(6).toInt());				
			ForgroundColor.setZ(strList.at(7).toInt());			
			ForgroundColor.setW(strList.at(8).toInt());

			BackgroundColor.setX(strList.at(9).toInt());				
			BackgroundColor.setY(strList.at(10).toInt());				
			BackgroundColor.setZ(strList.at(11).toInt());				
			BackgroundColor.setW(strList.at(12).toInt());

			strColorAngle = strList.at(13);				
			strImageCode = strList.at(14);				
			strBorder = strList.at(15);			
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText("File: Wrong Data in FILLAREA!");
			msgBox.exec();
			return false;
		}

		//创建多边形符号
		if(BackgroundColor == ForgroundColor)//用槽线模拟
		{
			CreatLinesSymbol(0, 1,m_PointArray);
		}
		else//用多边形模拟
		{
			CreatPolygon(nType, m_PointArray, ForgroundColor, true);
		}

		nLineNum--;
	}

	return true;
}

/************************************************************************
功能说明：读十四类数据文件中的NOTES_SYMBOL数据块
参数说明：strLine是有标识符“NOTES_SYMBOL”的数据行
          返回值为bool型，正确执行返回true，错误或异常返回false
作    者：renxue
完成日期：2011年11月21日
************************************************************************/
bool CReadSymbolFromM14File::ReadNotesSymbol(QString strLineHead)
{
	strLineHead.remove("NOTES_SYMBOL: ");
	int nLineNum = strLineHead.toInt();
	if(0 == nLineNum) return false;

	int  nCode;
	int  nCharNum, nStringAngle, nStrLength, nFontSize, nCharType; 
	QString   strContext, strFont, strLine;
	QStringList strList;
	QVector3D vetPosition;
	QVector4D color;
	while(nLineNum>0 && !m_txtStream.atEnd())
	{
		strLine = m_txtStream.readLine();
		while(strLine.isEmpty() && !m_txtStream.atEnd())    //遇到空行，重复读
		{
			strLine = m_txtStream.readLine();
		}

		//读每个符号的控制信息：编号、坐标、风向角度
		strList = strLine.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		if(strList.length() == 15)
		{
			nCode = strList.at(0).toInt();			
			vetPosition.setX(strList.at(1).toDouble());				
			vetPosition.setY(strList.at(2).toDouble());				
			vetPosition.setZ(strList.at(3).toDouble());			
			nCharNum = strList.at(4).toInt();				
			strContext = strList.at(5);				
			nStringAngle = strList.at(6).toInt();				
			nStrLength = strList.at(7).toInt();				
			strFont = strList.at(8);			
			nFontSize = strList.at(9).toInt();				
			nCharType = strList.at(10).toInt();				
			color.setX(strList.at(11).toInt());				
			color.setY(strList.at(12).toInt());					
			color.setZ(strList.at(13).toInt());				
			color.setW(strList.at(14).toInt());				
		}//读完一行数据
		else
		{
			QMessageBox msgBox;
			msgBox.setText("File：Wrong Datas in NOTES_SYMBOL!");
			msgBox.exec();
		}
		nLineNum--;

		//CreatSymbols(nCode, vetPosition, strAngle);    //根据读取的信息创建相应的符号
		if(nCode == 48) nCode = 1401;
		//QTextureFont *pText = new QTextureFont;
		//m_pSymbol = m_pSymbolPlugin->GetSymbol(nCode);
		//if (NULL == m_pSymbol)	return false;
		//pText->SetStyle(m_pSymbol->getType()); //设置文字符号类型
		//
		//
		//pText->SetColor(color);
		//pText->SetPosition(vetPosition); 
		//pText->initFont(strFont,strContext,nFontSize,nFontSize,nStringAngle); 

		//QVector3DArray *pOrignalArray = pText->OriginalArray();
		//pOrignalArray->push_back(vetPosition);

		//m_pSymbolLayer->GetLayerData()->InsertRenderable(pText); 

		//if (m_pSymbolData)
		//{
		//	m_pSymbolData = m_pSymbolLayer->getSymbolData();
		//	m_pSymbolData->getMarkSymbol()->push_back(pText); 
		//}
	}

	if(nLineNum != 0 && m_txtStream.atEnd()) return false;
	return true;
}


///////////////////////////////////////
/************************************************************************
功能说明：读十四类数据文件中的三维坐标位置数据块
参数说明：nPointNum是坐标的个数
          Array用于存储三维坐标；
          返回值为bool型，正确执行返回true，错误或异常返回false
 作    者：renxue
 完成日期：2011年11月21日
************************************************************************/
bool CReadSymbolFromM14File::ReadPositionArray(QVector3DArray &Array, int nPointNum)
{
	if(nPointNum < 1) return false;

	QString strLine;
	QStringList strList;
	int i = 0 ;
	while (i<nPointNum && !m_txtStream.atEnd())
	{
		strLine = m_txtStream.readLine();
		while(strLine.isEmpty() && !m_txtStream.atEnd())    //遇到空行，重复读
		{
			strLine = m_txtStream.readLine();
		}
		strList = strLine.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	
		if(strList.length()%3 == 0)
		{
			int j = 0;
			while(j<strList.length())
			{	
				QVector3D verPoint;
				verPoint.setX(strList.at(j).toDouble());
				verPoint.setY(strList.at(j+1).toDouble());
				verPoint.setZ(strList.at(j+2).toDouble());				
				j += 3;
				Array.push_back(verPoint);
				i++;
			}//处理完一行的数据
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText("File: Wrong Data in ReadPositionArray()!");
			msgBox.exec();
			return false;
		}
	}//处理完nPointNum行的数据

	return true;
}

/************************************************************************
功能说明：根据十四类数据文件中的编号创建cipas的编辑线符号
参数说明：nType是十四类数据的编号；
          nWidth是线的宽度；
 作    者：renxue
 完成日期：2011年08月01日
************************************************************************/
void CReadSymbolFromM14File::CreatLinesSymbol(int nType, int nWidth, QVector3DArray &PointArray)
{
	if(nType< 0) return;

	int nLineType = -1;
	//根据读取的信息创建相应的符号
	switch(nType)
	{
		case 0://槽线
			nLineType = 1201;
			break;
		case 2://冷锋		
			nLineType = 601;
			break;
		case 3://暖锋
			nLineType = 602;
			break;
		case 4://静止锋
			nLineType = 604;
			break;
		case 5://锢囚锋
			nLineType = 603;
			break;	
		case 1113://单线箭头
			nLineType = 1001;
			break;
		case 1110://双线箭头
			nLineType = 1002;
			break;	
		case 1114://双线填充箭头
			nLineType = 1003;
			break;
		case 1112://双线双箭头
			nLineType = 1004;
			break;
		case 1115://双线填充双箭头
			nLineType = 1005;
			break;
		case 1111://双线中间箭头
			nLineType = 1008;
			break;
		case 38://霜冻线
			nLineType = 1202;
			break;	
		case 1102://双实线
			nLineType = 1203;
			break;
		default:
			break;
	}
	m_pSymLine = new CSymbolLine;		
	m_pSymbol = m_pSymbolPlugin->GetSymbol(nLineType);
	if (NULL == m_pSymbol)	return;
	m_pSymLine->SetSymbolLineStyle(m_pSymbol);					

	//设置属性和顶点数组
	m_pSymLine->m_LineAttribute.SetLineType(QString(nType));
	m_pSymLine->m_LineAttribute.SetLineWidth(nWidth);
	if(nLineType == 1201)
	{
		m_pSymLine->m_LineAttribute.SetColor(QVector4D(0,0,0,1));
	}

	QVector3DArray *pArray = m_pSymLine->GetArray();
	QVector3DArray *pOriginalArray = m_pSymLine->OriginalArray();
	int i=0;
	for(i=0; i<PointArray.size(); i++)
	{
		pArray->push_back(PointArray.at(i));
		pOriginalArray->push_back(PointArray.at(i));
	}

	m_pSymbolLayer->GetLayerData()->InsertRenderable(m_pSymLine); 

	if (m_pSymbolData)
	{
		m_pSymbolData = m_pSymbolLayer->getSymbolData();
		m_pSymbolData->getSymbolLine()->push_back(m_pSymLine);
	}
}

/************************************************************************
功能说明：根据十四类数据文件中的编号创建cipas的点符号
参数说明：nType是十四类数据的编号；
          vetPosition是点的三维坐标；
          str可以是风向、字符、零值，根据nType确定内容意义
作    者：renxue
完成日期：2011年08月02日
************************************************************************/
void CReadSymbolFromM14File::CreatSymbols(int nType, QVector3D &vetPosition, QString str)
{
	if(nType < 0) return;

	int nPointType = 0;

	//通用符号
	if(nType>149 && nType<157)
	{
		switch(nType)
		{
		case 152://星号
			nPointType = 1301;
			break;
		case 151://空心圆
			nPointType = 1302;
			break;
		case 153://空三角
			nPointType = 1303;
			break;
		case 155://空正方形
			nPointType = 1304;
			break;
		case 150://实心圆
			nPointType = 1305;
			break;
		case 154://实三角
			nPointType = 1306;
			break;
		case 156://实方形
			nPointType = 1307;
			break;
		default:
			break;
		}
		m_pSymNormal = new NormalSymbol;		
		m_pSymbol = m_pSymbolPlugin->GetSymbol(nPointType);
		if (NULL == m_pSymbol) return;

		m_pSymbolAttri = new CSymbolAttribute;
		//m_pSymbolAttri->SetSymbolType(0);
		m_pSymNormal->SetPointStyle(m_pSymbol);	
		m_pSymNormal->SetPosition(vetPosition, m_pSymbolAttri);
		QVector3DArray *pArray = m_pSymNormal->OriginalArray();
		pArray->push_back(vetPosition);
		
	
		m_pSymbolLayer->GetLayerData()->InsertRenderable(m_pSymNormal);
		if (m_pSymbolData)
		{
			m_pSymbolData = m_pSymbolLayer->getSymbolData();
			m_pSymbolData->getNormalSymbol()->push_back(m_pSymNormal);
		}

		return;
	}

	//点符号
	switch(nType)
	{
		case 43://无风
			nPointType = 101;
			break;
		case 39://2-3级风
			nPointType = 103;
			break;
		case 40://3-4级风
			nPointType = 104;
			break;
		case 36://4-5级风
			nPointType = 105;
			break;
		case 33://5-6级风
			nPointType = 106;
			break;
		case 41://6-7级风
			nPointType = 107;
			break;
		case 34://7-8级风
			nPointType = 108;
			break;
		case 35://8-9级风
			nPointType = 109;
			break;
		case 101://9-10级风
			nPointType = 110;
			break;
		case 102://10-11级风
			nPointType = 111;
			break;
		case 103://11-12级风
			nPointType = 112;
			break;
		case 26://小雨
			nPointType = 201;
			break;
		case 47://中雨
			nPointType = 202;
			break;
		case 55://大雨
			nPointType = 203;
			break;
		case 64://暴雨
			nPointType = 204;
			break;
		case 221://大暴雨
			nPointType = 205;
			break;
		case 25://阵雨
			nPointType = 207;
			break;
		case 27://轻冻雨
			nPointType = 212;
			break;
		case 28://冻雨
			nPointType = 213;
			break;
		case 23://小雪
			nPointType = 301;
			break;
		case 22://中雪
			nPointType = 302;
			break;
		case 21://大雪
			nPointType = 303;
			break;
		case 66://暴雪
			nPointType = 304;
			break;
		case 65://阵雪
			nPointType = 305;
			break;
		case 24://雨夹雪
			nPointType = 306;
			break;
		case 231://特大暴雪
			nPointType = 307;
			break;
		case 201://浮尘
			nPointType = 401;
			break;
		case 45://扬沙
			nPointType = 402;
			break;
		case 46://轻沙暴
			nPointType = 403;
			break;
		case 211://沙暴
			nPointType = 404;
			break;
		case 50://晴天
			nPointType = 501;
			break;
		case 51://多云
			nPointType = 502;
			break;
		case 52://阴天
			nPointType = 503;
			break;
		case 29://轻雾
			nPointType = 504;
			break;
		case 30://雾
			nPointType = 505;
			break;
		case 32://雷暴
			nPointType = 506;
			break;
		case 31://冰雹
			nPointType = 507;
			break;
		case 44://霜冻
			nPointType = 508;
			break;
		case 42://旋转风
			nPointType = 509;
			break;
		case 37://台风
			nPointType = 510;
			break;
		case 48://文字标注,单独处理
			return;
		case 60://高值中心
			nPointType = 1402;
			break;
		case 61://低值中心
			nPointType = 1403;
			break;
		case 63://冷中心
			nPointType = 1404;
			break;
		case 62://暖中心
			nPointType = 1405;
			break;
		default:
			break;
	}

	m_pSymPoint = new SymbolPoint;
	m_pSymbol = m_pSymbolPlugin->GetSymbol(nPointType);
	if (NULL == m_pSymbol)	return;
	m_pSymPoint->SetPointStyle(m_pSymbol);
	QVector3DArray * pArray = m_pSymPoint->OriginalArray();
	pArray->push_back(vetPosition);
	m_pSymPoint->SetPosition(vetPosition);

	//设置风向角度
	if((nPointType>100 && nPointType<113))
	{
		m_pSymPoint->SetAngle(str.toDouble());
	}
	else m_pSymPoint->SetAngle(0.0f);

	m_pSymbolLayer->GetLayerData()->InsertRenderable(m_pSymPoint); 

	if (m_pSymbolData)
	{
		m_pSymbolData = m_pSymbolLayer->getSymbolData();
		m_pSymbolData->getSymbolPoint()->push_back(m_pSymPoint);
	}
}

/************************************************************************
功能说明：根据十四类数据文件中的编号创建cipas的等值线
参数说明：nLineWidth是线宽；
          strValue是线的标值；
		  nValueNum
          vetPosition是点的三维坐标；
          valuePointArray是标值所在的点，暂未使用
          bClosed为true表示闭合等值线，为false表示非闭合等值线
作    者：renxue
完成日期：2011年08月02日
*************************************************************************/
void CReadSymbolFromM14File::CreatContourLine(int nLineWidth, QString strValue, int nValueNum, QVector3DArray &VectorArray, QVector3DArray &valuePointArray, bool bClosed)
{
	if(nLineWidth<0) return;
	
	m_pSymContour = new CSymbolContourLine;		
	
	//设置顶点数组
	QVector3DArray *pArray = m_pSymContour->GetArray();
	QVector3DArray *pOriginalArray = m_pSymContour->OriginalArray();
	pArray->Clear();
	pOriginalArray->Clear();
	int i = 0;
	for(; i< VectorArray.size(); i++)
	{
		pArray->push_back(VectorArray.at(i));
		pOriginalArray->push_back(VectorArray.at(i));
	}

	//设置属性
	m_pSymbolAttri = new CSymbolAttribute;
	m_pSymbolAttri->SetLineWidth(nLineWidth);
	m_pSymbolAttri->SetValue(strValue.toFloat());
	m_pSymbolAttri->SetFillType(false);	
	m_pSymContour->SetCloseLine(bClosed);
	m_pSymbolAttri->SetLineType(QString(tr("实线")));
	m_pSymContour->SetAttribute(m_pSymbolAttri);

	m_pSymbolLayer->GetLayerData()->InsertRenderable(m_pSymContour); 

	if (m_pSymbolData)
	{
		m_pSymbolData = m_pSymbolLayer->getSymbolData();
		m_pSymbolData->getSymbolContourLine()->push_back(m_pSymContour);
	}
}

/************************************************************************
功能说明：根据十四类数据文件中的编号创建cipas的多边形
参数说明：nCode是多边形的天气，如雨区、雪区；
          PointArray是外围点的三维坐标数组；
          LineColor是线的颜色；
          bFilled为true表示填充，否则不填充多边形区域
作    者：renxue
完成日期：2011年08月02日
*************************************************************************/
void CReadSymbolFromM14File::CreatPolygon(int nCode, QVector3DArray &PointArray, QVector4D &LineColor, bool bFilled)
{
	m_pSymLine = new CSymbolLine;		
	m_pSymbol = m_pSymbolPlugin->GetSymbol(1308);
	if(m_pSymbol == NULL) return;
	m_pSymLine->SetSymbolLineStyle(m_pSymbol);

	//设置顶点数组
	QVector3DArray *pArray = m_pSymLine->GetArray();
	QVector3DArray *pOriginalArray = m_pSymLine->OriginalArray();
	pArray->Clear();
	pOriginalArray->Clear();
	int i = 0;
	for(i=0; i<PointArray.size(); i++)
	{
		pArray->push_back(PointArray.at(i));
		pOriginalArray->push_back(PointArray.at(i));
	}

	//设置属性
	QString strWeatherCode;
	switch(nCode)
	{
	case 1:
		strWeatherCode = tr("雨区");
		LineColor.setX(155);
		break;
	//待扩充
	case 2:
		strWeatherCode = tr("雪区");
		LineColor.setZ(155);
		break;
	case 4:
		strWeatherCode = tr("雷暴区");
		LineColor.setX(200);
		break;
	case 8:
		strWeatherCode = tr("雾区");
		LineColor.setY(100);
		break;
	case 16:
		strWeatherCode = tr("大风区");
		LineColor.setX(255);
		LineColor.setY(38);
		LineColor.setZ(255);
		break;
	case 32:
		strWeatherCode = tr("沙暴区");
		LineColor.setX(10);
		LineColor.setY(30);
		LineColor.setZ(10);
		break;
	default:
		break;
	}
	m_pSymLine->m_LineAttribute.SetValue(strWeatherCode);
	m_pSymLine->m_LineAttribute.SetColor(LineColor);
	m_pSymLine->m_LineAttribute.SetLineType(tr("实线"));
	//m_pSymLine->m_LineAttribute.SetFillType(bFilled);

	m_pSymbolLayer->GetLayerData()->InsertRenderable(m_pSymLine);
	if (m_pSymbolData)
	{
		m_pSymbolData = m_pSymbolLayer->getSymbolData();
		m_pSymbolData->getSymbolLine()->push_back(m_pSymLine);
	}

	return;
}


/************************************************************************
功能说明：根据十四类数据文件中的编号创建有填充色的cipas的等值线区域
参数说明：VectorArray是等值线的顶点数组集合；
          AttributArray是等值线的属性数组集合；
作    者：renxue
完成日期：2011年11月23日
*************************************************************************/
void CReadSymbolFromM14File::CreatClosedContourArea(QVector<QVector3DArray> &VectorArray, QVector<CSymbolAttribute*> &AttributArray)
{
	if(VectorArray.size()<1 || AttributArray.size()<1) return;

	int i, j;
	int nSize = VectorArray.size();

	//设置填充颜色；
	QMap<float, int> mapMark;
	for(i=0; i<nSize; i++)
	{
		mapMark.insert(AttributArray.at(i)->GetValue(), 0);
	}
	j = 0;
	QMap<float, int>::iterator im;
	for(im=mapMark.begin(); im!=mapMark.end(); im++)
	{
		for(i=0; i<nSize; i++)
		{
			if(im.key() == AttributArray.at(i)->GetValue())
			{
				AttributArray.at(i)->SetFillColor(m_ColorTable4D[j]);
				//mapMark.insert(im.key(), j);
			}
		}
		j++;
	}

	//double *pdArea = new double [nSize];
	//memset(pdArea, 0, nSize*sizeof(double));	
	//for(i=0; i<nSize; i++)
	//{
	//	pdArea[i] = ProfileArea2D(VectorArray.at(i));
	//}

	//设置Z值--未完成，考虑到用标值设置Z值，因此中断
	int *pDepZ = new int[nSize];
	memset(pDepZ, 0, nSize*sizeof(int));
	int nDep = 0;
	QVector<QVector<double>> Range;
	for(i=0; i<nSize; i++)
	{
		Range.push_back(MaxRangeXY(VectorArray.at(i)));
	}
	for(i=0; i<nSize; i++)
	{
		for(j=0; j<nSize; j++)
		{
			if(CompareRange(Range.at(i), Range.at(j)))
			{
				pDepZ[j] = pDepZ[i]+1;
				if(pDepZ[j]>nDep) nDep = pDepZ[j];
			}
		}
	}
	//end--设置Z值

	double dLayerSpan = 0.01; //固定值
	double dDepZ = dLayerSpan/(++nDep);
	QVector3D tPoint;
	QVector3DArray temp;
	for(i=0; i<nSize; i++)
	{
		m_pSymContour = new CSymbolContourLine;		
		//设置顶点数组
		QVector3DArray *pArray = m_pSymContour->GetArray();
		QVector3DArray *pOriginalArray = m_pSymContour->OriginalArray();
		pArray->Clear();
		pOriginalArray->Clear();
		
		//im = mapMark.find(AttributArray.at(i)->GetValue());
		//dLayerSpan = dDepZ * im.value();
		dLayerSpan = dDepZ * pDepZ[i];
		temp = VectorArray.at(i);
		for(int k = 0; k< temp.size(); k++)
		{
			tPoint = temp.at(k);
			tPoint.setZ(dLayerSpan);
			pArray->push_back(tPoint);
			pOriginalArray->push_back(tPoint);
		}

		//设置属性
		m_pSymContour->SetCloseLine(true);
		m_pSymContour->SetAttribute(AttributArray.at(i));

		m_pSymbolLayer->GetLayerData()->InsertRenderable(m_pSymContour);
		if (m_pSymbolData)
		{
			m_pSymbolData = m_pSymbolLayer->getSymbolData();
			m_pSymbolData->getSymbolContourLine()->push_back(m_pSymContour);
		}
	}
}

/************************************************************************
功能说明：计算任意多边形的面积
参数说明：pointArray是多边形的三维坐标数组；
来    源：wuenp
完成日期：2011年11月22日
*************************************************************************/
double CReadSymbolFromM14File::ProfileArea2D(QVector3DArray pointArray)
{
	double dArea = 0.0;

	QVector3D pt1, pt2;
	pt1 = pt2 = pointArray.at(0);
	//pointList.append(pt1);
	for (long i = 1; i < pointArray.count(); i++)
	{
		pt1 = pt2;
		pt2 = pointArray.at(i);
		dArea += (pt2.x() - pt1.x()) *  (pt2.y() + pt1.y()) *0.5;
	}
	return fabs(dArea);
}

/************************************************************************
功能说明：计算任意多边形的边界极限
参数说明：pointArray是多边形的三维坐标数组；
返回结果：QVector<double>里面存了四个double型数据，分别是minX, maxX, minY, maxY
作    者：renxue
完成日期：2011年11月23日
*************************************************************************/
QVector<double> CReadSymbolFromM14File::MaxRangeXY(QVector3DArray pointArray)
{
	double dMinX, dMaxX, dMinY, dMaxY;
	
	if(pointArray.size()<1) 
	{
		dMinX = dMaxX = dMinY = dMaxY = 0.0;
	}
	else
	{
		dMinX = dMaxX = pointArray.at(0).x();
		dMinY = dMaxY = pointArray.at(0).y();
	}

	for (long i = 1; i < pointArray.count(); i++)
	{
		if(pointArray.at(i).x() < dMinX) dMinX = pointArray.at(i).x();
		if(pointArray.at(i).x() > dMaxX) dMaxX = pointArray.at(i).x();
		if(pointArray.at(i).y() < dMinY) dMinY = pointArray.at(i).y();
		if(pointArray.at(i).y() > dMaxY) dMaxY = pointArray.at(i).y();
	}
	QVector<double> range;
	range<<dMinX<<dMaxX<<dMinY<<dMaxY;
	return range;
}

/************************************************************************
功能说明：比较二维矩形的边界，判断smallRange是否完全包含在bigRange内
参数说明：bigRange和smallRange是两个多边形的区域范围；
返回结果：true表示后面的范围在前面的范围内，否则不在前面的范围内
作    者：renxue
完成日期：2011年11月23日
*************************************************************************/
bool CReadSymbolFromM14File::CompareRange(QVector<double> bigRange, QVector<double> smallRange)
{
	if(bigRange.size()<4 || smallRange.size()<4) return false;

	if(smallRange.at(0)>bigRange.at(0) && smallRange.at(1)<bigRange.at(1)
	    && smallRange.at(2)>bigRange.at(2) && smallRange.at(3)<bigRange.at(3))
	{
		return true;
	}
	else return false;
}