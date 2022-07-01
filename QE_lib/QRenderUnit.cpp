#include "QRenderUnit.h"


QRenderUnit::QRenderUnit(DATA_COMPOSE compos) : compose(compos)
{
	vertexArray = 0;
	normalArray = 0;
	colorArray = 0;
	textureCoordArray0 = 0;
	mixinArray = 0;
	useCallList = false;
	bColorArray  = false;
	bTextureArray = false; 
	bCalculateA = true;				//初始化需要重新计算角度
	m_NodeMask = true;
}

QRenderUnit::~QRenderUnit(void)
{
    GLuint listID = (ULONGLONG)(this);
    glDeleteLists(listID, 1);
}

void QRenderUnit::Render()
{
	if (m_NodeMask==true)
	{
		if(vertexArray == 0 && mixinArray ==0)
		   return;

		/*glPushAttrib(GL_ALL_ATTRIB_BITS);*/
		attriSet.Applay();
		//m_pModleTransform.EnableTransforms();

		if(compose == SIMPLEX)
		{
		  /* ITER iter(typeSet);
		   while(iter.hasNext())
		   {
			   glEnableClientState(iter.next());
		   }*/
		   if(vertexArray)
		   {
			   vertexArray->AsDrawArray()->StartDraw();
		   }
		   if(colorArray)
		   {
			   colorArray->AsDrawArray()->StartDraw();
		   }
		   if(textureCoordArray0)
		   {
			   textureCoordArray0->AsDrawArray()->StartDraw();
		   }
		   if(normalArray)
		   {
			   normalArray->AsDrawArray()->StartDraw();
		   }
	   }
	   else if(compose == MIXIN && mixinArray)
	   {
		   glInterleavedArrays(mixinArray->DataUsage(), mixinArray->Stride(), mixinArray);
	   }
	   else
		   return;

	   //ApplyData();

	 /*  if(bTextureArray)
	   {
		   glEnable(GL_TEXTURE_2D);
		   glBindTexture(GL_TEXTURE_2D,*pTexture);
	   }*/

	   if(m_EnableDispList)
	   {
		   if(useCallList)
			   CallDisplayList();
		   else
		   {
               GLuint ListName = (ULONGLONG)this;
			   glNewList(ListName, GL_COMPILE_AND_EXECUTE);
			   //glDrawArrays(mode, 0, vertexArray->getDataCount());
			   if(vertexArray)
			   {
				   vertexArray->AsDrawArray()->Draw(mode);
			   }
			   glEndList();

			   useCallList = true;
		   }
	   }
	   else
	   {
		   //glDrawArrays(mode, 0, vertexArray->getDataCount());
		   if(vertexArray)
		   {
			   vertexArray->AsDrawArray()->Draw(mode);
		   }
	   }	

	   /*if(bColorArray)
	   {
		   glDisableClientState(GL_COLOR_ARRAY);
	   }*/
	   if(vertexArray)
	   {
		   vertexArray->AsDrawArray()->EndDraw();
	   }
	   if(colorArray)
	   {
		   colorArray->AsDrawArray()->EndDraw();
	   }
	   if(textureCoordArray0)
	   {
		   textureCoordArray0->AsDrawArray()->EndDraw();
	   }
	   if(normalArray)
	   {
		   normalArray->AsDrawArray()->EndDraw();
	   }
	   /*if(bTextureArray)
	   {
		   glDisable(GL_TEXTURE_2D);
		   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	   }*/
	   //m_pModleTransform.UnableTransforms();
	   attriSet.Recover();
	   //glPopAttrib();
   }	
}

void QRenderUnit::Render( QSet<QRenderable*> rs )
{
	rs.insert(this);
}

//void QRenderUnit::Render( const COMPOSE_KEY& subkey )
//{
//	Render();
//}

void QRenderUnit::SetArrayData(QDataArray* arry )
{
	GLenum loct = arry->DataUsage();

	if(compose == SIMPLEX)
	{
		switch(loct)
		{
		case GL_VERTEX_ARRAY:
			vertexArray = arry;
			break;
		case GL_NORMAL_ARRAY:
			normalArray = arry;
			break;
		case GL_COLOR_ARRAY:
			colorArray = arry;
			break;
		case GL_TEXTURE_COORD_ARRAY:
			textureCoordArray0 = arry;
			break;
		default:
			return;
		}
		//glEnableClientState(loct);
		typeSet.insert(loct);
	}
	else if(compose == MIXIN)
	{
		//glInterleavedArrays(loct, arry->Stride(), arry);
		mixinArray = arry;
	}
}

