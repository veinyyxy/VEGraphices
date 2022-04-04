/*=================================================================================== 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ������
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011��4��18��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����GISPolygon.h GISPolygon.cpp 
�� ���� GISPolygon
�� �ࣺ Geometry
�� �ࣺ �� 
����˵�������ݷ������㷨�������ݣ��ռ��������㷨�������
����˵����
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
	TessStruct *m_pTessData;		//ϸ�ֶ���
	bool m_bIsDrawTessPoly;
	/*GLUtesselator *glu_tess;*/
};

//}
#endif
