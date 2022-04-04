#pragma once
#include <QtPlugin>
#include <QString>
#include "Nccclosedcontour.h"
#include "dem.h"
#include "tin.h"
#include "NccStation.h"
using namespace NCC;
class FunPluginInterface
{
	
public:
	//FunPluginInterface(void);
	//~FunPluginInterface(void);
	virtual QString GetString()const=0;
	
	//���Ƶ�δ����
	virtual bool GetSortedContour(QList<NccClosedContour> *closedContourList,QList<NccClosedContour> *sortedClosedContourList)=0;
	//����ֵ���������ڵ�վ��ֵ
	virtual bool ProcessPointsNonInterpol(QList<NccStation> *stationList,QList<NccClosedContour> *sortedClosedContourList)=0;
		
	virtual bool GetContourFromQList(QList<NccClosedContour> *closedContourList, CBlockContour *pBlockContour)=0;

	//������Ƶ�
	/*************************************************
	Function:       GetContourFromQListPoint
	Description:    ��ȡ���Ƶ�
	Input:          ���Ƶ㼯��
					��ֵ�߼��ϵ�ַ
	Output:         
	Return:         true
	Others:         
	*************************************************/
	virtual bool GetContourFromQListPoint(QList<NccPoint> pointList, CBlockContour *pBlockContour)=0;
	
	//֧�ִ����Ƶ�����
	virtual bool CreateTin(CBlockContour* pBlockContour, CBlockTIN* pBlockTIN)=0;
	//���ش�����վ�㼯��
	virtual bool TriangleInterpol(QList<NccStation> *stationList,CBlockTIN* pBlockTIN)=0;
	//�жϵ��Ƿ��ڶ�����ڲ�
	virtual bool bIsPointInPolygon(QList<NccPoint> pointList, NccPoint* pt)=0;
	//����������� 2d
	virtual double ProfileArea2D(QList<NccPoint> pointList)=0;

};

Q_DECLARE_INTERFACE(FunPluginInterface, "CSS.NCC.FunPluginInterface") 