void QRenderUnit::SetArrayData( bool state, QDataArray* arry )
{
	GLenum loct = arry->DataUsage();

	if(compose == SIMPLEX)
	{
		switch(loct)
		{
		case GL_VERTEX_ARRAY:
			if(!state)
				vertexArray = 0;
			else
				vertexArray = arry;
			break;
		case GL_NORMAL_ARRAY:
			if(!state)
				normalArray = 0;
			else
				normalArray = arry;
			break;
		case GL_COLOR_ARRAY:
			if(!state)
				colorArray = 0;
			else
			{
				colorArray = arry;
				bColorArray = true;
			}
			break;
		case GL_TEXTURE_COORD_ARRAY:
			if(!state)
				textureCoordArray0 = 0;
			else
			{
				textureCoordArray0 = arry;
				bTextureArray = true;
			}
			break;
		default:
			return;
		}

		if(state)
			typeSet.insert(loct);
		else
		{
			glDisableClientState(loct);
			typeSet.remove(loct);
		}
	}
	else if(compose == MIXIN)
	{
		//glInterleavedArrays(loct, arry->Stride(), arry);
		if(!state)
			mixinArray = 0;
		else
			mixinArray = arry;
		mixinArray = 0;
	}

}

void QRenderUnit::ApplyData()
{
	if(compose == MIXIN) return;

	if(vertexArray)
		glVertexPointer(
		vertexArray->ArrayDimension(), 
		vertexArray->DataType(), 
		vertexArray->Stride(), 
		vertexArray->getDataPointer());
	if(normalArray)
		glNormalPointer(
		normalArray->DataType(), 
		normalArray->Stride(), 
		normalArray->getDataPointer());

	if(colorArray)
		glColorPointer(
		colorArray->ArrayDimension(), 
		colorArray->DataType(), 
		colorArray->Stride(), 
		colorArray->getDataPointer());

	if(textureCoordArray0)
		glTexCoordPointer(
		textureCoordArray0->ArrayDimension(), 
		textureCoordArray0->DataType(), 
		textureCoordArray0->Stride(), 
		textureCoordArray0->getDataPointer());
}

void QRenderUnit::Clear()
{
	vertexArray = 0;
	normalArray = 0;
	colorArray = 0;
	textureCoordArray0 = 0;
	//mode = 0;
	vertexCount = 0;
}

int QRenderUnit::InsertRenderable( const QRenderable* unit )
{
	return -1;
}

int QRenderUnit::RemoveRenderable( QRenderable* unit )
{
	return -1;
}

bool QRenderUnit::DumpRenderable( QRenderable** renderable )
{
	return false;
}

int QRenderUnit::FindDataUseSignal(GLenum type)
{
	if(vertexArray == 0 && mixinArray ==0)
		return false;

	switch(type)
	{
	case GL_VERTEX_ARRAY:
		if(vertexArray)
		{
			m_FoundRenderUnit->Found(vertexArray);
		}
		break;
	case GL_NORMAL_ARRAY:
		if(normalArray)
			m_FoundRenderUnit->Found(normalArray);
		break;
	case GL_COLOR_ARRAY:
		if(colorArray)
		{
			m_FoundRenderUnit->Found(vertexArray);
		}
		break;
	case GL_TEXTURE_COORD_ARRAY:
		if(textureCoordArray0)
			m_FoundRenderUnit->Found(textureCoordArray0);
		break;
	default:
		return false;
	}
	return true;
}

void QRenderUnit::FindUnitUseSignal()
{
	//return this;
}

bool QRenderUnit::IsContiner()
{
	return false;
}

void QRenderUnit::SetRenderNodeMask(bool bv)
{
	m_NodeMask = bv;
}

QDataArray* QRenderUnit::VertexArray()
{
	return vertexArray;
}

QDataArray* QRenderUnit::NormalArray()
{
	return normalArray;
}

QDataArray* QRenderUnit::ColorArray()
{
	return colorArray;
}

QDataArray* QRenderUnit::TextureArray()
{
	return textureCoordArray0;
}

GLsizei QRenderUnit::VertexCount()
{
	return vertexCount;
}

void QRenderUnit::VertexCount( GLsizei count )
{
	vertexCount = count;
}

GLenum QRenderUnit::Mode()
{
	return mode;
}

void QRenderUnit::Mode( GLenum m )
{
	mode = m;
}

QRenderUnit* QRenderUnit::AsRenderUnit()
{
	return this;
}

QRenderVolume* QRenderUnit::AsRenderVolume()
{
	return 0;
}

