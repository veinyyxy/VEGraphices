#pragma once

#include "QT_Include.h"
#include "GL_Include.h"

enum DATA_TYPE
{
	V4Dd = 1,
	V4Df,
	V3Dd,
	V3Df,
	V3Ds,
	V2Dd,
	V2Df,
	V2Di,
	V2Ds,
	V1Dui,
	V1Di,
	V1Dd,
	V1Df,
	V1Ds
};

enum DATA_COMPOSE
{
	SIMPLEX = 1,
	MIXIN,
	INDEX
};

#define QUSAGE GLenum
#define QDWORD unsigned

class QDrawArray;

class QDataArray : virtual public QObject
{
public:
	QDataArray()
	{
		stride = 0;
		dimension = 0;
		dataComps = (DATA_COMPOSE)0;
		dataUsage = 0;
		dataType = (DATA_TYPE)0;
	}
	QDataArray(GLsizei stid, GLenum arrDime, DATA_COMPOSE dataComp, QUSAGE usg, GLenum type) : 
		stride(stid),
		dimension(arrDime),
		dataComps(dataComp),
		dataUsage(usg),
		dataType(type)
	{

	}
	QDataArray(const QDataArray& it)
	{
		*this = it;
	}

	void DataComps(DATA_COMPOSE comps){dataComps = comps;}
	DATA_COMPOSE DataComps(){return dataComps;}

	//************************************
	// Method:    DataUsage
	// FullName:  QDataArray::DataUsage
	// Access:    public 
	// Returns:   void
	// Qualifier: 
	// Parameter: QUSAGE datatype
	//************************************
	void DataUsage(QUSAGE datatype){dataUsage = datatype;}
	QUSAGE DataUsage(){return dataUsage;}

	//************************************
	// Method:    ArrayDimension
	// FullName:  QDataArray::ArrayDimension
	// Access:    public 
	// Returns:   void
	// Qualifier: 
	// Parameter: GLint state
	//************************************
	void ArrayDimension(GLint state){dimension = state;}
	GLint ArrayDimension(){return dimension;}

	//************************************
	// Method:    getDataPointer
	// FullName:  QDataArray::getDataPointer
	// Access:    virtual public 
	// Returns:   GLvoid*
	// Qualifier: 
	//************************************
	virtual GLvoid* getDataPointer() = 0;
	//************************************
	// Method:    getDataCount
	// FullName:  QDataArray::getDataCount
	// Access:    virtual public 
	// Returns:   int
	// Qualifier: 
	//************************************
	virtual int getDataCount() = 0;
	//************************************
	// Method:    Resize
	// FullName:  QDataArray::Resize
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: 
	// Parameter: int size
	//************************************
	virtual void Resize(int size) = 0;
	virtual QDrawArray* AsDrawArray() = 0;
	//************************************
	// Method:    Clear
	// FullName:  QDataArray::Clear
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: 
	//************************************
	virtual void Clear() = 0;
	virtual void Delete(unsigned int iIndex) = 0;
	//************************************
	// Method:    Stride
	// FullName:  QDataArray::Stride
	// Access:    public 
	// Returns:   void
	// Qualifier: 
	// Parameter: GLsizei strid
	//************************************
	void Stride(GLsizei strid){stride = strid;}
	GLsizei Stride(){return stride;}

	//************************************
	// Method:    DataType
	// FullName:  QDataArray::DataType
	// Access:    public 
	// Returns:   void
	// Qualifier: 
	// Parameter: GLenum type
	//************************************
	void DataType(GLenum type){dataType = type;}
	GLenum DataType(){return dataType;}

	QDataArray& operator=(const QDataArray& it)
	{
		this->stride = it.stride;
		this->dimension = it.dimension;
		this->dataComps = it.dataComps;
		this->dataUsage = it.dataUsage;
		this->dataType = it.dataType;
		return *this;
	}
protected:
	GLsizei stride;
	GLint dimension;
	DATA_COMPOSE dataComps;
	QUSAGE dataUsage;
	GLenum dataType;
};

class QDrawArray
{
public:
	virtual void StartDraw() = 0;
	virtual void Draw() = 0;
	virtual void Draw(GLenum mode) = 0;
	virtual void EndDraw() = 0;
	virtual QDataArray* AsDataArray() = 0;
	virtual GLenum ArrayMode(){return m_Mode;}
	virtual void ArrayMode(GLenum mode){m_Mode = mode;}
protected:
	GLenum m_Mode;
};

