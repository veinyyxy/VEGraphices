#include "QBitmapText.h"


QBitmapText::QBitmapText(void) : QRenderUnit(SIMPLEX),m_fXAlpha(.0f),m_fYAlpha(.0f)
{
	size = 0;//GLUT_BITMAP_HELVETICA_18;
	postion.DataUsage(GL_VERTEX_ARRAY);
	SetArrayData(&postion);
	m_uiList = 0;
	m_iListRang = 0;
}


QBitmapText::~QBitmapText(void)
{
	glDeleteLists(m_uiList, m_iListRang);
}

void QBitmapText::Render()
{
	//保存当前颜色矩阵
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	attriSet.Applay();

	if(m_EnableDispList)
	{
		TextRenderUseCallList();
	}
	else
	{
		TextRender();
	}

	glPopAttrib();             //恢复当前颜色矩阵
}

int QBitmapText::InsertRenderable( const QRenderable* unit )
{
	return -1;
}

int QBitmapText::RemoveRenderable( QRenderable* unit )
{
	return -1;
}

void QBitmapText::TextRender()
{
	int len = 0;
	if(stringArray.size() < postion.size())
		len = stringArray.size();
	else
		len = postion.size();

	for(int stringCount = 0; stringCount < len; stringCount++)
	{
		/*GLfloat xp = postion[stringCount].x();
		GLfloat yp = postion[stringCount].y();
		GLfloat zp = postion[stringCount].z();*/
		
		QVector3D ScrPos;
		ogTOs(postion[stringCount], ScrPos);
		ScrPos.setX(ScrPos.x() - m_fXAlpha);
		ScrPos.setY(ScrPos.y() - m_fYAlpha);
		sTOog(ScrPos, ScrPos);

		glRasterPos3f(ScrPos.x(), ScrPos.y(), ScrPos.z());
		QString str = stringArray[stringCount];
		QChar* pText = str.data();

		while(!pText->isNull())
		{
			char c = pText->toLatin1();
			//glutBitmapCharacter(size, c);
			pText++;
		}
	}
}

QVector<QString>* QBitmapText::StringArray()
{
	return &stringArray;
}

QVector<StationValues>* QBitmapText::StationValueArray()
{
	return &m_SValuesArray;
}

QVector3DArray* QBitmapText::Pointion()
{
	return &postion;
}

void* QBitmapText::FontSize()
{
	return size;
}

void QBitmapText::FontSize( void* fs )
{
	size = fs;
}

void QBitmapText::SetColor( const QAttribute* color )
{
	attriSet.insert(QE_ATTRI_COLOR, (QAttribute*)color);
	//AttribSet(attriSet);
}

void QBitmapText::SetXOffset(float xsize)
{
	m_fXAlpha = xsize;
}

void QBitmapText::SetYOffset(float ysize)
{
	m_fYAlpha = ysize;
}

bool QBitmapText::PickUp( const QVector3D* pickPoint )
{
	QVector3D screenPoint;
	pickPrecision = 10;
	float* point = 0;
	QVector3D pickPointGL = *pickPoint;

	qreal* dataPoint = 0, x(0), y(0), z(0), x1(0), y1(0), z1(0);
	qreal dis(0);//, dis2, dis3;
	QVector3D startPoint, EndPoint, tempPoint;
	int iVertexCount = 0;
	QDataArray* vertexArray = VertexArray();
	if(vertexArray)
	{
		if(vertexArray->ArrayDimension() != 3)
			return false;

		dataPoint = (qreal*)(vertexArray->getDataPointer());
		iVertexCount = vertexArray->getDataCount();

		for(int i = 0; i < iVertexCount; i++)
		{
			if(vertexArray->DataType() == GL_FLOAT)
			{
				point = (float*)(vertexArray->getDataPointer());
				startPoint.setX(*(point + i * 3));
				startPoint.setY(*(point + i * 3 + 1));
				startPoint.setZ(*(point + i * 3 + 2));
			}
			else if(vertexArray->DataType() == GL_DOUBLE)
			{
				dataPoint = (qreal*)(vertexArray->getDataPointer());
				startPoint.setX(*(dataPoint + i * 3));
				startPoint.setY(*(dataPoint + i * 3 + 1));
				startPoint.setZ(*(dataPoint + i * 3 + 2));
			}

			ogTOs(startPoint, screenPoint);
			screenPoint.setZ(0);

			dis = pickPointGL.distanceToLine(screenPoint, QVector3D(0, 0, 0));

			if(dis - pickPrecision <= 0.001)
			{
				m_iPickIndex = i;
				return true;
			}
		}
	}
	return false;
}

int QBitmapText::GetPickIndex()
{
	return m_iPickIndex;
}

void QBitmapText::SetPickIndex()
{
	 m_iPickIndex = -1;
}

void QBitmapText::TextRenderUseCallList()
{
    //ListName = static_cast<GLuint>(this);
	int len = 0;
	if(stringArray.size() < postion.size())
		len = stringArray.size();
	else
		len = postion.size();

	m_iListRang = len;
	if(!useCallList)
	{
		m_uiList = glGenLists(m_iListRang);
		//glListBase(m_uiList);
	}
	for(int stringCount = 0; stringCount < len; stringCount++)
	{
		GLfloat xp = postion[stringCount].x();
		GLfloat yp = postion[stringCount].y();
		GLfloat zp = postion[stringCount].z();
		
		/*QVector3D ScrPos;
		ogTOs(postion[stringCount], ScrPos);
		ScrPos.setX(ScrPos.x() - m_fXAlpha);
		ScrPos.setY(ScrPos.y() - m_fYAlpha);
		sTOog(ScrPos, ScrPos);*/

		glRasterPos3f(xp, yp, zp);
		//glRasterPos3f(ScrPos.x(), ScrPos.y(), ScrPos.z());
		//QString str = (stringArray[stringCount]);
		QChar* pText = (&(stringArray[stringCount]))->data();

		if(useCallList)
		{
			glCallList(m_uiList + stringCount);
			//glCallLists(m_iListRang, GL_INT, (void*)m_uiList);
		}
		else
		{
			glNewList(m_uiList + stringCount, GL_COMPILE_AND_EXECUTE);
			while(!pText->isNull())
			{
				char c = pText->toLatin1();
				//glutBitmapCharacter(size, c);
				pText++;
			}
			glEndList();
		}
	}
	useCallList = true;
}

void QBitmapText::CallDisplayList(int i)
{
	glCallList(i);
}
