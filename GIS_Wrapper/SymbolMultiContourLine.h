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
#include "SymbolAttribute.h"
#include "QBitmapText.h"
//#include "QTextureFont.h"

class GIS_WRAPPER_EXPORT_IMPORT CSymbolMultiContourLine : public QRenderUnit
{
public:
	CSymbolMultiContourLine(void);
	~CSymbolMultiContourLine(void);

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
	void IsMark() { m_bMark = false; };
	void IsMark(bool ismark) {m_bMark = ismark;}
	bool IsIntersect();											//判断符号线中是否有交叉点;wuenp 20110615
	void SetKey(double key) {m_key = key;}
	const double GetKey()  {return m_key;}
	void SetName(const QString& name){m_name = name;}
	const QString GetName() {return m_name;}
	void SetColor(const QColor& color) {m_qcolor = color;}
	const QColor GetColor() {return m_qcolor;}

	float GetScreenDistance() {return m_ScreenDistance;}
	void ReDraw();

protected:
	int     m_nPos;
	bool    m_bClosed;              //等值线闭合为true，不闭合为false
	bool    m_bHadPicked;	
	bool    m_bMark;				//是否标值
	Line    *pLine;
	Line    *pLine2;
	float   m_ScreenDistance;       //绘制闭合等值线时首尾两点的间距
	QVector4D        m_Color,m_Color2;
	QBitmapText      *m_pTextValue, *m_pTextValue2;
	//QTextureFont     *m_pTexFontValue, *m_pTexFontValue2;
	QVector3DArray   m_InputArray;
	CSymbolAttribute *m_pAttribute;   //等值线的属性	
	TessStruct *m_pTessData;		  //细分对象
	QVector<GISPolygon*> m_TessObject;
	TessStruct *m_pTessData2;		  //细分对象2
	QVector<GISPolygon*> m_TessObject2;
private:
	double m_key;
	QString m_name;
	QColor m_qcolor;
private:
	virtual void   Draw();
	virtual void DrawLine();
	virtual void DrawLine2();
};

