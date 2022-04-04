#include "TextBox.h"
#include <QtGui/QKeyEvent>


CTextBox::~CTextBox(void)
{
}

CTextBox::CTextBox(QWidget * parent):QTextEdit(parent)
{
	setWordWrapMode(QTextOption::NoWrap);
}

void CTextBox::keyPressEvent( QKeyEvent * e )
{
	if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
	{
		QString strInput = this->toPlainText();
		emit SigEndInput(strInput);
	}else
	{
		QTextEdit::keyPressEvent(e);
	}
}