#pragma once
#include "GIS_WrapperConfig.h"
#include "QFile"
#include "../CDataElement/Station.h"
#include "QtXml/QXmlStreamReader"
#include <QPointer>
#include <QtSql/QSqlQuery>
#include "../QE_lib/QDataArray.h"


/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ��־ѧ
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011��4��12��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����StationInterpretation.h StationInterpretation.cpp 
�� ���� StationInterpretation
�� �ࣺ ��
�� �ࣺ �� 
����˵����վ��XML���ݽ�������
����˵���� 

-------------------------------------�汾����------------------------------------------------ 
V 1.2 
�� �ߣ���־ѧ
������ڣ�4��12
����˵���� 
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ����־ѧ
������ڣ�2011��4��11��
============================================================*/


typedef struct StationInfo                   //վ����Ϣ�ṹ��
{
	double   dvalue ;                        //����Ҫ��ֵ
	QString  id;                             //Id;
	QString  staNumb;						//վ��
	QString  staType;                        //վ������
	QVector3D pSymbolPoints;   //����������    
}StationInfo;

typedef QVector  <StationInfo>  StationDataArray;       //վ����Ϣ�ṹ������

class GIS_WRAPPER_EXPORT_IMPORT StationInterpretation // : public QXmlStreamReader
{
public:
	StationInterpretation(void);
	~StationInterpretation(void);
	   bool readFile(const QString &filename,const int type); //��XML�ļ�
	   bool isEmpty();              //�ж��Ƿ�����
       StationDataArray * getStationPointArray(const int type);  
		//QList<CStation*>   stList; 
private:
	   void readRowDataElement(); // ����һ������0������<ROW>Ԫ�ص�<ROWDATA>Ԫ��
	   void readRowElement();     // ����һ������0����������Ԫ�ص�<ROW>Ԫ�ء�
	QString readC0Element();      // ����һ��IDԪ��
	QString readCHNameElement();      // ����һ��NameԪ��
	QString readV01000Element();      // ����һ��V01000Ԫ��( ����	���֣�����Ҫ�ؾ�Ϊʵ��������Ϊ����վʱ���ֶ�Ϊ��ʿ�ٺ�)
	 double readV05001Element();      // ����һ��V05001Ԫ��(γ��	��λ:�� )
	 double readV06001Element();      // ����һ��V06001Ԫ��(����	��λ����)
	 double readV07001Element();      // ����һ��V07001Ԫ��(��վ�߶�	��λ��M)
	 double readV07033Element();      // ����һ��V07033Ԫ��
	QString readV02001Element();      // ����һ��V02001Ԫ��(̨վ����	�������02001)
	QString readCHAreaProvinceElement();      // ����һ��CH_AREA_PROVINCEԪ��
	QString readCHAreaCityElement();          // ����һ��CH_AREA_CITYԪ��
	QString readCHAreaCountyElement();        // ����һ��CH_AREA_COUNTYԪ��
	QString readBYStatusElement();            // ����һ��BY_STATUSԪ��
	QString readCHOBSTypeElement();           // ����һ��CH_OBS_TYPEԪ��
	QString readCHBusnessTypeElement();       // ����һ��CH_BUSINESS_TYPEԪ��
	QString readCHStartYearElement();         // ����һ��CH_START_YEARԪ��
	QString readCHStartMonthElement();        // ����һ��CH_START_MONTHԪ��
	QString readCHEndYearElement();           // ����һ��CH_END_YEARԪ��
	QString readCHEndMonthElement();          // ����һ��CH_END_MONTHԪ��
	QString readROWIDElement();			      // ����һ��ROWIDԪ��

	   void skipUnknownElement();             // ���Բ���ʶ���Ԫ��
	  // bool addDataIntoDB();                  // ���ڴ����ݿ��������
	  // bool selectFromDB();                   // ���ڴ����ݿ���ȡվ������
	  

private:
	       //CDataBase   db;
	QXmlStreamReader   reader;	 	  
	StationDataArray*  stationPointArray;       //����վ��
	StationDataArray*  stationPointArray1;      //һ��վ��
	StationDataArray*  stationPointArray2;      //����վ��
	StationDataArray*  stationPointArray3;      //��׼վ��
	int m_stationType;

};
