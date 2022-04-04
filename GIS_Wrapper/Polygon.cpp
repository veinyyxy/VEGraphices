#include "Polygon.h"
 
//namespace GIS
//{

GISPolygon::GISPolygon()
{
    Mode(GL_POLYGON);
// 	glu_tess =  gluNewTess();
	m_bIsDrawTessPoly = false;
}

GISPolygon::~GISPolygon()
{
	/*gluDeleteTess(glu_tess);*/
}

TessStruct* GISPolygon::TesselPolygon()
{
	QVector3DArray resultArray;
	int x = vertex.size();
	int n = resultArray.size();
	USERDATA temp;
	QTesselator tess;
	TessStruct* tessData = new TessStruct;
	tess.Tessellate3df(*tessData, vertex, vertex.size());
	/*vertex = resultArray;*/
	return tessData ;
}

void GISPolygon::Render()
{
	if (m_bIsDrawTessPoly)
	{
		DrawTessPolygon();
	}
	else
	{
		QRenderUnit::Render();
	}
}

void GISPolygon::DrawTessPolygon()
{
	if(m_pTessData == NULL)
	{
		m_TessObject.clear();
		GISPolygon *pPolygon = new GISPolygon;
		QVector3DArray *pPolygonArray = getVectexArray();
		QVector4DArray *pPolygonColor = getColorArray();
		QVector4D m_Color = pPolygonColor->at(0);

		m_pTessData  = TesselPolygon();
		delete pPolygon;

		int nStart = 0, nEnd = 0 ;
		int nCount = m_pTessData->ModeArray.count();

		for(int j = 0 ; j< nCount; j++)
		{
			GISPolygon* tessPol = new GISPolygon;
			tessPol->Mode(m_pTessData->ModeArray.at(j));
			nEnd = m_pTessData->IndexArray.at(j);

			QVector3DArray* pArray =tessPol->getVectexArray();
			QVector4DArray* pColor = tessPol->getColorArray();
			int numCount = m_pTessData->TessVectorArray.size();
			for(int iCount = nStart ; iCount <nEnd; iCount++ )
			{
				pArray->push_back(m_pTessData->TessVectorArray.at(iCount));
				pColor->push_back(m_Color);
			}
			tessPol->SetColorArray();
			tessPol->Render();
			m_TessObject.push_back(tessPol);
			nStart = nEnd;
		}
	}
	else
	{
		for(int i = 0 ; i < m_TessObject.count(); i++)
		{
			m_TessObject.at(i)->Render();
		}
	}
}
//
//QVector4DArray* GISPolygon::getColorArray()
//{
//	return 0;
//}

//QVector2DArray* GISPolygon::getTextureAttray()
//{
//	return 0;
//}

QVector3DArray* GISPolygon::getNormalAttray()
{
	return 0;
}

Line* GISPolygon::AsLine()
{
	return 0;
}

Plane* GISPolygon::AsPlane()
{
	return 0;
}

GISPolygon* GISPolygon::AsPolygon()
{
	return this;
}

Point* GISPolygon::AsPoint()
{
	return 0;
}

//}

qreal GISPolygon::left_right(const QVector3D* a, const QVector3D* b, const QVector3D* c)
{
	qreal a_c_x, a_c_y, b_c_x, b_c_y;
	a_c_x = a->x() - c->x(); a_c_y = a->y() - c->y();
	b_c_x = b->x() - c->x(); b_c_y = b->y() - c->y();
	return a_c_x * b_c_y - a_c_y * b_c_x;
	//return   a->x() * b->y() - a->y() * b->x();
}

bool GISPolygon::is_tu(QVector3DArray* p)  
{
	int n = p->size();
	if(n < 4) return false;
	qreal a, b;
	a = left_right(&((*p)[0]),&((*p)[1]),&((*p)[2]));
	QVector3DArray::iterator i = p->begin();

	for(; ++i,--n>2; a=b)
	{
		b = left_right(&(*i),&(*(i+1)),&(*(i+2)));
		if(a * b < 0)
		{
			//qDebug()<<i->x()<<i->y();
			return false;
		}
	}
	return true;
}

bool GISPolygon::Is_Tu()
{
	return is_tu(&vertex);
}

// void GISPolygon::Render()
// {
// 	if( m_listId == 0 ) 
// 		m_listId = tessellate();
// 
// 	glCallList(m_listId);	
// // 
// // 	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
// // 	glDisable(GL_DEPTH_TEST);
// // 	glDisable(GL_CULL_FACE);
// }
