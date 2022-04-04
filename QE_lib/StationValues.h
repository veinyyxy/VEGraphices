
/*============================================================ 
��Ŀ����CIPAS 
�� �ߣ� �Ը���	
�� λ�� css
����޸�ʱ�䣺 2011��6��15��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����
�ɲ������Դ�����
2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ����� StationValues.h StationValues.cpp 
�� ����  StationValues 
�� �ࣺ  
�� �ࣺ �� 
����˵���� �洢ĳ��վ������Ҫ�ص�ֵ
����˵���� 
-------------------------------------�汾����------------------------------------------------ 
V 0.1
�� ��: �Ը���
�������: 2011-6-15
����˵��: 
----------------------------------------------------------------------------------------
V 0.0 
�� �ߣ�
������ڣ�
����˵���� 
============================================================*/ 
#pragma once
#include "QRenderUnit.h"
struct ElementValue
{
	QString strName;
	float fvalue;
};
class QE_LIB_EXPORT_IMPORT StationValues
{
public:
	StationValues(void);
	~StationValues(void);
	void SetStationValue(QString strKey,float fvalue);	
	QMap<int,ElementValue>* GetStationValue();
	int GetIndex() {return m_nindex;}

private:
	QMap<int,ElementValue> m_AllElementValue;
	int m_nindex;
};

