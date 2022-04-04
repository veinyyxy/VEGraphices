#include "SymbolLineAlgorithmProvider.h"

SymbolLineAlgorithmPrvider::SymbolLineAlgorithmPrvider()
{

}

SymbolLineAlgorithmPrvider::~SymbolLineAlgorithmPrvider()
{

}

QVector<QPolygonF> SymbolLineAlgorithmPrvider::getSymbolLines(QPolygonF polygonf, qreal width,qreal type)
{
	QVector<QPolygonF> SymbolLine;
	if (type == 0 || type == 1)
	{

	}

	if (type == 2 || type == 6)
	{

	}

	if (type == 3 || type == 7)
	{

	}

	if (type == 4)
	{

	}	

	if (type == 5)
	{

	}

	if (type == 8)
	{

	}

	if (type == 9)
	{

	}	

	if (type == 38)
	{

	}

	if (type == 39)
	{

	}
	return SymbolLine;
}
int SymbolLineAlgorithmPrvider::direction(QPointF a, QPointF b, QPointF p)//判断p与ab的相对位置
{
	qreal zero=1e-6;
	qreal tmp=(p.x()-a.x())*(p.y()-b.y())-(p.x()-b.x())*(p.y()-a.y());
	if (tmp > zero) return 1;
	if (tmp < -1*zero) return 4;
	return 0;
}

bool SymbolLineAlgorithmPrvider::isPointInTriangle(QPointF a, QPointF b, QPointF c, QPointF p)//判断p是否在abc三个点组成的三角形区域内
{
	int t= direction(a,b,p)+ direction(b,c,p) + direction(c,a,p);

	if (t>=1 && t<=4) return true;
	if (t==8 || t==12) return true;
	return false;    
}

QVector<QVector3D> SymbolLineAlgorithmPrvider::getPolyLineFormula(QPolygonF polygonf)//根据一条折线点集求各个线段的直线方程(型如ax+by+c=0),b只有两个取值0或-1；
{
	QVector<QVector3D> polylineformula;

	int i, size = polygonf.size();
	for(i = 0; i < size - 1; i++)
	{
		polylineformula << LineHelper::getLineFormula(polygonf[i], polygonf[i+1]);
	}

	return polylineformula;
}

QVector<QVector3D> SymbolLineAlgorithmPrvider::getEquidistanceLines(QVector<QVector3D> polylineformula, qreal width)//根据已知的折线方程，求距其width的等距离折线方程
{
	QVector<QVector3D> temp;	
	QVector<QVector3D> equidistancelineformula;
	int i, size = polylineformula.size();

	for(i = 0; i < size; i++)
	{
		temp = LineHelper::getEquidistanceLines(polylineformula[i],width);		
		equidistancelineformula << temp[0] << temp[1];			
	}

	return equidistancelineformula;
}

QVector<QVector3D> SymbolLineAlgorithmPrvider::getAngularBisectrixFormula(QPolygonF polygonf, QVector<QVector3D> polylineformula)//获得一条折线上的所有夹角角平分线的方程
{
	int i, size = polylineformula.size();
	QVector<QVector3D> angularlines;//用于存放所有合理的角平分线
	QVector<QVector3D> angularformula;//用于暂时存放两条线的角平分线
	QPointF point[2];
	QVector3D  templine;

	if(size>2)
	{
		for(i = 0; i < size - 1; i++)
		{
			//计算两条直线之间的角平分线
			angularformula = LineHelper::getTwoAngularBisectrixFormula(polylineformula[i],polylineformula[i+1]);		

			//筛选合适的角平分线
			templine = LineHelper::getLineFormula(polygonf[i], polygonf[i+2]);//计算折线组成三角形的第三条边
			if(LineHelper::isLinesIntersected(templine, angularformula[0]) && LineHelper::isLinesIntersected(templine, angularformula[1]))
			{//三角形的第三条边和两条角平分线都有交点
				point[0] = LineHelper::getPointofIntersection(templine, angularformula[0]);
				point[1] = LineHelper::getPointofIntersection(templine, angularformula[1]);
				point[0] = LineHelper::getMidpoint(point[0], polygonf[i+1]);
				point[1] = LineHelper::getMidpoint(point[1], polygonf[i+1]);
				//判断中点是否在三角形区域内
				if(isPointInTriangle(polygonf[i], polygonf[i+1], polygonf[i+2], point[0]))
					angularlines<<angularformula[0];
				else 
					angularlines<<angularformula[1];		
			}
			else if(LineHelper::isLinesIntersected(templine, angularformula[0]))
			{
				angularlines<<angularformula[0];
			}
			else
				angularlines<<angularformula[1];
		}
	}	
	return angularlines;
}

