/*============================================================ 
�ļ�����SymbolLine.h SymbolLine.cpp 
�� ���� CSymbolLine
�� �ࣺ QRenderUnit
�� �ࣺ �� 
����˵�������ݷ������㷨�������ݣ��ռ��������㷨�������
����˵����

---------------------------�汾����---------------------------
V 1.0 
ԭ���� ����ѩ
������ڣ�2011��03��13��

V 1.1 
�� �ߣ�������
������ڣ�2011��4��18��
����˵�������������ʰȡ���� 
============================================================*/
#pragma once
#include <QPair>
#include "GIS_WrapperConfig.h"
#include "Line.h"
#include "Symbol.h"
#include "SymbolAttribute.h"
#include "AlgorithmInterface.h"

// class CSymbolLine;
// typedef QPair<CSymbolLine*,CSymbolLine*> SymbolLinePair;
// typedef QList<SymbolLinePair>  SymbolLinePairList;

class GIS_WRAPPER_EXPORT_IMPORT CSymbolMultiLine : public Line //QRenderUnit
{
public:
	CSymbolMultiLine(void);
	~CSymbolMultiLine(void);
	void Render();                                      //�����㷨����ͼ����ʾ���Ƶĺ���
	void SetSymbolLineStyle(Symbol *pAlgo);                   //���÷����ߵ��㷨
	QVector3DArray * GetArray() { return &m_InputArray;} //���ط����ߵ�����㼯����
	Symbol*  GetLineStyle();                            //��÷����ߵ�����
	//QVector<QVector3DArray>* GetOutputData();           //���ط����ߵ�����㼯����
	virtual bool PickUp(const QVector3D* pickPoint);    //����--ʰȡ�㷨
	int  GetPickIndex();
	void setLineAttributeType(int nType);
	void SetSelected(bool bSelect){m_bHadPicked = bSelect;}//�����ߵ�ѡ�б�־λ
	void SetPosition(int nPos){m_nPos = nPos;}
	int  GetPosition(){return m_nPos;}
private:
	void DrawSymbolLine2();
public:
	CSymbolAttribute m_LineAttribute;

protected:
	Symbol * m_pSymLine;
	Line *pLine2;
	bool     m_bHadPicked;
	int      m_nPos;                  //ʰȡ���ĵ������λ��
	QVector3DArray m_InputArray;

};


