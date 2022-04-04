/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� �
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
-------------------------------------------------------------------------------------
�ļ�����NormalSymbol.h NormalSymbol.cpp
�� ���� NormalSymbol
�� �ࣺ QRenderUnit
�� �ࣺ ��
����˵�������б�ǩ��ͨ����
����˵����

-------------------------------------�汾����------------------------------------------------
V 1.0 
ԭ���� ��
������ڣ�2011�ꣿ�£���

V 1.1 
�޸����ߣ�
������ڣ�2011��4��18��
����˵����
============================================================*/


#pragma once
#include "Symbol.h"
#include "GIS_WrapperConfig.h"
#include "QRenderUnit.h"
#include "CoordinateTransform.h"
#include "Proj4Transform.h"
#include "SymbolPoint.h"

class GIS_WRAPPER_EXPORT_IMPORT NormalSymbol : public SymbolPoint
{
public:
	NormalSymbol(void);
	~NormalSymbol(void);

	void Render();
	void SetPointStyle(Symbol* ps);
	Symbol* SetPointStyle();
	void RenderStyle(QVector3D& pos, CSymbolAttribute *sa);
	void SetPosition(QVector3D& pos,CSymbolAttribute *nIndex);
	void SetAngle(GLfloat fangle);

	inline QVector3DArray* Scales();
	inline  void CoordTrans(Proj4Transform *pProj4){coordTrans = pProj4;}
	virtual bool PickUp(const QVector3D* pickPoint);
	int GetPickIndex();
	void SetPickIndex() {m_iPickIndex = -1;}
	void SetSelected(bool bHadPicked) {m_bHadPicked = bHadPicked;}
	QVector3DArray *getQVector3DArray(){return &m_posion;}
	QVector<CSymbolAttribute*> * getIndex(){return &m_Attr;}
	QVector<GLfloat>* AngleArray();

public:
	CSymbolAttribute *m_SymbolAttribute;

protected:
	QVector3DArray m_posion;
	QVector<CSymbolAttribute*> m_Attr;
	QVector <GLfloat> m_angleArray;
	QVector <GLfloat> m_IniAngleArray;
	QVector3DArray m_scales;
	Symbol* m_pointStyle;
	Proj4Transform *m_Proj4Trans;		//���ڽ�ͶӰ����ת���ɾ�γ����
	Proj4Transform* coordTrans;			//���ܵ�ǰͶӰ��������ת��
	int m_iPickIndex;
	bool m_bHadPicked ;
	
};