QVector<QPolygonF> SymbolLineAlgorithmPrvider::getEquidistancePolygonFs(QPolygonF polygonf, qreal width)//根据一个折线方程的点集求解其等距离双线的点集,width是新折线与原来折线的距离
{
	QVector<QVector3D> polyline;
	QVector<QVector3D> equidistancelines;
	QVector<QVector3D> angularlines;
	polyline = getPolyLineFormula(polygonf);
	equidistancelines = getEquidistanceLines(polyline, width);
	angularlines = getAngularBisectrixFormula(polygonf, polyline);

	QPolygonF temppolygon1, temppolygon2;

	//分别计算初始点
	QPointF startpoint[2], endpoint[2];
	QVector3D startnormal, endnormal;
	startnormal = LineHelper::getNormalFormula(polygonf[0], polyline[0]);
	endnormal = LineHelper::getNormalFormula(polygonf[polygonf.size()-1], polyline[polyline.size()-1]);
	startpoint[0] = LineHelper::getPointofIntersection(startnormal, equidistancelines[0]);
	startpoint[1] = LineHelper::getPointofIntersection(startnormal, equidistancelines[1]);
	endpoint[0] = LineHelper::getPointofIntersection(endnormal, equidistancelines[equidistancelines.size()-2]);
	endpoint[1] = LineHelper::getPointofIntersection(endnormal, equidistancelines[equidistancelines.size()-1]);
	temppolygon1<<startpoint[0];
	temppolygon2<<startpoint[1];

	if(equidistancelines.size()>2)
	{
		//计算折线的拐点
		QPointF point[4], temppoint;
		int num = polygonf.size()-2;
		int i;
		for(i=0; i<num; i++)
		{
			point[0] = LineHelper::getPointofIntersection(equidistancelines[2*i], equidistancelines[2*i+2]);
			point[1] = LineHelper::getPointofIntersection(equidistancelines[2*i], equidistancelines[2*i+3]);

			if(LineHelper::distancePointToLine(point[0], angularlines[i]) < LineHelper::distancePointToLine(point[1], angularlines[i]))
				temppoint = point[0];				
			else
				temppoint = point[1];				
			if(LineHelper::distancePointToLine(temppolygon1[temppolygon1.size()-1], equidistancelines[2*i]) < LineHelper::distancePointToLine(temppolygon2[temppolygon2.size()-1], equidistancelines[2*i]))
				temppolygon1<<temppoint;
			else
				temppolygon2<<temppoint;

			point[2] = LineHelper::getPointofIntersection(equidistancelines[2*i+1], equidistancelines[2*i+2]);
			point[3] = LineHelper::getPointofIntersection(equidistancelines[2*i+1], equidistancelines[2*i+3]);
			if(LineHelper::distancePointToLine(point[2], angularlines[i]) < LineHelper::distancePointToLine(point[3], angularlines[i]))
				temppoint = point[2];				
			else	
				temppoint = point[3];
			if(temppolygon1.size() < temppolygon2.size())
				temppolygon1<<temppoint;
			else
				temppolygon2<<temppoint;			
		}
	}

	//将尾点加入到折线的点集中
	int tsize1 = temppolygon1.size();
	int tsize2 = temppolygon2.size();
	int esize = equidistancelines.size();
	if(LineHelper::distancePointToLine(temppolygon1[tsize1-1], equidistancelines[esize-2]) < LineHelper::distancePointToLine(temppolygon2[tsize2-1], equidistancelines[esize-2]))
	{
		temppolygon1<<endpoint[0];
		temppolygon2<<endpoint[1];
	}
	else
	{
		temppolygon1<<endpoint[1];
		temppolygon2<<endpoint[0];
	}
	QVector<QPolygonF> polygonfs;
	return polygonfs<<temppolygon1<<temppolygon2;
}

