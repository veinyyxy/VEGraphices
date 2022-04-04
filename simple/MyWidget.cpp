#include "MyWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>

CMyWidget::CMyWidget(void)
{
}


CMyWidget::~CMyWidget(void)
{
}

void CMyWidget::paintEvent( QPaintEvent *_event )
{
	QPainter localPainter(this);
	QBrush localBrush(Qt::CrossPattern);
	localBrush.setColor(QColor(23, 123, 34, 30));
	QRect rect = _event->rect();
	rect.setLeft(rect.left() + 1);
	rect.setTop(rect.top() + 1);
	rect.setWidth(rect.width()-1);
	rect.setHeight(rect.height() - 1);
	
	localPainter.setBrush(localBrush);
	localPainter.drawRect(rect);
}

void CMyWidget::mousePressEvent( QMouseEvent *_event )
{

}

void CMyWidget::mouseMoveEvent( QMouseEvent *_event )
{

}

void CMyWidget::mouseReleaseEvent( QMouseEvent *_event )
{

}
