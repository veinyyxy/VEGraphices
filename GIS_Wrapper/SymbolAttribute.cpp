/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ��ѩ
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����SymbolAttribute.h SymbolAttribute.cpp 
�� ���� CSymbolAttribute
�� �ࣺ ��
�� �ࣺ �� 
����˵�������ݷ��ŵ�����
����˵����

-------------------------------------�汾����------------------------------------------------ 
V 1.0 
ԭ���� ����ѩ
������ڣ�2011��4��18��
============================================================*/

#include "SymbolAttribute.h"


CSymbolAttribute::CSymbolAttribute(void)
{
	m_bFilled = false;//�Ƿ����
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
