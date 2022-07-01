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
�ļ�����SymbleLayer.h SymbleLayer.cpp
�� ���� SymbleLayer
�� �ࣺ ��
�� �ࣺ ��
����˵����վ���
����˵����

-------------------------------------�汾����------------------------------------------------
V 1.3
�� �ߣ�¬�ߕN
������ڣ�4��14
����˵�����޸ķ��Ŵ洢��ʽ
V 1.2
�� �ߣ���־ѧ
������ڣ�4��14
����˵����
V 1.1
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵����
V 1.0
ԭ���� ����־ѧ
������ڣ�2011��4��14��
============================================================*/
#pragma once
#include "vectorlayer.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QPluginLoader>
#include "Nccstation.h"
#include "Nccclosedcontour.h"
#include "Point.h"
#include "ContourData.h"
#include "FunPluginInterface.h"
//#include "StationInterpretation.h"
#include "SymbolData.h"
//#include "Nccstation.cpp"

#include "../GIS_Wrapper/Const.h"

using namespace NCC;

class Nccstation;

class COLOR_PROPERTY
{
public:
	COLOR_PROPERTY(QString Name,QColor color,int Zcount):_Name(Name),_color(color),_Zcount(Zcount){}
	COLOR_PROPERTY(){}
	~COLOR_PROPERTY() {}
public:
	QString _Name;
	QColor _color;
	int _Zcount;
};

typedef QMap<double,COLOR_PROPERTY> COLORTABLE;				//��ɫ��
//typedef QMap<double,QColor> COLORTABLE;				//��ɫ��
typedef QMap<QString,COLORTABLE> COLORTABLELIST;    //��ɫ���б��洢�����ɫ������ݽṹ
static COLORTABLELIST colortablelist;

class GIS_WRAPPER_EXPORT_IMPORT SymbolLayer :
	public VectorLayer
{
	Q_OBJECT
public:
	SymbolLayer(void);
	~SymbolLayer(void);
	//void setMapTree(MapTree* mapTree);
	//QVector<CSymbolContourLine*>  m_contArray;								//����ѡ�ĵ�ֵ��
	/*StationDataArray  *m_stArray;*/
	//StationInterpretation * GetStation();
	QList<NccStation> stationList;//վ��List���⣩
	QList<NccClosedContour> closedContourList;//��ֵ��List���⣩
public:
	bool addData(const QString filepath,const int type);					// �������
	void  DrawStation();													// ����վ��
	void  StartProfile(QVector3D & point);									//��ʼ����ѡ��
	void  ProcessProfile(QVector3D & point);								//����ѡ��
	void  EndProfile(QVector3D & point);									//����ѡ��
	//void UpdateLayer(QGLWidget* widget);
	void UpdateLayer(ILayerAttribute* iLA);
	//StationDataArray*  GlobalStationPoints(const int staType=4);
	void  Inversion(bool isGlobal);											//����
	SymbolData* getSymbolData();					//��ô������
	inline void setSymbolData(SymbolData* sd){m_SymobolData = sd;}
	void  SelectContourLines(QVector<CSymbolContourLine *> &m_contArray);
	QList<NccStation>& GetStationList(bool isGlobal,const int staType=4);
	QList<NccClosedContour>  GetClosedContour(bool isGlobal);
	void SetStationType(int staType=4);
	void SetIsInterpol(bool isInterpol);
	void DisplayInverRestult(QString);// ���ؿ�ѡ�ĵ�ֵ��
	inline void RemoveSymbolContourLine();
	inline void ReloadSymbolContourLine(bool isfill);

public:
	virtual void InitAttribute();
	
private:
	void  DrawID();															// ����ID
	int   getAttribut();													// ������Щվ��
#if 0
	StationDataArray*  SelectStationPoints(const int staType=4);								// ���ؿ�ѡ��վ������
#endif
	int initMutualLayerAttribute();
private:
	int  bDraw;
	//StationInterpretation  *stationData;									//վ������Դ
	Point  *stPoint;
	GISPolygon  *m_pRect;													//���ڻ���ѡ��
	bool  m_bStart;															//��ʶΪ��ʼ����ѡ��
	QVector3D  m_IniPoint;													//ѡ�����Ͻǵ�
	QVector3D  m_EndniPoint;												//ѡ�����½ǵ�
	Proj4Transform  *m_Proj4Trans;											//���ڽ�ͶӰ����ת���ɾ�γ����
	bool isGlobal;
	SymbolData*   m_SymobolData;
//	MapTree* m_MapTree;
	Layer* pLayer;
	bool bFirst;															//��һ�λ���
	int m_StaType;															//վ������ ���� һ��ȵ�
	bool m_IsInterpol;														//�Ƿ��ֵ
signals:
	void KXsingals(QList<NccClosedContour> *closedContourList,QList<NccStation> *stationList, bool isInterpol);
};