void QRenderUnit::FindDataUseCallBack(GLenum type)
{
	if((vertexArray == 0 && mixinArray ==0) || dataCallBack == 0)
		return;

	switch(type)
	{
	case GL_VERTEX_ARRAY:
		if(vertexArray)
		{
			dataCallBack->FindedData(vertexArray);
		}
		break;
	case GL_NORMAL_ARRAY:
		if(normalArray)
			dataCallBack->FindedData(normalArray);
		break;
	case GL_COLOR_ARRAY:
		if(colorArray)
		{
			dataCallBack->FindedData(vertexArray);
		}
		break;
	case GL_TEXTURE_COORD_ARRAY:
		if(textureCoordArray0)
			dataCallBack->FindedData(textureCoordArray0);
		break;
	default:
		return;
	}
}

void QRenderUnit::FindUnitUseCallBack()
{
	if(unitCallBack)
		unitCallBack->FindedUnit(this);
}

void QRenderUnit::FindVolumeUseCallBack()
{

}

void QRenderUnit::CallListDirty( bool bv )
{
	useCallList = bv;
}

void QRenderUnit::SetTexture( GLuint & texture)
{
	//glBindTexture(GL_TEXTURE_2D,texture);
	pTexture = &texture;
}


QVector3DArray* QRenderUnit::OriginalArray()
{
	return &m_OriginalArray;
}

void QRenderUnit::SetBakData()
{
	int num = vertexArray->getDataCount();
	GLfloat* pfData = NULL;
	pfData = (GLfloat*)(vertexArray->getDataPointer());
	m_OriginalArray.Clear();
	for(int i = 0; i < num; i++)
	{
		QVector3D temp(pfData[i * 3 + 0], pfData[i * 3 + 1], pfData[i * 3 + 2]);
		m_OriginalArray.push_back(temp);
	}
}

bool QRenderUnit::PickUp( const QVector3D* pickPoint )
{
	QVector3D screenPoint;
	pickPrecision = 10;
	float* point = 0;
	QVector3D pickPointGL = *pickPoint;
	//sTOog(*pickPoint, pickPointGL);
	
	qreal* dataPoint = 0, x(0), y(0), z(0), x1(0), y1(0), z1(0);
	qreal dis(0), dis2(0)/*, dis3(0)*/;
	QVector3D startPoint, EndPoint, tempPoint;
	int iVertexCount = 0;
	QDataArray* vertexArray = VertexArray();
	if(vertexArray)
	{
		if(vertexArray->ArrayDimension() != 3)
			return false;

		dataPoint = (qreal*)(vertexArray->getDataPointer());
		iVertexCount = vertexArray->getDataCount();
		if(iVertexCount >= 2)
		{
			for(int i = 0; i < iVertexCount; i+=2)
			{
				if(vertexArray->DataType() == GL_FLOAT)
				{
					point = (float*)(vertexArray->getDataPointer());
					startPoint.setX(point[i * 3]);
					startPoint.setY(point[i * 3 + 1]);
					startPoint.setZ(point[i * 3 + 2]);

					EndPoint.setX(point[i * 3 + 3]);
					EndPoint.setY(point[i * 3 + 4]);
					EndPoint.setZ(point[i * 3 + 5]);
				}
				else if(GL_DOUBLE)
				{
					dataPoint = (qreal*)(vertexArray->getDataPointer());
					startPoint.setX(dataPoint[i * 3]);
					startPoint.setY(dataPoint[i * 3 + 1]);
					startPoint.setZ(dataPoint[i * 3 + 2]);

					EndPoint.setX(dataPoint[i * 3 + 3]);
					EndPoint.setY(dataPoint[i * 3 + 4]);
					EndPoint.setZ(dataPoint[i * 3 + 5]);
				}
				
				dis = pickPointGL.distanceToLine(startPoint, EndPoint);
				dis2 = startPoint.distanceToLine(EndPoint, QVector3D(0, 0, 0));

				if(dis - pickPrecision <= 0.001)
				{
					return true;
				}
			}
		}
		else if(iVertexCount == 1)
		{
			if(vertexArray->ArrayDimension() != 3)
				return false;

			if(vertexArray->DataType() == GL_FLOAT)
			{
				point = (float*)(vertexArray->getDataPointer());
				startPoint.setX(*(point));
				startPoint.setY(*(point + 1));
				startPoint.setZ(*(point + 2));

			}
			else if(GL_DOUBLE)
			{
				dataPoint = (qreal*)(vertexArray->getDataPointer());
				startPoint.setX(*(dataPoint));
				startPoint.setY(*(dataPoint + 1));
				startPoint.setZ(*(dataPoint + 2));
			}
			
			ogTOs(startPoint, screenPoint);
			screenPoint.setZ(0);
			/*qreal ty = pickPointGL.y();
			pickPointGL.setY(-ty);*/

			/*ty = screenPoint.y();
			screenPoint.setY(-ty);*/
			dis = pickPointGL.distanceToLine(screenPoint, QVector3D(0, 0, 0));

			if(dis - pickPrecision <= 0.001)
			{
				return true;
			}
		}
	}
	return false;
}


