/*============================================================ 
项目名： 人机交互平台
作 者： 杨东
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：SaveSymbolToXML.h SaveSymbolToXML.cpp 
类 名： SaveSymbolToXML
父 类： QWidget
子 类： 无 
功能说明：将编辑符号写入XML
调用说明：

-------------------------------------版本更新------------------------------------------------ 
V 1.0 
原作者 ：杨东
完成日期：2011年5月3日

V 1.1 
作 者：
完成日期：
更新说明：
============================================================*/
#pragma once
#include "GIS_WrapperConfig.h"
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QXmlStreamWriter>
#include "SymbolData.h"
class GIS_WRAPPER_EXPORT_IMPORT SaveSymbolToXML : public QWidget
{
public:
	SaveSymbolToXML(void);
	~SaveSymbolToXML(void);
	void SaveSymbolToXMLFile(SymbolData *symbolData,QString fileName);
private:
	bool beginWrite();
	void writeFileHead();
	bool endWrite();
	void SavePointSymbolToXML(SymbolData *symbolData); 
	void SaveLineSymbolToXML(SymbolData *symbolData);
	void SaveMultiLineSymbolToXML(SymbolData *symbolData);
	void SaveNormalSymbolToXML(SymbolData *symbolData);
	void SaveContourLineToXML(SymbolData *symbolData);
	void SaveRegionSymbolToXML(SymbolData *symbolData);
	void SaveMarkSymbolToXML(SymbolData *symbolData);

private:
	QFile *m_pFile;
	QXmlStreamWriter *m_pXmlWriter;
	QString m_fileName;
};

