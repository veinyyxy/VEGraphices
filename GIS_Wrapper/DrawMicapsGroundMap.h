/*============================================================ 
项目名：CIPAS 
作 者： 赵高攀	
单 位： css
最后修改时间： 2011年4月20日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 
------------------------------------------------------------------------------------- 
文件名： DrawMicapsGroundMap.h DrawMicapsGroundMap.cpp 
类 名：  QRenderUnit 
父 类：   
子 类： 无 
功能说明： 对micapsdataxml文件的操作（读写）,用于初始化界面按钮，为按钮提供操作属性
调用说明： 返回所需的Vector容器
-------------------------------------版本更新------------------------------------------------ 
V 0.1
作 者: 赵高攀
完成日期: 2011-05-13
更新说明: 添加绘制算法和函数。做相应的注释
----------------------------------------------------------------------------------------
V 0.0 
作 者：
完成日期：
更新说明： 
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
	//----------------------------------------------高空填图
	void SetAllAirDataArray(QVector3D& pos,float TempFlogPGap,float Hight,float Temperature);
	void SetBshowAirData(bool bTempFlogPGap,bool bHight,bool bTemperature);
	//----------------------------------------------高空填图
private:
	void    Draw();
	void ScreenToGL( const QVector3D& screenPoint, QVector3D& glPoint );
	void InitSymbolPointArray();
	void DrawWeatherSymbol(QVector3D& pos,float fWeather,int nDraw);
	void DrawBitmapText(QVector3D& pos,float fText,int nDraw);
	//----------------------------------------------高空填图
	void DrawAirmap();
	//----------------------------------------------高空填图

private:
	Symbol *m_pSymbol;
	CSymbolPluginInterface *m_pSymbolPlugin;           //接收外部对象指针
	QRenderable* m_container;
	SymbolPoint *m_pSymbolPoint;
	QBitmapText *m_PTempText;
	//QTextureFont *m_PTempText;
	//CData *m_pTempDataAll;									//指向外部数据

	ColorAttribute * m_pTextColorAttri;							//颜色属性

	QVector<SymbolPoint *>m_pWeatherSymbolPointArray;		//存储天气图标指针

	float m_fdistance;										//存储地面填图中各要素的间距
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

	//----------------------------------------------高空填图
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

