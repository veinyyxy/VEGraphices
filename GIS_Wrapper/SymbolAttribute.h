/*============================================================ 
�ļ�����SymbolAttribute.h SymbolAttribute.cpp 
�� ���� CSymbolAttribute
�� �ࣺ ��
�� �ࣺ �� 
����˵�������ݷ��ŵ�����
����˵����
----------------------------�汾����-------------------------- 
V 1.0 
ԭ���� ����ѩ
������ڣ�2011��4��18��
============================================================*/

#pragma once
#include "ILayerAttribute.h"

class GIS_WRAPPER_EXPORT_IMPORT CSymbolAttribute
{
public:
	CSymbolAttribute(void);
	~CSymbolAttribute(void);
	void SetColor(QVector4D color);      //������ɫ
	void SetLineWidth(int width);        //�����߿�
	void SetLineType(QString Type);      //��������
	void SetSymbolType(int type);        //�����ߵ����ͣ����ͷ��˫�ߵ�
	void SetFillType(bool filled);       //��������־��true��ʾ��䣻
	void SetValue(float  fValue);        //�����ߵı�ֵ��
	void SetValue(QString strValue);     //��������ֵ�������εı�ֵ��
	void SetFillColor(QVector4D color);  //���������ɫ
	void SetLayerZ(float Z){m_LayerZ = Z;}//����ͼ���Zֵ

	QVector4D GetColor();                  //���������ɫ
	QVector4D GetFillColor();              //��ȡ�����ɫ
	int       GetLineWidth();              //��������߿�	
	int       GetSymbolType();             //��õ����ͣ����ͷ��˫�ߵ�
	int       GetLayerZ(){return m_LayerZ;}
	bool      GetFillType();               //�������ֵ��true��ʾ��䣻
	float     GetValue();                  //����ߵı�ֵ��
	QString   GetLineType();               //����������ͣ�
	QString   GetStringValue();            //���String���͵ı�ֵ��

protected:	
	QVector4D m_nColor;             //��ɫ
	QVector4D m_nFillColor;         //�����ɫ
	float     m_LayerZ;             //ͼ���Zֵ
	int       m_nLineWidth;         //�߿�	
	int       m_nSymbolType;        //��������
	bool      m_bFilled;            //�Ƿ����
	float     m_fValue;             //�ߵı�ֵ�����ֵ�ߣ�
	QString   m_LineType;           //����
	QString   m_strValue;           //����
};

