/*============================================================ 
��Ŀ����CIPAS 
�� �ߣ� �Ը���	
�� λ�� css
����޸�ʱ�䣺 2011��4��20��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 
------------------------------------------------------------------------------------- 
�ļ����� DrawMicapsGroundMap.h DrawMicapsGroundMap.cpp 
�� ����  QRenderUnit 
�� �ࣺ   
�� �ࣺ �� 
����˵���� ��micapsdataxml�ļ��Ĳ�������д��,���ڳ�ʼ�����水ť��Ϊ��ť�ṩ��������
����˵���� ���������Vector����
-------------------------------------�汾����------------------------------------------------ 
V 0.1
�� ��: �Ը���
�������: 2011-05-13
����˵��: ��ӻ����㷨�ͺ���������Ӧ��ע��
----------------------------------------------------------------------------------------
V 0.0 
�� �ߣ�
������ڣ�
����˵���� 
============================================================*/ 
#pragma once
#include "GIS_WrapperConfig.h"
#include "SymbolPoint.h"
#include "SymbolPluginInterface.h"
#include "Layer.h"
//#include "CElement.h"
#include "Line.h"
//#include "QTextureFont.h"
#include "QBitmapText.h"

class GIS_WRAPPER_EXPORT_IMPORT DrawMicapsGroundMap : public QRenderUnit
{
public:
	DrawMicapsGroundMap(void);
	DrawMicapsGroundMap(QRenderable* container,CSymbolPluginInterface *pSymbolPlugin,bool bAirMap = false);
	~DrawMicapsGroundMap(void);
	void Render();
	void SetAllDataArray(QVector3D& pos,float fg,float p3,float sp,float tm,float cw);
	void SetBshowData(bool bfg,bool bp3,bool bsp,bool btm,bool bcw);
	//----------------------------------------------�߿���ͼ
	void SetAllAirDataArray(QVector3D& pos,float TempFlogPGap,float Hight,float Temperature);
	void SetBshowAirData(bool bTempFlogPGap,bool bHight,bool bTemperature);
	//----------------------------------------------�߿���ͼ
private:
	void    Draw();
	void ScreenToGL( const QVector3D& screenPoint, QVector3D& glPoint );
	void InitSymbolPointArray();
	void DrawWeatherSymbol(QVector3D& pos,float fWeather,int nDraw);
	void DrawBitmapText(QVector3D& pos,float fText,int nDraw);
	//----------------------------------------------�߿���ͼ
	void DrawAirmap();
	//----------------------------------------------�߿���ͼ

private:
	Symbol *m_pSymbol;
	CSymbolPluginInterface *m_pSymbolPlugin;           //�����ⲿ����ָ��
	QRenderable* m_container;
	SymbolPoint *m_pSymbolPoint;
	QBitmapText *m_PTempText;
	//QTextureFont *m_PTempText;
	//CData *m_pTempDataAll;									//ָ���ⲿ����

	ColorAttribute * m_pTextColorAttri;							//��ɫ����

	QVector<SymbolPoint *>m_pWeatherSymbolPointArray;		//�洢����ͼ��ָ��

	float m_fdistance;										//�洢������ͼ�и�Ҫ�صļ��
	int m_ndrawtimes;

	QVector3DArray m_PosArray;
	QVector<float> m_fgArray;
	QVector<float> m_p3Array;
	QVector<float> m_spArray;
	QVector<float> m_tmArray;
	QVector<float> m_cwArray;

	bool m_bfg;
	bool m_bp3;
	bool m_bsp;
	bool m_btm;
	bool m_bcw;

	//----------------------------------------------�߿���ͼ
	QVector<float> m_TempFlogPGapArray;
	QVector<float> m_HightArray;
	QVector<float> m_TemperatureArray;

	bool m_bTempFlogPGap;
	bool m_bHight;
	bool m_bTemperature;

	bool m_bAirFillMap;
	//----------------------------------------------
	QVector3D m_prePoint;
};

