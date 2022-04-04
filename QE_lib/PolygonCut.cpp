
#include "PolygonCut.h"

CPolygonCut::CPolygonCut(void)
{

}


CPolygonCut::~CPolygonCut(void)
{

}

double CPolygonCut::TranCircleDirection(double x1,double y1,double x2,double y2,double x3,double y3)
{
	return (x2-x1)*(y3-y2)-(y2-y1)*(x3-x2);
}

void CPolygonCut::CutPolygon(FLAG_POINT_ARRAY &Parent_FlagPointArray,FLAG_POINT_ARRAY &Mask_FlagPointArray,QVector<QVector3DArray*> *DivVector)
{
	QVector3DArray* PolyNew = NULL; //输出多边形

	while (FindInterPoint(Parent_FlagPointArray))
	{
		bool isCutStart = false;  //判断是否正在进行裁剪
		bool isCutFinished = false;
		bool isInParentPoly = true; //默认开始从被裁减多边形进行遍历
		int StartPos = -1; //默认起始ID为-1

		int i=0;
		int j=0;

		while(!isCutFinished)
		{
			if (!isInParentPoly&&isCutStart)
			{
				if (Mask_FlagPointArray.at(j)->m_type ==VERTEX_FREE)
				{
					if(PolyNew!=NULL)
						PolyNew->push_back(QVector3D(Mask_FlagPointArray.at(j)->x,Mask_FlagPointArray.at(j)->y,Mask_FlagPointArray.at(j)->z));

					j++; //continue
					if (j==Mask_FlagPointArray.count()-1)
					{
						j=0;
					}
				}			  
				else if (Mask_FlagPointArray.at(j)->m_type ==VERTEX_IN)
				{
					i =  ReturnIndex(Parent_FlagPointArray,Mask_FlagPointArray.at(j));	

					if(PolyNew!=NULL)
						PolyNew->push_back(QVector3D(Parent_FlagPointArray.at(i)->x,Parent_FlagPointArray.at(i)->y,Parent_FlagPointArray.at(i)->z));

					Parent_FlagPointArray.at(i)->SetType(VERTEX_FREE);

                    if(i==StartPos)
					{
						isCutStart = false; //Stop cutting		
						DivVector->push_back(PolyNew);
						PolyNew = NULL;
						isCutFinished = true;								
						// i=0; //
					}					

					isInParentPoly = true; //jump to parent polygon  
				}
			}
			else if (isInParentPoly)
			{			
				if (!isCutStart)
				{
					if (Parent_FlagPointArray.at(i)->m_type ==VERTEX_IN)
					{
						StartPos = i;
						isCutStart = true;
						j =  ReturnIndex(Mask_FlagPointArray,Parent_FlagPointArray.at(i)); // start cutting

						PolyNew = new QVector3DArray;
						PolyNew->push_back(QVector3D(Parent_FlagPointArray.at(i)->x,Parent_FlagPointArray.at(i)->y,Parent_FlagPointArray.at(i)->z));

						//  Parent_FlagPointArray.at(i).m_type =VERTEX_FREE;
						i++;

						if (i==Parent_FlagPointArray.count()-1)
						{
							i = 0;
						}
					}
					else
					{
						i++;
						if (i==Parent_FlagPointArray.count()-1)
						{
							i = 0;
						}
					}
					
				}
				if (isCutStart)
				{
					if (Parent_FlagPointArray.at(i)->m_type ==VERTEX_FREE)
					{
						if (PolyNew!=NULL)
						{
							PolyNew->push_back(QVector3D(Parent_FlagPointArray.at(i)->x,Parent_FlagPointArray.at(i)->y,Parent_FlagPointArray.at(i)->z));
						}
						i++;
						if (i ==Parent_FlagPointArray.count()-1)
						{
							i = 0;
						}
					}
					else if (Parent_FlagPointArray.at(i)->m_type ==VERTEX_IN)
					{
						if (i!=StartPos)
						{	  
							if(PolyNew!=NULL)
								PolyNew->push_back(QVector3D(Parent_FlagPointArray.at(i)->x,Parent_FlagPointArray.at(i)->y,Parent_FlagPointArray.at(i)->z));

							Parent_FlagPointArray.at(i)->SetType(VERTEX_FREE);

							i++;
							if (i==Parent_FlagPointArray.count()-1)
								i = 0;							  
						}
					}
					else if (Parent_FlagPointArray.at(i)->m_type ==VERTEX_OUT)
					{
						isInParentPoly = false; //Jump to mask polygon
						j = ReturnIndex(Mask_FlagPointArray,Parent_FlagPointArray.at(i));

						if(PolyNew!=NULL)
						{
							PolyNew->push_back(QVector3D(Parent_FlagPointArray.at(i)->x,Parent_FlagPointArray.at(i)->y,Parent_FlagPointArray.at(i)->z));
							Parent_FlagPointArray.at(i)->SetType(VERTEX_FREE);

							j++;		
							if (j==Mask_FlagPointArray.count()-1)
							{
								j=0;
							}
						}	
					}
				}			
			}
		}		
	}	
}

