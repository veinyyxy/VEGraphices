#pragma once
#include <QtWidgets\QTextEdit>
#include "GIS_WrapperConfig.h"


class GIS_WRAPPER_EXPORT_IMPORT CTextBox : public QTextEdit
{
	Q_OBJECT
public:
	CTextBox(QWidget * parent = 0);
	~CTextBox(void);
signals:
	void SigEndInput(QString sInput);                 //鼠标在图层上释放事件
protected:
	void keyPressEvent ( QKeyEvent * e );
};

