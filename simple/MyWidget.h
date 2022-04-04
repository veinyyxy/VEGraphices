#pragma once
#include <QWidget>

class CMyWidget : public QWidget
{
public:
	CMyWidget(void);
	virtual ~CMyWidget(void);
protected:
	void paintEvent(QPaintEvent *_event);
	void mousePressEvent(QMouseEvent *_event);
	void mouseMoveEvent(QMouseEvent *_event);
	void mouseReleaseEvent(QMouseEvent *_event);
};

