#pragma once
#include "QT_Include.h"
#include "QDataArray.h"

#ifndef CALLBACK 
	#define CALLBACK
#endif

typedef void (CALLBACK *pC_Fun)(GLvoid *vertex);
typedef void (CALLBACK *pCPP_Fun)(void);

class QE_LIB_EXPORT_IMPORT QTesselator : public QObject
{
public:
	QTesselator(void);
	~QTesselator(void);
	void SetProperty(GLenum which, GLdouble value);
	int Tessellate3df(TessStruct& des, QDataArray& src, GLsizei vertexCount);
	//int TessellateCombine(USERDATA* pOutDes, int* iDesSize, const QDataArray* pInSrc, int iSrcSize);
	int TessellateCombine(TessStruct* pOutDes, int* iDesSize, const QVector<QDataArray*>* pInSrc, int iSrcSize);

private:
	GLUtesselator *tess;
	void StartTess();
	void StartTessContour();
	void EndTess();
	void EndTessContour();
	static void CALLBACK VertexProcess3df(GLvoid *vertex, void* polygonData);
	static void CALLBACK TessEndData(void* polygonData);
	static void CALLBACK TessBeginData(GLenum type, void * polygon_data);
    static void CALLBACK TessComBine(GLdouble coords[3], GLdouble *vertex_data[4], GLfloat weight[4], void **outData, void * polygon_data);
	static void CALLBACK TessErrorData(GLenum errno, void *polygon_data);
           void* pPolygonData;
	static int dimension;
	GLdouble* vertexData;
	static int buffSize;
};



