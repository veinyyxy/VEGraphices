#include "QTesselator.h"

//QE_LIB_EXPORT_IMPORT void* QTesselator::pPolygonData = 0;
//GLfloat* QTesselator::pfBuffer = 0;
QE_LIB_EXPORT_IMPORT int QTesselator::dimension = 0;
//QVector3DArray QTesselator::temp;
QE_LIB_EXPORT_IMPORT int QTesselator::buffSize = 0;

QTesselator::QTesselator(void) : vertexData(0)
{
	tess = gluNewTess();
	pPolygonData = 0;
}


QTesselator::~QTesselator(void)
{
	if(vertexData)
		delete vertexData;

	gluDeleteTess(tess);
}

void CALLBACK QTesselator::TessEndData(void* polygonData)
{
		//do nothing 
	TessStruct *tessPlogon = static_cast<TessStruct*>(polygonData);
	int index = tessPlogon->TessVectorArray.count();
	tessPlogon->IndexArray.push_back(index);
}
void CALLBACK QTesselator::TessBeginData(GLenum type, void * polygon_data)
{

	TessStruct *tessPlogon = static_cast<TessStruct*>(polygon_data);

	tessPlogon->ModeArray.push_back(type);
}
void CALLBACK QTesselator::TessComBine(GLdouble coords[3], GLdouble *vertex_data[4], GLfloat weight[4], void **outData, void * polygon_data)
{
}

void CALLBACK QTesselator::VertexProcess3df(GLvoid *vertex, void* polygonData)
{
	GLdouble x, y, z;
	x = *((GLdouble*)vertex + 0);
	y = *((GLdouble*)vertex + 1);
	z = *((GLdouble*)vertex + 2);
	TessStruct *tessPlogon = static_cast<TessStruct*>(polygonData);
	tessPlogon->TessVectorArray.push_back(QVector3D(x, y, z));
}

void QTesselator::SetProperty( GLenum which, GLdouble value )
{
	gluTessProperty(tess, which, value);
}

void QTesselator::StartTess()
{
	gluTessBeginPolygon(tess, pPolygonData);
}

void QTesselator::EndTess()
{
	gluTessEndPolygon(tess);
}

int QTesselator::Tessellate3df( TessStruct& des, QDataArray& src, GLsizei vertexCount)
{
	QVector<GLdouble*> tempPointer;
	GLfloat* pfData = 0;
	GLdouble* pdData = 0;
	GLdouble* vertex = 0;

	if(!tess)
		return 0;
 	dimension = src.ArrayDimension();


	pPolygonData = &des;

	//设置分格化的回调函数
	gluTessCallback(tess, GLU_TESS_VERTEX_DATA, (pCPP_Fun)VertexProcess3df);
	gluTessCallback(tess, GLU_TESS_BEGIN_DATA, (pCPP_Fun)TessBeginData);
	gluTessCallback(tess, GLU_TESS_END_DATA, (pCPP_Fun)TessEndData);
	//gluTessCallback(tess, GLU_TESS_COMBINE_DATA,(pCPP_Fun)TessComBine);
	gluTessCallback(tess, GLU_TESS_ERROR_DATA, (pCPP_Fun)TessErrorData);
	
	StartTess();
	StartTessContour();
	////////////////////////////////////////////////////////
	vertex = new GLdouble[3*vertexCount];
	buffSize = 3 * vertexCount;
	memset(vertex, 0, sizeof(GLdouble) * 3*vertexCount);

	for(GLsizei c = 0; c < vertexCount; c++)
	{
		//vertex = new GLdouble[3];
		//tempPointer.push_back(vertex);
		//memset(vertex, 0, sizeof vertex);
		if(src.DataType() == GL_FLOAT && dimension == 3)
		{
			pfData = (GLfloat*)(src.getDataPointer()) + src.Stride() + c * 3;
			vertex[c * 3] = *(pfData + 0);
			vertex[c * 3 + 1] = *(pfData + 1);
			vertex[c * 3 + 2] = *(pfData + 2);
			gluTessVertex(tess, vertex + c * 3, vertex + c * 3);
			//gluTessVertex(tess, vertex + c * 3+1, vertex + c * 3+1);
			//gluTessVertex(tess, vertex + c * 3+2, vertex + c * 3+2);
		}
		else if(src.DataType() == GL_DOUBLE && dimension == 3)
		{
			pdData = (GLdouble*)(src.getDataPointer()) + src.Stride() + c * 3;
			vertex[0] = *(pdData + 0);
			vertex[1] = *(pdData + 1);
			vertex[2] = *(pdData + 2);
			//gluTessVertex(tess, vertex[0], vertex[]);
		}
		else
			return 0;
	}
	////////////////////////////////////////////////////////
	EndTessContour();
	EndTess();

	delete [] vertex;
	return 1;
}

void CALLBACK QTesselator::TessErrorData( GLenum errno, void *polygon_data )
{

}

void QTesselator::StartTessContour()
{
	gluTessBeginContour(tess);
}

