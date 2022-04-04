#pragma once
#include "GIS_WrapperConfig.h"
#include "QT_Include.h"

class ILayerAttribute;
typedef QMap<QString,ILayerAttribute*> LAYER_ATTRIBUTE, *P_LAYER_ATTRIBUTE;
class  GIS_WRAPPER_EXPORT_IMPORT IChange
{
public:
	 virtual   void Change(ILayerAttribute*) = 0;
};
class GIS_WRAPPER_EXPORT_IMPORT ILayerAttribute : public QObject
{
public:
	IChange* m_Change;
	ILayerAttribute(void);
	ILayerAttribute(QString fIndex,
	QString strName,
	QString strLableName,
	QVariant vValue,
	QVariant::Type tType,
	int iMin,
	int iMax,
	QStringList strEnumValue);

	ILayerAttribute(QString fIndex,
		QString strName,
		QString strLableName,
		QVariant vValue,
		QVariant::Type tType,
		double dMin,
		double dMax,
		QStringList strEnumValue);

	~ILayerAttribute(void);

	QString& Name();
	void Name(const QString& name);

	QString& LableName();
	void LableName(const QString& labName);

	QVariant& Value();
	void Value(const QVariant& vvalue);

	QVariant::Type Type();
	void Type(QVariant::Type vtype);

	int Min();
	void Min(int iMin);

	int Max();
	void Max(int iMax);

	double DMin();
	void DMin(double dMin);

	double DMax();
	void DMax(double dMax);

	QString ID();
	void ID(QString);

	QStringList& EnumValue();
	void EnumValue(const QStringList& strList);
	void InsertSubItem(ILayerAttribute* item) ;
	void InsertAdvancedItem(ILayerAttribute* item) ;
	QMap<QString,ILayerAttribute*> * GetSubItems(){ return &VectorSubAttribute;}
	QMap<QString,ILayerAttribute*> * GetAdvancedItems(){ return &AdvancedProperties;}
	
	QPixmap Pixmap(unsigned int &nPixmapIndex,unsigned int &nSymbolSize,float &fsymbolAngle);
	void Pixmap(unsigned int nPixmapIndex,QPixmap& pixmap,unsigned int nSymbolSize,float fsymbolAngle);

	void SetBshowValue(bool bShowValue,float fmaxvalue,float fminvalue);
	void GetBshowValue(bool &bShowValue,float &fmaxvalue,float &fminvalue);

	void SetFontAndColor(QFont vFont ,QColor vColor, int fontShow);
	void GetFontAndColor(QFont &vFont ,QColor &vColor,int &fontShow);

protected:
	QString m_strName;
	QString m_strLableName;
	QVariant m_Value;
	QVariant::Type m_Type;
	int m_Min;
	int m_Max;

	double m_dMin;
	double m_dMax;

	QString m_strIndexName;              //  Ù–‘ID
	QStringList m_EnumValue;
	QMap<QString , ILayerAttribute*> VectorSubAttribute;
	QMap<QString , ILayerAttribute*> AdvancedProperties;

	QPixmap m_Pixmap;
	unsigned int m_nPixmapIndex;
	unsigned int m_nSymbolSize;
	float m_fSymbolAngle;
	bool m_bShowValue;
	float m_fMaxValue;
	float m_fMinValue;
	QFont m_Font;
	QColor m_FontColor;
	int m_FontShow;
};

typedef QMap<QString,ILayerAttribute*> LAYER_ATTRIBUTE, *P_LAYER_ATTRIBUTE;