///////////////////////////////
//Weiler－Atherton算法对多边形裁减
//PolygonParent为被裁减多边形
//PolygonMask为裁减多边形
//////////////////////////////
void CPolygonCut::DivisonbyPolygon(QVector<QVector3DArray*> *DivVector, QVector3DArray *polygonParent,QVector3DArray *polygonMask,POLY_CUT_TYPE divetype)
{
	QVector3DArray* PolyParent = NULL; //顺时针存储节点
	QVector3DArray* PolyMask = NULL;//顺时针存储节点

//	QVector3DArray* PolyNew = NULL; //输出多边形

	FLAG_POINT_ARRAY Parent_FlagPointArray;
	FLAG_POINT_ARRAY Mask_FlagPointArray;
	FLAG_POINT_ARRAY InterSectPointArray;
	FLAG_POINT_ARRAY FlagPointArrayTemp; 

	bool isPolyCut = false;	

	//判断多边形环绕方向
	double ss1 = 0;
	double ss2 = 0;

	double x1 = 0 ; 
	double x2 = 0 ;
	double x3 = 0 ; 

	double y1 = 0 ; 
	double y2 = 0 ;	
	double y3 = 0 ; 

	double xmin_parent,ymin_parent,xmax_parent,ymax_parent;
	double xmin_mask,ymin_mask,xmax_mask,ymax_mask;		
	
	//STEP 1  Process Polygons	
	if (polygonParent->count()>2)
	{
		int MaxPoint_index = 0;

		xmin_parent = xmax_parent = polygonParent->at(0).x(); 
		ymin_parent = ymax_parent = polygonParent->at(0).y();

		for (int i=1;i<polygonParent->count()-1;i++)
		{
			x1 = polygonParent->at(i).x();
			y1 = polygonParent->at(i).y();			

			if (x1<xmin_parent) xmin_parent = x1;
			if (x1>xmax_parent) xmax_parent = x1;
			if (y1<ymin_parent) ymin_parent = y1;			
			
			if (y1>ymax_parent) 
			{
              MaxPoint_index = i;
			  ymax_parent = y1;
			}
		}	

		if (MaxPoint_index>0)
		{
			x1 = polygonParent->at(MaxPoint_index-1).x();
			y1 = polygonParent->at(MaxPoint_index-1).y();
			
		}
		else 
		{
			x1 = polygonParent->at(polygonParent->count()-2).x();
			y1 = polygonParent->at(polygonParent->count()-2).y();
		}

		x2 = polygonParent->at(MaxPoint_index).x();
		y2 = polygonParent->at(MaxPoint_index).y();
		x3 = polygonParent->at(MaxPoint_index+1).x();
		y3 = polygonParent->at(MaxPoint_index+1).y();		

        ss1 = TranCircleDirection(x1,y1,x2,y2,x3,y3);
 
 		if(ss1>0) //逆时针
 		{
			PolyParent = new QVector3DArray;
 			for (int i=0;i<polygonParent->count();i++)
 			{
 				PolyParent->push_back(polygonParent->at(polygonParent->count()-i-1));
 			}
 		}
		else if (ss1<=0)  //顺时针
		{
          PolyParent = new QVector3DArray;
		  for (int i=0;i<polygonParent->count();i++)
		  {
			  PolyParent->push_back(polygonParent->at(i));
		  }
			// PolyParent = polygonParent;
		}
	}

	//// Process PolygonMask
	if (polygonMask->count()>2)
	{
		int MaxPoint_index = 0;

		xmin_mask = xmax_mask = polygonMask->at(0).x();
        ymin_mask = ymax_mask = polygonMask->at(0).y();

		for (int i=1;i<polygonMask->count()-1;i++)
		{
			x1 = polygonMask->at(i).x();
			y1 = polygonMask->at(i).y();			

			if (x1<xmin_mask) xmin_mask = x1;
			if (y1<ymin_mask) ymin_mask = y1;
			if (x1>xmax_mask) xmax_mask = x1;
			if (y1>ymax_mask)
			{
				ymax_mask = y1;
				MaxPoint_index = i;
			}				
		}		
		
		if (MaxPoint_index>0)
		{
			x1 = polygonMask->at(MaxPoint_index-1).x();
			y1 = polygonMask->at(MaxPoint_index-1).y();

		}
		else 
		{
			x1 = polygonMask->at(polygonMask->count()-2).x();
			y1 = polygonMask->at(polygonMask->count()-2).y();
		}
		x2 = polygonMask->at(MaxPoint_index).x();
		y2 = polygonMask->at(MaxPoint_index).y();
		x3 = polygonMask->at(MaxPoint_index+1).x();
		y3 = polygonMask->at(MaxPoint_index+1).y();

		ss2 = TranCircleDirection(x1,y1,x2,y2,x3,y3);

		if(ss2>0)  //逆时针
		{
			PolyMask = new QVector3DArray;
			for (int i=0;i<polygonMask->count();i++)
			{
				PolyMask->push_back(polygonMask->at(polygonMask->count()-i-1));
			}
		}
		else if (ss2<=0)  //顺时针
		{
			PolyMask = new QVector3DArray;
			for (int i=0;i<polygonMask->count();i++)
			{
				PolyMask->push_back(polygonMask->at(i));
			}			
		}
	}			

	// 如果多边形外接矩形相交或包含
	if(!((xmin_parent>=xmax_mask)||(xmin_mask>=xmax_parent)||(ymin_parent>=ymax_mask)||(ymin_mask>=ymax_parent)))
	{
		//STEP 2 intersectPolygons			
		bool isIntersect1 = InterSectPolygons(Parent_FlagPointArray,Mask_FlagPointArray,InterSectPointArray,PolyParent,PolyMask);
			
		//如果存在交点		
		if (isIntersect1)
		{
			//STEP 3 cutPolygon			
			CutPolygon(Parent_FlagPointArray,Mask_FlagPointArray,DivVector);
		}	
		//如果被裁减多边形在裁减多边形内部
		else if ((xmin_parent>=xmin_mask)&&(xmax_parent<=xmax_mask)&&(ymin_parent>=ymin_mask)&&(ymax_parent<=ymax_mask))
		{
			DivVector->push_back(PolyParent);
		}
	}	
	
}

