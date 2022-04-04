#include "SymbolPoint.h"
#include <math.h>
#include <QtCore/QSettings>
#include <QtCore/QTextCodec>

SymbolPoint::SymbolPoint(void) : QRenderUnit(SIMPLEX), m_pointStyle(0), m_iPickIndex(-1),m_bHadPicked(false)
{
	m_posion.DataUsage(GL_VERTEX_ARRAY);
	SetArrayData(&m_posion);
	m_Proj4Trans = new Proj4Transform;
	coordTrans = NULL;
	n=0;
}


SymbolPoint::~SymbolPoint(void)
{
	if(NULL !=  m_Proj4Trans)
	{
		delete m_Proj4Trans;
		m_Proj4Trans = NULL;
	}
}

void SymbolPoint::Render()
{
	m_scales.push_back(QVector3D(0,0,0));
	//重新计算角度
	GLfloat  CurrentZero = 0 ; 
	GLfloat  k = 0 ; 
	QVector3DArray *  pOrigin = OriginalArray();

	m_pointStyle->SetInitCount();			//初始化为0

	for(int i = 0; i < m_posion.size(); i++)
	{
		if(NULL == coordTrans) 
		{
			CurrentZero = 0 ; 
		}else
		{
			if(bCalculateA)
			{
				QVector3D temp  = pOrigin->at(i);
				QVector3D Origin1(temp.x(), temp.y()+10, temp.z()),glPoint1, glPoint2;
				QSettings setting("ProjectionType.ini", QSettings::IniFormat);
				setting.setIniCodec(QTextCodec::codecForName("UTF-8"));

				m_Proj4Trans->setSourceCRS(setting.value("/GeographicCoordinate_CONFIG/WGS84").toString());
				m_Proj4Trans->setDestCRS(coordTrans->getDesCRS());
				glPoint1 = m_Proj4Trans->Transform(temp);
				glPoint2 = m_Proj4Trans->Transform(Origin1);
				k = (glPoint1.y() - glPoint2.y()) /(glPoint1.x() - glPoint2.x());
				CurrentZero = atan(k);
				if (CurrentZero<0)
				{
					CurrentZero = 90-CurrentZero * 180 / 3.1415926;
				}else
					CurrentZero = 270 -CurrentZero * 180 / 3.1415926;
				m_IniAngleArray[i] = CurrentZero;
			}
		}			
		if(m_bHadPicked && m_iPickIndex == i)
		{
			m_pointStyle->SetSelected(true);
		}else
		{
			m_pointStyle->SetSelected(false);
		}
		RenderStyle(m_posion[i], m_IniAngleArray[i]+m_angleArray[i]/*,m_scales.last()*/);
	}

	bCalculateA = false;
}

void SymbolPoint::SetPosition(QVector3D& pos)
{
	m_posion.push_back(pos);
}

void SymbolPoint::SetPointStyle( Symbol* ps )
{
	m_pointStyle = ps;
}

Symbol* SymbolPoint::SetPointStyle()
{
	return m_pointStyle;
}

void SymbolPoint::RenderStyle(QVector3D& pos, GLfloat angle/*, QVector3D& sc*/)
{
	glPushMatrix();
	m_pointStyle->RotateSymble(angle, pos);
	m_pointStyle->SetPosition(pos);

	glTranslatef(pos.x(), pos.y(), pos.z());

	if(m_EnableDispList)
	{
		if(useCallList)
			CallDisplayList();
		else
		{
			//可能会产生渲染列覆盖
			ListName = (GLuint)this;
			glNewList(ListName, GL_COMPILE_AND_EXECUTE);
			m_pointStyle->Draw();
			glEndList();
			useCallList = true;
		}
	}
	else
	{
		m_pointStyle->Draw();
	}
	glLoadIdentity();
	glPopMatrix();
}

void  SymbolPoint::SetAngle(GLfloat fangle)
{
	m_angleArray.push_back(fangle);
	m_IniAngleArray.push_back(0);

}

QVector3DArray* SymbolPoint::Scales()
{
	return &m_scales;
}

bool SymbolPoint::PickUp( const QVector3D* pickPoint )
{
	QVector3D screenPoint;
	pickPrecision = 15;
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

int SymbolPoint::GetPickIndex()
{
	return m_iPickIndex;
}

QVector<GLfloat>* SymbolPoint::AngleArray()
{
	return &m_angleArray;
}

QVector<StationValues>* SymbolPoint::StationValueArray()
{
	return &m_SValuesArray;
}