QVector<QPolygonF> SymbolLineAlgorithmPrvider::getEquidistancePolygonArrowFs(QPolygonF polygonf, qreal width)//根据一个折线方程的点集求解其等距离带箭头的双线点集,width是新折线与原来折线的距离
{
	QPointF A, B;
	A = polygonf[polygonf.size()-2];
	B = polygonf[polygonf.size()-1];	

	//求解箭头的顶点
	QPointF arrowpoint = LineHelper::getPointInVectorABToP(A, B, B, width);

	QVector<QPolygonF> doubleline;
	doubleline = getEquidistancePolygonFs(polygonf, width);//求解其等距双线
	if (doubleline.size()<=0)
	{
//		cout<<"求每条折线的箭头时：douobleline.size() = "<<doubleline.size()<<endl;
		exit(1);
	}
	QPointF C = doubleline[0][doubleline[0].size()-1];
	QPointF D = doubleline[1][doubleline[1].size()-1];
	QPointF arrowleftpoint = LineHelper::getPointInVectorABToP(arrowpoint, C, C, width);
	QPointF arrowrightpoint = LineHelper::getPointInVectorABToP(arrowpoint, D, D, width);
	QPolygonF arrowline;
	arrowline<<arrowrightpoint<<arrowpoint<<arrowleftpoint;

	return doubleline<<arrowline;
}

qreal  SymbolLineAlgorithmPrvider::distanceOfPolyLine(QPolygonF polygonf)
{
	int i, size = polygonf.size();
	qreal sumdistance = 0.0;
	for(i=0; i<size-1; i++)
	{
		sumdistance += LineHelper::distancePointFToPointF(polygonf[i], polygonf[i+1]);
	}
	return sumdistance;
}

QPointF SymbolLineAlgorithmPrvider::getPoint(QPolygonF polygonf, QPointF P, qreal width)//在折线polygonf上，在其正方向上求解距离P点为width的点，P点在折线集polygonf上；
{	
	int size = polygonf.size();
	int i = 0;
	while((P!=polygonf[i]) && (i<size))
		i++;
	if (i == size) exit(1);//P不在折线的点集上

	qreal sumdistance, distance;
	sumdistance = distance = 0.0;
	while (sumdistance<width)
	{
		if(i==size) exit(1);
		else
		{
			distance = LineHelper::distancePointFToPointF(polygonf[i], polygonf[i+1]);
			sumdistance += distance;
			i++;
		}
	}
	if(abs(sumdistance-width-distance)<1e-6) return polygonf[i];
	else
	{
		QPointF point = LineHelper::getPointInVectorABToP(polygonf[i-1],polygonf[i],polygonf[i-1],width+distance-sumdistance);
		return point;
	}
}

