/*============================================================ 
�ļ�����SaveSymbolToM14File.cpp
��  ���� CSaveSymbolToM14File
��  �ࣺ QWidget
��  �ࣺ �� 
����˵�������ֻ���ű����Micaps14�������ļ��ĸ�ʽ
����˵����

----------------------------�汾����--------------------------
V 1.0 
ԭ���� ��renxue
������ڣ�2011��08��08��
============================================================*/
#include <QtWidgets/QMessageBox>
#include <QtCore/QTime>
#include <QtCore/QDate>
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
 ����˵�������ֻ���ű����Micapsʮ�������ݸ�ʽ���ļ�
 ����˵����fileName�ǰ���·�����ļ������ַ���
           ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
 ��    �ߣ�renxue
 ������ڣ�2011��08��09��
************************************************************************/
bool CSaveSymbolToM14::SaveSymbolToM14File(QString fileName, SymbolData *pSymbolData)
{
	if(fileName.isEmpty() || pSymbolData == NULL) return false;

	//�����ļ�
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
    //m_pTextStream->setCodec("GB2312");

	WriteFileHead();
	bool ret;
	ret = SaveContourSymbol(pSymbolData, false);
    if(!ret) *m_pTextStream<<"LINES: 0"<<Qt::endl;

	ret = SaveLineSymbol(pSymbolData);
    if(!ret) *m_pTextStream<<"LINES_SYMBOL: 0"<<Qt::endl;

	SavePointSymbol(pSymbolData);	

	ret = SaveContourSymbol(pSymbolData, true);
    if(!ret) *m_pTextStream<<"CLOSED_CONTOURS: 0"<<Qt::endl;

    *m_pTextStream<<"STATION_SITUATION"<<Qt::endl;

	ret = SavePolygonSymbol(pSymbolData);
    if(!ret) *m_pTextStream<<"WEATHER_REGION: 0"<<Qt::endl;

    *m_pTextStream<<"FILLAREA: 0"<<Qt::endl;

	ret = SaveMarkSymbol(pSymbolData);
    if(!ret) *m_pTextStream<<"NOTES_SYMBOL: 0"<<Qt::endl;
    *m_pTextStream<<"WithProp_LINESYMBOLS: 0 "<<Qt::endl;
	return true;
}

/************************************************************************
 ����˵����дʮ�������ݵ��ļ�ͷ���ļ����ͣ�����ʱ��
 ����˵��������ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
 ��    �ߣ�renxue
 ������ڣ�2011��08��09��
************************************************************************/
void CSaveSymbolToM14::WriteFileHead()
{
	*m_pTextStream<<"diamond 14  CIPAS";
    *m_pTextStream<<tr("��������")<<Qt::endl;

	//д������ʱ��
	QDate curDate = QDate::currentDate();
	*m_pTextStream<<curDate.year()<<" ";
	*m_pTextStream<<curDate.month()<<" ";
	*m_pTextStream<<curDate.day()<<" ";
	QTime curTime = QTime::currentTime();
	*m_pTextStream<<curTime.hour()<<" ";
    *m_pTextStream<<0<<Qt::endl;

	return;
}

