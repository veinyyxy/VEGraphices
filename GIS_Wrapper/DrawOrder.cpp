/*============================================================ 
��Ŀ����CIPAS 
�� �ߣ� ����	
�� λ�� css
����޸�ʱ�䣺 2011��7��18��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����
�ɲ������Դ�����
2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ����� DrawOrder.h DrawOrder.cpp 
�� ����  CDrawOrder 
�� �ࣺ  ��
�� �ࣺ �� 
����˵���� �жϻ��λ��Ʒ���Ϊ˳ʱ�뷽������ʱ�뷽��
����˵���� �����˵����һ��������������һΪ���жϵĻ��α�����һ������Ϊ�����Ƿ�պϣ�trueΪ�պϣ�falseΪ���պϡ�
-------------------------------------�汾����------------------------------------------------ 
============================================================*/  
#include "DrawOrder.h"


CDrawOrder::CDrawOrder(void)
{
}


CDrawOrder::~CDrawOrder(void)
{
}
/************************************************************************/
/* �������ܣ� �жϻ��λ��Ʒ���						                        */
/* ����˵��: ����1Ϊ���ƵĻ��α�������2Ϊ�����Ƿ�պϣ�����ֵΪtrue���򻡶�Ϊ��ʱ�룬����Ϊ˳ʱ��														*/
/* ��    ��:  ����														*/
/* ��    ��:  2011 �� 07 �� 18 ��										*/
/************************************************************************/
bool CDrawOrder::isConvert(QVector3DArray *pData,bool bisClosed)
{

	int nCount = pData->count();
	QVector3D p1, p2, p3,pMax,pMaxTemp; 
	int i;
	pMax = pData->at(0);
	int pMaxIndex =0;
	for(i=0; i<nCount; i++)	
	{   
		pMaxTemp =  pData->at(i);	  
		if(pMaxTemp.y()>pMax.y())
		{  
			pMax = pMaxTemp;
			pMaxIndex = i;
		}
	}
	if(bisClosed = true)   //���isClosed = true�������߱պϣ���ȡ�������㼴��Χ����
	{
		if(pMaxIndex==nCount-1)
		{
			p1 = pData->at(pMaxIndex-1);
			p2 = pData->at(pMaxIndex);
			p3 = pData->at(0);
		}
		else if (pMaxIndex == 0 )
		{
			p1 = pData->at(nCount-2);
			p2 = pData->at(pMaxIndex);
			p3 = pData->at(pMaxIndex+1);
		}
		else
		{
			p1 = pData->at(pMaxIndex-1);
			p2 = pData->at(pMaxIndex);
			p3 = pData->at(pMaxIndex+1);
		}// ������ȡҪ�༭�ĵȸ��ߵ�y�������ĵ�
	}
	else    //������߲��պϣ���ȡ0�㣬�е㣬�յ�
	{
		p1 = pData->at(0);
		p2 = pData->at(pData->count() / 2);
		p3 = pData->at(pData->count() - 1);
	}

	

	//�ж�ԭ������ʱ�뻹��˳ʱ��
	float fVector =( p2.x() - p1.x()) * (p3.y() - p2.y()) - (p2.y() - p1.y()) *(p3.x() - p2.x());
	bool bIsConvert ; 
	if(fVector > 0 )
	{
		bIsConvert = true;
	}
	else
	{
		bIsConvert = false;
	}
	return bIsConvert;
}