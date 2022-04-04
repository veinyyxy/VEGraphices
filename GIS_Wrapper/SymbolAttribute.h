/*============================================================ 
文件名：SymbolAttribute.h SymbolAttribute.cpp 
类 名： CSymbolAttribute
父 类： 无
子 类： 无 
功能说明：传递符号的属性
调用说明：
----------------------------版本更新-------------------------- 
V 1.0 
原作者 ：任雪
完成日期：2011年4月18日
============================================================*/

#pragma once
#include "ILayerAttribute.h"

class GIS_WRAPPER_EXPORT_IMPORT CSymbolAttribute
{
public:
	CSymbolAttribute(void);
	~CSymbolAttribute(void);
	void SetColor(QVector4D color);      //设置颜色
	void SetLineWidth(int width);        //设置线宽
	void SetLineType(QString Type);      //设置线型
	void SetSymbolType(int type);        //设置线的类型，如箭头、双线等
	void SetFillType(bool filled);       //设置填充标志，true表示填充；
	void SetValue(float  fValue);        //设置线的标值；
	void SetValue(QString strValue);     //设置区域值，如多边形的标值；
	void SetFillColor(QVector4D color);  //设置填充颜色
	void SetLayerZ(float Z){m_LayerZ = Z;}//设置图层的Z值

	QVector4D GetColor();                  //获得属性颜色
	QVector4D GetFillColor();              //获取填充颜色
	int       GetLineWidth();              //获得属性线宽	
	int       GetSymbolType();             //获得的类型，如箭头、双线等
	int       GetLayerZ(){return m_LayerZ;}
	bool      GetFillType();               //获得填充标值，true表示填充；
	float     GetValue();                  //获得线的标值；
	QString   GetLineType();               //获得属性线型；
	QString   GetStringValue();            //获得String类型的标值；

protected:	
	QVector4D m_nColor;             //颜色
	QVector4D m_nFillColor;         //填充颜色
	float     m_LayerZ;             //图层的Z值
	int       m_nLineWidth;         //线宽	
	int       m_nSymbolType;        //符号类型
	bool      m_bFilled;            //是否填充
	float     m_fValue;             //线的标值（如等值线）
	QString   m_LineType;           //线型
	QString   m_strValue;           //区域
};

