/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ��ѩ
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����AlgorithmInterface.h AlgorithmInterface.cpp 
�� ���� CObject
�� �ࣺ ��
�� �ࣺ �� 
����˵���� ˫�ߡ���ͷ�����ַ��ߡ�˪�����㷨���㷨�ӿ�
����˵���� �麯��

---------------------------�汾����-------------------------------
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ����ѩ
������ڣ�2011��04��19��
============================================================*/

#pragma once
#include "QT_Include.h"
#include "QRenderVolume.h"
#include "Line.h"


class GIS_WRAPPER_EXPORT_IMPORT CAlgorithmInterface : public QObject
{
public:
	CAlgorithmInterface(void);	
	virtual ~CAlgorithmInterface(void);
	virtual void SetLineAlgorithmWidth(float width)=0;
	virtual bool SymbolLineAlgorithm(QVector3DArray &inputLine,  QVector<QVector3DArray> &outputLine) = 0;
	virtual void SetLineAttributeType(int nType){m_nAttributeType = nType;}
	virtual bool WindAlgorithm(float fSpeed, QVector<QVector3DArray> &outputLine);

protected:
	float m_width; //˫�ߵĿ��
	float m_sideLength;//���ַ��ߵ������α߳����Բ��ֱ��
    int   m_nAttributeType;
};

