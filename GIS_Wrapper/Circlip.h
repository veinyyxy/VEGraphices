#pragma once
#include "geometry.h"
class Circlip :
	public Geometry
{
public:
	Circlip(void);
	~Circlip(void);
	void InitCirclip(GLfloat radius, GLint slices, QColor& scolor);
	//QVector3DArray* getNormalAttray();
	inline Line* AsLine(){return 0;}
	inline Plane* AsPlane(){return 0;}
	inline GISPolygon* AsPolygon(){return 0;}
	inline Point* AsPoint(){return 0;}
	void Render();
	void SetColorBlend(bool bcb){m_bBlend = bcb;}
	void DisableRotate(bool br){m_bRotate = br;}
	void DisableTranslate(bool bt){m_bTrans = bt;}

private:
	void ConcrateCirclip(float radius, GLint slices, float fZ, float fO, float fI);
	bool m_bBlend;
	bool m_bRotate;
	bool m_bTrans;
};

