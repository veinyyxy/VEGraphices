/*============================================================ 
�ļ�����SymbolContourLine.h 
�� ���� CSymbolContourLine
�� �ࣺ QRenderUnit
�� �ࣺ �� 
����˵���������ռ��ĵ�ֵ�ߵ㼯���Ƶ�ֵ�ߣ������ϵ�ֵ�ߵ�ֵ
����˵����

---------------------------�汾����---------------------------
V 1.0 
���� ��   renxue��zhanglei
������ڣ�2011��08��18��
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
	inline void SetAttribute(CSymbolAttribute *pAttribute){m_pAttribute = pAttribute;}         //���õ�ֵ�ߵ�����
	inline CSymbolAttribute* GetAttribute(){return m_pAttribute;}                              //��õ�ֵ�ߵ�����
	void IsMark() { m_bMark = false; };
	void IsMark(bool ismark) {m_bMark = ismark;}
	bool IsIntersect();											//�жϷ��������Ƿ��н����;wuenp 20110615
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
	bool    m_bClosed;              //��ֵ�߱պ�Ϊtrue�����պ�Ϊfalse
	bool    m_bHadPicked;	
	bool    m_bMark;				//�Ƿ��ֵ
	Line    *pLine;
	Line    *pLine2;
	float   m_ScreenDistance;       //���Ʊպϵ�ֵ��ʱ��β����ļ��
	QVector4D        m_Color,m_Color2;
	QBitmapText      *m_pTextValue, *m_pTextValue2;
	//QTextureFont     *m_pTexFontValue, *m_pTexFontValue2;
	QVector3DArray   m_InputArray;
	CSymbolAttribute *m_pAttribute;   //��ֵ�ߵ�����	
	TessStruct *m_pTessData;		  //ϸ�ֶ���
	QVector<GISPolygon*> m_TessObject;
	TessStruct *m_pTessData2;		  //ϸ�ֶ���2
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

