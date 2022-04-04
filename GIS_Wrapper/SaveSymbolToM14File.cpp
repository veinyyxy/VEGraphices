/*============================================================ 
文件名：SaveSymbolToM14File.cpp
类  名： CSaveSymbolToM14File
父  类： QWidget
子  类： 无 
功能说明：将手绘符号保存成Micaps14类数据文件的格式
调用说明：

----------------------------版本更新--------------------------
V 1.0 
原作者 ：renxue
完成日期：2011年08月08日
============================================================*/

#include <QtCore/QTime>
#include <QtCore/QDate>
#include <Qtgui/QMessageBox>
#include <Qtcore/QTextCodec>
#include "SaveSymbolToM14File.h"


CSaveSymbolToM14::CSaveSymbolToM14(void)
{
	m_pFile = new QFile;
	m_pTextStream = new QTextStream;
}

CSaveSymbolToM14::~CSaveSymbolToM14(void)
{
	delete m_pFile;
	delete m_pTextStream;
	m_pFile = NULL;
	m_pTextStream = NULL;
}

/************************************************************************
 功能说明：将手绘符号保存成Micaps十四类数据格式的文件
 参数说明：fileName是包含路径和文件名的字符串
           返回值为bool型，正确执行返回true，错误或异常返回false
 作    者：renxue
 完成日期：2011年08月09日
************************************************************************/
bool CSaveSymbolToM14::SaveSymbolToM14File(QString fileName, SymbolData *pSymbolData)
{
	if(fileName.isEmpty() || pSymbolData == NULL) return false;

	//创建文件
	m_FileName = fileName;
	m_pFile->setFileName(m_FileName);
	if(!m_pFile->open(QFile::WriteOnly | QFile::Text))
	{
		QMessageBox msgBox;
		QMessageBox::warning(this,"Cannot open file for writing:",m_pFile->errorString(), QMessageBox::Ok);
		msgBox.exec();
		return false;
	}
	if(m_pTextStream == NULL) return false;
	m_pTextStream->setDevice(m_pFile);
	m_pTextStream->setCodec("GB2312");

	WriteFileHead();
	bool ret;
	ret = SaveContourSymbol(pSymbolData, false);
	if(!ret) *m_pTextStream<<"LINES: 0"<<endl;

	ret = SaveLineSymbol(pSymbolData);
	if(!ret) *m_pTextStream<<"LINES_SYMBOL: 0"<<endl;

	SavePointSymbol(pSymbolData);	

	ret = SaveContourSymbol(pSymbolData, true);
	if(!ret) *m_pTextStream<<"CLOSED_CONTOURS: 0"<<endl;

	*m_pTextStream<<"STATION_SITUATION"<<endl;

	ret = SavePolygonSymbol(pSymbolData);
	if(!ret) *m_pTextStream<<"WEATHER_REGION: 0"<<endl;

	*m_pTextStream<<"FILLAREA: 0"<<endl;

	ret = SaveMarkSymbol(pSymbolData);
	if(!ret) *m_pTextStream<<"NOTES_SYMBOL: 0"<<endl;
	*m_pTextStream<<"WithProp_LINESYMBOLS: 0 "<<endl;
	return true;
}

/************************************************************************
 功能说明：写十四类数据的文件头：文件类型，创建时间
 参数说明：返回值为bool型，正确执行返回true，错误或异常返回false
 作    者：renxue
 完成日期：2011年08月09日
************************************************************************/
void CSaveSymbolToM14::WriteFileHead()
{
	*m_pTextStream<<"diamond 14  CIPAS";
	*m_pTextStream<<tr("交互符号")<<endl;

	//写入日期时间
	QDate curDate = QDate::currentDate();
	*m_pTextStream<<curDate.year()<<" ";
	*m_pTextStream<<curDate.month()<<" ";
	*m_pTextStream<<curDate.day()<<" ";
	QTime curTime = QTime::currentTime();
	*m_pTextStream<<curTime.hour()<<" ";
	*m_pTextStream<<0<<endl;

	return;
}

