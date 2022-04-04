#include "CIPASMessageBox.h"

CCIPASMessageBox::CCIPASMessageBox()
{

}

CCIPASMessageBox::~CCIPASMessageBox(void)
{
}

int CCIPASMessageBox::CIPASWaring( QWidget *parent, const CIPAS_MESSAGE message, const QString & title, const QString & text )
{
	int ret=0;
	switch (message)
	{
	case 0:
		ret = QMessageBox::warning(parent, title, text);
		break;
	case 1:
		ret = QMessageBox::warning(parent, title, text, QMessageBox::Save, QMessageBox::Cancel);
		break;
	case 2:
		ret = QMessageBox::warning(parent, title, text, QMessageBox::SaveAll, QMessageBox::Save, QMessageBox::Cancel);
		break;
	case 3:
		ret = QMessageBox::warning(parent, title, text, QMessageBox::Retry, QMessageBox::Cancel);
		break;
	case 4:
		ret = QMessageBox::warning(parent, title, text, QMessageBox::Ok, QMessageBox::Cancel);
		break;
	default:
		break;
	}
	return ret;
}

int CCIPASMessageBox::CIPASQuestion( QWidget *parent, const CIPAS_MESSAGE message, const QString & title, const QString & text )
{
	int ret=0;
	switch (message)
	{
	case 0:
		ret = QMessageBox::question(parent, title, text);
		break;
	case 1:
		ret = QMessageBox::question(parent, title, text, QMessageBox::Save, QMessageBox::Cancel);
		break;
	case 2:
		ret = QMessageBox::question(parent, title, text, QMessageBox::SaveAll, QMessageBox::Save, QMessageBox::Cancel);
		break;
	case 3:
		ret = QMessageBox::question(parent, title, text, QMessageBox::Retry, QMessageBox::Cancel);
		break;
	case 4:
		ret = QMessageBox::question(parent, title, text, QMessageBox::Ok, QMessageBox::Cancel);
		break;
	default:
		break;
	}
	return ret;
}

int CCIPASMessageBox::CIPASInformation( QWidget *parent, const CIPAS_MESSAGE message, const QString & title, const QString & text )
{
	int ret=0;
	switch (message)
	{
	case 0:
		ret = QMessageBox::information(parent, title, text);
		break;
	case 1:
		ret = QMessageBox::information(parent, title, text, QMessageBox::Save, QMessageBox::Cancel);
		break;
	case 2:
		ret = QMessageBox::information(parent, title, text, QMessageBox::SaveAll, QMessageBox::Save, QMessageBox::Cancel);
		break;
	case 3:
		ret = QMessageBox::information(parent, title, text, QMessageBox::Retry, QMessageBox::Cancel);
		break;
	case 4:
		ret = QMessageBox::information(parent, title, text, QMessageBox::Ok, QMessageBox::Cancel);
		break;
	default:
		break;
	}
	return ret;
}

int CCIPASMessageBox::CIPASCritical( QWidget *parent, const CIPAS_MESSAGE message, const QString & title, const QString & text )
{
	int ret=0;
	switch (message)
	{
	case 0:
		ret = QMessageBox::critical(parent, title, text);
		break;
	case 1:
		ret = QMessageBox::critical(parent, title, text, QMessageBox::Save, QMessageBox::Cancel);
		break;
	case 2:
		ret = QMessageBox::critical(parent, title, text, QMessageBox::SaveAll, QMessageBox::Save, QMessageBox::Cancel);
		break;
	case 3:
		ret = QMessageBox::critical(parent, title, text, QMessageBox::Retry, QMessageBox::Cancel);
		break;
	case 4:
		ret = QMessageBox::critical(parent, title, text, QMessageBox::Ok, QMessageBox::Cancel);
		break;
	default:
		break;
	}
	return ret;
}