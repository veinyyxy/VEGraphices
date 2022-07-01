/*============================================================ 
�ļ�����ReadSymbolFromM14File.cpp 
�� ���� CReadSymbolFromM14File
�� �ࣺ QWidget
�� �ࣺ �� 
����˵������Micaps14�������ļ��ж�ȡ�༭����
����˵����

----------------------------�汾����----------------------------
V 1.0 
ԭ���� ��renxue
������ڣ�2011��08��01��
============================================================*/
#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>
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
	//����������ɫ���ࡢ�ȡ��ơ��̡������ϡ���
	m_ColorTable4D<<QVector4D(0, 1.0f, 1.0f, 1.0f)<<QVector4D(1.0f, 0.6f, 0, 1.0f)<<QVector4D(1.0f, 1.0f, 0, 1.0f);
	m_ColorTable4D<<QVector4D(0, 1.0f, 0.5f, 1.0f)<<QVector4D(0, 0, 1.0f, 1.0f)<<QVector4D(0.9f, 0.5f, 0.9f, 1.0f)<<QVector4D(1.0f, 0, 0, 1.0f);
}

CReadSymbolFromM14File::~CReadSymbolFromM14File(void)
{
}

/************************************************************************
 ����˵������ʮ���������ļ�
 ����˵����fileName�ǰ���·�����ļ������ַ���
           ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
 ��    �ߣ�renxue
 ������ڣ�2011��11��21��
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
		strLine = m_txtStream.readLine();//��ʱ����Ϣ�����ں�������
		while(strLine.isEmpty() && !m_txtStream.atEnd())
		{
			strLine = m_txtStream.readLine();
		}

		if(strLine.indexOf("LINES_SYMBOL") == 0)        //��LINES_SYMBOL���ݿ�		
		{
			ReadLinesSymbol(strLine);
		}
		else if(strLine.indexOf("LINES") == 0)          //��LINES���ݿ�
		{
			ReadLines(strLine);			
		}
		else if(strLine.indexOf("SYMBOLS") == 0)        //��SYMBOLS���ݿ�
		{
			ReadSymbol(strLine);
		}
		else if(strLine.indexOf("CLOSED_CONTOURS") == 0)//��CLOSED_CONTOURS���ݿ�
		{
			ReadContour(strLine);
		}
		else if(strLine.indexOf("STATION_SITUATION") == 0)//��STATION_SITUATION���ݿ�
		{
			ReadStationSituation(strLine);//û��ʵ�֣�ԭ�����ƽ����Ҫʵ��
		}
		else if(strLine.indexOf("WEATHER_REGION") == 0)//��WEATHER_REGION���ݿ�
		{
			ReadWeatherRegion(strLine);
		}
		else if(strLine.indexOf("FILLAREA") == 0)      //��FILLAREA���ݿ�
		{
			ReadFillArea(strLine);
		}
		else if(strLine.indexOf("NOTES_SYMBOL") == 0)  //��NOTES_SYMBOL���ݿ�
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
 ����˵������ʮ���������ļ��е�LINES���ݿ�,��ӦCIPAS�ķǱպϵ�ֵ��
 ����˵����strLine���б�ʶ����LINES����������
           ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
 ��    �ߣ�renxue
 ������ڣ�2011��11��21��
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
		
		//��ÿ���ߵĿ�����Ϣ����š��߿���ĸ���
        strList = strLine.split(("\\s+"), Qt::SkipEmptyParts);
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

		//����γ�����
		m_PointArray.Clear();
		if(nPointNum>0) ReadPositionArray(m_PointArray, nPointNum);
		
		//��������Ϣ����š�����
		QString  strValue;
		strLine = m_txtStream.readLine();
		while(strLine.isEmpty() && !m_txtStream.atEnd())
		{
			strLine = m_txtStream.readLine();
		}

		if(strLine.contains("NoLabel 0"))//�ޱ�ֵ
		{
			CreatContourLine(nLineWidth, 0, nPointNum, m_PointArray, m_PointArray, false);
		}
		else //�б�ֵ
		{
			nPointNum = 0;
            strList = strLine.split(("\\s+"), Qt::SkipEmptyParts);
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

			//����ŵ�����λ��
			QVector3DArray tArray;
			ReadPositionArray(tArray, nPointNum);

			//�����Ǳպϵ�ֵ�߷���
			CreatContourLine(nLineWidth, strValue, nPointNum, m_PointArray, tArray, false);
		}
		nLineNum--;
	}

	return true;
}

/************************************************************************
����˵������ʮ���������ļ��е�LINES_SYMBOL���ݿ�
����˵����strLine���б�ʶ����LINES_SYMBOL����������
          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
 ��    �ߣ�renxue
 ������ڣ�2011��11��21��
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
		
		//��ÿ���ߵĿ�����Ϣ����š��߿���ĸ���
        strList = strLine.split(("\\s+"), Qt::SkipEmptyParts);
		if(strList.length() == 3)
		{
			nType = strList.at(0).toInt();
			nLineWidth = strList.at(1).toInt();
			nPointNum = strList.at(2).toInt();
		}//������һ������
		else
		{
			QMessageBox msgBox;
			msgBox.setText("File: Wrong Data in  LINES_SYMBOL!");
			msgBox.exec();
			return false;
		}
		
		//����γ�����	
		m_PointArray.Clear();                   
		ReadPositionArray(m_PointArray, nPointNum);	
		
		strLine = m_txtStream.readLine();       //������С�NoLabel 0����һ��
		while((strLine.isEmpty() && !m_txtStream.atEnd()) || !strLine.contains("NoLabel 0"))
		{
			strLine = m_txtStream.readLine();
		}

		nLineNum--;

		CreatLinesSymbol(nType, nLineWidth, m_PointArray);//������Ӧ���߷���
	}

	return true;
}


/************************************************************************
����˵������ʮ���������ļ��е�SYMBOLS���ݿ�
����˵����strLine���б�ʶ����SYMBOLS����������
          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
 ��    �ߣ�renxue
 ������ڣ�2011��11��21��
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
		
		//��ÿ�����ŵ���Ϣ����š����ꡢ����Ƕ�
        strList = strLine.split(("\\s+"), Qt::SkipEmptyParts);
		if(strList.length() == 5)
		{
			nType = strList.at(0).toInt();
			vetPosition.setX(strList.at(1).toDouble());
			vetPosition.setY(strList.at(2).toDouble());
			vetPosition.setZ(strList.at(3).toDouble());
			strAngle = strList.at(4);
		}//����һ������
		else
		{
			QMessageBox msgBox;
			msgBox.setText("File��Wrong Datas in SYMBOLS!");
			msgBox.exec();
		}
		nLineNum--;

		CreatSymbols(nType, vetPosition, strAngle);    //���ݶ�ȡ����Ϣ������Ӧ�ķ���
	}

	if(nLineNum != 0 && m_txtStream.atEnd()) return false;
	return true;
}

/************************************************************************
 ����˵������ʮ���������ļ��е�CLOSED_CONTOURS���ݿ�
 ����˵����strLine���б�ʶ����CLOSED_CONTOURS����������
           ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
 ��    �ߣ�renxue
 ������ڣ�2011��11��21��
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
		//��ÿ���ߵĿ�����Ϣ���߿���ĸ���		
		strLine = m_txtStream.readLine();
		while(strLine.isEmpty() && !m_txtStream.atEnd())    //�������У��ظ���
		{
			strLine = m_txtStream.readLine();
		}
        strList = strLine.split(("\\s+"), Qt::SkipEmptyParts);
		
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

		//����γ�����
		//m_PointArray.Clear();		
		//if(nPointNum>0) ReadPositionArray(m_PointArray, nPointNum);
		QVector3DArray  PointsArray;
		if(nPointNum>0) ReadPositionArray(PointsArray, nPointNum);
		m_ClosedContourArray.push_back(PointsArray);
		
		//��������Ϣ����š�����
		QString strValue;	
		strLine = m_txtStream.readLine();
		while(strLine.isEmpty() && !m_txtStream.atEnd())    //�������У��ظ���
		{
			strLine = m_txtStream.readLine();
		}
		if(strLine.contains("NoLabel 0"))
		{
			//���ݶ�ȡ����Ϣ������ֵ��
			//CreatContourLine(nLineWidth, 0, nPointNum, m_PointArray, m_PointArray, true);
			strValue = "0";
		}
		else
		{
            strList = strLine.split(("\\s+"), Qt::SkipEmptyParts);
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
			//����ŵ�����λ��
			QVector3DArray valuePointArray;
			ReadPositionArray(valuePointArray, nPointNum);

			//���ݶ�ȡ����Ϣ������ֵ��
			//CreatContourLine(nLineWidth, strValue, nPointNum, m_PointArray, valuePointArray, true);
		}
		CSymbolAttribute *pContourAttri = new CSymbolAttribute();
		pContourAttri->SetLineWidth(nLineWidth);
		pContourAttri->SetFillType(true);	
		pContourAttri->SetLineType(QString(tr("ʵ��")));
		pContourAttri->SetValue(strValue.toFloat());
		m_ClosedContourAttributs.push_back(pContourAttri);
		nLineNum--;
	}
	CreatClosedContourArea(m_ClosedContourArray, m_ClosedContourAttributs);
	return true;
}


/************************************************************************
����˵������ʮ���������ļ��е�STATION_SITUATION���ݿ�
����˵����strLine���б�ʶ����STATION_SITUATION����������
          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
��    �ߣ�renxue
������ڣ�2011��08��02��
************************************************************************/
bool CReadSymbolFromM14File::ReadStationSituation(QString strLineHead)
{
	return false;
}


