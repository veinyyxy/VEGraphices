#include "CLineAnalysis.h"
#include <math.h>

bool CLineAnalysis::isLineintersect(float x0,float x1,float x2,float x3,float y0,float y1,float y2,float y3)
{
	//两条线段是否相交X0X1 AND X1X2
	float x, y;
    float Minx01 = std::min(x0, x1);
    float Miny01 = std::min(y0, y1);
    float Minx23 = std::min(x2, x3);
    float Miny23 = std::min(y2, y3);
    float Maxx01 = std::max(x0, x1);
    float Maxy01 = std::max(y0, y1);
    float Maxx23 = std::max(x2, x3);
    float Maxy23 = std::max(y2, y3);

	if(x1!=x0 && x2!=x3)
	{
		float k1 = (y1-y0)/(x1-x0);
		float k2 = (y3-y2)/(x3-x2);
		float Den = (y1-y0)*(x3-x2) - (y3-y2)*(x1-x0);
		if(k1==k2)
		{ //平行不相交
			float d1 = abs(y0*(x1-x0)-x0*(y1-y0)-y2*(x3-x2)+x2*(y3-y2)); //距离公式d = abs(c1-c2) / sqrt(a*a+b*b)
			if(d1==0)
			{//直线重合
				if((x2>Minx01 && x2<Maxy01 && y2>Miny01 && y2<Maxy01) || (x3>Minx01 && x3<Maxy01 && y3>Miny01 && y3<Maxy01)
					|| (x0>Minx23 && x0<Maxy23 && y0>Miny23 && y0<Maxy23) || (x1>Minx23 && x1<Maxy23 && y1>Miny23 && y1<Maxy23))
				{  //实际碰撞问题线段重合认为相交了
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}   
		}
		x = ((y2-y0)*(x1-x0)*(x3-x2)+(y1-y0)*(x3-x2)*x0-(y3-y2)*(x1-x0)*x2)/Den;
		y = ((y1-y0)*(x-x0))/(x1-x0) + y0;

		if(Minx01<x && x<Maxx01 && Miny01<y && y<Maxy01 && Minx23<x && x<Maxx23 && Miny23<y && y<Maxy23)
		{
			//	InterX = x;
			//	InterY = y;
			return true;
		}
	}
	else if(x1==x0 && x2!=x3)
	{
		x = x0;
		y = ((y3-y2)*(x0-x2))/(x3-x2) + y2;
		if(Minx01<x && x<Maxx01 && Miny01<y && y<Maxy01 && Minx23<x && x<Maxx23 && Miny23<y && y<Maxy23)
		{
			//	InterX = x;
			//	InterY = y;
			return true;
		}
	}
	else if(x1!=x0 && x2==x3)
	{
		x = x2;
		y = ((y1-y0)*(x2-x0))/(x1-x0) + y0;
		if(Minx01<x && x<Maxx01 && Miny01<y && y<Maxy01 && Minx23<x && x<Maxx23 && Miny23<y && y<Maxy23)
		{
			//	InterX = x;
			//	InterY = y;
			return true;
		}       
	}
	return false;
}

bool CLineAnalysis::isPolygonintersect(QVector3DArray &dataArray)
{
	//bool isintersect = false;
	if (dataArray.count()>0)
	{
		float x0,x1,x2,x3,y0,y1,y2,y3;
		for (int i=0;i<dataArray.count()-1;i++)
		{
			for (int j=0;j<dataArray.count()-1;j++)
			{
				if (i<j)
				{
					x0 = dataArray.at(i).x();
					x1 = dataArray.at(i+1).x();
					y0 = dataArray.at(i).y();
					y1 = dataArray.at(i+1).y();
					x2 = dataArray.at(j).x();
					x3 = dataArray.at(j+1).x();
					y2 = dataArray.at(j).y();
					y3 = dataArray.at(j+1).y();

					if (isLineintersect(x0,x1,x2,x3,y0,y1,y2,y3))
					{
						return true;
					}
				}				
			}
		}		
	}
	return false;
}
