/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� lugsh
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����CardinalSpline.h CardinalSpline.cpp 
�� ���� CCardinalSpline
�� �ࣺ NULL
�� �ࣺ �� 
����˵���� ����������ֵ�㷨
����˵���� 

-------------------------------------�汾����------------------------------------------------ 
V 1.2 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ��lugsh
������ڣ�2011�ꣿ�£���
============================================================*/
#pragma once
#include  "QDataArray.h"
class QE_LIB_EXPORT_IMPORT CCardinalSpline
{
public:
	CCardinalSpline(void);
	CCardinalSpline(const CCardinalSpline&);
	~CCardinalSpline(void);
	void GetSmoothPoints(QVector3DArray &inputData, QVector3DArray &OutputData);
	void MergeSmoothLines(QVector3DArray &OutPutData, QVector3DArray &OutputData2,QVector3DArray &OutPutDataSum);

private :
	static QVector3D Eq(float t, const QVector3D& p1, const QVector3D& p2, const QVector3D& p3, const QVector3D& p4);
	QVector3D GetInterpolatedSplinePoint(float t);      //ȡ��ƽ�������ߵ�
	int GetNumPoints();									//�õ�ָ�����߿��Ƶ������
	QVector3D& GetNthPoint(int n);						//����ָ�����߶�Ӧ�Ŀ��Ƶ�
	void ClearSplinePoint();							//������ߵ����п��Ƶ�
	void AddSplinePoint(const QVector3D& v);			//������ָ��2D\3D���߿��Ƶ�����
	QVector3DArray m_ControlPoints;
	float m_delta;
};

