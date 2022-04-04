/*=================================================================================== 
项目名： 人机交互平台
作 者： 杨晓宇
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年4月18日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：SymbolPoint.h SymbolPoint.cpp 
类 名： SymbolPoint
父 类： QRenderUnit
子 类： 无 
功能说明：传递符号线算法数据数据，收集符号线算法输出数据
调用说明：
------------------------------------------------------------------------------------*/

#pragma once
#include "Symbol.h"
#include "GIS_WrapperConfig.h"
#include "QRenderUnit.h"
#include "CoordinateTransform.h"
#include "Proj4Transform.h"
#include "StationValues.h"

class GIS_WRAPPER_EXPORT_IMPORT SymbolPoint : public QRenderUnit
{
public:
	SymbolPoint(void);
	~SymbolPoint(void);
	void Render();
	virtual void SetPointStyle(Symbol* ps);
	virtual Symbol* SetPointStyle();
	virtual void RenderStyle(QVector3D& pos, GLfloat angle/*, QVector3D& sc*/);
	virtual void SetPosition(QVector3D& pos);
	virtual void SetAngle(GLfloat fangle);
	virtual inline QVector3DArray* Scales();
	virtual inline  void CoordTrans(Proj4Transform *pProj4){coordTrans = pProj4;}
	virtual bool PickUp(const QVector3D* pickPoint);
	virtual int GetPickIndex();
	virtual void SetPickIndex() {m_iPickIndex = -1;}
	virtual void SetSelected(bool bHadPicked) {m_bHadPicked = bHadPicked;}
	virtual Symbol *getSymbol(){return m_pointStyle;}
	virtual QVector<GLfloat>* AngleArray();
	QVector3DArray * SetPositions() { return &m_posion;}
	inline QVector<StationValues>* StationValueArray();
	int n;
protected:

	QVector3DArray m_posion;
	QVector <GLfloat> m_angleArray;
	QVector <GLfloat> m_IniAngleArray;
	QVector3DArray m_scales;
	Symbol* m_pointStyle;
	Proj4Transform *m_Proj4Trans;		//用于将投影坐标转换成经纬坐标
	Proj4Transform* coordTrans;			//接受当前投影坐标用于转换
	int m_iPickIndex;
	bool m_bHadPicked ;

	QVector<StationValues> m_SValuesArray;//站点数据数组
};

