#pragma once
#include "Config.h"
#include <QtGui/QMouseEvent>
#include "QMouseManipulator.h"
class QE_LIB_EXPORT_IMPORT QDefaultMouseManipulator :
	public QMouseManipulator
{
public:
	QDefaultMouseManipulator(void);
	~QDefaultMouseManipulator(void);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseDoubleClickEvent (QMouseEvent * event);
	virtual void wheelEvent(QWheelEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
	virtual void leaveEvent(QEvent * even);
private:
	QPoint lastPos;
};

