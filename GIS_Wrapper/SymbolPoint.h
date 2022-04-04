/*=================================================================================== 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ������
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011��4��18��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����SymbolPoint.h SymbolPoint.cpp 
�� ���� SymbolPoint
�� �ࣺ QRenderUnit
�� �ࣺ �� 
����˵�������ݷ������㷨�������ݣ��ռ��������㷨�������
����˵����
------------------------------------------------------------------------------------*/

#pragma once
#include "Symbol.h"
#include "GIS_WrapperConfig.h"
#include "QRenderUnit.h"
#include "CoordinateTransform.h"
#include "Proj4Transform.h"
#include "StationValues.h"

class GIS_WRAPPER_EXPORT_IMPORT SymbolPoint : public QRenderUnit
{
public:
	SymbolPoint(void);
	~SymbolPoint(void);
	void Render();
	virtual void SetPointStyle(Symbol* ps);
	virtual Symbol* SetPointStyle();
	virtual void RenderStyle(QVector3D& pos, GLfloat angle/*, QVector3D& sc*/);
	virtual void SetPosition(QVector3D& pos);
	virtual void SetAngle(GLfloat fangle);
	virtual inline QVector3DArray* Scales();
	virtual inline  void CoordTrans(Proj4Transform *pProj4){coordTrans = pProj4;}
	virtual bool PickUp(const QVector3D* pickPoint);
	virtual int GetPickIndex();
	virtual void SetPickIndex() {m_iPickIndex = -1;}
	virtual void SetSelected(bool bHadPicked) {m_bHadPicked = bHadPicked;}
	virtual Symbol *getSymbol(){return m_pointStyle;}
	virtual QVector<GLfloat>* AngleArray();
	QVector3DArray * SetPositions() { return &m_posion;}
	inline QVector<StationValues>* StationValueArray();
	int n;
protected:

	QVector3DArray m_posion;
	QVector <GLfloat> m_angleArray;
	QVector <GLfloat> m_IniAngleArray;
	QVector3DArray m_scales;
	Symbol* m_pointStyle;
	Proj4Transform *m_Proj4Trans;		//���ڽ�ͶӰ����ת���ɾ�γ����
	Proj4Transform* coordTrans;			//���ܵ�ǰͶӰ��������ת��
	int m_iPickIndex;
	bool m_bHadPicked ;

	QVector<StationValues> m_SValuesArray;//վ����������
};

