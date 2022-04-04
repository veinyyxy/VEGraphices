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
	//三维顶点数组
	QVector3DArray vertex;

	//四维颜色数组，详细帮助参见OpenGL定点颜色的设置
	QVector4DArray colorAttr;

	//二维纹理数组
	QVector2DArray texture;

	//法线数组
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
	// Parameter: GLfloat angle 旋转的角度
	// Parameter: QVector3D aix 绕那个轴旋转
	//************************************
	int Rotation(GLfloat angle, QVector3D aix);

	//************************************
	// Method:    Move
	// FullName:  Geometric::Move
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: QVector3D pos(此几何对象移动到什么位置)
	//************************************
	int Move(QVector3D pos);

	//************************************
	// Method:    Scale
	// FullName:  Geometric::Scale
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: GLfloat scale(此几何对象中的所有方向上的缩放比例)
	//************************************
	int Scale(GLfloat scale);

	//************************************
	// Method:    Color
	// FullName:  Geometric::Color
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: const QVector4D c(此几何对象中的所有顶点的颜色)
	//************************************
	int Color(const QVector4D c);

	//************************************
	// Method:    getVectexArray
	// FullName:  Geometric::getVectexArray
	// Access:    public 
	// Returns:   QVector3DArray*(此几何对象中的顶点数组指针)
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
	// Returns:   QVector4DArray* (此几何对象中的颜色数组指针)
	// Qualifier:
	//************************************
	virtual QVector4DArray* getColorArray();

	//************************************
	// Method:    getTextureAttray
	// FullName:  Geometric::getTextureAttray
	// Access:    public 
	// Returns:   QVector2DArray* (此几何对象中的纹理数组指针)
	// Qualifier:
	//************************************
	virtual QVector2DArray* getTextureArray();

	//************************************
	// Method:    getNormalAttray
	// FullName:  Geometric::getNormalAttray
	// Access:    public 
	// Returns:   QVector3DArray*(此几何对象中的定点发现数组指针)
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
