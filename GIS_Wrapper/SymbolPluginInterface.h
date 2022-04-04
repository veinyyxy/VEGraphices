#include <QtCore\QObject>
#include "GIS_WrapperConfig.h"
/*=================================================================================== 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ¬�ߕN
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011��4��18��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����CSymbolPluginInterface.h 
�� ���� CSymbolPluginInterface
�� �ࣺ Object
�� �ࣺ �� 
����˵�������ݷ������㷨�������ݣ��ռ��������㷨�������
����˵����
------------------------------------------------------------------------------------*/
#pragma once
#include "Symbol.h"
class GIS_WRAPPER_EXPORT_IMPORT CSymbolPluginInterface :public QObject
{
public:
	virtual Symbol* GetSymbol(int type,float fSpeed =0) = 0;
	virtual bool DeleteSymbol(int type) = 0;
	virtual bool CreateSymbol(int type) = 0;
	virtual bool DeleteAllSymbol() = 0 ;
};