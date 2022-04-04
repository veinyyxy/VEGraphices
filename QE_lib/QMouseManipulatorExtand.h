#pragma once
#include "Config.h"
#include "QMouseManipulator.h"

class QE_LIB_EXPORT_IMPORT QMouseManipulatorExtand :
	public QMouseManipulator
{
public:
	QMouseManipulatorExtand(void);
	~QMouseManipulatorExtand(void);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
	virtual void mouseDoubleClickEvent (QMouseEvent * event);
	virtual void leaveEvent(QEvent * even);
	inline void Extand(QMouseManipulator* pMM){m_pMouseManipulator = pMM;}

	void isExportingPicture(bool isExportPic){m_isExportingPicture = isExportPic;}
	bool isExportingPicture() {return m_isExportingPicture;}
protected:
	QMouseManipulator* m_pMouseManipulator;
	bool m_isExportingPicture;
};