/************************************************************************
 功能说明：写等值线符号数据“LINES:”或“CLOSED_CONTOURS:”
 参数说明：pSymbolData是手绘符号数据指针
           bFlag为true时，考虑闭合等值线，当为false时，只考虑非闭合等值线
           返回值为bool型，正确执行返回true，错误或异常返回false
 作    者：renxue
 完成日期：2011年08月09日
************************************************************************/
bool  CSaveSymbolToM14::SaveContourSymbol(SymbolData *pSymbolData, bool bFlag)
{
	if(NULL == pSymbolData) return false;

	QVector<CSymbolContourLine*> *pContourSymbol = pSymbolData->getSymbolContourLine();
	if(NULL==pContourSymbol || pContourSymbol->size()<1) return false;		
	int nCounts = pContourSymbol->size();
	
	int i, nType, nWidth, nPointNum, nLineCounts;
	bool bClosed;
	QVector3D vecPosition;
	QVector3DArray     *pArray;
	CSymbolAttribute   *pAttribute;
	CSymbolContourLine *pContourLine;
	nLineCounts = 0;
	for(i=0; i<nCounts; i++)
	{
		pContourLine = pContourSymbol->at(i);
		bClosed = pContourLine->GetClosedLine();
		if(bClosed)
		{
			nLineCounts++;
		}
	}
	
	//写数据
	if(bFlag)
	{
		*m_pTextStream<<("CLOSED_CONTOURS: ")<<nLineCounts<<endl;
		for (i=0; i<nCounts; i++)
		{
			pContourLine = pContourSymbol->at(i);
			bClosed = pContourLine->GetClosedLine();
			if(!bClosed) continue;
			pArray = pContourLine->OriginalArray();
			pAttribute = pContourLine->GetAttribute();
			nType = pAttribute->GetSymbolType();      //获取 SymbolID,如601
			nWidth = pAttribute->GetLineWidth();      //线宽
			nPointNum = pArray->size();
			
			//写线宽、点数
			*m_pTextStream<<nWidth<<" ";
			*m_pTextStream<<nPointNum<<endl;

			//写三维坐标点
			for (int j=0; j<nPointNum; j++)
			{
				vecPosition = pArray->at(j);
				*m_pTextStream<<vecPosition.x()<<" ";
				*m_pTextStream<<vecPosition.y()<<" ";
				*m_pTextStream<<vecPosition.z()<<endl;		
			}

			*m_pTextStream<<"NoLabel 0 "<<endl;//micaps3.1.1
			////micaps3写标号，坐标
			//*m_pTextStream<<pAttribute->GetValue()<<" "<<1<<endl;
			//vecPosition = pArray->at(0);
			//*m_pTextStream<<vecPosition.x()<<" ";
			//*m_pTextStream<<vecPosition.y()<<" ";
			//*m_pTextStream<<vecPosition.z()<<endl;
		}
	}
	else 
	{
		nLineCounts = nCounts - nLineCounts;
		*m_pTextStream<<("LINES: ")<<nLineCounts<<endl;
		for (i=0; i<nLineCounts; i++)
		{
			pContourLine = pContourSymbol->at(i);
			bClosed = pContourLine->GetClosedLine();
			if(bClosed) continue;
			pArray = pContourLine->OriginalArray();
			pAttribute = pContourLine->GetAttribute();
			nType = pAttribute->GetSymbolType();      //获取 SymbolID,如601
			nWidth = pAttribute->GetLineWidth();      //线宽

			nPointNum = pArray->size();
	
			//写线宽、点数
			*m_pTextStream<<nWidth<<" ";
			*m_pTextStream<<nPointNum<<endl;

			//写三维坐标点
			for (int j=0; j<nPointNum; j++)
			{
				vecPosition = pArray->at(j);
				*m_pTextStream<<vecPosition.x()<<" ";
				*m_pTextStream<<vecPosition.y()<<" ";
				*m_pTextStream<<vecPosition.z()<<endl;		
			}

			*m_pTextStream<<"NoLabel 0 "<<endl;//micaps3.1.1

			////micaps3.1.1写标号，位置坐标
			//*m_pTextStream<<pAttribute->GetValue()<<" "<<2<<endl;
			//vecPosition = pArray->at(0);
			//*m_pTextStream<<vecPosition.x()<<" ";
			//*m_pTextStream<<vecPosition.y()<<" ";
			//*m_pTextStream<<vecPosition.z()<<endl;	
			//vecPosition = pArray->at(nPointNum-1);
			//*m_pTextStream<<vecPosition.x()<<" ";
			//*m_pTextStream<<vecPosition.y()<<" ";
			//*m_pTextStream<<vecPosition.z()<<endl;
		}
	}//处理完一条符号线
	return true;
}

