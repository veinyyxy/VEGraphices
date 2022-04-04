/*============================================================
文件名： ConvertExponentToCipasFormat.h
类  名： CConvertFormat
父  类： QWidget
子  类： 无
功能说明：将指数文件保存成CIPAS格式的文件
调用说明：
1、头信息中的缺省值使用“-999”；
2、数据集中缺省值使用“999999.00”或“999999”

----------------------------版本更新----------------------------
V 1.0
原作者 ： renxue
完成日期：2011年10月09日
============================================================*/

#pragma once
#include "GIS_WrapperConfig.h"
#include <Qtcore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QFileDialog>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QApplication>
#include "../GIS_Wrapper/Const.h"

class GIS_WRAPPER_EXPORT_IMPORT CConvertFormat
{
public:
	CConvertFormat(void);
	~CConvertFormat(void);

	/************************************************************************
     功能说明：设置需要读入的文件的文件路径和文件名
     参数说明：fileName是包含路径和文件名的字符串
              返回值为bool型，正确执行返回true，错误或异常返回false
     作    者：renxue
     完成日期：2011年10月10日
    ************************************************************************/
	bool SetSourceFileName(QString fileName);

private:

	/************************************************************************
     功能说明：读取指数文件
     参数说明：fileName是包含路径和文件名的字符串
              返回值为bool型，正确执行返回true，错误或异常返回false
     作    者：renxue
     完成日期：2011年10月09日
    ************************************************************************/
	bool ReadExponentFile(QString fileName);

	/************************************************************************
     功能说明：写入指数文件的年月信息
     参数说明：nYear是文件头信息中的年数据
              返回值为bool型，正确执行返回true，错误或异常返回false
     作    者：renxue
     完成日期：2011年10月09日
    ************************************************************************/
	void WriteDate(int &nYear, int &nMonth, QTextStream *ptxtStream);
	void WriteRC015FileHead(QTextStream *ptxtStream);
	void WriteRS015FileHead(QTextStream *ptxtStream);
	void WriteTC008FileHead(QTextStream *ptxtStream);
	void WriteHC074FileHead(QTextStream *ptxtStream);
	void WriteSC009FileHead(QTextStream *ptxtStream);
	bool ConvertRC015(QTextStream &txtStream);
	bool ConvertRS015(QTextStream &txtStream);
	bool ConvertTC008(QTextStream &txtStream);
	bool ConvertHC074(QTextStream &txtStream);
	bool ConvertSC009(QTextStream &txtStream);

private:
	int          *m_pIntData;
	QFile        *m_pFile;
	QString      m_FileName;
	QTextStream  m_txtStream;
	QTextStream  *m_pDestTxtStream;
};