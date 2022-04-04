/*============================================================ 
��Ŀ����CIPAS 
�� �ߣ� �	
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
�ļ����� DrawTroughLine.h DrawTroughLine.cpp 
�� ����  DrawTroughLine 
�� �ࣺ  ManualDraw(����) 
�� �ࣺ �� 
����˵���� ʵ�ֲ��ߵ��ֻ湦��
����˵���� �����˵���������õ�ֵ�ߵ����Գ�Ա��
-------------------------------------�汾����------------------------------------------------ 
V 0.1
�� ��: 
�������: 
����˵��: 
----------------------------------------------------------------------------------------
V 0.0 
�� �ߣ�
������ڣ�
����˵���� 
============================================================*/ 

#pragma once
#include "manualdraw.h"
#include "Line.h"
#include "..\PluginSymbol\AlgorithmInterface.h"
#include "SymbolLine.h"
#include "CardinalSpline.h"
#include "SymbolData.h"
#include "SymbolLayer.h"

class GIS_WRAPPER_EXPORT_IMPORT DrawTroughLine :public ManualDraw
{
public:
	DrawTroughLine(int type);
	~DrawTroughLine(void);

	void MouseEvent(QMouseEvent* event);
	void MousePress(QMouseEvent *event);
	void MouseRelease(QMouseEvent *event);
	void LeaveEvent();
	void SetSymbol(Symbol *pSymbol) {m_Symbol = pSymbol;}
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void initAttribute(int nType);
	void SetParentLayer(Layer * Layer){m_ParentLayer = Layer;}

private:
	QPoint lastPos;
	Line  *plineSymbol, *line;
	bool  bFinished ;			//�ж��Ƿ��������
	QVector3DArray *pArray ;
	QVector3DArray m_InputData, m_OutputData;
	Line *m_symbolLine;
	QVector<QVector3DArray> *pOutputData;
	int m_OutLineNum;//��¼���������ߵĸ���
	//bool m_bFirst;
	CCardinalSpline * m_pSpLine;
	SymbolData *m_pSymbolData;
};