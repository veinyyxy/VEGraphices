/*=================================================================================== 
项目名： 人机交互平台
作 者： 杨晓宇
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年4月18日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：GISPolygon.h GISPolygon.cpp 
类 名： GISPolygon
父 类： Geometry
子 类： 无 
功能说明：传递符号线算法数据数据，收集符号线算法输出数据
调用说明：
------------------------------------------------------------------------------------*/
#ifndef POLYGON_H
#define POLYGON_H
#include "GIS_WrapperConfig.h"
#include "Geometry.h"
#include "QTesselator.h"
//namespace GIS
//{
typedef void (CALLBACK *pCPP_Fun)(void);

class GIS_WRAPPER_EXPORT_IMPORT GISPolygon : public Geometry
{
public:
    GISPolygon();
	~GISPolygon();
	inline void SetPolygonMode(GLenum mode){Mode(mode);}
	TessStruct* TesselPolygon();
	/*inline QVector4DArray* getColorArray();*/
	//inline QVector2DArray* getTextureAttray();
	inline QVector3DArray* getNormalAttray();
	inline Line* AsLine();
	inline Plane* AsPlane();
	inline GISPolygon* AsPolygon();
	inline Point* AsPoint();
	bool Is_Tu();
	void SetIsDrawTessPolygon(bool isDraw) {m_bIsDrawTessPoly = isDraw;}
	bool isDrawTessPolygon() {return m_bIsDrawTessPoly;} 
	void Render();
private:
	void DrawTessPolygon();
	//void Render();
private :
	/*GLuint tessellate();*/
// 	static void CALLBACK tessBeginCB(GLenum which);
// 	static void CALLBACK tessEndCB();
// 	static void CALLBACK tessVertexCB(const GLvoid *data);
	qreal left_right(const QVector3D* a, const QVector3D* b, const QVector3D* c);
	bool is_tu(QVector3DArray* p);
private:
	//QVector3DArray vertex;
	GLuint m_listId;
	QVector<GISPolygon*> m_TessObject;
	TessStruct *m_pTessData;		//细分对象
	bool m_bIsDrawTessPoly;
	/*GLUtesselator *glu_tess;*/
};

//}
#endif
