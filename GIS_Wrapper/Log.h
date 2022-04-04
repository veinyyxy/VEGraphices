#pragma once
#include "GIS_WrapperConfig.h"
#include "QT_Include.h"

typedef struct LogItem
{
	QString ItemName;
	QString ItemTimeData;
	QString ItemContent;
	QString ItemLogLevel;
} LOG_ITEM, P_LOG_ITEM;

class GIS_WRAPPER_EXPORT_IMPORT CLog : public QObject
{
public:
	CLog(void);
	~CLog(void);
	virtual int WriteLog() = 0;
	virtual int ReadLog() = 0;
	virtual int OpenLog() = 0;
	virtual int CloseLog() = 0;
	QVector<LOG_ITEM>* GetLogItem();

protected:
	QVector<LOG_ITEM> m_logItem;
};

