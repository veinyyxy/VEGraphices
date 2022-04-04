/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ������
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011��4��15��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����QAttribute.h QAttribute.cpp 
�� ���� QAttribute
�� �ࣺ QObject
�� �ࣺ ��
����˵���� ͼ�����������л�ͼ������ĸ���
����˵���� 
================================================================*/ 
#pragma once
#include "QT_Include.h"
#include "GL_Include.h"

typedef enum _ATTRI_TYPE
{
	QE_ATTRI_UNKONW = 0,
	QE_ATTRI_POINT_SIZE = 1,
	QE_ATTRI_LINE_STIPPLE,
	QE_ATTRI_LINE_WIDTH,
	QE_ATTRI_COLOR,
	QE_ATTRI_TEXTURE0
} ATTRI_TYPE, *P_ATTRI_TYPE;

class QE_LIB_EXPORT_IMPORT QAttribute
{
public:
	QAttribute(ATTRI_TYPE t = QE_ATTRI_UNKONW);
	virtual ~QAttribute(void);
	ATTRI_TYPE Type();
	/************************************************************************/
	/* �������ܣ� ����������Ӧ����OpenGL��	     	                        */
	/* ����˵��:  NULL														*/
	/* ��    ��:  lugsh														*/
	/* ��    ��:  2011 �� 04 �� 15 ��										*/
	/************************************************************************/
	virtual int Applay() = 0;
	virtual int Recover() = 0;
protected:
	ATTRI_TYPE type;
};

typedef QVector<QAttribute*> ATTRIBUTE_VECTOR, *P_ATTRIBUTE_VECTOR;
typedef QVectorIterator<QAttribute*> ATTRIBUTE_VECTOR_ITERATOR, *P_ATTRIBUTE_VECTOR_ITERATOR;
typedef QMap<ATTRI_TYPE, QAttribute*> ATTRIBUTE_MAP, *P_ATTRIBUTE_MAP;
typedef QMapIterator<ATTRI_TYPE, QAttribute*>  ATTRIBUTE_MAP_ITERATOR, *P_ATTRIBUTE_MAP_ITERATOR;