/************************************************************************
 功能说明：写点符号数据“SYMBOLS”
 参数说明：返回值为bool型，正确执行返回true，错误或异常返回false
 作    者：renxue
 完成日期：2011年08月09日
************************************************************************/
bool CSaveSymbolToM14::SavePointSymbol(SymbolData *pSymbolData)
{	
	//点符号
	int nPointCounts;
	QVector<SymbolPoint*> *pPointSymbol = pSymbolData->getSymbolPoint();	
	if (pPointSymbol==NULL || pPointSymbol->size() < 1) nPointCounts = 0;
	else nPointCounts = pPointSymbol->size();

	//通用符号
	int nNormalCounts;
	QVector<NormalSymbol*> *pNormalSymbol = pSymbolData->getNormalSymbol();
	if (pNormalSymbol==NULL || pNormalSymbol->size() < 1) nNormalCounts = 0;
	else nNormalCounts = pNormalSymbol->size();

	//写标志头
	int  i, nCounts, nType;
	SymbolPoint    *pPoint;
	QVector3DArray *pArray;
	nCounts = 0;
	for(i=0; i<nPointCounts; i++)
	{
		pPoint = pPointSymbol->at(i);
		pArray = pPoint->OriginalArray();
		nCounts += pArray->size();
	}
	for(i=0; i<nNormalCounts; i++)
	{
		pPoint = pNormalSymbol->at(i);
		pArray = pPoint->OriginalArray();
		nCounts += pArray->size();
	}
	*m_pTextStream<<"SYMBOLS: "<<nCounts<<endl;
	if(nCounts == 0) return false;

	//写数据编号、位置（X、Y、Z）、风向角度或字符串
	Symbol      *pSymbol;
	QVector3D   vecPosition;
	for (i=0; i<nPointCounts; i++)
	{
		//改变符号点对应的ID编号
		pPoint = pPointSymbol->at(i);
		pSymbol = pPoint->getSymbol();
		QVector<GLfloat> *angleArray = pPoint->AngleArray();
		if (pSymbol != NULL)
		{
			nType = pSymbol->getType();
			nType = ChangeCodeToM14(nType);
		}	

		pArray = pPoint->OriginalArray();
		for (int j=0, k=0; j<pArray->size(); k++, j++)
		{
			//写编号
			*m_pTextStream<<nType<<" ";				

			//写位置坐标
			vecPosition = pArray->at(j);
			*m_pTextStream<<vecPosition.x()<<" ";
			*m_pTextStream<<vecPosition.y()<<" ";
			*m_pTextStream<<vecPosition.z()<<" ";

			//写风向或字符串
			GLfloat angle_ = angleArray->at(k);
			*m_pTextStream<<angle_<<endl;
		}
	}	
	
	//写数据编号、位置（X、Y、Z）、风向角度(0)
	NormalSymbol *pNormal;
	for (i=0; i<nNormalCounts; i++)
	{
		//改变符号点对应的ID编号
		pNormal = pNormalSymbol->at(i);
		pSymbol = pNormal->SetPointStyle();
		if (pSymbol != NULL)
		{
			nType = pSymbol->getType();
			nType = ChangeCodeToM14(nType);
		}	

		pArray = pNormal->OriginalArray();	
		for (int j=0; j<pArray->size(); j++)
		{
			//写编号
			*m_pTextStream<<nType<<" ";				

			//写位置坐标
			vecPosition = pArray->at(j);
			*m_pTextStream<<vecPosition.x()<<" ";
			*m_pTextStream<<vecPosition.y()<<" ";
			*m_pTextStream<<vecPosition.z()<<" ";

			//写风向或字符串
			*m_pTextStream<<0<<endl;
		}
	}
	return true;
}

