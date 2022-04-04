#include "StationValues.h"
StationValues::StationValues(void)
{
	m_nindex = 0;
}

StationValues::~StationValues(void)
{
}
/************************************************************************
功能说明：存入这个站点的要素值
作      者：赵高攀
完成日期：2011年6月21日
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
功能说明：返回这个站点的要素值
作      者：赵高攀
完成日期：2011年6月21日
************************************************************************/
QMap<int,ElementValue> * StationValues::GetStationValue()
{
	return &m_AllElementValue;
}


