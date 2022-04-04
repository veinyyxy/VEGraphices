#include "Line.h"

Line::Line() 
{
    Mode(GL_LINE_STRIP);
	//SetLineStyle(1, 1, 0xFFFF);
	//
}

void Line::SetLineMode( GLenum mode )
{
	if(mode >= GL_LINES && mode <= GL_LINE_STRIP)
		Mode(mode);
}

void Line::SetLineStyle(GLfloat width, GLint factor, GLushort pattern )
{
	lineStipple.Factor(factor);
	lineStipple.LinePattern(pattern);
	lineWidth.Width(width);
	attribSet.insert(QE_ATTRI_LINE_STIPPLE, &lineStipple);
	attribSet.insert(QE_ATTRI_LINE_WIDTH, &lineWidth);
	AttribSet(attribSet);
}


QVector2DArray* Line::getTextureAttray()
{
	return 0;
}

QVector3DArray* Line::getNormalAttray()
{
	return 0;
}

Line* Line::AsLine()
{
	return this;
}

Plane* Line::AsPlane()
{
	return 0;
}

GISPolygon* Line::AsPolygon()
{
	return 0;
}

Point* Line::AsPoint()
{
	return 0;
}

void Line::ScaleLine(float xscale,float yscale)
{
	QVector3DArray *pData = dynamic_cast<QVector3DArray*>(this->VertexArray());
	//获取symbolline的边界信息
	float fxmin,fymin,fxmax,fymax,xtemp,ytemp;
	QVector3D geosymbolCenter,geoposTemp;
	fxmin = fxmax = pData->at(0).x();
	fymin = fymax = pData->at(0).y();

	for (int i=0;i<pData->count();i++)
	{
		xtemp = pData->at(i).x();
		ytemp = pData->at(i).y();

		if(xtemp>fxmax) fxmax = xtemp;
		if(xtemp<fxmin) fxmin = xtemp;
		if(ytemp>fymax) fymax = ytemp;
		if(ytemp<fymin) fymin = ytemp;			 
	}
	geosymbolCenter = QVector3D((fxmin+fxmax)/2,(fymin+fymax)/2,0);

	m_glcenter = geosymbolCenter;
	m_boundminx = fxmin;
	m_boundmaxx = fxmax;
	m_boundminy = fymin;
	m_boundmaxy = fymax;
	//替换点
	for (int i=0;i<pData->count();i++)
	{
		float xtemp,ytemp;		
		xtemp = xscale*(pData->at(i).x()-geosymbolCenter.x())+geosymbolCenter.x();
		ytemp = yscale*(pData->at(i).y()-geosymbolCenter.y())+geosymbolCenter.y();

		geoposTemp = QVector3D(xtemp,ytemp,0.0f);
		pData->replace(i,geoposTemp);			 
	}

//	CalGeometryBound();
}
/*
void Line::CalGeometryBound()
{
	QVector3DArray *pData = dynamic_cast<QVector3DArray*>(this->VertexArray());
	
	if (pData->count()>0)
	{
		float xtemp,ytemp;

		m_boundmaxx = m_boundminx = pData->at(0).x();
		m_boundmaxy = m_boundminy = pData->at(0).y();

		for (int i=0;i<pData->count();i++)
		{
			xtemp = pData->at(i).x();
			ytemp = pData->at(i).y();

			if(xtemp>m_boundmaxx) m_boundmaxx = xtemp;
			if(xtemp<m_boundminx) m_boundminx = xtemp;
			if(ytemp>m_boundmaxy) m_boundmaxy = ytemp;
			if(ytemp<m_boundminy) m_boundminy = ytemp;		
		}
	}	
}
*/

bool Line::isInYScale(QVector3D& mouseGlPos,float dx)
{
//	static float dis1old,dis2old,dis1new,dis2new;
	float dis1,dis2;
	QVector3D screenPoint,screenPoint1,screenPoint2,glPos;
	QVector3D screenTop,screenButom,screenLeft,screenRight;
	//sTOog(mouseGlPos,glPos);
	
	ogTOs(mouseGlPos, screenPoint);
	ogTOs(QVector3D(m_boundminx,m_boundminy,0.0f), screenPoint1);
	ogTOs(QVector3D(m_boundmaxx,m_boundmaxy,0.0f), screenPoint2);

	ogTOs(QVector3D((m_boundminx+m_boundmaxx)/2,m_boundminy,0.0f), screenButom);
	ogTOs(QVector3D((m_boundminx+m_boundmaxx)/2,m_boundmaxy,0.0f), screenTop);
	//ogTOs(QVector3D(m_boundminx,(m_boundminy+m_boundmaxy)/2,0.0f), screenLeft);
	//ogTOs(QVector3D(m_boundmaxx,(m_boundminy+m_boundmaxy)/2,0.0f), screenRight);

	float xtemp = screenPoint.x();
	float ytemp = screenPoint.y();

	dis1 = screenPoint.distanceToLine(screenTop,QVector3D(0.0f,0.0f,0.0f));
	dis2 = screenPoint.distanceToLine(screenButom,QVector3D(0.0f,0.0f,0.0f));

	//if((xtemp<screenPoint2.x())&&(xtemp>screenPoint1.x())&&(ytemp<screenPoint2.y())&&(ytemp>screenPoint1.y()))
	if((dis1-dx)<0.001 || (dis2-dx)<0.001)
	{		
		return true;
	}
	else
		return false;	
}