/************************************************************************
功能说明：写点符号数据“LINES_SYMBOL”
参数说明：返回值为bool型，正确执行返回true，错误或异常返回false
作    者：renxue
完成日期：2011年08月09日
************************************************************************/
bool CSaveSymbolToM14::SaveLineSymbol(SymbolData *pSymbolData)
{
	if(pSymbolData == NULL) return false;

	QVector<CSymbolLine*> *pLineSymbol = pSymbolData->getSymbolLine();
	if (pLineSymbol==NULL || pLineSymbol->size() < 1) return false;
			
	//写标志头
	int nLineCounts = pLineSymbol->size();
	int i, nType, nPolygons;
	CSymbolLine *pLine;
	QVector3DArray *pArray;
	nPolygons = 0;
	for(i=0; i<nLineCounts; i++)
	{
		pLine = pLineSymbol->at(i);
		nType = pLine->GetLineStyle()->getType(); 
		if(nType == 1308) nPolygons++;
	}
	*m_pTextStream<<("LINES_SYMBOL: ")<<(nLineCounts-nPolygons)<<endl;

	//写数据
	int nWidth, nPointNum;
	QVector3D vecPosition;
	for (i=0; i<nLineCounts; i++)
	{
		pLine = pLineSymbol->at(i);
		pArray = pLine->OriginalArray();

		nType = pLine->GetLineStyle()->getType();       //获取 SymbolID,如601
		nWidth = pLine->m_LineAttribute.GetLineWidth(); //线宽
		nPointNum = pArray->size();
				
		if(nType == 1308) continue;

		//写编号、线宽、点数
		nType = ChangeCodeToM14(nType);
		*m_pTextStream<<nType<<" ";
		*m_pTextStream<<nWidth<<" ";
		*m_pTextStream<<nPointNum<<endl;

		//写三维坐标点
		for (int j=0; j<nPointNum; j++)
		{
			vecPosition = pArray->at(j);
			*m_pTextStream<<vecPosition.x()<<" ";
			*m_pTextStream<<vecPosition.y()<<" ";
			*m_pTextStream<<vecPosition.z()<<endl;		
		}
		*m_pTextStream<<" NoLabel 0 "<<endl;
	}
	return true;
}

