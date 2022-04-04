/*============================================================ 
项目名： 人机交互平台
作 者： 任雪
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：SymbolAttribute.h SymbolAttribute.cpp 
类 名： CSymbolAttribute
父 类： 无
子 类： 无 
功能说明：传递符号的属性
调用说明：

-------------------------------------版本更新------------------------------------------------ 
V 1.0 
原作者 ：任雪
完成日期：2011年4月18日
============================================================*/

#include "SymbolAttribute.h"


CSymbolAttribute::CSymbolAttribute(void)
{
	m_bFilled = false;//是否填充
	m_nColor.setX(300);
	m_nColor.setY(300);
	m_nColor.setZ(300);
	m_nLineWidth = 19;
	m_LineType = QString("1");
	m_nSymbolType = 0;
}


CSymbolAttribute::~CSymbolAttribute(void)
{
}

void  CSymbolAttribute::SetColor(QVector4D color)
{
	m_nColor = color;
}

void  CSymbolAttribute::SetLineWidth(int width)
{
	m_nLineWidth = width;
}

void  CSymbolAttribute::SetLineType(QString Type)
{
	m_LineType = Type;
}

void  CSymbolAttribute::SetSymbolType(int type)
{
	m_nSymbolType = type;
}

void  CSymbolAttribute::SetFillType(bool filled)
{
	m_bFilled = filled;
}

void CSymbolAttribute::SetValue(float fValue)
{
	m_fValue = fValue;
}

void CSymbolAttribute::SetValue(QString strValue)
{
	m_strValue = strValue;
}

QVector4D CSymbolAttribute::GetColor()
{
	return m_nColor;
}

int  CSymbolAttribute::GetLineWidth()
{
	return m_nLineWidth;
}

QString  CSymbolAttribute::GetLineType()
{
	return m_LineType;
}

int  CSymbolAttribute::GetSymbolType()
{
	return m_nSymbolType;
}

bool CSymbolAttribute::GetFillType()
{
	return m_bFilled;
}

float  CSymbolAttribute::GetValue()
{
	return m_fValue;
}

QString CSymbolAttribute::GetStringValue()
{
	return m_strValue;
}

QVector4D CSymbolAttribute::GetFillColor()
{
	return m_nFillColor;
}

void CSymbolAttribute::SetFillColor( QVector4D color )
{
	m_nFillColor = color;
}