/************************************************************************
 ����˵����д��ֵ�߷������ݡ�LINES:����CLOSED_CONTOURS:��
 ����˵����pSymbolData���ֻ��������ָ��
           bFlagΪtrueʱ�����Ǳպϵ�ֵ�ߣ���Ϊfalseʱ��ֻ���ǷǱպϵ�ֵ��
           ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
 ��    �ߣ�renxue
 ������ڣ�2011��08��09��
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
	
	//д����
	if(bFlag)
	{
        *m_pTextStream<<("CLOSED_CONTOURS: ")<<nLineCounts<<Qt::endl;
		for (i=0; i<nCounts; i++)
		{
			pContourLine = pContourSymbol->at(i);
			bClosed = pContourLine->GetClosedLine();
			if(!bClosed) continue;
			pArray = pContourLine->OriginalArray();
			pAttribute = pContourLine->GetAttribute();
			nType = pAttribute->GetSymbolType();      //��ȡ SymbolID,��601
			nWidth = pAttribute->GetLineWidth();      //�߿�
			nPointNum = pArray->size();
			
			//д�߿�����
			*m_pTextStream<<nWidth<<" ";
            *m_pTextStream<<nPointNum<<Qt::endl;

			//д��ά�����
			for (int j=0; j<nPointNum; j++)
			{
				vecPosition = pArray->at(j);
				*m_pTextStream<<vecPosition.x()<<" ";
				*m_pTextStream<<vecPosition.y()<<" ";
                *m_pTextStream<<vecPosition.z()<<Qt::endl;
			}

            *m_pTextStream<<"NoLabel 0 "<<Qt::endl;//micaps3.1.1
			////micaps3д��ţ�����
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
        *m_pTextStream<<("LINES: ")<<nLineCounts<<Qt::endl;
		for (i=0; i<nLineCounts; i++)
		{
			pContourLine = pContourSymbol->at(i);
			bClosed = pContourLine->GetClosedLine();
			if(bClosed) continue;
			pArray = pContourLine->OriginalArray();
			pAttribute = pContourLine->GetAttribute();
			nType = pAttribute->GetSymbolType();      //��ȡ SymbolID,��601
			nWidth = pAttribute->GetLineWidth();      //�߿�

			nPointNum = pArray->size();
	
			//д�߿�����
			*m_pTextStream<<nWidth<<" ";
            *m_pTextStream<<nPointNum<<Qt::endl;

			//д��ά�����
			for (int j=0; j<nPointNum; j++)
			{
				vecPosition = pArray->at(j);
				*m_pTextStream<<vecPosition.x()<<" ";
				*m_pTextStream<<vecPosition.y()<<" ";
                *m_pTextStream<<vecPosition.z()<<Qt::endl;
			}

            *m_pTextStream<<"NoLabel 0 "<<Qt::endl;//micaps3.1.1

			////micaps3.1.1д��ţ�λ������
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
	}//������һ��������
	return true;
}

/************************************************************************
 ����˵����д��������ݡ�SYMBOLS��
 ����˵��������ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
 ��    �ߣ�renxue
 ������ڣ�2011��08��09��
************************************************************************/
bool CSaveSymbolToM14::SavePointSymbol(SymbolData *pSymbolData)
{	
	//�����
	int nPointCounts;
	QVector<SymbolPoint*> *pPointSymbol = pSymbolData->getSymbolPoint();	
	if (pPointSymbol==NULL || pPointSymbol->size() < 1) nPointCounts = 0;
	else nPointCounts = pPointSymbol->size();

	//ͨ�÷���
	int nNormalCounts;
	QVector<NormalSymbol*> *pNormalSymbol = pSymbolData->getNormalSymbol();
	if (pNormalSymbol==NULL || pNormalSymbol->size() < 1) nNormalCounts = 0;
	else nNormalCounts = pNormalSymbol->size();

	//д��־ͷ
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
    *m_pTextStream<<"SYMBOLS: "<<nCounts<<Qt::endl;
	if(nCounts == 0) return false;

	//д���ݱ�š�λ�ã�X��Y��Z��������ǶȻ��ַ���
	Symbol      *pSymbol;
	QVector3D   vecPosition;
	for (i=0; i<nPointCounts; i++)
	{
		//�ı���ŵ��Ӧ��ID���
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
			//д���
			*m_pTextStream<<nType<<" ";				

			//дλ������
			vecPosition = pArray->at(j);
			*m_pTextStream<<vecPosition.x()<<" ";
			*m_pTextStream<<vecPosition.y()<<" ";
			*m_pTextStream<<vecPosition.z()<<" ";

			//д������ַ���
			GLfloat angle_ = angleArray->at(k);
            *m_pTextStream<<angle_<<Qt::endl;
		}
	}	
	
	//д���ݱ�š�λ�ã�X��Y��Z��������Ƕ�(0)
	NormalSymbol *pNormal;
	for (i=0; i<nNormalCounts; i++)
	{
		//�ı���ŵ��Ӧ��ID���
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
			//д���
			*m_pTextStream<<nType<<" ";				

			//дλ������
			vecPosition = pArray->at(j);
			*m_pTextStream<<vecPosition.x()<<" ";
			*m_pTextStream<<vecPosition.y()<<" ";
			*m_pTextStream<<vecPosition.z()<<" ";

			//д������ַ���
            *m_pTextStream<<0<<Qt::endl;
		}
	}
	return true;
}