void QTesselator::EndTessContour()
{
	gluTessEndContour(tess);
}
/*
int QTesselator::TessellateCombine( USERDATA* pOutDes, int* iDesSize, const QDataArray* pInSrc, int iSrcSize )
{
	QVector<GLdouble*> tempPointer;
	GLfloat* pfData = 0;
	GLdouble* pdData = 0;
	GLdouble* vertex = 0;
	int vertexCount = 0;
	if(!tess)
		return 0;
	void* src = 0;
	//dimension = pInSrc->ArrayDimension();


	pPolygonData = pOutDes;

	//设置分格化的回调函数
	gluTessCallback(tess, GLU_TESS_VERTEX_DATA, (pCPP_Fun)VertexProcess3df);
	gluTessCallback(tess, GLU_TESS_BEGIN_DATA, (pCPP_Fun)TessBeginData);
	gluTessCallback(tess, GLU_TESS_END_DATA, (pCPP_Fun)TessEndData);
	gluTessCallback(tess, GLU_TESS_COMBINE_DATA,(pCPP_Fun)TessComBine);
	gluTessCallback(tess, GLU_TESS_ERROR_DATA, (pCPP_Fun)TessErrorData);
	gluTessProperty(tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NEGATIVE);
	StartTess();
	for(int i = 0; i < iSrcSize; i++)
	{
		QDataArray* pVertexArray = ((QDataArray*)dynamic_cast<const QDataArray*>(pInSrc) + i);
		vertexCount = pVertexArray->getDataCount();

		StartTessContour();
		////////////////////////////////////////////////////////
		vertex = new GLdouble[3*vertexCount];
		buffSize = 3 * vertexCount;
		memset(vertex, 0, sizeof(GLdouble) * 3*vertexCount);

		for(GLsizei c = 0; c < vertexCount; c++)
		{
			if(pVertexArray->DataType() == GL_FLOAT && dimension == 3)
			{
				pfData = (GLfloat*)(pVertexArray->getDataPointer()) + pVertexArray->Stride() + c * 3;
				vertex[c * 3] = *(pfData + 0);
				vertex[c * 3 + 1] = *(pfData + 1);
				vertex[c * 3 + 2] = *(pfData + 2);
				gluTessVertex(tess, vertex + c * 3, vertex + c * 3);
			}
			else if(pVertexArray->DataType() == GL_DOUBLE && dimension == 3)
			{
				pdData = (GLdouble*)(pVertexArray->getDataPointer()) + pVertexArray->Stride() + c * 3;
				vertex[0] = *(pdData + 0);
				vertex[1] = *(pdData + 1);
				vertex[2] = *(pdData + 2);
				gluTessVertex(tess, vertex + c * 3, vertex + c * 3);
			}
			else
				return 0;
		}
		////////////////////////////////////////////////////////
		EndTessContour();
		delete [] vertex;
	}
	EndTess();

	return 1;
}
*/

int QTesselator::TessellateCombine( TessStruct* pOutDes, int* iDesSize, const QVector<QDataArray*>* pInSrc, int iSrcSize )
{
	QVector<GLdouble*> tempPointer;
	GLfloat* pfData = 0;
	GLdouble* pdData = 0;
	GLdouble* vertex = 0;
	int vertexCount = 0;
	if(!tess)
		return 0;
	void* src = 0;
	//dimension = pInSrc->ArrayDimension();


	pPolygonData = pOutDes;

	//设置分格化的回调函数
	gluTessCallback(tess, GLU_TESS_VERTEX_DATA, (pCPP_Fun)VertexProcess3df);
	gluTessCallback(tess, GLU_TESS_BEGIN_DATA, (pCPP_Fun)TessBeginData);
	gluTessCallback(tess, GLU_TESS_END_DATA, (pCPP_Fun)TessEndData);
	gluTessCallback(tess, GLU_TESS_COMBINE_DATA,(pCPP_Fun)TessComBine);
	gluTessCallback(tess, GLU_TESS_ERROR_DATA, (pCPP_Fun)TessErrorData);
	//gluTessProperty(tess, GLU_TESS_TOLERANCE, 0);
	//gluTessCallback(tess, GLU_TESS_ERROR_DATA, (pCPP_Fun)TessErrorData);
	gluTessProperty(tess,GLU_TESS_WINDING_RULE,GLU_TESS_WINDING_POSITIVE);
	StartTess();
	for(int i = 0; i < iSrcSize; i++)
	{
		QDataArray* pVertexArray = (*pInSrc)[i];
		vertexCount = pVertexArray->getDataCount();

		//StartTessContour();
		////////////////////////////////////////////////////////
		vertex = new GLdouble[3*vertexCount];
		buffSize = 3 * vertexCount;
		memset(vertex, 0, sizeof(GLdouble) * 3*vertexCount);
		gluTessBeginContour(tess);
		for(GLsizei c = 0; c < vertexCount; c++)
		{
			dimension = pVertexArray->ArrayDimension();
			if(pVertexArray->DataType() == GL_FLOAT && dimension == 3)
			{
				pfData = (GLfloat*)(pVertexArray->getDataPointer()) + pVertexArray->Stride() + c * 3;
				vertex[c * 3] = *(pfData + 0);
				vertex[c * 3 + 1] = *(pfData + 1);
				vertex[c * 3 + 2] = *(pfData + 2);
				gluTessVertex(tess, vertex + c * 3, vertex + c * 3);
			}
			else if(pVertexArray->DataType() == GL_DOUBLE && dimension == 3)
			{
				pdData = (GLdouble*)(pVertexArray->getDataPointer()) + pVertexArray->Stride() + c * 3;
				vertex[0] = *(pdData + 0);
				vertex[1] = *(pdData + 1);
				vertex[2] = *(pdData + 2);
				gluTessVertex(tess, vertex + c * 3, vertex + c * 3);
			}
			else
				return 0;
		}
		////////////////////////////////////////////////////////
		gluTessEndContour(tess);
		tempPointer.push_back(vertex);
		//delete [] vertex;
	}
	EndTess();
	for(QVector<GLdouble*>::iterator index = tempPointer.begin(); index != tempPointer.end(); index++)
	{
		GLdouble* p = *index;
		if(p) delete[] p;
	}
	return 1;
}
