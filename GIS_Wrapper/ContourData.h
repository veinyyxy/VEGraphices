#pragma once
/*============================================================ 
项目名： 人机交互平台
作 者： lugsh
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：CContourData.h CContourData.cpp 
类 名： CContourData
父 类： NULL
子 类： 无 
功能说明： 记录等值对应的数据
调用说明： 

-------------------------------------版本更新------------------------------------------------ 
V 1.2 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：lugsh
完成日期：2011年？月？日
============================================================*/

#include "QBitmapText.h"
#include "SymbolContourLine.h"
class CContourData
{
public:
	CContourData(void);
	virtual ~CContourData(void);

	void SetLineData(CSymbolContourLine *line);								//设置等值线边框数据
	CSymbolContourLine * GetLineData();										//取得等值线边框数据点
	void SetLineValue(float & value);							//设置等值线值
	float GetLineValue();										//取得等值线的值
	void SetLineType(QString strType);							//设置等值线类型
	QString GetLineType();										//取得等值线的类型
	void SetClose(bool bClose) {m_bClose = bClose;}				//设置等值线类型
	bool GetClose() {return m_bClose;}							//取得等值线的类型
	void SetText1(QBitmapText *pText1) { textValue1 =pText1; }	//设定等值线画值对象指针
	void SetText2(QBitmapText *pText2) { textValue2 = pText2;}	//设定等值线画值对象指针
	QBitmapText * GetText1() { return textValue1;}
	QBitmapText * GetText2() { return textValue2;}
private:
	CSymbolContourLine * m_pLine;												//记录当前绘制好的等值线对象
	float  m_fValue;											//记录当前等值线的值
	QString m_strType;											//记录等值线的类型 如等温线、等压线等
	bool m_bClose;												//标志为等值线
	QBitmapText * textValue1;									//等值线的值一  如若闭合等值线时，只有该变量有值
	QBitmapText * textValue2;									//等值线的值二  如若闭合等值线时，该值为空

};

