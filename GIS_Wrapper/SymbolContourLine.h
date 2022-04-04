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
	inline void SetAttribute(CSymbolAttribute *pAttribute){m_pAttribute = pAttribute;}         //���õ�ֵ�ߵ�����
	inline CSymbolAttribute* GetAttribute(){return m_pAttribute;}                              //��õ�ֵ�ߵ�����
	bool IsMark() { return m_bMark; }
	void IsMark(bool ismark) {m_bMark = ismark;}
	void HideBound(bool hide) {m_hidebound = hide;}
	bool HideBound() {return m_hidebound;}
	bool IsIntersect();											//�жϷ��������Ƿ��н����;wuenp 20110615
	void CalContourCircleDistance();
	float GetScreenDistance() {return m_ScreenDistance;}

//	void SetContourType(CONTOUR_TYPE type){m_contourtype = type;}
	void SetContourType(int type){m_layertype = type;}
	int GetContourType() {return m_layertype;}
//	CONTOUR_TYPE GetContourType() {return m_contourtype;}
	void ReDraw();										//�������ɫ�����»���
protected:
	int     m_nPos;
	bool    m_bClosed;              //��ֵ�߱պ�Ϊtrue�����պ�Ϊfalse
	bool    m_bHadPicked;	
	bool    m_bMark;				//�Ƿ��ֵ
	bool    m_bshowMark;
	bool    m_hidebound;            //�Ƿ���ʾ�߿�
	Line    *pLine;
	float   m_ScreenDistance;       //���Ʊպϵ�ֵ��ʱ��β����ļ��
	float   m_ContourCircleDistance;
	QVector4D        m_Color;
	QEText     *m_pTexFontValue, *m_pTexFontValue2;
	QVector3DArray   m_InputArray;
	CSymbolAttribute *m_pAttribute;   //��ֵ�ߵ�����	
//	CONTOUR_TYPE m_contourtype;
	int m_layertype;
	TessStruct *m_pTessData;		//ϸ�ֶ���
	QVector<GISPolygon*> m_TessObject;
	QFont m_font;					//��������
	QColor m_fontcolor;				//������ɫ
private:
	virtual void   Draw();
};

