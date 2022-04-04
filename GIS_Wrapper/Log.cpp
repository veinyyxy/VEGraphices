#include "Log.h"


CLog::CLog(void)
{
}


CLog::~CLog(void)
{
}

QVector<LOG_ITEM>* CLog::GetLogItem()
{
	return &m_logItem;
}
