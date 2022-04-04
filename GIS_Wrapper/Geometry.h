#ifndef GEOMETRIC_H
#define GEOMETRIC_H
#include "GIS_WrapperConfig.h"
#include "QT_Include.h"
#include "QRenderUnit.h"
#include "QRenderVolume.h"
#include "QAttribute.h"
#include "ColorAttribute.h"
#include "CoordinateTransform.h"

typedef QRenderVolume GISMultiPolygon;
typedef QRenderVolume GISMultiLine;
typedef QRenderVolume GISMultiPoint;

class Line;
class Plane;
class Point;
class GISPolygon;

typedef struct  _CustomizeAttribute
{
	QString name;
	QVariant value;
} CUSTOMIZE_ATTRIBUTE, *P_CUSTOMIZE_ATTRIBUTE;

class GIS_WRAPPER_EXPORT_IMPORT Geometry : public QRenderUnit
{
	//Q_OBJECT;
public:
	typedef QMap<int, QVariant> ATTRIBUTE_MAP, *P_ATTRIBUTE_MAP;

protected:
	//��ά��������
	QVector3DArray vertex;

	//��ά��ɫ���飬��ϸ�����μ�OpenGL������ɫ������
	QVector4DArray colorAttr;

	//��ά��������
	QVector2DArray texture;

	//��������
	//QVector3DArray normal;

	QObject dataRoot;
	ColorAttribute color;
	//QAttribute size;
	QSet<CUSTOMIZE_ATTRIBUTE> customAttribue;
	RESTRICT_TRAMSFORM transformRestrict;
	QAttributeSet attribSet;
	CoordinateTransform* coordTrans;
	ATTRIBUTE_MAP m_attributeMap;
	QString m_strAtribtName;
	bool m_Validity;
	bool m_IsDirty;
	int m_iGeometryID;

public:
	Geometry();
	
	//************************************
	// Method:    Rotation
	// FullName:  Geometric::Rotation
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: GLfloat angle ��ת�ĽǶ�
	// Parameter: QVector3D aix ���Ǹ�����ת
	//************************************
	int Rotation(GLfloat angle, QVector3D aix);

	//************************************
	// Method:    Move
	// FullName:  Geometric::Move
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: QVector3D pos(�˼��ζ����ƶ���ʲôλ��)
	//************************************
	int Move(QVector3D pos);

	//************************************
	// Method:    Scale
	// FullName:  Geometric::Scale
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: GLfloat scale(�˼��ζ����е����з����ϵ����ű���)
	//************************************
	int Scale(GLfloat scale);

	//************************************
	// Method:    Color
	// FullName:  Geometric::Color
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: const QVector4D c(�˼��ζ����е����ж������ɫ)
	//************************************
	int Color(const QVector4D c);

	//************************************
	// Method:    getVectexArray
	// FullName:  Geometric::getVectexArray
	// Access:    public 
	// Returns:   QVector3DArray*(�˼��ζ����еĶ�������ָ��)
	// Qualifier:
	//************************************
	inline QVector3DArray* getVectexArray()
	{
		//SetArrayData(&vertex);
		return &vertex;
	}
	//************************************
	// Method:    setVectexArray
	// FullName:  Geometric::setVectexArray
	// Access:    public 
	// Returns:   void 
	// Qualifier:
	//************************************
	inline void setVectexArray(QVector3DArray &Array)
	{
		vertex = Array;
	}

	//************************************
	// Method:    setVectexArray
	// FullName:  Geometric::setVectexArray
	// Access:    public 
	// Returns:   void 
	// Qualifier:
	//************************************
	inline void setColorArrayData(QVector4DArray &Array)
	{
		colorAttr = Array;
	}
	//************************************
	// Method:    getColorArray
	// FullName:  Geometric::getColorArray
	// Access:    public 
	// Returns:   QVector4DArray* (�˼��ζ����е���ɫ����ָ��)
	// Qualifier:
	//************************************
	virtual QVector4DArray* getColorArray();

	//************************************
	// Method:    getTextureAttray
	// FullName:  Geometric::getTextureAttray
	// Access:    public 
	// Returns:   QVector2DArray* (�˼��ζ����е���������ָ��)
	// Qualifier:
	//************************************
	virtual QVector2DArray* getTextureArray();

	//************************************
	// Method:    getNormalAttray
	// FullName:  Geometric::getNormalAttray
	// Access:    public 
	// Returns:   QVector3DArray*(�˼��ζ����еĶ��㷢������ָ��)
	// Qualifier:
	//************************************
	virtual QVector3DArray* getNormalAttray() = 0;
	virtual Line* AsLine() = 0;
	virtual Plane* AsPlane() = 0;
	virtual GISPolygon* AsPolygon() = 0;
	virtual Point* AsPoint() = 0;
	inline void CoordTrans(const CoordinateTransform* ct);
	inline CoordinateTransform* CoordTrans();
	virtual void ProjectionTransform();
	inline void SetAttributeMap(const P_ATTRIBUTE_MAP pMap){m_attributeMap = *pMap;}
	inline P_ATTRIBUTE_MAP GetAttributeMap(){return &m_attributeMap;}
	inline void SetAttributeName(const QString& name){m_strAtribtName = name;}
	inline QString& GetAtributeName(){return m_strAtribtName;}
	inline void SetID(int id){m_iGeometryID = id;}
	inline int GetID(){return m_iGeometryID;}

	void SetColorArray()
	{
		SetArrayData(true,&colorAttr);
	}

	void SetColorArray(bool bEnable)
	{
		SetArrayData(bEnable,&colorAttr);
	}

	void SetTextureArray()
	{
		SetArrayData(true, &texture);
	}
public slots:
	void FindedData(QDataArray* data);
	//void InitGeometric();

};

#endif
