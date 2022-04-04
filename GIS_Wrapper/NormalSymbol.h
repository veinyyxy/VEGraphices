/*============================================================ 
项目名： 人机交互平台
作 者： 杨东
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
-------------------------------------------------------------------------------------
文件名：NormalSymbol.h NormalSymbol.cpp
类 名： NormalSymbol
父 类： QRenderUnit
子 类： 无
功能说明：所有标签的通用类
调用说明：

-------------------------------------版本更新------------------------------------------------
V 1.0 
原作者 ：
完成日期：2011年？月？日

V 1.1 
修改作者：
完成日期：2011年4月18日
更新说明：
============================================================*/


#pragma once
#include "Symbol.h"
#include "GIS_WrapperConfig.h"
#include "QRenderUnit.h"
#include "CoordinateTransform.h"
#include "Proj4Transform.h"
#include "SymbolPoint.h"

class GIS_WRAPPER_EXPORT_IMPORT NormalSymbol : public SymbolPoint
{
public:
	NormalSymbol(void);
	~NormalSymbol(void);

	void Render();
	void SetPointStyle(Symbol* ps);
	Symbol* SetPointStyle();
	void RenderStyle(QVector3D& pos, CSymbolAttribute *sa);
	void SetPosition(QVector3D& pos,CSymbolAttribute *nIndex);
	void SetAngle(GLfloat fangle);

	inline QVector3DArray* Scales();
	inline  void CoordTrans(Proj4Transform *pProj4){coordTrans = pProj4;}
	virtual bool PickUp(const QVector3D* pickPoint);
	int GetPickIndex();
	void SetPickIndex() {m_iPickIndex = -1;}
	void SetSelected(bool bHadPicked) {m_bHadPicked = bHadPicked;}
	QVector3DArray *getQVector3DArray(){return &m_posion;}
	QVector<CSymbolAttribute*> * getIndex(){return &m_Attr;}
	QVector<GLfloat>* AngleArray();

public:
	CSymbolAttribute *m_SymbolAttribute;

protected:
	QVector3DArray m_posion;
	QVector<CSymbolAttribute*> m_Attr;
	QVector <GLfloat> m_angleArray;
	QVector <GLfloat> m_IniAngleArray;
	QVector3DArray m_scales;
	Symbol* m_pointStyle;
	Proj4Transform *m_Proj4Trans;		//用于将投影坐标转换成经纬坐标
	Proj4Transform* coordTrans;			//接受当前投影坐标用于转换
	int m_iPickIndex;
	bool m_bHadPicked ;
	
};

