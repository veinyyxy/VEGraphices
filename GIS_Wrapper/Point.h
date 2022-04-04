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
�ļ�����Point.h Point.cpp 
�� ���� Point
�� �ࣺ Geometry
�� �ࣺ �� 
����˵������װGIS�㹦�ܵĻ���
����˵����
------------------------------------------------------------------------------------*/
#ifndef POINT_H
#define POINT_H
#include "GIS_WrapperConfig.h"
#include "Geometry.h"
#include "Symbol.h"
#include "StationValues.h"

class GIS_WRAPPER_EXPORT_IMPORT Point : public Geometry
{
public:
    Point();
	inline Line* AsLine();
	inline Plane* AsPlane();
	inline GISPolygon* AsPolygon();
	inline Point* AsPoint();
	inline Symbol* PointSymbol();
	/*inline QVector4DArray* getColorArray();*/
	inline QVector2DArray* getTextureAttray();
	inline QVector3DArray* getNormalAttray();
	inline void setPointSize(float fsize){glPointSize(fsize);};//��Ĵ�С

	inline QVector<StationValues>* StationValueArray();//վ������zhaogaopan
private:
	Symbol* symbol;
	QVector<StationValues> m_SValuesArray;//վ����������
};

#endif