bool CPolygonCut::FindInterPoint(FLAG_POINT_ARRAY &PointFlagArray)
{
	bool flag = false;
	for(int i=0;i<PointFlagArray.count();i++)
	{
		if(PointFlagArray.at(i)->m_type ==VERTEX_IN) 
		{
			flag = true;
			break;			
		} 
	}
	return flag;
}

bool CPolygonCut::TwoLineRectIntersect(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
	float x, y;
	float Minx01 = Getmin(x0, x1);
	float Miny01 = Getmin(y0, y1);
	float Minx23 = Getmin(x2, x3);
	float Miny23 = Getmin(y2, y3);
	float Maxx01 = Getmax(x0, x1);
	float Maxy01 = Getmax(y0, y1);
	float Maxx23 = Getmax(x2, x3);
	float Maxy23 = Getmax(y2, y3);

	if ((Minx01>=Maxx23)||(Minx23>=Maxx01)||(Miny01>=Maxy23) || (Miny23>=Maxy01))
	{
		return false;
	}
	else
		return true;
}

bool CPolygonCut::TwoLineIsIntersect(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float &InterX, float &InterY)
{ //两条线段是否相交X0X1 AND X1X2
	float x, y;
	float Minx01 = Getmin(x0, x1);
	float Miny01 = Getmin(y0, y1);
	float Minx23 = Getmin(x2, x3);
	float Miny23 = Getmin(y2, y3);
	float Maxx01 = Getmax(x0, x1);
	float Maxy01 = Getmax(y0, y1);
	float Maxx23 = Getmax(x2, x3);
	float Maxy23 = Getmax(y2, y3);

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
		else
		{
			x = ((y2-y0)*(x1-x0)*(x3-x2)+(y1-y0)*(x3-x2)*x0-(y3-y2)*(x1-x0)*x2)/Den;
			y = ((y1-y0)*(x-x0))/(x1-x0) + y0;

			if((x>=Minx01&&x<=Maxy01)||(x>=Minx23&&x<=Maxx23)) //线段相交而不是直线相交
			{
				InterX = x;
				InterY = y;
				return true;
			}
		}
	}
	else if(x1==x0 && x2!=x3)
	{
		x = x0;
		y = ((y3-y2)*(x0-x2))/(x3-x2) + y2;
		if(Minx01<=x && x<=Maxx01 && Miny01<=y && y<=Maxy01 && Minx23<=x && x<=Maxx23 && Miny23<=y && y<=Maxy23)
		{
			InterX = x;
			InterY = y;
			return true;
		}
	}
	else if(x1!=x0 && x2==x3)
	{
		x = x2;
		y = ((y1-y0)*(x2-x0))/(x1-x0) + y0;
		if(Minx01<=x && x<=Maxx01 && Miny01<=y && y<=Maxy01 && Minx23<=x && x<=Maxx23 && Miny23<=y && y<=Maxy23)
		{
			InterX = x;
			InterY = y;
			return true;
		}       
	}
	return false;
}

