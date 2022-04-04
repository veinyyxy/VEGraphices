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
�ļ�����CustomizeArea.h CustomizeArea.cpp 
�� ���� CCustomizeArea
�� �ࣺ ManualDraw
�� �ࣺ �� 
����˵������ѡ����
����˵����
1��Ŀǰ���þ��εķ�ʽ����Ļ��ѡ��һ��������
2����

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
ԭ���� ����ѩ
������ڣ�2011�ꣿ�£���
============================================================*/


#pragma once
#include "manualdraw.h"
#include "Line.h"
#include "SymbolLine.h"


class GIS_WRAPPER_EXPORT_IMPORT CCustomizeArea :	public ManualDraw
{
public:
	CCustomizeArea(void);
	~CCustomizeArea(void);

	void MouseEvent(QMouseEvent* event)  ;
	void MousePress(QMouseEvent *event) ;
	void MouseRelease(QMouseEvent *event);
	void SetSymbol(Symbol *pSymbol) {m_Symbol = pSymbol;} ;
	void setILayerAttrib(ILayerAttribute *Ilayer);
private:
	QVector3D topLeftPos, bottomRightPos;
	Line *pLine;
	QVector3DArray *pArray;
	bool  bFinished ;			//�ж��Ƿ����
	CSymbolLine *m_symbolLine;
	int   nNum;
};