QVector<QPolygonF> SymbolLineAlgorithmPrvider::ColdFrontPolygonFs(QPolygonF polygonf, int width)//根据一个折线的点集求解冷锋线的点集,width是冷锋线上的三角形边长
{
	QVector<QPolygonF> coldfront;
	QPointF p1, p2, p3, p4, p5, startp;	
	qreal length = distanceOfPolyLine(polygonf);
	qreal len;

	int coldfrontsize = 4*width;//冷锋线的单位长度，暂设定为三角形边长的四倍
	startp = polygonf[0];
	coldfront<<polygonf;
	if (length < 2*width)  return coldfront;//折线长度小于三角形边长的两倍
	else //求双三角形的五个顶点
	{ 
		int num, i;
		num = length/coldfrontsize;

		if(num == 0)
		{
			len = (length - 2*width)/2;
			num = 1;
		}
		else
			len = (length - num*coldfrontsize)/2;		

		//计算在折线上距离某点为width的点的坐标
		i = 0;
		QPointF mid;
		while(i<num)
		{
			p1 = getPoint(polygonf, startp, len);
			p3 = getPoint(polygonf, startp, len+width);
			p5 = getPoint(polygonf, startp, len+2*width);
			mid = LineHelper::getMidpoint(p1, p3);
			p2 = LineHelper::getPointToPOnRightHand(p1, p3, mid, width/2, 1);
			mid = LineHelper::getMidpoint(p3, p5);
			p4 = LineHelper::getPointToPOnRightHand(p3, p5, mid, width/2, 1);

			QPolygonF topolygon;
			topolygon<<p1<<p2<<p3<<p4<<p5;
			coldfront<<topolygon;
			len += coldfrontsize;
			i++;
		}
	}	
	return coldfront;
}

QVector<QPolygonF> SymbolLineAlgorithmPrvider::WarmFrontPolygonFs(QPolygonF polygonf, int width)//根据一个折线方程的点集求解暖锋线的点集,width是暖锋线上圆的直径
{
	QVector<QPolygonF> warmfront;
	QPointF p1, p2, p3, p4, p5, startp, M;	
	QPointF ml1, ml2, ml3, mr1, mr2, mr3;
	QPointF QL1, QL2, QL3, QR1, QR2, QR3;
	qreal length = distanceOfPolyLine(polygonf);
	qreal len;
	qreal h1, h2, h3;

	int warmfrontsize = 4*width;//暖锋线的单位长度，暂设定直径的四倍
	startp = polygonf[0];
	warmfront<<polygonf;
	if (length < 2*width)  return warmfront;//折线长度小于直径的两倍
	else //求半圆突起的关键顶点
	{ 
		int num, i;
		QPointF M, ml, mr;
		num = length/warmfrontsize;		

		i = 0;
		if(num == 0)//折线长度大于直径的两倍，小于直径的四倍
		{
			len = (length - 2*width)/2;
			num = 1;
		}
		else
			len = (length - num*warmfrontsize)/2;//折线长度大于直径的四倍

		//计算在折线上距离某点为width的点的坐标
		h1 = width*sqrt(5.0)/8;
		h2 = width*sqrt(3.0)/4;
		h3 = width*sqrt(15.0)/8;
		while(i<num)
		{
			p1 = getPoint(polygonf, startp, len);
			p3 = getPoint(polygonf, startp, len+width);
			p5 = getPoint(polygonf, startp, len+2*width);
			M = LineHelper::getMidpoint(p1, p3);
			p2 = LineHelper::getPointToPOnRightHand(p1, p3, M, width/2, 1);

			ml2 = LineHelper::getMidpoint(p1, M);
			ml1 = LineHelper::getMidpoint(p1, ml2);
			ml3 = LineHelper::getMidpoint(ml2, M);
			mr2 = LineHelper::getMidpoint(M, p3);
			mr3 = LineHelper::getMidpoint(M, mr2);
			mr1 = LineHelper::getMidpoint(mr2, p3);
			QL1 = LineHelper::getPointToPOnRightHand(p1, p3, ml1, h1, 1);
			QL2 = LineHelper::getPointToPOnRightHand(p1, p3, ml2, h2, 1);
			QL3 = LineHelper::getPointToPOnRightHand(p1, p3, ml3, h3, 1);
			QR1 = LineHelper::getPointToPOnRightHand(p1, p3, mr1, h1, 1);
			QR2 = LineHelper::getPointToPOnRightHand(p1, p3, mr2, h2, 1);
			QR3 = LineHelper::getPointToPOnRightHand(p1, p3, mr3, h3, 1);

			QPolygonF topolygon;
			topolygon<<p1<<QL1<<QL2<<QL3<<p2<<QR3<<QR2<<QR1<<p3;			

			M = LineHelper::getMidpoint(p3, p5);
			p4 = LineHelper::getPointToPOnRightHand(p3, p5, M, width/2, 1);
			ml2 = LineHelper::getMidpoint(p3, M);
			ml1 = LineHelper::getMidpoint(p3, ml2);
			ml3 = LineHelper::getMidpoint(ml2, M);
			mr2 = LineHelper::getMidpoint(M, p5);
			mr3 = LineHelper::getMidpoint(M, mr2);
			mr1 = LineHelper::getMidpoint(mr2, p5);
			QL1 = LineHelper::getPointToPOnRightHand(p3, p5, ml1, h1, 1);
			QL2 = LineHelper::getPointToPOnRightHand(p3, p5, ml2, h2, 1);
			QL3 = LineHelper::getPointToPOnRightHand(p3, p5, ml3, h3, 1);
			QR1 = LineHelper::getPointToPOnRightHand(p3, p5, mr1, h1, 1);
			QR2 = LineHelper::getPointToPOnRightHand(p3, p5, mr2, h2, 1);
			QR3 = LineHelper::getPointToPOnRightHand(p3, p5, mr3, h3, 1);

			topolygon<<p3<<QL1<<QL2<<QL3<<p4<<QR3<<QR2<<QR1<<p5;
			warmfront<<topolygon;
			len += warmfrontsize;
			i++;
		}
	}	
	return warmfront;
}

