/*============================================================ 
文件名：SaveSymbolToM14File.h  
类  名： CSaveSymbolToM14File
父  类： QWidget
子  类： 无 
功能说明：将手绘符号保存成Micaps14类数据文件的格式
调用说明：

----------------------------版本更新----------------------------
V 1.0 
原作者 ：renxue
完成日期：2011年08月08日
============================================================*/


#pragma once
#include "GIS_WrapperConfig.h"
#include <Qtcore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QFileDialog>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

#include "SymbolData.h"


class GIS_WRAPPER_EXPORT_IMPORT CSaveSymbolToM14 : public QWidget
{
public:
	CSaveSymbolToM14(void);
	~CSaveSymbolToM14(void);

	/*************************************************************
	功能说明：将手绘符号保存成Micaps十四类数据格式的文件
	参数说明：fileName是包含路径和文件名的字符串
	          返回值为bool型，正确执行返回true，错误或异常返回false
	**************************************************************/
	bool SaveSymbolToM14File(QString fileName, SymbolData *pSymbolData);

private:
	void WriteFileHead();
	bool SavePointSymbol(SymbolData *pSymbolData); 
	bool SaveLineSymbol(SymbolData *pSymbolData);
	bool SaveContourSymbol(SymbolData *pSymbolData, bool bFlag);
	bool SavePolygonSymbol(SymbolData *pSymbolData);
	bool SaveMarkSymbol(SymbolData *pSymbolData);
	int  ChangeCodeToM14(int nType);

private:
	QFile        *m_pFile;
	QString      m_FileName;
	QTextStream  *m_pTextStream;
};
