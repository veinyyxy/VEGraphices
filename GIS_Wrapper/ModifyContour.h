/*============================================================ 
��Ŀ����CIPAS 
�� �ߣ� ¬�ߕN	
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
�ļ����� ModifyContour.h ModifyContour.cpp 
�� ����  CModifyContour 
�� �ࣺ  ManualDraw(����) 
�� �ࣺ �� 
����˵���� �޸ĵ�ֵ��
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
//#include "UndoSymbolLineMoveCommand.h"
//#include "UndoContourEditCommand.h"
//#include "UndoMultiContourEditCommand.h"
//#include "UndoDeleteDrawCommand.h"
//#include "UndoDelPointCommand.h"
//#include "UndoSymbolPointMoveCommand.h"
#include "CardinalSpline.h"
#include "DrawOrder.h"
#include "CLineAnalysis.h"
#include "SymbolMultiContourLine.h"

class GIS_WRAPPER_EXPORT_IMPORT CModifyContour :public ManualDraw
{
	Q_OBJECT
public:
	CModifyContour(void);
	~CModifyContour(void);
	void MouseEvent(QMouseEvent* event);
	void MousePress(QMouseEvent *event);
	void MouseRelease(QMouseEvent *event);
	void LeaveEvent();
	void MouseDoubleClick(QMouseEvent *event);			//���˫���¼�

	void SetSymbol(Symbol *pSymbol) {m_Symbol = pSymbol;}
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void SetContourAttrib(CSymbolAttribute *pAttribute);//��������ֵ
	bool EditSymbol(QRenderable *pPickObj,QVector3DArray *pPoint, int nPosStart, int nPosEnd); //�༭��ֵ��
	//bool EditSymbol(QRenderable *pPickObj,QVector3DArray *pPoint, int nPosStart, int nPosEnd); //�༭��ֵ��
private:
	void SelectSymbol(QRenderable *pPickObj);
	void ReleaseSymbol(QRenderable *pPickObj);
	void DrawControlLine();								//���ƿ�����
	void ClearControlLine();							//�Ƴ�������
	void DrawCrossArray(QVector3DArray&  InputData);
	void ClearCrossArray(QVector3DArray&  InputData);
	bool CheckDirection();								//�����༭�����ֵ���Ƿ�
signals:
	void UpdateAttribute(ManualDraw *);                 //
private:
	QRenderable* m_pPickObj;							//�õ���ǰʰȡ���󣬱༭����
	//QRenderable* m_pPickObj, *m_pEditObj;				//�õ���ǰʰȡ���󣬱༭����
	//CUndoSymbolPointMoveCommand* m_pUndoSymbolPointEdit;
	//CUndoSymbolLineMoveCommand* m_pUndoLineMoveEdit;
	//CUndoContourEditCommand*    m_pUndoContourEdit;
	//CUndoContourEditCommand* m_pUndoContourEdit2;
	//CUndoMultiContourEditCommand* m_pUndoMultiContourEdit;
	QVector3DArray m_PressPoints, m_SmoothPoints,m_SmoothPointsTemp;       //�ռ��ı༭��
	CCardinalSpline m_SpLine;                           //ƽ���㷨
	bool m_bEditStart;
	int nPosStart, nPosEnd;
	Line *m_pShowLine;
	QVector3DArray *pArray;                             //Line�Ķ���ָ��
	QVector4DArray *pColor;                             //Line����ɫָ��
	CSymbolAttribute *m_pAttribute;		                //��������ֵ
	bool m_bClose;										//���ַ����
	float m_currentZ;									//��ǰʰȡ�����Zֵ
	CLineAnalysis m_lineanalysis;
	QVector<Line*> m_CrossArray;
	QVector3D m_lastScreen;								//���Ŀ��Ƶ�
	bool m_bPositive;
	CDrawOrder m_DrawOrder;								//�ж�˳ʱ�����
};