/************************************************************************
 ����˵������ʮ���������ļ��е�WEATHER_REGION���ݿ�
 ����˵����strLine���б�ʶ����WEATHER_REGION����������
           ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
 ��    �ߣ�renxue
 ������ڣ�2011��11��21��
 ///////////////////////meiyou��֤////////////////////////////////////////
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
		while(strLine.isEmpty() && !m_txtStream.atEnd())    //�������У��ظ���
		{
			strLine = m_txtStream.readLine();
		}

		//��ÿ���ߵĿ�����Ϣ����š���ĸ���
        strList = strLine.split(("\\s+"), Qt::SkipEmptyParts);
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
	
		m_PointArray.Clear();                   //����γ�����	
		ReadPositionArray(m_PointArray, nPointNum);	

		nLineNum--;
		QVector4D color(0, 0, 0, 1);
		CreatPolygon(nCode, m_PointArray, color, true);
	}

	return true;
}

/************************************************************************
����˵������ʮ���������ļ��е�FILLAREA���ݿ�
����˵����strLine���б�ʶ����FILLAREA����������
          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
��    �ߣ�renxue
������ڣ�2011��11��21��
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
		while(strLine.isEmpty() && !m_txtStream.atEnd())    //�������У��ظ���
		{
			strLine = m_txtStream.readLine();
		}

		//��ÿ������Ŀ�����Ϣ����š���ĸ���
        strList = strLine.split(("\\s+"), Qt::SkipEmptyParts);
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

		//����γ�����
		m_PointArray.Clear();
		if(nPointNum>0) ReadPositionArray(m_PointArray, nPointNum);

		//��������Ϣ��������͡���ɫ��ǰ��ɫ������ɫ������ɫ�Ƕȡ�ͼ�����롢�Ƿ񻭱߿�
		QString  strLineType, strColorAngle, strImageCode, strBorder;
		QVector4D  LineColor, ForgroundColor, BackgroundColor;
		strLine = m_txtStream.readLine();
		while(strLine.isEmpty() && !m_txtStream.atEnd())    //�������У��ظ���
		{
			strLine = m_txtStream.readLine();
		}
        strList = strLine.split(("\\s+"), Qt::SkipEmptyParts);
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

		//��������η���
		if(BackgroundColor == ForgroundColor)//�ò���ģ��
		{
			CreatLinesSymbol(0, 1,m_PointArray);
		}
		else//�ö����ģ��
		{
			CreatPolygon(nType, m_PointArray, ForgroundColor, true);
		}

		nLineNum--;
	}

	return true;
}

/************************************************************************
����˵������ʮ���������ļ��е�NOTES_SYMBOL���ݿ�
����˵����strLine���б�ʶ����NOTES_SYMBOL����������
          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
��    �ߣ�renxue
������ڣ�2011��11��21��
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
		while(strLine.isEmpty() && !m_txtStream.atEnd())    //�������У��ظ���
		{
			strLine = m_txtStream.readLine();
		}

		//��ÿ�����ŵĿ�����Ϣ����š����ꡢ����Ƕ�
        strList = strLine.split(("\\s+"), Qt::SkipEmptyParts);
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
		}//����һ������
		else
		{
			QMessageBox msgBox;
			msgBox.setText("File��Wrong Datas in NOTES_SYMBOL!");
			msgBox.exec();
		}
		nLineNum--;

		//CreatSymbols(nCode, vetPosition, strAngle);    //���ݶ�ȡ����Ϣ������Ӧ�ķ���
		if(nCode == 48) nCode = 1401;
		//QTextureFont *pText = new QTextureFont;
		//m_pSymbol = m_pSymbolPlugin->GetSymbol(nCode);
		//if (NULL == m_pSymbol)	return false;
		//pText->SetStyle(m_pSymbol->getType()); //�������ַ�������
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
����˵������ʮ���������ļ��е���ά����λ�����ݿ�
����˵����nPointNum������ĸ���
          Array���ڴ洢��ά���ꣻ
          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
 ��    �ߣ�renxue
 ������ڣ�2011��11��21��
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
		while(strLine.isEmpty() && !m_txtStream.atEnd())    //�������У��ظ���
		{
			strLine = m_txtStream.readLine();
		}
        strList = strLine.split(("\\s+"), Qt::SkipEmptyParts);
	
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
			}//������һ�е�����
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText("File: Wrong Data in ReadPositionArray()!");
			msgBox.exec();
			return false;
		}
	}//������nPointNum�е�����

	return true;
}

/************************************************************************
����˵��������ʮ���������ļ��еı�Ŵ���cipas�ı༭�߷���
����˵����nType��ʮ�������ݵı�ţ�
          nWidth���ߵĿ�ȣ�
 ��    �ߣ�renxue
 ������ڣ�2011��08��01��
************************************************************************/
void CReadSymbolFromM14File::CreatLinesSymbol(int nType, int nWidth, QVector3DArray &PointArray)
{
	if(nType< 0) return;

	int nLineType = -1;
	//���ݶ�ȡ����Ϣ������Ӧ�ķ���
	switch(nType)
	{
		case 0://����
			nLineType = 1201;
			break;
		case 2://���		
			nLineType = 601;
			break;
		case 3://ů��
			nLineType = 602;
			break;
		case 4://��ֹ��
			nLineType = 604;
			break;
		case 5://������
			nLineType = 603;
			break;	
		case 1113://���߼�ͷ
			nLineType = 1001;
			break;
		case 1110://˫�߼�ͷ
			nLineType = 1002;
			break;	
		case 1114://˫������ͷ
			nLineType = 1003;
			break;
		case 1112://˫��˫��ͷ
			nLineType = 1004;
			break;
		case 1115://˫�����˫��ͷ
			nLineType = 1005;
			break;
		case 1111://˫���м��ͷ
			nLineType = 1008;
			break;
		case 38://˪����
			nLineType = 1202;
			break;	
		case 1102://˫ʵ��
			nLineType = 1203;
			break;
		default:
			break;
	}
	m_pSymLine = new CSymbolLine;		
	m_pSymbol = m_pSymbolPlugin->GetSymbol(nLineType);
	if (NULL == m_pSymbol)	return;
	m_pSymLine->SetSymbolLineStyle(m_pSymbol);					

	//�������ԺͶ�������
    m_pSymLine->m_LineAttribute.SetLineType(QString("%1").arg(nType));
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
����˵��������ʮ���������ļ��еı�Ŵ���cipas�ĵ����
����˵����nType��ʮ�������ݵı�ţ�
          vetPosition�ǵ����ά���ꣻ
          str�����Ƿ����ַ�����ֵ������nTypeȷ����������
��    �ߣ�renxue
������ڣ�2011��08��02��
************************************************************************/
void CReadSymbolFromM14File::CreatSymbols(int nType, QVector3D &vetPosition, QString str)
{
	if(nType < 0) return;

	int nPointType = 0;

	//ͨ�÷���
	if(nType>149 && nType<157)
	{
		switch(nType)
		{
		case 152://�Ǻ�
			nPointType = 1301;
			break;
		case 151://����Բ
			nPointType = 1302;
			break;
		case 153://������
			nPointType = 1303;
			break;
		case 155://��������
			nPointType = 1304;
			break;
		case 150://ʵ��Բ
			nPointType = 1305;
			break;
		case 154://ʵ����
			nPointType = 1306;
			break;
		case 156://ʵ����
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

	//�����
	switch(nType)
	{
		case 43://�޷�
			nPointType = 101;
			break;
		case 39://2-3����
			nPointType = 103;
			break;
		case 40://3-4����
			nPointType = 104;
			break;
		case 36://4-5����
			nPointType = 105;
			break;
		case 33://5-6����
			nPointType = 106;
			break;
		case 41://6-7����
			nPointType = 107;
			break;
		case 34://7-8����
			nPointType = 108;
			break;
		case 35://8-9����
			nPointType = 109;
			break;
		case 101://9-10����
			nPointType = 110;
			break;
		case 102://10-11����
			nPointType = 111;
			break;
		case 103://11-12����
			nPointType = 112;
			break;
		case 26://С��
			nPointType = 201;
			break;
		case 47://����
			nPointType = 202;
			break;
		case 55://����
			nPointType = 203;
			break;
		case 64://����
			nPointType = 204;
			break;
		case 221://����
			nPointType = 205;
			break;
		case 25://����
			nPointType = 207;
			break;
		case 27://�ᶳ��
			nPointType = 212;
			break;
		case 28://����
			nPointType = 213;
			break;
		case 23://Сѩ
			nPointType = 301;
			break;
		case 22://��ѩ
			nPointType = 302;
			break;
		case 21://��ѩ
			nPointType = 303;
			break;
		case 66://��ѩ
			nPointType = 304;
			break;
		case 65://��ѩ
			nPointType = 305;
			break;
		case 24://���ѩ
			nPointType = 306;
			break;
		case 231://�ش�ѩ
			nPointType = 307;
			break;
		case 201://����
			nPointType = 401;
			break;
		case 45://��ɳ
			nPointType = 402;
			break;
		case 46://��ɳ��
			nPointType = 403;
			break;
		case 211://ɳ��
			nPointType = 404;
			break;
		case 50://����
			nPointType = 501;
			break;
		case 51://����
			nPointType = 502;
			break;
		case 52://����
			nPointType = 503;
			break;
		case 29://����
			nPointType = 504;
			break;
		case 30://��
			nPointType = 505;
			break;
		case 32://�ױ�
			nPointType = 506;
			break;
		case 31://����
			nPointType = 507;
			break;
		case 44://˪��
			nPointType = 508;
			break;
		case 42://��ת��
			nPointType = 509;
			break;
		case 37://̨��
			nPointType = 510;
			break;
		case 48://���ֱ�ע,��������
			return;
		case 60://��ֵ����
			nPointType = 1402;
			break;
		case 61://��ֵ����
			nPointType = 1403;
			break;
		case 63://������
			nPointType = 1404;
			break;
		case 62://ů����
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

	//���÷���Ƕ�
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
����˵��������ʮ���������ļ��еı�Ŵ���cipas�ĵ�ֵ��
����˵����nLineWidth���߿�
          strValue���ߵı�ֵ��
		  nValueNum
          vetPosition�ǵ����ά���ꣻ
          valuePointArray�Ǳ�ֵ���ڵĵ㣬��δʹ��
          bClosedΪtrue��ʾ�պϵ�ֵ�ߣ�Ϊfalse��ʾ�Ǳպϵ�ֵ��
��    �ߣ�renxue
������ڣ�2011��08��02��
*************************************************************************/
void CReadSymbolFromM14File::CreatContourLine(int nLineWidth, QString strValue, int nValueNum, QVector3DArray &VectorArray, QVector3DArray &valuePointArray, bool bClosed)
{
	if(nLineWidth<0) return;
	
	m_pSymContour = new CSymbolContourLine;		
	
	//���ö�������
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

	//��������
	m_pSymbolAttri = new CSymbolAttribute;
	m_pSymbolAttri->SetLineWidth(nLineWidth);
	m_pSymbolAttri->SetValue(strValue.toFloat());
	m_pSymbolAttri->SetFillType(false);	
	m_pSymContour->SetCloseLine(bClosed);
	m_pSymbolAttri->SetLineType(QString(tr("ʵ��")));
	m_pSymContour->SetAttribute(m_pSymbolAttri);

	m_pSymbolLayer->GetLayerData()->InsertRenderable(m_pSymContour); 

	if (m_pSymbolData)
	{
		m_pSymbolData = m_pSymbolLayer->getSymbolData();
		m_pSymbolData->getSymbolContourLine()->push_back(m_pSymContour);
	}
}

/************************************************************************
����˵��������ʮ���������ļ��еı�Ŵ���cipas�Ķ����
����˵����nCode�Ƕ���ε���������������ѩ����
          PointArray����Χ�����ά�������飻
          LineColor���ߵ���ɫ��
          bFilledΪtrue��ʾ��䣬���������������
��    �ߣ�renxue
������ڣ�2011��08��02��
*************************************************************************/
void CReadSymbolFromM14File::CreatPolygon(int nCode, QVector3DArray &PointArray, QVector4D &LineColor, bool bFilled)
{
	m_pSymLine = new CSymbolLine;		
	m_pSymbol = m_pSymbolPlugin->GetSymbol(1308);
	if(m_pSymbol == NULL) return;
	m_pSymLine->SetSymbolLineStyle(m_pSymbol);

	//���ö�������
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

	//��������
	QString strWeatherCode;
	switch(nCode)
	{
	case 1:
		strWeatherCode = tr("����");
		LineColor.setX(155);
		break;
	//������
	case 2:
		strWeatherCode = tr("ѩ��");
		LineColor.setZ(155);
		break;
	case 4:
		strWeatherCode = tr("�ױ���");
		LineColor.setX(200);
		break;
	case 8:
		strWeatherCode = tr("����");
		LineColor.setY(100);
		break;
	case 16:
		strWeatherCode = tr("�����");
		LineColor.setX(255);
		LineColor.setY(38);
		LineColor.setZ(255);
		break;
	case 32:
		strWeatherCode = tr("ɳ����");
		LineColor.setX(10);
		LineColor.setY(30);
		LineColor.setZ(10);
		break;
	default:
		break;
	}
	m_pSymLine->m_LineAttribute.SetValue(strWeatherCode);
	m_pSymLine->m_LineAttribute.SetColor(LineColor);
	m_pSymLine->m_LineAttribute.SetLineType(tr("ʵ��"));
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
����˵��������ʮ���������ļ��еı�Ŵ��������ɫ��cipas�ĵ�ֵ������
����˵����VectorArray�ǵ�ֵ�ߵĶ������鼯�ϣ�
          AttributArray�ǵ�ֵ�ߵ��������鼯�ϣ�
��    �ߣ�renxue
������ڣ�2011��11��23��
*************************************************************************/
void CReadSymbolFromM14File::CreatClosedContourArea(QVector<QVector3DArray> &VectorArray, QVector<CSymbolAttribute*> &AttributArray)
{
	if(VectorArray.size()<1 || AttributArray.size()<1) return;

	int i, j;
	int nSize = VectorArray.size();

	//���������ɫ��
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

	//����Zֵ--δ��ɣ����ǵ��ñ�ֵ����Zֵ������ж�
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
	//end--����Zֵ

	double dLayerSpan = 0.01; //�̶�ֵ
	double dDepZ = dLayerSpan/(++nDep);
	QVector3D tPoint;
	QVector3DArray temp;
	for(i=0; i<nSize; i++)
	{
		m_pSymContour = new CSymbolContourLine;		
		//���ö�������
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

		//��������
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
����˵���������������ε����
����˵����pointArray�Ƕ���ε���ά�������飻
��    Դ��wuenp
������ڣ�2011��11��22��
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
����˵���������������εı߽缫��
����˵����pointArray�Ƕ���ε���ά�������飻
���ؽ����QVector<double>��������ĸ�double�����ݣ��ֱ���minX, maxX, minY, maxY
��    �ߣ�renxue
������ڣ�2011��11��23��
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
����˵�����Ƚ϶�ά���εı߽磬�ж�smallRange�Ƿ���ȫ������bigRange��
����˵����bigRange��smallRange����������ε�����Χ��
���ؽ����true��ʾ����ķ�Χ��ǰ��ķ�Χ�ڣ�������ǰ��ķ�Χ��
��    �ߣ�renxue
������ڣ�2011��11��23��
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
