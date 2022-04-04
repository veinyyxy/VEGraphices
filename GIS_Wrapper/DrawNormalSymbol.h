/*============================================================ 
��Ŀ����CIPAS 
�� �ߣ� �	
�� λ�� css
����޸�ʱ�䣺 2011��3��19��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����
�ɲ������Դ�����
2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ����� DrawNormalSymbol.h DrawNormalSymbol.cpp 
�� ����  DrawNormalSymbol 
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
#include "AlgorithmInterface.h"
#include "SymbolLine.h"
#include "CardinalSpline.h"
#include "SymbolPoint.h"
#include "NormalSymbol.h"
#include "SymbolLayer.h"
#include "SymbolAttribute.h"
//#include "UndoAddPointCommand.h"

class GIS_WRAPPER_EXPORT_IMPORT DrawNormalSymbol : public ManualDraw
{
public:
	DrawNormalSymbol(void);
	~DrawNormalSymbol(void);

	void MouseEvent(QMouseEvent* event);
	void MousePress(QMouseEvent *event);
	void MouseRelease(QMouseEvent *event);
	void LeaveEvent();
	void SetSymbol(Symbol *pSymbol) {m_Symbol = pSymbol;}
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void initAttribute(int nType);
	void SetParentLayer(Layer * Layer){m_ParentLayer = Layer;}
	void GetAttribute();

private:
	QPoint lastPos;
	Line  *plineSymbol, *line;
	bool  bFinished ;			//�ж��Ƿ��������
	QVector3DArray *pArray ;
	QVector3DArray m_InputData, m_OutputData;
	Line *m_symbolLine;
	QVector<QVector3DArray> *pOutputData;
	int m_OutLineNum;//��¼���������ߵĸ���
	bool m_bFirst;
	CCardinalSpline * m_pSpLine;
	SymbolPoint *symbolpoint0;
	SymbolPoint *symbolpoint1;
	SymbolPoint *symbolpoint2;
	SymbolPoint *symbolpoint3;
	SymbolPoint *symbolpoint4;
	SymbolPoint *symbolpoint5;
	SymbolPoint *symbolpoint6;
	LAYER_ATTRIBUTE *m_CSlayerAttribute;
	ILayerAttribute *m_nLayerAttr;
	bool b1;
	bool b2;
	NormalSymbol *symbolpoint;
	SymbolData * symboldata;
	SymbolLayer *nsLayer;
	QVector<NormalSymbol*> ns;
	CSymbolAttribute *pAttri;
};