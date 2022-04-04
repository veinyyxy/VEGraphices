/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�ļ�����DrawPolygonSymbol.h 
�� ���� CDrawPolygonSymbol
�� �ࣺ ManualDraw
�� �ࣺ �� 
����˵�������ƶ����
����˵����

-------------------------------------�汾����-------------------- 
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ����ѩ
������ڣ�2011��07��22��
============================================================*/

#pragma once
#include "manualdraw.h"
#include "Line.h"
#include "SymbolMultiLine.h"
#include "SymbolData.h"
//#include "UndoAddDrawCommand.h"
//#include "UndoAddDrawMultiCommand.h"
#include "SymbolLayer.h"
#include "SymbolAttribute.h"

//class CLinesSymbol;

class GIS_WRAPPER_EXPORT_IMPORT CDrawShapeSymbol :	public ManualDraw
{
public:
	CDrawShapeSymbol(int nType);
	~CDrawShapeSymbol(void);

	void MouseEvent(QMouseEvent* event);
	void MousePress(QMouseEvent *event);
	void MouseRelease(QMouseEvent *event);
	void MouseMove(QMouseEvent *event);
	void LeaveEvent();
	void SetSymbol(Symbol *pSymbol) {m_Symbol = pSymbol;}	
	void SetShadeSymbol(Symbol *pSymbol);
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void initAttribute(int nType);
	void SetParentLayer(Layer * Layer){m_ParentLayer = Layer;}
	void GetUIAttribute();
	void GetSymbolAttribute(CSymbolMultiLine *pSymbolLine);
private:
//	inline Line* DrawEclipse(QVector3D Pos1,QVector3D Pos2,QVector4D color);
	inline void DrawRect(int x1,int x2,int y1,int y2,QVector4D color);
    inline void DrawEclipse(int x1,int x2,int y1,int y2,QVector4D color);

private:
//	QVector3DArray  m_InputData;
//	QVector<QVector3DArray> *pOutputData;	
	CSymbolAttribute m_PolygonAttribute;
	SymbolData *m_pSymboldata;	
	bool  m_bisStart;
	int   m_nType;
	QVector3D m_posold,m_posnew;
	CSymbolMultiLine *m_PSymbolMultiLine;
	QVector3DArray SreenPointArray;
	Line *m_pLineTemp;	
	Line *m_pRectTemp;
	QVector3D m_vectorSreen;
};