bool Line::isInXScale(QVector3D& mouseGlPos,float dx)
{
	float dis1,dis2;
	QVector3D screenPoint,screenPoint1,screenPoint2,glPos;
	QVector3D screenTop,screenButom,screenLeft,screenRight;
	//sTOog(mouseGlPos,glPos);

	ogTOs(mouseGlPos, screenPoint);
	ogTOs(QVector3D(m_boundminx,m_boundminy,0.0f), screenPoint1);
	ogTOs(QVector3D(m_boundmaxx,m_boundmaxy,0.0f), screenPoint2);

	//ogTOs(QVector3D((m_boundminx+m_boundmaxx)/2,m_boundminy,0.0f), screenButom);
	//ogTOs(QVector3D((m_boundminx+m_boundmaxx)/2,m_boundmaxy,0.0f), screenTop);
	ogTOs(QVector3D(m_boundminx,(m_boundminy+m_boundmaxy)/2,0.0f), screenLeft);
	ogTOs(QVector3D(m_boundmaxx,(m_boundminy+m_boundmaxy)/2,0.0f), screenRight);

	float xtemp = screenPoint.x();
	float ytemp = screenPoint.y();

	dis1 = screenPoint.distanceToLine(screenLeft,QVector3D(0.0f,0.0f,0.0f));
	dis2 = screenPoint.distanceToLine(screenRight,QVector3D(0.0f,0.0f,0.0f));

	//if((xtemp<screenPoint2.x())&&(xtemp>screenPoint1.x())&&(ytemp<screenPoint2.y())&&(ytemp>screenPoint1.y()))
	if((dis1-dx)<0.001 ||(dis2-dx)<0.001 )
	{
		return true;
	}
	else
		return false;	
}

bool Line::isInXYScale(QVector3D& mouseGlPos,float dx)
{
	float dis1,dis2,dis3,dis4;
	QVector3D screenPoint,screenPoint1,screenPoint2,glPos;
	QVector3D screenTopLeft,screenButomLeft,screenTopRight,screenButomRight;
	//sTOog(mouseGlPos,glPos);

	ogTOs(mouseGlPos, screenPoint);
	ogTOs(QVector3D(m_boundminx,m_boundminy,0.0f), screenPoint1);
	ogTOs(QVector3D(m_boundmaxx,m_boundmaxy,0.0f), screenPoint2);

	ogTOs(QVector3D(m_boundminx,m_boundmaxy,0.0f), screenTopLeft);
	ogTOs(QVector3D(m_boundminx,m_boundminy,0.0f), screenButomLeft);
	ogTOs(QVector3D(m_boundmaxx,m_boundmaxy,0.0f), screenTopRight);
	ogTOs(QVector3D(m_boundmaxx,m_boundminy,0.0f), screenButomRight);

	float xtemp = screenPoint.x();
	float ytemp = screenPoint.y();

	dis1 = screenPoint.distanceToLine(screenTopLeft,QVector3D(0.0f,0.0f,0.0f));
	dis2 = screenPoint.distanceToLine(screenButomLeft,QVector3D(0.0f,0.0f,0.0f));
	dis3 = screenPoint.distanceToLine(screenTopRight,QVector3D(0.0f,0.0f,0.0f));
	dis4 = screenPoint.distanceToLine(screenButomRight,QVector3D(0.0f,0.0f,0.0f));

	//if((xtemp<screenPoint2.x())&&(xtemp>screenPoint1.x())&&(ytemp<screenPoint2.y())&&(ytemp>screenPoint1.y()))
	if((dis1-dx)<0.001 ||(dis2-dx)<0.001 || (dis3-dx)<0.001 ||(dis4-dx)<0.001 )
	{
		return true;
	}
	else
		return false;	
}


bool Line::PickUp(const QVector3D* pickPoint)
{
	QVector3D screenPoint;
	pickPrecision = 10;
	float* point = 0;
	QVector3D pickPointGL = *pickPoint;

	qreal* dataPoint = 0, x(0), y(0), z(0), x1(0), y1(0), z1(0);
	qreal dis, dis2, dis3;
	QVector3D startPoint, EndPoint, tempPoint;

	for(int i = 0; i < vertex.size(); i++)
	{
		ogTOs(vertex[i], screenPoint);
		screenPoint.setZ(0);
		dis = pickPointGL.distanceToLine(screenPoint, QVector3D(0, 0, 0));
		if(dis - pickPrecision <= 0.001)
		{
			SetPosition(i);
			return true;
		}
	}
	return false;
}