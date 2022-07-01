/*============================================================
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ������
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011��4��15��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
-------------------------------------------------------------------------------------
�ļ�����Layer.h Layer.cpp
�� ���� Layer
�� �ࣺ QFork
�� �ࣺ ��
����˵���� ������֯��ͼ��һ���߼�������Ҫ������֯���ݣ�ͶӰ�任
����˵����
================================================================*/
#pragma once
#include "GIS_WrapperConfig.h"
#include "QNode.h"
#include "QLeaf.h"
#include "QFork.h"
#include "CoordinateTransform.h"
#include "DataProvider.h"
#include "ILayerAttribute.h"
#include <QtCore/QPointer>
#include <QtCore/QSettings>
#include <QtWidgets/QMessageBox>
#include <QtCore/QHash>
#include "SymbolContourLine.h"
#include "SymbolPoint.h"
#include "QBitmapText.h"
#include "CipasField.h"

class DrawSurface;

 class COLORITEM
 {
 public:
	 COLORITEM(QColor color,QString name):_color(color),_name(name){}
	 COLORITEM(){}
	 ~COLORITEM() {}
 public:
	 QColor& Color() {return _color;}
	 QString Name() {return _name;}
	 void SetColor(const QColor& color){ _color = color;}
	 void SetName(const QString& name) {_name = name; }
 private:
	 QColor _color;
	 QString _name;
 };

 typedef QMap<double,COLORITEM> COLORITEM_TABLE;
class GIS_WRAPPER_EXPORT_IMPORT Layer : public QFork
{
	Q_OBJECT

public:
	Layer(DataProvider* data, CoordinateTransform* ct);

	typedef enum ELayerType
	{
		Basic_Layer          = 1,			//
		Symbol_Layer         = 2,			//
		Coutour_Layer        = 3,			//��ֵ��
		Shade_Layer          = 4,			//ɫ��ͼ
		Vector_Layer         = 5,			//ʸ��ͼ
		Barb_Layer           = 6,			//����ͼ
		Stream_Layer         = 7,			//����ͼ
		Station_Layer        = 8,			//վ��ͼ
		SurFaceFill_Layer    = 9,			//����
		AirFill_Layer        = 10,			//�߿�
		Typhoos_Layer        = 11,			//̨��
		Temperature_Layer    = 12,			//
		Rain_Layer           = 13,			//
		UnKnown_Layer        = 14,			//
		Group_Layer          = 15,			//���ͼ�� wuenping
		Rain_Yubao_Layer     = 16,			//��ˮԤ��
		Disaster_Layer
	};
	typedef QMap<int, QVector<QRenderUnit*> > GEOMETRY_SET, *P_GEOMETRY_SET;
	typedef enum ELayerMode
	{
		DEFAULT_MODE = 0,
		YUBAO_MODE = 1
	};
	virtual ~Layer(void);
	inline QNode* DrawLayer(){return layerRoot;}
	inline void LayerName(const QString& name);
	inline QString LayerName();
	/** Get this layer's unique ID */
	inline QString id() const;
	inline void setId(const QString& name);
	inline void ElementName(const QString& name);
	inline QString ElementName();
	inline GLdouble MinimumScale();
	inline void MinimumScale(GLdouble mix);
	inline void MaximumScale(GLdouble max);
	inline GLdouble MaximumScale();
	inline void LayerZ(GLfloat z);
	inline GLfloat LayerZ();
	inline void SetPolygonType(int type);
	inline int GetPolygonType();
	virtual int CreateLayer(const QString& filename) = 0;
	virtual int CreateLayer(QNode* node) = 0;
	virtual int CreateLayer(const QStringList& namelist) = 0;		
	virtual int ReloadLayer() = 0;
	Layer& operator+=(const Layer& layer);
	Layer& operator-=(const Layer& layer);

	virtual QString GetProInfo(void) = 0;
	virtual void Clear() = 0;
	virtual void TransformLayer() = 0;
	virtual QRenderable* GetLayerData() = 0;
	virtual int SaveLayerData();
	//virtual void UpdateLayer(QGLWidget* widget);
	virtual void UpdateLayer(ILayerAttribute* iLA);
	inline virtual void Provider(DataProvider* provid);
	virtual DataProvider* Provider();
	inline virtual void CoordTrans(CoordinateTransform* ct);
	inline virtual CoordinateTransform* CoordTrans();
	inline DrawSurface* GetDrawSurface(){return m_bDrawSurface;}
	inline void SetDrawSurface(DrawSurface* pds){m_bDrawSurface = pds;}
	QAttributeSet *getAttributeSet();
	ColorAttribute *getColorAttr();
	LineWidthAttribute *getWidthAttr();
	LineStippleAttribute *getLineStippleAttr();
	Proj4Transform *getNewTrans();
	void setColor(const QVector4D& c);
	QVector4D getColor();
	void setWidth(GLfloat w);
	GLfloat getWidth();
	void setStipple(GLint f, GLshort p);
	GLshort getShortStipple();
	GLint getGLintStipple();
	void LayerType(ELayerType layerType) {eLayerType = layerType;}
	ELayerType LayerType() { return eLayerType;}
	void SetProfile(bool bProfile){m_bProfile = bProfile;}
	bool GetProfile(){ return m_bProfile;}
	void Draw();

	P_LAYER_ATTRIBUTE GetAttributeList();
	QList<LAYER_ATTRIBUTE>* GetMultiAttribute();

// 		inline const void SetColorIndex(QMap<double,QColor> colorIndex){m_mapColorIndex = colorIndex;};
// 		inline const QMap<double,QColor>* GetColorIndex(){return &m_mapColorIndex;};
	inline const void SetColorIndex(COLORITEM_TABLE colorIndex){m_mapColorIndex = colorIndex;};
	inline const COLORITEM_TABLE* GetColorIndex(){return &m_mapColorIndex;};
	void InsertColorItem(double key,COLORITEM& coloritem);
	void RemoveColorItem(double key);