template<typename T, GLenum TYPE, QUSAGE USGE, DATA_COMPOSE COMPS, GLenum ARRSTATE, GLsizei STRIDE>
//class QTemplateDataArray : public QDataArray, public QVector<T>
class QTemplateDataArray : public QDataArray, public QVector<T>, QDrawArray
{
public:
	QTemplateDataArray(
		GLsizei stid = STRIDE, GLenum arrDime = ARRSTATE, 
		DATA_COMPOSE dataComp = COMPS, QUSAGE usg = USGE, 
		GLenum type = TYPE) :
	  QDataArray(stid, arrDime, dataComp, usg, type){}

	//QTemplateDataArray(void){}
	~QTemplateDataArray(void){}
	virtual GLvoid* getDataPointer(){return QVector<T>::data();}
	QTemplateDataArray(const QTemplateDataArray& me)
	{
		*this = me;
	}
	virtual int getDataCount()
	{
		return QVector<T>::size();
	}
	void Resize(int size)
	{
		QVector<T>::resize(size);
	}
	void Clear()
	{
		QVector<T>::clear();
	}

	void Delete(unsigned int iIndex)
	{
		QVector<T>::remove(iIndex);
	}

	QTemplateDataArray<T, TYPE, USGE, COMPS, ARRSTATE, STRIDE>& operator=(const QTemplateDataArray& it)
	{
		this->stride = it.stride;
		this->dimension = it.dimension;
		this->dataComps = it.dataComps;
		this->dataUsage = it.dataUsage;
		this->dataType = it.dataType;
		
		QVector<T>::operator =(it);
		return *this;
	}

	void StartDraw()
	{
		glEnableClientState(dataUsage);
		if(dataUsage == GL_VERTEX_ARRAY)
			glVertexPointer(dimension, dataType, stride, getDataPointer());
		else if(dataUsage == GL_NORMAL_ARRAY)
			glNormalPointer(dataType, stride, getDataPointer());
		else if(dataUsage == GL_COLOR_ARRAY)
			glColorPointer(dimension, dataType, stride, getDataPointer());
		else if(dataUsage == GL_TEXTURE_COORD_ARRAY)
			glTexCoordPointer(dimension, dataType, stride, getDataPointer());
		else if(dataType == GL_EDGE_FLAG_ARRAY)
			glEdgeFlagPointer(stride, getDataPointer());
		else if(dataType == GL_INDEX_ARRAY)
			glIndexPointer(dataType, stride, getDataPointer());
		else
			return;
	}

	void Draw()
	{
		if(dataUsage == GL_VERTEX_ARRAY)
			glDrawArrays(m_Mode, 0, QVector<T>::size());
	}

	void Draw(GLenum mode)
	{
		if(dataUsage == GL_VERTEX_ARRAY)
			glDrawArrays(mode, 0, QVector<T>::size());
	}

	void EndDraw()
	{
		glDisableClientState(dataUsage);
	}

	QDataArray* AsDataArray()
	{
		return (QDataArray*)this;
	}

	QDrawArray* AsDrawArray()
	{
		return (QDrawArray*)this;
	}
};
typedef QTemplateDataArray<QVector4D, GL_DOUBLE, 0, SIMPLEX, 4, 0> QVector4DArrayd;
typedef QTemplateDataArray<QVector4D, GL_FLOAT, 0, SIMPLEX, 4, 0> QVector4DArray;
typedef QTemplateDataArray<QVector3D, GL_DOUBLE, 0, SIMPLEX, 3, 0> QVector3DArrayd;
typedef QTemplateDataArray<QVector3D, GL_FLOAT, 0, SIMPLEX, 3, 0> QVector3DArray;
typedef QTemplateDataArray<QVector2D, GL_FLOAT, 0, SIMPLEX, 2, 0> QVector2DArray;
typedef QTemplateDataArray<QDWORD, GL_DOUBLE, 0, INDEX, 1, 0> QDWordArray;
//typedef QTemplateDataArray<QColor, GL_FLOAT, 0, SIMPLEX, 4, 0> QColorArray;

typedef QTemplateDataArray<GLdouble, GL_DOUBLE, 0, MIXIN, 0, 0> QDoubleArray;

typedef QTemplateDataArray<GLfloat, GL_FLOAT, 0, MIXIN, 0, 0> QFloatArray;
typedef QTemplateDataArray<GLint, GL_INT, 0, SIMPLEX, 1, 0> QIntArray;
typedef QTemplateDataArray<GLshort, GL_SHORT, 0, SIMPLEX, 1, 0> QShortArray;
//typedef QTemplateDataArray<GLbyte> QByteArray;

struct TessStruct
{
	//unsigned int  TessMode;
	QVector<unsigned int> ModeArray;
	QVector3DArray TessVectorArray;	
	QVector<int> IndexArray;
};
typedef QVector<TessStruct*> USERDATA;

typedef  QVector<QVector3DArray> ALGORITHM_DATA;