/************************************************************************
功能说明：写多边形符号数据“WEATHER_REGION”
参数说明：返回值为bool型，正确执行返回true，错误或异常返回false
作    者：renxue
完成日期：2011年08月10日
************************************************************************/
bool CSaveSymbolToM14::SavePolygonSymbol(SymbolData *pSymbolData)
{
	if(pSymbolData == NULL) return false;

	QVector<CSymbolLine*> *pLineSymbol = pSymbolData->getSymbolLine();
	if (pLineSymbol==NULL || pLineSymbol->size() < 1) return false;

	//写标志头
	int nLineCounts = pLineSymbol->size();
	int i, nType, nPolygons;
	CSymbolLine *pLine;
	QVector3DArray *pArray;
	nPolygons = 0;
	for(i=0; i<nLineCounts; i++)
	{
		pLine = pLineSymbol->at(i);
		nType = pLine->GetLineStyle()->getType(); 
		if(nType == 1308) nPolygons++;
	}
	*m_pTextStream<<("WEATHER_REGION: ")<<nPolygons<<endl;

	//写数据
	int nWidth, nPointNum;
	QVector3D vecPosition;
	for (i=0; i<nLineCounts; i++)
	{
		pLine = pLineSymbol->at(i);
		pArray = pLine->OriginalArray();

		nType = pLine->GetLineStyle()->getType();       //获取 SymbolID,如601
		nWidth = pLine->m_LineAttribute.GetLineWidth(); //线宽
		nPointNum = pArray->size();

		if(nType != 1308) continue;
		QString strRegion = pLine->m_LineAttribute.GetStringValue();
		if(strRegion.compare(tr("雨区")) == 0) nType = 1;
		else if(strRegion.compare(tr("雪区")) == 0)	nType = 2;
		else if(strRegion.compare(tr("雷暴区")) == 0) nType = 4;
		else if(strRegion.compare(tr("雾区")) == 0) nType = 8;
		else if(strRegion.compare(tr("大风区")) == 0) nType = 16;
		else if(strRegion.compare(tr("沙暴区")) == 0) nType = 32;

		*m_pTextStream<<nType<<" "<<(nPointNum+1)<<endl;

		//写三维坐标点
		for (int j=0; j<nPointNum; j++)
		{
			vecPosition = pArray->at(j);
			*m_pTextStream<<vecPosition.x()<<" ";
			*m_pTextStream<<vecPosition.y()<<" ";
			*m_pTextStream<<vecPosition.z()<<endl;		
		}
		vecPosition = pArray->at(0);
		*m_pTextStream<<vecPosition.x()<<" ";
		*m_pTextStream<<vecPosition.y()<<" ";
		*m_pTextStream<<vecPosition.z()<<endl;		
	}//处理完一条符号线
	return true;
}

/************************************************************************
功能说明：写点符号数据“NOTES_SYMBOL”
参数说明：返回值为bool型，正确执行返回true，错误或异常返回false
作    者：renxue
完成日期：2011年08月09日
************************************************************************/
bool CSaveSymbolToM14::SaveMarkSymbol(SymbolData *pSymbolData)
{
	//if(pSymbolData == NULL) return false;

	//QVector<QTextureFont*> *pMarkSymbol = pSymbolData->getMarkSymbol();
	//if (pMarkSymbol==NULL || pMarkSymbol->size() < 1) return false;
	//
	//int nCounts = pMarkSymbol->size();
	//*m_pTextStream<<"NOTES_SYMBOL: "<<nCounts<<endl; //写标志头
	////读数据编号
	//int i, nType;
	//Symbol       *pSymbol;
	//QVector3D    vecPosition;
	//QTextureFont *pText;
	//for (i=0; i<nCounts; i++)
	//{
	//	//改变符号点对应的ID编号
	//	pText = pMarkSymbol->at(i);
	//	nType = 48;
	//	QVector3DArray *pArray = pText->OriginalArray();
	//	QString  txtValue = pText->StringArray();
	//	for (int j=0; j<pArray->size(); j++)
	//	{
	//		//写编号
	//		*m_pTextStream<<nType<<" ";				

	//		//写位置坐标
	//		vecPosition = pArray->at(j);
	//		*m_pTextStream<<vecPosition.x()<<" ";
	//		*m_pTextStream<<vecPosition.y()<<" ";
	//		*m_pTextStream<<vecPosition.z()<<" ";

	//		//字符个数、字符、角度
	//		*m_pTextStream<<txtValue.size()<<" ";
	//		*m_pTextStream<<txtValue<<" ";
	//		*m_pTextStream<<0<<" ";

	//		//字体名长度、字体名称、字体大小、字型
	//		QString strFontName = pText->GetFontName();
	//		*m_pTextStream<<strFontName.size()<<" ";
	//		*m_pTextStream<<strFontName<<" ";
	//		*m_pTextStream<<pText->PixelHeight()<<" ";
	//		*m_pTextStream<<1<<" ";
	//		
	//		//字色
	//		QVector4D color = pText->GetColor();
	//		*m_pTextStream<<color.x()*255<<" ";
	//		*m_pTextStream<<color.y()*255<<" ";
	//		*m_pTextStream<<color.z()*255<<" ";
	//		*m_pTextStream<<color.w()*255<<endl;
	//	}
	//}
	return true;
}

