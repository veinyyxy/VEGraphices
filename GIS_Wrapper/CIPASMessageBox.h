#pragma once

#include "GIS_WrapperConfig.h"
#include <QtWidgets/QMessageBox>


class GIS_WRAPPER_EXPORT_IMPORT CCIPASMessageBox : QWidget
{
public:
	CCIPASMessageBox();
	~CCIPASMessageBox(void);

	enum StandardButton
	{
		CIPAS_OK = 0,	//确认		
		CIPAS_Save,		//保存
		CIPAS_SaveAll,	//保存全部
		CIPAS_Retry,	//重试
		CIPAS_OK_Cancle	//确认
	};
	typedef StandardButton CIPAS_MESSAGE;

	static int CIPASQuestion	(QWidget *parent, const CIPAS_MESSAGE message, const QString & title, const QString & text);	//疑问
	static int CIPASInformation	(QWidget *parent, const CIPAS_MESSAGE message, const QString & title, const QString & text);	//消息
	static int CIPASWaring		(QWidget *parent, const CIPAS_MESSAGE message, const QString & title, const QString & text);	//警告
	static int CIPASCritical	(QWidget *parent, const CIPAS_MESSAGE message, const QString & title, const QString & text);	//错误

};

