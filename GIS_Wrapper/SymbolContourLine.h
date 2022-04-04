/*============================================================ 
文件名：SymbolContourLine.h 
类 名： CSymbolContourLine
父 类： QRenderUnit
子 类： 无 
功能说明：根据收集的等值线点集绘制等值线，并贴上等值线的值
调用说明：

---------------------------版本更新---------------------------
V 1.0 
作者 ：   renxue、zhanglei
完成日期：2011年08月18日
============================================================*/

#pragma once
#include "GIS_WrapperConfig.h"
#include "Line.h"
//#include "Layer.h"
#include "SymbolAttribute.h"
#include "QEText.h"

//typedef QString CONTOUR_TYPE;

// const QString  DEFAULT_TYPE = QString("0");
// const QString  TEMPRATRUE_AREA = QString("1");
// const QString  RAIN_AREA = QString("2");
// const QString  RAIN_YUBAO_AREA = QString("3");

typedef enum CONTOUR_TYPE
{
	DEFAULT_TYPE = 0,
	TEMPRATRUE_AREA = 1,
	RAIN_AREA = 2,
	RAIN_YUBAO_AREA = 3
};


class GIS_WRAPPER_EXPORT_IMPORT CSymbolContourLine : public QRenderUnit
{
public:
	CSymbolContourLine(void);
	~CSymbolContourLine(void);
	void Render();
	QVector3DArray * GetArray() { return &m_InputArray;}
	QVector<QVector3DArray>* GetOutputData();
	virtual bool PickUp(const QVector3D* pickPoint);
	int  GetPickIndex(){return -1;}
	
	inline void SetCloseLine(bool closed){m_bClosed = closed;}
	inline bool GetClosedLine(){return m_bClosed;}
	inline void SetSelected(bool bSelect){m_bHadPicked = bSelect;}
	inline bool GetSelected(){return m_bHadPicked;}
	inline void SetPosition(int nPos){m_nPos = nPos;}
	inline int  GetPosition(){return m_nPos;}
	inline void SetAttribute(CSymbolAttribute *pAttribute){m_pAttribute = pAttribute;}         //设置等值线的属性
	inline CSymbolAttribute* GetAttribute(){return m_pAttribute;}                              //获得等值线的属性
	bool IsMark() { return m_bMark; }
	void IsMark(bool ismark) {m_bMark = ismark;}
	void HideBound(bool hide) {m_hidebound = hide;}
	bool HideBound() {return m_hidebound;}
	bool IsIntersect();											//判断符号线中是否有交叉点;wuenp 20110615
	void CalContourCircleDistance();
	float GetScreenDistance() {return m_ScreenDistance;}

//	void SetContourType(CONTOUR_TYPE type){m_contourtype = type;}
	void SetContourType(int type){m_layertype = type;}
	int GetContourType() {return m_layertype;}
//	CONTOUR_TYPE GetContourType() {return m_contourtype;}
	void ReDraw();										//当填充颜色后重新绘制
protected:
	int     m_nPos;
	bool    m_bClosed;              //等值线闭合为true，不闭合为false
	bool    m_bHadPicked;	
	bool    m_bMark;				//是否标值
	bool    m_bshowMark;
	bool    m_hidebound;            //是否显示边框
	Line    *pLine;
	float   m_ScreenDistance;       //绘制闭合等值线时首尾两点的间距
	float   m_ContourCircleDistance;
	QVector4D        m_Color;
	QEText     *m_pTexFontValue, *m_pTexFontValue2;
	QVector3DArray   m_InputArray;
	CSymbolAttribute *m_pAttribute;   //等值线的属性	
//	CONTOUR_TYPE m_contourtype;
	int m_layertype;
	TessStruct *m_pTessData;		//细分对象
	QVector<GISPolygon*> m_TessObject;
	QFont m_font;					//绘制字体
	QColor m_fontcolor;				//字体颜色
private:
	virtual void   Draw();
};