/************************************************************************
功能说明：j将CIPAS符号编号改为Micaps的编号
参数说明：nType是CIPAS的手绘符号编号
作    者：renxue
完成日期：2011年08月09日
************************************************************************/
int CSaveSymbolToM14::ChangeCodeToM14(int nType)
{
	if(nType<-1) return nType;

	//点符号
	switch(nType)
	{
	case 101://无风
		return 43;
	case 103://2-3级风
		return 39;
	case 104://3-4级风
		return 40;
	case 105://4-5级风
		return 36;
	case 106://5-6级风
		return 33;
	case 107://6-7级风
		return 41;
	case 108://7-8级风
		return 34;
	case 109://8-9级风
		return 35;
	case 110://9-10级风
		return 101;
	case 111://10-11级风
		return 102;
	case 112://11-12级风
		return 103;		
	case 201://小雨
		return 26;		
	case 202://中雨
		return 47;		
	case 203://大雨
		return 55;
		
	case 204://暴雨
		return 64;
		
	case 205://大暴雨
		return 221;
		
	case 207://阵雨
		return 25;
		
	case 212://轻冻雨
		return 27;
		
	case 213://冻雨
		return 28;
		
	case 301://小雪
		return 23;
		
	case 302://中雪
		return 22;
		
	case 303://大雪
		return 21;
		
	case 304://暴雪
		return 66;
		
	case 305://阵雪
		return 65;
		
	case 306://雨夹雪
		return 24;
		
	case 307://特大暴雪
		return 231;
		
	case 401://浮尘
		return 201;
		
	case 402://扬沙
		return 45;
		
	case 403://轻沙暴
		return 46;
		
	case 404://沙暴
		return 211;
		
	case 501://晴天
		return 50;
		
	case 502://多云
		return 51;
		
	case 503://阴天
		return 52;
		
	case 504://轻雾
		return 29;
		
	case 505://雾
		return 30;		
	case 506://雷暴
		return 32;		
	case 507://冰雹
		return 31;
		
	case 508://霜冻
		return 44;
		
	case 509://旋转风
		return 42;
		
	case 510://台风
		return 37;
		
	case 1402://高值中心
		return 60;
		
	case 1403://低值中心
		return 61;
		
	case 1404://冷中心
		return 63;
		
	case 1405://暖中心
		return 62;
		
	case 1301://星号
		return 152;
		
	case 1302://空心圆
		return 151;
	case 1303://空三角
		return 153;
		
	case 1304://空正方形
		return 155;
		
	case 1305://实心圆
		return 150;
		
	case 1306://实三角
		return 154;
		
	case 1307://实方形
		return 156;
	
	case 1201://槽线
		return 0;		
	case 601://冷锋
		return 2;		
	case 602://暖锋
		return 3;	
	case 603://锢囚锋
		return 5;		
	case 604://静止锋
		return 4;
	case 1001://单线箭头
		return 1113;
	case 1002://双线箭头
		return 1110;		
	case 1003://双线填充箭头
		return 1114;
	case 1004://双线双箭头
		return 1112;
	case 1005://双线填充双箭头
		return 1115;
	case 1008://双线中间箭头
		return 1111;
	case 1202://霜冻线
		return 38;			
	case 1203://双实线
		return 1102;

	default:
		return -1;
	}
}