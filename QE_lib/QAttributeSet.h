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
�ļ�����QAttributeSet.h QAttributeSet.cpp 
�� ���� QAttributeSet
�� �ࣺ ATTRIBUTE_MAP
�� �ࣺ ��
����˵���� ͼ�����������л�ͼ���Եļ�������
����˵���� 
================================================================*/ 
#pragma once
#include "QT_Include.h"
#include "GL_Include.h"
#include "QAttribute.h"

class QE_LIB_EXPORT_IMPORT QAttributeSet : public ATTRIBUTE_MAP
{
public:
	QAttributeSet(void);
	~QAttributeSet(void);
	void Applay();
	void Recover();
};

