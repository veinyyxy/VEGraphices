/*============================================================ 
��Ŀ����CIPAS 
�� �ߣ� ����	
�� λ�� css
����޸�ʱ�䣺 2011��7��18��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ�������������Ա��δ����
�ɲ������Դ�����
2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ����� DrawOrder.h DrawOrder.cpp 
�� ����  CDrawOrder 
�� �ࣺ  ��
�� �ࣺ �� 
����˵���� �жϻ��λ��Ʒ���Ϊ˳ʱ�뷽������ʱ�뷽��
����˵���� �����˵����һ��������������һΪ���жϵĻ��α�������һ������Ϊ�����Ƿ�պϣ�trueΪ�պϣ�falseΪ���պϡ�
-------------------------------------�汾����------------------------------------------------ 
============================================================*/  
#include "QDataArray.h"
#pragma once
class CDrawOrder
{
public:
	CDrawOrder(void);
	~CDrawOrder(void);
	bool isConvert(QVector3DArray *pData,bool bisClosed);
};
