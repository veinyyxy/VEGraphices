#include "StationValues.h"
StationValues::StationValues(void)
{
	m_nindex = 0;
}

StationValues::~StationValues(void)
{
}
/************************************************************************
����˵�����������վ���Ҫ��ֵ
��      �ߣ��Ը���
������ڣ�2011��6��21��
************************************************************************/
void StationValues::SetStationValue(QString strKey,float fvalue)
{
	ElementValue temp;
	temp.strName = strKey;
	temp.fvalue = fvalue;
	m_AllElementValue.insert(m_nindex,temp);
	m_nindex++;
}
/************************************************************************
����˵�����������վ���Ҫ��ֵ
��      �ߣ��Ը���
������ڣ�2011��6��21��
************************************************************************/
QMap<int,ElementValue> * StationValues::GetStationValue()
{
	return &m_AllElementValue;
}


