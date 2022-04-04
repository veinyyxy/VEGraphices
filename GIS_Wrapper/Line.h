#ifndef LINE_H
#define LINE_H
#include "GIS_WrapperConfig.h"
#include "Geometry.h"
#include "LineStippleAttribute.h"
#include "LineWidthAttribute.h"
#include "Polygon.h"

class GIS_WRAPPER_EXPORT_IMPORT Line : public Geometry
{
public:
    Line();
	inline void SetLineMode(GLenum mode);
	void SetLineStyle(GLfloat width, GLint factor, GLushort pattern);
	inline QVector2DArray* getTextureAttray();
	inline QVector3DArray* getNormalAttray();
	inline Line* AsLine();
	inline Plane* AsPlane();
	GISPolygon* AsPolygon();
	Point* AsPoint();
	virtual bool PickUp(const QVector3D* pickPoint);
	void SetPosition(int nPos){m_nPos = nPos;}//设置拾取点的位置--renxue
	int  GetPosition(){return m_nPos;}//获得拾取点的位置--renxue
	void ScaleLine(float xscale,float yscale); //对线进行缩放

public:
	bool isInYScale(QVector3D& mouseGlPos,float dx); //是否选择了符号外接矩形的X轴
	bool isInXScale(QVector3D& mouseGlPos,float dx);
	bool isInXYScale(QVector3D& mouseGlPos,float dx);
	inline QVector3D GetglCenter() {return m_glcenter;}
	inline float GetBoundH() {return m_boundmaxy-m_boundminy;}
	inline float GetBoundW() {return m_boundmaxx-m_boundminx;}
protected:
//	inline void CalGeometryBound();

private:
	LineStippleAttribute lineStipple;
	LineWidthAttribute lineWidth;
	int m_nPos;
private:
	float m_boundminx;
	float m_boundmaxx;
	float m_boundminy;
	float m_boundmaxy;
	QVector3D m_glcenter;
};

#endif
