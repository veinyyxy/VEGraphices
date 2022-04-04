#include "Geometry.h"

Geometry::Geometry() : QRenderUnit(SIMPLEX), coordTrans(0)
{
	vertex.DataUsage(GL_VERTEX_ARRAY);
	colorAttr.DataUsage(GL_COLOR_ARRAY);
	texture.DataUsage(GL_TEXTURE_COORD_ARRAY);
	/*normal.DataUsage(GL_NORMAL_ARRAY);
	*/
	SetArrayData(&vertex);
	//SetArrayData(&colorAttr);
	//connect(this, SIGNAL(signal_Finded(QDataArray*)), this, SLOT(FindedData(QDataArray*)));
	///////////////////////////////////////////
}

int Geometry::Move(QVector3D pos) 
{
	QMatrix4x4 rot;
	rot.translate(pos);

	//RenderableTransform *= rot;
	//ApplyTransform();
	return 0;
}

int Geometry::Color(const QVector4D c) 
{
	color.Color(c);
	attribSet.insert(QE_ATTRI_COLOR, &color);
	AttribSet(attribSet);
    return 0;
}

int Geometry::Rotation(GLfloat angle, QVector3D aix)
{
	QMatrix4x4 rot;
	rot.rotate(angle, aix);

	//RenderableTransform *= rot;
	//ApplyTransform();
	return 0;
}

int Geometry::Scale( GLfloat scale )
{
	QMatrix4x4 rot;
	rot.scale(scale);

	//RenderableTransform *= rot;
	//ApplyTransform();
	return 0;
}

void Geometry::CoordTrans( const CoordinateTransform* ct )
{
	coordTrans = (CoordinateTransform*)ct;
}

CoordinateTransform* Geometry::CoordTrans()
{
	return coordTrans;
}

void Geometry::ProjectionTransform()
{
	if(!coordTrans) return;

	QVector3D v;
	for(int i = 0; i < vertex.size(); i++)
	{
		v = coordTrans->Transform(vertex[i]);
		vertex.replace(i, v);
	}
}

void Geometry::FindedData( QDataArray* data )
{
	if(!data || !coordTrans) return;

	GLint dimen = data->ArrayDimension();
	if( dimen != 3) return;
	GLint arrayLen = data->getDataCount();
	GLdouble* pdData;
	GLfloat* pfData;

	switch(data->DataType())
	{
	case GL_DOUBLE:
		pdData = (GLdouble*)(data->getDataPointer());
		for(int i = 0; i < arrayLen; i++)
		{
			QVector3D temp(pdData[i * 3 + 0], pdData[i * 3 + 1], pdData[i * 3 + 2]), res;
			res = coordTrans->Transform(temp);
			pdData[i * 3 + 0] = res.x();
			pdData[i * 3 + 1] = res.y();
			pdData[i * 3 + 2] = res.z();
		}
		break;
	case GL_FLOAT:
		pfData = (GLfloat*)(data->getDataPointer());
		for(int i = 0; i < arrayLen; i++)
		{
			QVector3D temp(pfData[i * 3 + 0], pfData[i * 3 + 1], pfData[i * 3 + 2]), res;
			res = coordTrans->Transform(temp);
			pfData[i * 3 + 0] = res.x();
			pfData[i * 3 + 1] = res.y();
			pfData[i * 3 + 2] = res.z();
		}
		break;
	}
}

QVector4DArray* Geometry::getColorArray()
{
	return &colorAttr;
}

QVector2DArray* Geometry::getTextureArray()
{
	return &texture;
}


//void Geometric::InitGeometric()
//{
//	int v, c, t, n;
//	v = vertex.size();
//	if(v)
//		SetArrayData(&vertex);
//	c = colorAttr.size();
//	if(c)
//		SetArrayData(&colorAttr);
//	t = texture.size();
//	n = normal.size();
//
//	//SetArrayData()
//}
