#pragma once
#include "Geometry.h"
class GIS_WRAPPER_EXPORT_IMPORT EarthSphere : public Geometry
{
public:
	EarthSphere(void);
	~EarthSphere(void);
	void InitShpere(GLfloat radius, GLint slices, QColor& scolor);
	QVector3DArray* getNormalAttray();
	inline Line* AsLine(){return 0;}
	inline Plane* AsPlane(){return 0;}
	inline GISPolygon* AsPolygon(){return 0;}
	inline Point* AsPoint(){return 0;}
	void Render();
	void SetColorBlend(bool bcb){m_bBlend = bcb;}
	void DisableRotate(bool br){m_bRotate = br;}
	void DisableTranslate(bool bt){m_bTrans = bt;}
	void EnableTextrue(bool bt){
		if(bt)
			SetArrayData(&texture);
		else
			SetArrayData(false, &texture);
	}

private:
	void getPoint(GLfloat radius, GLfloat a, GLfloat b, QVector3D &p);
	void drawSlice(QVector3D &p1, QVector3D &p2, QVector3D &p3, QVector3D &p4, GLuint mode);
	QVector3D* getPointMatrix(GLfloat radius, GLint slices);
	int drawSphere(GLfloat radius, GLint slices, GLuint mode);
	void NormalVertor(QVector3D out, const QVector3D& in, const QVector3D& in1);

	//法线数组
	QVector3DArray m_v3Normal;
	QColor m_color;
	bool m_bBlend;
	bool m_bRotate;
	bool m_bTrans;
	bool m_bEnableTextrue;
};

