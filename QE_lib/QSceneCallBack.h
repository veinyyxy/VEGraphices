#pragma once
#include "Config.h"
#include "QT_Include.h"
#include "QObserver.h"
#include "QRenderWindow.h"
#include "QTransform3D.h"

class QE_LIB_EXPORT_IMPORT QSceneCallBack : public QObject
{
public:
	QSceneCallBack(void);
	virtual ~QSceneCallBack(void);
	virtual void ApplyObserver(const QString name, QObserver* observer);
	virtual void ApplyRenderWindow(QRenderWindow* renderWin);
	virtual void ApplyTransform(QTransform3D* pTrans);
};

