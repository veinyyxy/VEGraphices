#pragma once
#include "QT_Include.h"
#include "QNode.h"

class QE_LIB_EXPORT_IMPORT QAccessor : public QObject
{
public:
	QAccessor(void);
	virtual ~QAccessor(void);
	virtual int Read(const QDataStream& input) = 0;
	virtual int Read(const QString& filename) = 0;
	virtual int Write(QDataStream& output) = 0;
	virtual int Write(const QString& filename) = 0;
	virtual QNode* Get() = 0;

};


typedef QAccessor* (*P_GetAccessor)();