QVector<QPolygonF> SymbolLineAlgorithmPrvider::OcclusionFrontPolygonFs(QPolygonF polygonf, int width)//根据一个折线方程的点集求解锢囚锋线的点集,width是锢囚锋线上圆的直径
{
	QVector<QPolygonF> occlusionfront;
	QPointF p1, p2, p3, p4, p5, startp, M;	
	QPointF ml1, ml2, ml3, mr1, mr2, mr3;
	QPointF QL1, QL2, QL3, QR1, QR2, QR3;
	qreal length = distanceOfPolyLine(polygonf);
	qreal len;
	qreal h1, h2, h3;

	int occlusionfrontsize = 4*width;//暖锋线的单位长度，暂设定直径的四倍
	startp = polygonf[0];
	occlusionfront<<polygonf;
	if (length < 2*width)  return occlusionfront;//折线长度小于直径的两倍
	else //求半圆突起的关键顶点
	{ 
		int num, i;
		QPointF M, ml, mr;
		num = length/occlusionfrontsize;		

		i = 0;
		if(num == 0)//折线长度大于直径的两倍，小于直径的四倍
		{
			len = (length - 2*width)/2;
			num = 1;
		}
		else
			len = (length - num*occlusionfrontsize)/2;//折线长度大于直径的四倍

		//计算在折线上距离某点为width的点的坐标
		h1 = width*sqrt(5.0)/8;
		h2 = width*sqrt(3.0)/4;
		h3 = width*sqrt(15.0)/8;
		while(i<num)
		{
			p1 = getPoint(polygonf, startp, len);
			p3 = getPoint(polygonf, startp, len+width);
			p5 = getPoint(polygonf, startp, len+2*width);
			M = LineHelper::getMidpoint(p1, p3);
			p2 = LineHelper::getPointToPOnRightHand(p1, p3, M, width/2, 1);			

			QPolygonF topolygon;
			topolygon<<p1<<p2;			

			M = LineHelper::getMidpoint(p3, p5);
			p4 = LineHelper::getPointToPOnRightHand(p3, p5, M, width/2, 1);
			ml2 = LineHelper::getMidpoint(p3, M);
			ml1 = LineHelper::getMidpoint(p3, ml2);
			ml3 = LineHelper::getMidpoint(ml2, M);
			mr2 = LineHelper::getMidpoint(M, p5);
			mr3 = LineHelper::getMidpoint(M, mr2);
			mr1 = LineHelper::getMidpoint(mr2, p5);
			QL1 = LineHelper::getPointToPOnRightHand(p3, p5, ml1, h1, 1);
			QL2 = LineHelper::getPointToPOnRightHand(p3, p5, ml2, h2, 1);
			QL3 = LineHelper::getPointToPOnRightHand(p3, p5, ml3, h3, 1);
			QR1 = LineHelper::getPointToPOnRightHand(p3, p5, mr1, h1, 1);
			QR2 = LineHelper::getPointToPOnRightHand(p3, p5, mr2, h2, 1);
			QR3 = LineHelper::getPointToPOnRightHand(p3, p5, mr3, h3, 1);

			topolygon<<p3<<QL1<<QL2<<QL3<<p4<<QR3<<QR2<<QR1<<p5;
			occlusionfront<<topolygon;
			len += occlusionfrontsize;
			i++;
		}
	}	
	return occlusionfront;
}

