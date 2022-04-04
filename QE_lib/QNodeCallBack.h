#pragma once

#include "Config.h"
#include "QT_Include.h"
#include "QTransform3D.h"
#include "QObserver.h"
#include "QERenderContext.h"

class QNode;

class QE_LIB_EXPORT_IMPORT QNodeCallBack: public QObject
{
public:
	QNodeCallBack(void);
	virtual ~QNodeCallBack(void);
	virtual bool IsRender() = 0;
	virtual void StartRender(QERenderContext* renderContext) = 0;
	virtual void EndRender() = 0;
	virtual void Apply(QNode& node) = 0;
protected:
	QERenderContext* m_pRenderContext;
};

