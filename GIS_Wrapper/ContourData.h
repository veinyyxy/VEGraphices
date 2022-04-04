#pragma once
/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� lugsh
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����CContourData.h CContourData.cpp 
�� ���� CContourData
�� �ࣺ NULL
�� �ࣺ �� 
����˵���� ��¼��ֵ��Ӧ������
����˵���� 

-------------------------------------�汾����------------------------------------------------ 
V 1.2 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ��lugsh
������ڣ�2011�ꣿ�£���
============================================================*/

#include "QBitmapText.h"
#include "SymbolContourLine.h"
class CContourData
{
public:
	CContourData(void);
	virtual ~CContourData(void);

	void SetLineData(CSymbolContourLine *line);								//���õ�ֵ�߱߿�����
	CSymbolContourLine * GetLineData();										//ȡ�õ�ֵ�߱߿����ݵ�
	void SetLineValue(float & value);							//���õ�ֵ��ֵ
	float GetLineValue();										//ȡ�õ�ֵ�ߵ�ֵ
	void SetLineType(QString strType);							//���õ�ֵ������
	QString GetLineType();										//ȡ�õ�ֵ�ߵ�����
	void SetClose(bool bClose) {m_bClose = bClose;}				//���õ�ֵ������
	bool GetClose() {return m_bClose;}							//ȡ�õ�ֵ�ߵ�����
	void SetText1(QBitmapText *pText1) { textValue1 =pText1; }	//�趨��ֵ�߻�ֵ����ָ��
	void SetText2(QBitmapText *pText2) { textValue2 = pText2;}	//�趨��ֵ�߻�ֵ����ָ��
	QBitmapText * GetText1() { return textValue1;}
	QBitmapText * GetText2() { return textValue2;}
private:
	CSymbolContourLine * m_pLine;												//��¼��ǰ���ƺõĵ�ֵ�߶���
	float  m_fValue;											//��¼��ǰ��ֵ�ߵ�ֵ
	QString m_strType;											//��¼��ֵ�ߵ����� ������ߡ���ѹ�ߵ�
	bool m_bClose;												//��־Ϊ��ֵ��
	QBitmapText * textValue1;									//��ֵ�ߵ�ֵһ  �����պϵ�ֵ��ʱ��ֻ�иñ�����ֵ
	QBitmapText * textValue2;									//��ֵ�ߵ�ֵ��  �����պϵ�ֵ��ʱ����ֵΪ��

};

