#pragma once
/*============================================================ 
��Ŀ����CIPAS 
�� �ߣ�  �Ը���	
�� λ�� css
����޸�ʱ�䣺 2011��3��17��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����
�ɲ������Դ�����
2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ����� DrawWindSymbol.h DrawWindSymbol.cpp 
�� ���� CDrawWindSymbol 
�� �ࣺ  ManualDraw(����) 
�� �ࣺ �� 
����˵���� ʵ�ַ����ƹ���
����˵���� �����˵���������ͼ��ת
-------------------------------------�汾����------------------------------------------------ 
V 0.1 
�� �ߣ��Ը���
������ڣ�
����˵���� 
============================================================*/ 
#include "ManualDraw.h"
//#include "QTextureFont.h"
#include <stdlib.h>
#include "SymbolPoint.h"
#include "ColorAttribute.h"
//#include "UndoAddPointCommand.h"
#include "SymbolData.h"
#include "SymbolLayer.h"

class GIS_WRAPPER_EXPORT_IMPORT CDrawWindSymbol: public ManualDraw
{
public:
	CDrawWindSymbol(void);
	~CDrawWindSymbol(void);
	//����¼���Ӧ
	virtual void MouseEvent(QMouseEvent* event);	//����ƶ���������ת�Ƕȣ����ߣ�������ת�Ƕ�
	virtual void MousePress(QMouseEvent *event);	//�����ʼ����СԲȦ���Ҽ����Ƕ������
	virtual void MouseRelease(QMouseEvent *event);	//
	void LeaveEvent();
	void SetSymbol(Symbol *pSymbol);
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void SetTextOnLine(double dAngle);									//����ʱ��������
	double CalculateAngle();
private :
	Line *m_Circular;						//Բ
	Line m_line;							//����
	//QTextureFont *m_text;					//�ı�
	bool m_bStart;							//�Ƿ�ʼ����
	QVector3D m_CircularCenter;				//the center of cricle���洢�߶����
	QVector3D m_LineEndPoint;				//�洢�߶��յ�
	SymbolPoint *m_symbolpoint;				//��ͼ��ָ��
	GLfloat m_fAngle;						//��ͼ��ת�Ƕ�
	bool m_bFirst;							//�����Ƿ��һ��
	ColorAttribute * m_pColorAttri;			//��ɫ����
	SymbolData *m_nSymbolData;
	SymbolLayer *m_nSymbolLayer;
};