/************************************************************************
����˵����д��������ݡ�LINES_SYMBOL��
����˵��������ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
��    �ߣ�renxue
������ڣ�2011��08��09��
************************************************************************/
bool CSaveSymbolToM14::SaveLineSymbol(SymbolData *pSymbolData)
{
	if(pSymbolData == NULL) return false;

	QVector<CSymbolLine*> *pLineSymbol = pSymbolData->getSymbolLine();
	if (pLineSymbol==NULL || pLineSymbol->size() < 1) return false;
			
	//д��־ͷ
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
    *m_pTextStream<<("LINES_SYMBOL: ")<<(nLineCounts-nPolygons)<<Qt::endl;

	//д����
	int nWidth, nPointNum;
	QVector3D vecPosition;
	for (i=0; i<nLineCounts; i++)
	{
		pLine = pLineSymbol->at(i);
		pArray = pLine->OriginalArray();

		nType = pLine->GetLineStyle()->getType();       //��ȡ SymbolID,��601
		nWidth = pLine->m_LineAttribute.GetLineWidth(); //�߿�
		nPointNum = pArray->size();
				
		if(nType == 1308) continue;

		//д��š��߿�����
		nType = ChangeCodeToM14(nType);
		*m_pTextStream<<nType<<" ";
		*m_pTextStream<<nWidth<<" ";
        *m_pTextStream<<nPointNum<<Qt::endl;

		//д��ά�����
		for (int j=0; j<nPointNum; j++)
		{
			vecPosition = pArray->at(j);
			*m_pTextStream<<vecPosition.x()<<" ";
			*m_pTextStream<<vecPosition.y()<<" ";
            *m_pTextStream<<vecPosition.z()<<Qt::endl;
		}
        *m_pTextStream<<" NoLabel 0 "<<Qt::endl;
	}
	return true;
}

/************************************************************************
����˵����д����η������ݡ�WEATHER_REGION��
����˵��������ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
��    �ߣ�renxue
������ڣ�2011��08��10��
************************************************************************/
bool CSaveSymbolToM14::SavePolygonSymbol(SymbolData *pSymbolData)
{
	if(pSymbolData == NULL) return false;

	QVector<CSymbolLine*> *pLineSymbol = pSymbolData->getSymbolLine();
	if (pLineSymbol==NULL || pLineSymbol->size() < 1) return false;

	//д��־ͷ
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
    *m_pTextStream<<("WEATHER_REGION: ")<<nPolygons<<Qt::endl;

	//д����
	int nWidth, nPointNum;
	QVector3D vecPosition;
	for (i=0; i<nLineCounts; i++)
	{
		pLine = pLineSymbol->at(i);
		pArray = pLine->OriginalArray();

		nType = pLine->GetLineStyle()->getType();       //��ȡ SymbolID,��601
		nWidth = pLine->m_LineAttribute.GetLineWidth(); //�߿�
		nPointNum = pArray->size();

		if(nType != 1308) continue;
		QString strRegion = pLine->m_LineAttribute.GetStringValue();
		if(strRegion.compare(tr("����")) == 0) nType = 1;
		else if(strRegion.compare(tr("ѩ��")) == 0)	nType = 2;
		else if(strRegion.compare(tr("�ױ���")) == 0) nType = 4;
		else if(strRegion.compare(tr("����")) == 0) nType = 8;
		else if(strRegion.compare(tr("�����")) == 0) nType = 16;
		else if(strRegion.compare(tr("ɳ����")) == 0) nType = 32;

        *m_pTextStream<<nType<<" "<<(nPointNum+1)<<Qt::endl;

		//д��ά�����
		for (int j=0; j<nPointNum; j++)
		{
			vecPosition = pArray->at(j);
			*m_pTextStream<<vecPosition.x()<<" ";
			*m_pTextStream<<vecPosition.y()<<" ";
            *m_pTextStream<<vecPosition.z()<<Qt::endl;
		}
		vecPosition = pArray->at(0);
		*m_pTextStream<<vecPosition.x()<<" ";
		*m_pTextStream<<vecPosition.y()<<" ";
        *m_pTextStream<<vecPosition.z()<<Qt::endl;
	}//������һ��������
	return true;
}