QVector<QPolygonF> SymbolLineAlgorithmPrvider::Quasi_StationaryFrontPolygonFs(QPolygonF polygonf, int width)//根据一个折线方程的点集求解静止锋线的点集,width是静止锋线上圆的直径
{
	QVector<QPolygonF> front;
	QPointF p1, p2, p3, p4, p5, startp, M;	
	QPointF ml1, ml2, ml3, mr1, mr2, mr3;
	QPointF QL1, QL2, QL3, QR1, QR2, QR3;
	qreal length = distanceOfPolyLine(polygonf);
	qreal len;
	qreal h1, h2, h3;

	int frontsize = 4*width;//静止锋线的单位长度，暂设定直径的四倍
	startp = polygonf[0];
	front<<polygonf;
	if (length < 2*width)  return front;//折线长度小于直径的两倍
	else //求半圆突起的关键顶点
	{ 
		int num, i;
		QPointF M, ml, mr;
		num = length/frontsize;		

		i = 0;
		if(num == 0)//折线长度大于直径的两倍，小于直径的四倍
		{
			len = (length - 2*width)/2;
			num = 1;
		}
		else
			len = (length - num*frontsize)/2;//折线长度大于直径的四倍

		//计算在折线上距离某点为width的点的坐标
		h1 = width*sqrt(5.0)/8;
		h2 = width*sqrt(3.0)/4;
		h3 = width*sqrt(15.0)/8;
		while(i<num)
		{
			p1 = getPoint(polygonf, startp, len);
			p3 = getPoint(polygonf, startp, len+width);
			p5 = getPoint(polygonf, startp, len+2*width);
			M = LineHelper::getMidpoint(p1, p3);
			p2 = LineHelper::getPointToPOnRightHand(p1, p3, M, width/2, 1);

			ml2 = LineHelper::getMidpoint(p1, M);
			ml1 = LineHelper::getMidpoint(p1, ml2);
			ml3 = LineHelper::getMidpoint(ml2, M);
			mr2 = LineHelper::getMidpoint(M, p3);
			mr3 = LineHelper::getMidpoint(M, mr2);
			mr1 = LineHelper::getMidpoint(mr2, p3);
			QL1 = LineHelper::getPointToPOnRightHand(p1, p3, ml1, h1, 1);
			QL2 = LineHelper::getPointToPOnRightHand(p1, p3, ml2, h2, 1);
			QL3 = LineHelper::getPointToPOnRightHand(p1, p3, ml3, h3, 1);
			QR1 = LineHelper::getPointToPOnRightHand(p1, p3, mr1, h1, 1);
			QR2 = LineHelper::getPointToPOnRightHand(p1, p3, mr2, h2, 1);
			QR3 = LineHelper::getPointToPOnRightHand(p1, p3, mr3, h3, 1);

			QPolygonF topolygon;
			topolygon<<p1<<QL1<<QL2<<QL3<<p2<<QR3<<QR2<<QR1<<p3;			

			M = LineHelper::getMidpoint(p3, p5);
			p4 = LineHelper::getPointToPOnRightHand(p3, p5, M, width/2, 0);

			topolygon<<p4<<p5;
			front<<topolygon;
			len += frontsize;
			i++;
		}
	}	
	return front;
}