bool CPolygonCut::InterSectPolygons(FLAG_POINT_ARRAY &PointFlagArray,FLAG_POINT_ARRAY &PointFlagArray2,FLAG_POINT_ARRAY& InterSectPointArray,QVector3DArray *polygon1,QVector3DArray *polygon2)
{
	double xtemp,ytemp; //交点的临时坐标
	double k1 = 0;
	double k2 = 0;
	double m1 = 0;
	double m2 = 0;
	
	//double s1 = 0;
	//double s2 = 0;

	double x1 = 0 ; 
	double x2 = 0 ;
	double y1 = 0 ; 
	double y2 = 0 ;

	double x3 = 0 ; 
	double x4 = 0 ;
	double y3 = 0 ; 
	double y4 = 0 ;
		
	/////////////STEP 3.1		
	//求交点，设置交点属性		
	for (int i=0;i<polygon1->count()-1;i++)
	{
		x1 = polygon1->at(i).x();
		y1 = polygon1->at(i).y();
		x2 = polygon1->at(i+1).x();
		y2 = polygon1->at(i+1).y();		
		
		for (int j=0;j<polygon2->count()-1;j++)
		{
			x3 = polygon2->at(j).x();
			y3 = polygon2->at(j).y();
			x4 = polygon2->at(j+1).x();
			y4 = polygon2->at(j+1).y();		
			
			float interX;
			float interY;
			bool testbool;

			//if (isRangeCross(x1,x2,x3,x4)&&isRangeCross(y1,y2,y3,y4)) //线段包围矩形相交
			
			if(TwoLineRectIntersect(x1,y1,x2,y2,x3,y3,x4,y4))
			{ 				
				if(TwoLineIsIntersect(x1,y1,x2,y2,x3,y3,x4,y4,interX,interY)) //如果两线段相交
				{
					//新建交点
					FlagPoint* PointFlagTemp = new FlagPoint;
					PointFlagTemp->x = interX;
					PointFlagTemp->y = interY;	
					PointFlagTemp->z = polygon1->at(i).z();
							
					PointFlagTemp->index_mask = j;
					PointFlagTemp->index_parent = i;

						//判断交点是“入”交点还是“出”交点
						//如果点在直线的右边，则为“入”交点
					if (TranCircleDirection(x3,y3,x4,y4,x2,y2)<0) 
					{						
						PointFlagTemp->m_type = VERTEX_IN;			
					}
					else 
					{							
						PointFlagTemp->m_type = VERTEX_OUT;											
					}				
				
					InterSectPointArray.push_back(PointFlagTemp);
				}					
			} //if
		} //for
	} //for

	
	//////////////////////////////////////////////////////////////////////////
	//MergePolygon
	//////////////////////////////////////////////////////////////////////////
	if (InterSectPointArray.size()>0)
	{
	     //Merge ParentPolygon
		{
			
			double DistanceTemp,DistanceTemp1;

			for (int i=0;i<polygon1->count();i++)
			{
				FLAG_POINT_ARRAY InsertPointsTemp;

				//	PolyParent2->push_back(PolyParent->at(i));
				FlagPoint* PointFlagTemp = new FlagPoint;
				PointFlagTemp->x = polygon1->at(i).x();
				PointFlagTemp->y = polygon1->at(i).y();
				PointFlagTemp->z =  polygon1->at(i).z();
				PointFlagTemp->m_type = VERTEX_FREE;

				PointFlagTemp->index_mask = -1;
				PointFlagTemp->index_parent = i;		

				PointFlagArray.push_back(PointFlagTemp);

				for (int j=0;j<InterSectPointArray.count();j++)
				{
					if(InterSectPointArray.at(j)->index_parent==i)						 
					{
						FlagPoint* PointFlagTemp = new FlagPoint;
						PointFlagTemp->Colne(InterSectPointArray.at(j));
						InsertPointsTemp.push_back(PointFlagTemp);						
					}
						
				}
				
				//insert points
				if (InsertPointsTemp.count()==1)
				{
					PointFlagArray.push_back(InsertPointsTemp.at(0));
				}
				//Sort insert points,bubble sorts
				else if(InsertPointsTemp.count()>1)
				{
					FlagPoint* temp = new FlagPoint;

					for (int pos1=0;pos1<InsertPointsTemp.count()-1;pos1++)
					{
						for (int pos2=0;pos2<InsertPointsTemp.count()-1-pos1;pos2++)
						{
							DistanceTemp = abs(InsertPointsTemp.at(pos2)->x-polygon1->at(i).x())+abs(InsertPointsTemp.at(pos2)->y-polygon1->at(i).y());
							DistanceTemp1 = abs(InsertPointsTemp.at(pos2+1)->x-polygon1->at(i).x())+abs(InsertPointsTemp.at(pos2+1)->y-polygon1->at(i).y());
							if (DistanceTemp>DistanceTemp1)
							{
								temp->Colne(InsertPointsTemp.at(pos2));
								InsertPointsTemp.at(pos2)->Colne(InsertPointsTemp.at(pos2+1));
								InsertPointsTemp.at(pos2+1)->Colne(temp);							
							}
						}
					} 

					for (int pos3=0;pos3<InsertPointsTemp.count();pos3++)
					{
						PointFlagArray.push_back(InsertPointsTemp.at(pos3));
					//	delete InsertPointsTemp.at(pos3);
					}				
				}
				InsertPointsTemp.clear();
			}		
		}
         //Merge MaskPolygon
		{			
			double DistanceTemp,DistanceTemp1;
			for (int i=0;i<polygon2->count();i++)
			{
				FLAG_POINT_ARRAY InsertPointsTemp;
				
				FlagPoint* PointFlagTemp = new FlagPoint;
				PointFlagTemp->x = polygon2->at(i).x();
				PointFlagTemp->y = polygon2->at(i).y();
				PointFlagTemp->z =  polygon2->at(i).z();
				PointFlagTemp->m_type = VERTEX_FREE;

				PointFlagTemp->index_mask = i;
				PointFlagTemp->index_parent = -1;

				PointFlagArray2.push_back(PointFlagTemp);

				for (int j=0;j<InterSectPointArray.count();j++)
				{
					if(InterSectPointArray.at(j)->index_mask==i)
					{
 						FlagPoint* PointFlagTemp = new FlagPoint;
 						PointFlagTemp->Colne(InterSectPointArray.at(j));
 						InsertPointsTemp.push_back(PointFlagTemp);						
					}
					//PointFlagArray.push_back(InterSectPointArray.at(j));  //add intersect point to PointFlagArray           
				}

				//////////////////////////////////////////////////////////////////////////
				if (InsertPointsTemp.count()==1)
				{
					PointFlagArray2.push_back(InsertPointsTemp.at(0));
				}
				//Sort insert points,bubble sorts
				else if(InsertPointsTemp.count()>1)
				{
					FlagPoint* temp = new FlagPoint;

					for (int pos1=0;pos1<InsertPointsTemp.count()-1;pos1++)
					{
						for (int pos2=0;pos2<InsertPointsTemp.count()-1-pos1;pos2++)
						{
							DistanceTemp = abs(InsertPointsTemp.at(pos2)->x-polygon2->at(i).x())+abs(InsertPointsTemp.at(pos2)->y-polygon2->at(i).y());
							DistanceTemp1 = abs(InsertPointsTemp.at(pos2+1)->x-polygon2->at(i).x())+abs(InsertPointsTemp.at(pos2+1)->y-polygon2->at(i).y());
							if (DistanceTemp>DistanceTemp1)
							{
								temp->Colne(InsertPointsTemp.at(pos2));
								InsertPointsTemp.at(pos2)->Colne(InsertPointsTemp.at(pos2+1));
								InsertPointsTemp.at(pos2+1)->Colne(temp);						 
							}
						}
					} 	
					for (int pos3=0;pos3<InsertPointsTemp.count();pos3++)
					{
						PointFlagArray2.push_back(InsertPointsTemp.at(pos3));
				//		delete InsertPointsTemp.at(pos3);
					}			
				}
				InsertPointsTemp.clear();
			}		
		}

	//	InterSectPointArray.clear();
	
		return true;
	}
	else
		return false;		

}


//////////////////////////////////////////////////////////////////////////
//返回PointFlag在PointFlagArray中的ID
//////////////////////////////////////////////////////////////////////////
int CPolygonCut::ReturnIndex(FLAG_POINT_ARRAY &PointFlagArray,FlagPoint* PointFlag)
{
	int index = -1;
	if (PointFlagArray.count()>0)
	{
            for (int i=0;i<PointFlagArray.count();i++)
			{
				//if (PointFlagArray.at(i)->m_type==PointFlag->m_type&&PointFlagArray.at(i)->index_mask ==PointFlag->index_mask&&PointFlagArray.at(i)->index_parent ==PointFlag->index_parent)
				if(PointFlagArray.at(i)->index_mask ==PointFlag->index_mask&&PointFlagArray.at(i)->index_parent ==PointFlag->index_parent)
				{
					if(abs(PointFlagArray.at(i)->x-PointFlag->x)<0.00001&&abs(PointFlagArray.at(i)->y-PointFlag->y)<0.00001)
					{
						//return i;	
						index = i;
						break;
					}
				}
				
			}		
	}
//	else 
	return index;
}