int QRenderUnit::ChildCount()
{
	return 0;
}

void QRenderUnit::sTOog( const QVector3D& src, QVector3D& des )
{
	GLint    viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ = 0.0f;
	GLdouble posX, posY, posZ;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	winX = src.x();
	winY = viewport[3] - src.y();
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(src.x(), src.y(), src.z(), modelview, projection, viewport, &posX, &posY, &posZ);
	des.setX(posX);
	des.setY(posY);
	des.setZ(posZ);
}

void QRenderUnit::ogTOs( const QVector3D& src, QVector3D& des )
{
	qreal x(0), y(0), z(0), x1(0), y1(0), z1(0);
	GLint    viewport[4] = {0};
	GLdouble modelview[16] = {0};
	GLdouble projection[16] = {0};
	GLfloat winX = 0.0f, winY = 0.0f, winZ = 0.0f;
	GLdouble posX = 0.0, posY = 0.0, posZ = 0.0;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	//glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluProject(src.x(), src.y(), src.z(), modelview, projection, viewport, &x, &y, &z);
	des.setX(x);
	des.setY(y);
	des.setZ(z);
}

QRenderUnit& QRenderUnit::operator=( const QRenderUnit& other )
{
	this->typeSet = other.typeSet;
	//GLenum type;
	this->compose = other.compose;
	this->mode = other.mode;
	this->vertexCount = other.vertexCount;
	if(other.vertexArray)
	{
		if(other.vertexArray->ArrayDimension() == 3)
		{
			this->vertexArray = new QVector3DArray;
			this->vertexArray = other.vertexArray;
		}
	}
	if(other.normalArray)
	{
		if(other.normalArray->ArrayDimension() == 3)
		{
			this->normalArray = new QVector3DArray;
			this->normalArray = other.normalArray;
		}
	}
	if(other.colorArray)
	{
		if(other.colorArray->ArrayDimension() == 4)
		{
			this->colorArray = new QVector4DArray;
			this->colorArray = other.colorArray;
		}
	}
	if(other.textureCoordArray0)
	{
		if(other.textureCoordArray0->ArrayDimension() == 2)
		{
			this->textureCoordArray0 = new QVector2DArray;
			this->textureCoordArray0 = other.textureCoordArray0;
		}
	}
	if(other.textureCoordArray1)
	{
		if(other.textureCoordArray1->ArrayDimension() == 2)
		{
			this->textureCoordArray1 = new QVector2DArray;
			this->textureCoordArray1 = other.textureCoordArray1;
		}
	}
	if(other.textureCoordArray2)
	{
		if(other.textureCoordArray2->ArrayDimension() == 2)
		{
			this->textureCoordArray2 = new QVector2DArray;
			this->textureCoordArray2 = other.textureCoordArray2;
		}
	}
	if(other.textureCoordArray3)
	{
		if(other.textureCoordArray3->ArrayDimension() == 2)
		{
			this->textureCoordArray3 = new QVector2DArray;
			this->textureCoordArray3 = other.textureCoordArray3;
		}
	}
	if(other.textureCoordArray4)
	{
		if(other.textureCoordArray4->ArrayDimension() == 2)
		{
			this->textureCoordArray4 = new QVector2DArray;
			this->textureCoordArray4 = other.textureCoordArray4;
		}
	}
	/*if(other.mixinArray)
	{
	if(other.textureCoordArray4->DataType() == 2)
	{
	this->textureCoordArray4 = new QVector2DArray;
	this->textureCoordArray4 = other.textureCoordArray4;
	}
	}*/
	//(this->vertexArray);
	//QDataArray* normalArray;
	//QDataArray* colorArray;
	//QDataArray* textureCoordArray0;
	//QDataArray* textureCoordArray1;
	//QDataArray* textureCoordArray2;
	//QDataArray* textureCoordArray3;
	//QDataArray* textureCoordArray4;
	//QDataArray* mixinArray;
	//QDataArray* materialArray;
	this->bColorArray = other.bColorArray;
	this->bTextureArray = other.bTextureArray;
	//GLuint *pTexture;
	this->m_OriginalArray = other.m_OriginalArray;
	return *this;
}
