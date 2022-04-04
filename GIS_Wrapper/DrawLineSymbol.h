/*============================================================ 
�ļ�����DrawLineSymbol.h DrawLineSymbol.cpp 
�� ���� CDrawLineSymbol
�� �ࣺ ManualDraw
�� �ࣺ �� 
����˵������ʸ�������ߵ��㷨
����˵����
1����ȡ�û���PING�����������㣬�����²��㷨�����ؼ���õ��ĵ㼯��
2������Ļ��ת����OPENGL�����꣬��������Ӧ�Ļ�ͼ�ܵ���

--------------------------�汾����----------------------------
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
������ڣ�2011��7��27��
============================================================*/


#pragma once
#include "manualdraw.h"
#include "Line.h"
#include "AlgorithmInterface.h"
#include "SymbolLine.h"
#include "CardinalSpline.h"
//#include "UndoAddDrawCommand.h"
#include "SymbolLayer.h"

class GIS_WRAPPER_EXPORT_IMPORT CDrawLineSymbol :	public ManualDraw
{
public:
	CDrawLineSymbol(int nType);
	CDrawLineSymbol(int nType,CoordinateTransform* proj);
	~CDrawLineSymbol(void);

	void MouseEvent(QMouseEvent* event);
	void MousePress(QMouseEvent *event);
	void MouseRelease(QMouseEvent *event);
	void LeaveEvent();
	void SetSymbol(Symbol *pSymbol) {m_Symbol = pSymbol;}
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void InitAttribute(int nType);
	void SetParentLayer(Layer * Layer){m_ParentLayer = Layer;}
	void GetSymbolAttribute();
	void SetCoordinateTransform(CoordinateTransform* proj){m_proj = dynamic_cast<RadProjTransform*> (proj);}
private:
	int    m_nType;	
	Line   *m_pDisplayLine;	
	bool   m_bFinished, m_bFirst;			//�ж��Ƿ��������
	QVector3DArray  *m_pArray ;
	QVector3DArray  m_InputData, m_OutputData;
	CSymbolLine     *m_pSymbolLine;
	SymbolData      *m_pSymboldata;	
	CCardinalSpline *m_pSpLine;
	RadProjTransform* m_proj;
	bool cardline;
	//int m_OutLineNum;//��¼���������ߵĸ���
	//QPoint lastPos;	
	//bool m_bFirst;
	//QVector<QVector3DArray> *pOutputData;
};

