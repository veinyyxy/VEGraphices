#include "ILayerAttribute.h"


ILayerAttribute::ILayerAttribute(void)
{
}

ILayerAttribute::ILayerAttribute(QString fIndex, QString strName, QString strLableName, QVariant vValue, QVariant::Type tType, int iMin, int iMax, QStringList strEnumValue )
{
	m_strIndexName = fIndex;
	m_strName = strName;
	m_strLableName = strLableName;
	m_Value = vValue;
	m_Type = tType;
	m_Min = iMin;
	m_Max = iMax;
	m_EnumValue = strEnumValue;
}

ILayerAttribute::ILayerAttribute( QString fIndex, QString strName, QString strLableName, QVariant vValue, QVariant::Type tType, double dMin, double dMax, QStringList strEnumValue )
{
	m_strIndexName = fIndex;
	m_strName = strName;
	m_strLableName = strLableName;
	m_Value = vValue;
	m_Type = tType;
	m_dMin = dMin;
	m_dMax = dMax;
	m_EnumValue = strEnumValue;
}

ILayerAttribute::~ILayerAttribute(void)
{
	QMap<QString , ILayerAttribute*>::Iterator it;
	for(it = VectorSubAttribute.begin(); it != VectorSubAttribute.end(); it++)
	{
		delete it.value();
		it.value()= 0;
	}
	VectorSubAttribute.clear();
}

QString& ILayerAttribute::Name()
{
	return m_strName;
}

void ILayerAttribute::Name( const QString& name )
{
	m_strName = name;
}

QString& ILayerAttribute::LableName()
{
	return m_strLableName;
}

void ILayerAttribute::LableName( const QString& labName )
{
	m_strLableName = labName;
}

QVariant& ILayerAttribute::Value()
{
	return m_Value;
}

void ILayerAttribute::Value( const QVariant& vvalue )
{
	m_Value = vvalue;
}

QVariant::Type ILayerAttribute::Type()
{
	return m_Type;
}

void ILayerAttribute::Type( QVariant::Type vtype )
{
	m_Type = vtype;
}

int ILayerAttribute::Min()
{
	return m_Min;
}

void ILayerAttribute::Min( int iMin )
{
	m_Min = iMin;
}

int ILayerAttribute::Max()
{
	return m_Max;
}

void ILayerAttribute::Max( int iMax )
{
	m_Max = iMax;
}
////////////////////////////////////////////Double///////////////////////

double ILayerAttribute::DMin()
{
	return m_dMin;
}

void ILayerAttribute::DMin( double dMin )
{
	m_dMin = dMin;
}

double ILayerAttribute::DMax()
{
	return m_dMax;
}

void ILayerAttribute::DMax( double dMax )
{
	m_dMax = dMax;
}
////////////////////////////////////////////Double///////////////////////

QString ILayerAttribute::ID()
{
	return m_strIndexName;
}

void ILayerAttribute::ID(QString index)
{
	m_strIndexName = index;
}

QStringList& ILayerAttribute::EnumValue()
{
	return m_EnumValue;
}

void ILayerAttribute::EnumValue( const QStringList& strList )
{
	m_EnumValue = strList;
}
void ILayerAttribute::InsertSubItem(ILayerAttribute* item) 
{
	VectorSubAttribute.insert(item->ID(),item);
}

QPixmap ILayerAttribute::Pixmap(unsigned int &nPixmapIndex,unsigned int &nSymbolSize,float &fsymbolAngle)
{
	nPixmapIndex = m_nPixmapIndex;
	nSymbolSize = m_nSymbolSize;
	fsymbolAngle = m_fSymbolAngle;
	return m_Pixmap;
}

void ILayerAttribute::Pixmap(unsigned int nPixmapIndex,QPixmap& pixmap,unsigned int nSymbolSize,float fsymbolAngle)
{
	m_nPixmapIndex = nPixmapIndex;
	m_Pixmap = pixmap;
	m_nSymbolSize = nSymbolSize;
	m_fSymbolAngle = fsymbolAngle;
}

void ILayerAttribute::SetBshowValue(bool bShowValue,float fmaxvalue,float fminvalue)
{
	m_bShowValue = bShowValue;
	m_fMaxValue = fmaxvalue;
	m_fMinValue = fminvalue;
}
void ILayerAttribute::GetBshowValue(bool &bShowValue,float &fmaxvalue,float &fminvalue)
{
	bShowValue = m_bShowValue;
	fmaxvalue = m_fMaxValue;
	fminvalue = m_fMinValue;
}

void ILayerAttribute::InsertAdvancedItem( ILayerAttribute* item )
{
	AdvancedProperties.insert(item->Name(),item);
}

void ILayerAttribute::SetFontAndColor(QFont vFont ,QColor vColor,int fontShow)
{
	m_Font = vFont;
	m_FontColor = vColor;
	m_FontShow = fontShow;
}

void ILayerAttribute::GetFontAndColor(QFont &vFont ,QColor &vColor, int &fontShow)
{
	vFont = m_Font;
	vColor = m_FontColor;
	fontShow = m_FontShow;
}