	int GraphicsType();
	void GraphicsType(int iType);
	int GraphicsIndex();
	void GraphicsIndex(int iIndex);
	QString GraphicsTime();
	void GraphicsTime(QString iTim);
	int GraphicsEnsemble();
	void GraphicsEnsemble(int iEns);
	QString GraphicsLevel();
	void GraphicsLevel(QString iLev);

	int GraphicsIndex2();
	void GraphicsIndex2(int iIndex);
	QString GraphicsTime2();
	void GraphicsTime2(QString iTim);
	int GraphicsEnsemble2();
	void GraphicsEnsemble2(int iEns);
	QString GraphicsLevel2();
	void GraphicsLevel2(QString iLev);
	inline void setBaseMap(bool bm){isBaseMap = bm;}
	inline bool getBaseMap(){return isBaseMap;}

	void LayerAlpha(GLfloat alf);
	GLfloat LayerAlpha();

	virtual QRenderable* PickUp(const QVector3D& point);
	virtual void StartPick();
	virtual void EndPick();
	QHash<QString,GISPolygon*> * Get2DFigureData(){ return &m_HashShadeList;}
	QHash<QString,GISPolygon*> * Get2DFDnoTessel(){ return &m_HashShadeNoTessel;}
	QHash<QString,CSymbolContourLine*> * Get2DHashLineList(){ return &m_HashLineList;}
	QBitmapText * GetBitTextList(){return m_pBitmapText;}
	void  SetBitTextList(QBitmapText * pBitmapData) {m_pBitmapText = pBitmapData;}
	QBitmapText * GetStationData() { return m_pStation;}
	void  SetStation(QBitmapText * pStationData) {m_pStation = pStationData;}

	virtual void StartProfile(QVector3D & point);				//��ʼ���Ʋ�����
	virtual void ProcessProfile(QVector3D & point);				//����������
	virtual void EndProfile(QVector3D & point);					//����������
	virtual void DoubleMouse(QMouseEvent * event){ };				//���˫���¼�

	virtual void initBaseMapAttribute(QString sWidth, QString sKind, int x, int y, int z);		//��ͼ����

	QList<SymbolPoint *>* GetQListSymbolPoint();

	void isLayerHide(bool islayerhide) {m_isLayerHide = islayerhide;}
	bool isLayerHide(){return m_isLayerHide;}
//	void AddRainCountorLine();
//	void AddTempCountorLine();
	void SetIsDraw(bool DrawContour) {m_bDrawContour = DrawContour;}
	bool GetIsDraw () {return m_bDrawContour;}
	inline void SetNodeCallBack(QNodeCallBack* pncb) { layerRoot->SetNodeCallBack(pncb);}
		
	void LayerWarning();

	void LayerMode(ELayerMode layerMode) {eLayerMode = layerMode;}
	ELayerMode LayerMode() { return eLayerMode;}
	inline P_FIELD_MAP GetFieldMap(){return &m_PendingFieldMap;}
	virtual void DumpGraphicsAttribute(GEOMETRY_SET& resultSet);
public slots:
	virtual void FindedData(QDataArray* data);
	virtual void FindedUnit(GLenum type, QRenderable* data);
protected:
	COLORITEM_TABLE m_mapColorIndex;

	QString m_GraphicsLevel;
	QString m_GraphicsTime;
	int m_GraphicsIndex;
	int m_GraphicsEnsemble;

	QString m_GraphicsLevel2;
	QString m_GraphicsTime2;
	int m_GraphicsIndex2;
	int m_GraphicsEnsemble2;

	int m_GraphicsType;

	QNode* layerRoot;
	QString layerName;
	QString elementName;
	GLfloat layerZ;
	GLdouble minScale;
	GLdouble maxScale;
	CoordinateTransform* coordTrans;
	DataProvider* dataProvider;

	QAttributeSet* m_nAttributeSet;
	/*QPointer<LineWidthAttribute> m_nWidthAttr;
	QPointer<ColorAttribute> m_nColorAttr;
	QPointer<LineStippleAttribute> m_nLineStippleAttr;*/
	LineWidthAttribute* m_nWidthAttr;
	ColorAttribute* m_nColorAttr;
	LineStippleAttribute* m_nLineStippleAttr;
	int PolygonType;					//1 :���߻��ƶ����  2 ����ɫ���ƶ����
	ELayerType eLayerType;						//�ǿɱ༭�㻹�ǲ��ɱ༭��
	ELayerMode eLayerMode;
	bool m_bProfile;					//�ж��Ƿ����������
	//QPointer<Proj4Transform> m_nNewTrans;
	//ColorAttribute *m_nColorAttr;
	LAYER_ATTRIBUTE m_layerAttribute;
	QList<LAYER_ATTRIBUTE> m_listLayerAttribute;
	GLfloat m_layerAlpha;
	QRenderVolume* renderVol;
	QList<SymbolPoint *>  m_pListSymbolpoint;
private:
	bool isBaseMap;		
	QHash<QString,GISPolygon*>  m_HashShadeList; // ��¼��ͼ������
	QHash<QString,GISPolygon*>  m_HashShadeNoTessel; //�ǲü������
	QHash<QString,CSymbolContourLine *>  m_HashLineList;
	QBitmapText * m_pBitmapText;						
	QBitmapText * m_pStation;					//����վ������
	QString mID;
	bool m_isLayerHide;
	bool m_bDrawContour;						//��ʾ���ƽ����ֵ��
	DrawSurface* m_bDrawSurface;
	FIELD_MAP m_PendingFieldMap;
};
//}
