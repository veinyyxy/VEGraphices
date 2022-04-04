#pragma once

#include "GIS_WrapperConfig.h"
#include <QtGui/QMessageBox>


class GIS_WRAPPER_EXPORT_IMPORT CCIPASMessageBox : QWidget
{
public:
	CCIPASMessageBox();
	~CCIPASMessageBox(void);

	enum StandardButton
	{
		CIPAS_OK = 0,	//ȷ��		
		CIPAS_Save,		//����
		CIPAS_SaveAll,	//����ȫ��
		CIPAS_Retry,	//����
		CIPAS_OK_Cancle	//ȷ��
	};
	typedef StandardButton CIPAS_MESSAGE;

	static int CIPASQuestion	(QWidget *parent, const CIPAS_MESSAGE message, const QString & title, const QString & text);	//����
	static int CIPASInformation	(QWidget *parent, const CIPAS_MESSAGE message, const QString & title, const QString & text);	//��Ϣ
	static int CIPASWaring		(QWidget *parent, const CIPAS_MESSAGE message, const QString & title, const QString & text);	//����
	static int CIPASCritical	(QWidget *parent, const CIPAS_MESSAGE message, const QString & title, const QString & text);	//����

};

