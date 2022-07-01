#include "NormalSymbol.h"
#include <math.h>
#include <QtCore/QSettings>

NormalSymbol::NormalSymbol(void) :  m_pointStyle(0), m_iPickIndex(-1),m_bHadPicked(false)
{
	m_posion.DataUsage(GL_VERTEX_ARRAY);
	SetArrayData(&m_posion);
	m_Proj4Trans = new Proj4Transform;
	coordTrans = NULL;
	m_SymbolAttribute = new CSymbolAttribute;
}


NormalSymbol::~NormalSymbol(void)
{
	if(NULL !=  m_Proj4Trans)
	{
		delete m_Proj4Trans;
		m_Proj4Trans = NULL;
	}
}

void NormalSymbol::Render()
{
	m_scales.push_back(QVector3D(0,0,0));
	//重新计算角度
	GLfloat  CurrentZero = 0 ; 
	GLfloat  k = 0 ; 
	QVector3DArray *  pOrigin = OriginalArray();

	m_pointStyle->SetInitCount();			//初始化为0
/*	m_pointStyle->SetAttribute(m_pointStyle->GetAttribute());*/
	
	for(int i = 0; i < m_posion.size(); i++)
	{
 		int m_index =  m_Attr[i]->GetSymbolType();
		if(m_bHadPicked && m_iPickIndex == i)
		{
			m_pointStyle->SetSelected(true);
		}else
		{
			m_pointStyle->SetSelected(false);
		}
		RenderStyle(m_posion[i], m_Attr[i]);
	}
	bCalculateA = false;
}

void NormalSymbol::SetPosition(QVector3D& pos, CSymbolAttribute *nIndex)
{
	m_posion.push_back(pos);
	m_Attr.push_back(nIndex);
}

void NormalSymbol::SetPointStyle( Symbol* ps )
{
	m_pointStyle = ps;
}

Symbol* NormalSymbol::SetPointStyle()
{
	return m_pointStyle;
}

void NormalSymbol::RenderStyle(QVector3D& pos, CSymbolAttribute *symbolAttr)
{
	glPushMatrix();
	//m_pointStyle->RotateSymble(angle, pos);
	m_pointStyle->SetPosition(pos);
	m_pointStyle->SetAttribute(symbolAttr);
	//symbolAttr->SetSymbolType(m_index);
	glTranslatef(pos.x(), pos.y(), pos.z());

	if(m_EnableDispList)
	{
		if(useCallList)
			CallDisplayList();
		else
		{
			//可能会产生渲染列覆盖
            GLuint ListName = (LONGLONG)this;
			glNewList(ListName, GL_COMPILE_AND_EXECUTE);
			m_pointStyle->SetSelected(m_bHadPicked);
			m_pointStyle->Draw();
			glEndList();
			useCallList = true;
		}
	}
	else
	{
		//m_pointStyle->SetSelected(m_bHadPicked);
		m_pointStyle->Draw();
	}
	glLoadIdentity();
	glPopMatrix();
}

void  NormalSymbol::SetAngle(GLfloat fangle)
{
	m_angleArray.push_back(fangle);
	m_IniAngleArray.push_back(0);
}

QVector3DArray* NormalSymbol::Scales()
{
	return &m_scales;
}

bool NormalSymbol::PickUp( const QVector3D* pickPoint )
{
	QVector3D screenPoint;
	pickPrecision = 10;
	float* point = 0;
	QVector3D pickPointGL = *pickPoint;

	qreal* dataPoint = 0, x(0), y(0), z(0), x1(0), y1(0), z1(0);
	qreal dis, dis2, dis3;
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

int NormalSymbol::GetPickIndex()
{
	return m_iPickIndex;
}

/************************************************************************/
/* 函数功能： 返回角度							                        */
/* 参数说明:  NULL														*/
/* 返 回 值:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
QVector<GLfloat>* NormalSymbol::AngleArray()
{
	return &m_angleArray;
}