/************************************************************************
����˵����д��������ݡ�NOTES_SYMBOL��
����˵��������ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
��    �ߣ�renxue
������ڣ�2011��08��09��
************************************************************************/
bool CSaveSymbolToM14::SaveMarkSymbol(SymbolData *pSymbolData)
{
	//if(pSymbolData == NULL) return false;

	//QVector<QTextureFont*> *pMarkSymbol = pSymbolData->getMarkSymbol();
	//if (pMarkSymbol==NULL || pMarkSymbol->size() < 1) return false;
	//
	//int nCounts = pMarkSymbol->size();
	//*m_pTextStream<<"NOTES_SYMBOL: "<<nCounts<<endl; //д��־ͷ
	////�����ݱ��
	//int i, nType;
	//Symbol       *pSymbol;
	//QVector3D    vecPosition;
	//QTextureFont *pText;
	//for (i=0; i<nCounts; i++)
	//{
	//	//�ı���ŵ��Ӧ��ID���
	//	pText = pMarkSymbol->at(i);
	//	nType = 48;
	//	QVector3DArray *pArray = pText->OriginalArray();
	//	QString  txtValue = pText->StringArray();
	//	for (int j=0; j<pArray->size(); j++)
	//	{
	//		//д���
	//		*m_pTextStream<<nType<<" ";				

	//		//дλ������
	//		vecPosition = pArray->at(j);
	//		*m_pTextStream<<vecPosition.x()<<" ";
	//		*m_pTextStream<<vecPosition.y()<<" ";
	//		*m_pTextStream<<vecPosition.z()<<" ";

	//		//�ַ��������ַ����Ƕ�
	//		*m_pTextStream<<txtValue.size()<<" ";
	//		*m_pTextStream<<txtValue<<" ";
	//		*m_pTextStream<<0<<" ";

	//		//���������ȡ��������ơ������С������
	//		QString strFontName = pText->GetFontName();
	//		*m_pTextStream<<strFontName.size()<<" ";
	//		*m_pTextStream<<strFontName<<" ";
	//		*m_pTextStream<<pText->PixelHeight()<<" ";
	//		*m_pTextStream<<1<<" ";
	//		
	//		//��ɫ
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
����˵����j��CIPAS���ű�Ÿ�ΪMicaps�ı��
����˵����nType��CIPAS���ֻ���ű��
��    �ߣ�renxue
������ڣ�2011��08��09��
************************************************************************/
int CSaveSymbolToM14::ChangeCodeToM14(int nType)
{
	if(nType<-1) return nType;

	//�����
	switch(nType)
	{
	case 101://�޷�
		return 43;
	case 103://2-3����
		return 39;
	case 104://3-4����
		return 40;
	case 105://4-5����
		return 36;
	case 106://5-6����
		return 33;
	case 107://6-7����
		return 41;
	case 108://7-8����
		return 34;
	case 109://8-9����
		return 35;
	case 110://9-10����
		return 101;
	case 111://10-11����
		return 102;
	case 112://11-12����
		return 103;		
	case 201://С��
		return 26;		
	case 202://����
		return 47;		
	case 203://����
		return 55;
		
	case 204://����
		return 64;
		
	case 205://����
		return 221;
		
	case 207://����
		return 25;
		
	case 212://�ᶳ��
		return 27;
		
	case 213://����
		return 28;
		
	case 301://Сѩ
		return 23;
		
	case 302://��ѩ
		return 22;
		
	case 303://��ѩ
		return 21;
		
	case 304://��ѩ
		return 66;
		
	case 305://��ѩ
		return 65;
		
	case 306://���ѩ
		return 24;
		
	case 307://�ش�ѩ
		return 231;
		
	case 401://����
		return 201;
		
	case 402://��ɳ
		return 45;
		
	case 403://��ɳ��
		return 46;
		
	case 404://ɳ��
		return 211;
		
	case 501://����
		return 50;
		
	case 502://����
		return 51;
		
	case 503://����
		return 52;
		
	case 504://����
		return 29;
		
	case 505://��
		return 30;		
	case 506://�ױ�
		return 32;		
	case 507://����
		return 31;
		
	case 508://˪��
		return 44;
		
	case 509://��ת��
		return 42;
		
	case 510://̨��
		return 37;
		
	case 1402://��ֵ����
		return 60;
		
	case 1403://��ֵ����
		return 61;
		
	case 1404://������
		return 63;
		
	case 1405://ů����
		return 62;
		
	case 1301://�Ǻ�
		return 152;
		
	case 1302://����Բ
		return 151;
	case 1303://������
		return 153;
		
	case 1304://��������
		return 155;
		
	case 1305://ʵ��Բ
		return 150;
		
	case 1306://ʵ����
		return 154;
		
	case 1307://ʵ����
		return 156;
	
	case 1201://����
		return 0;		
	case 601://���
		return 2;		
	case 602://ů��
		return 3;	
	case 603://������
		return 5;		
	case 604://��ֹ��
		return 4;
	case 1001://���߼�ͷ
		return 1113;
	case 1002://˫�߼�ͷ
		return 1110;		
	case 1003://˫������ͷ
		return 1114;
	case 1004://˫��˫��ͷ
		return 1112;
	case 1005://˫�����˫��ͷ
		return 1115;
	case 1008://˫���м��ͷ
		return 1111;
	case 1202://˪����
		return 38;			
	case 1203://˫ʵ��
		return 1102;

	default:
		return -1;
	}
}
