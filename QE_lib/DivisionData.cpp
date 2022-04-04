#include "DivisionData.h"
#define RValue  -77
#define LValue  -77.00004

CDivisionData::CDivisionData(void)
{
	m_nType = 2;				//默认为面分隔
	m_fLValue = -77.00004;
	m_fRValue = -77;
}

CDivisionData::~CDivisionData(void)
{
}

void CDivisionData::DefaultSplit()
{
	m_fLValue = -77.00004;
	m_fRValue = -77;
}

void CDivisionData::SetSplit(float splitPoint)
{
	m_fLValue = splitPoint-0.00004;
	m_fRValue = splitPoint;
}

void CDivisionData::Division(QVector<QVector3DArray*> *DivVector, QVector3DArray *polyline)
{
	QVector3DArray* ParentLine = NULL;
	QVector3DArray* SubLine = NULL;

	bool DivStart = false;				//记录是否需要分割
	bool HasParent = false;				//记录是经度为－77的值
	bool RStart	 = false;
	double x1 = 0 ; 
	double x2 = 0 ;
	double y1 = 0 ; 
	double y2 = 0 ;
	double yTempParent = 0 ;
	double yTempsub = 0 ;
	double xLast = 0 ; 
	double yLast = 0 ;
	
	if(polyline->at(0).x()>m_fRValue)
	{
		for(int i = 0 ; i< polyline->count(); i++)
		{
			if(polyline->at(i).x() >=m_fRValue)
			{
				if(ParentLine == NULL)
				{
					ParentLine = new QVector3DArray;
					HasParent = true; 
				}

				if(DivStart)
				{
					x2 = polyline->at(i).x();
					y2= polyline->at(i).y();
					if(i>=0)
					{
						xLast = polyline->at(i-1).x();
						yLast = polyline->at(i-1).y();
						yTempParent = polyline->at(i-1).y();
					}
					SubLine->push_back(QVector3D(m_fLValue, yTempParent, polyline->at(i-1).z()));		//将分割后的线闭合
					SubLine->push_back(QVector3D(m_fLValue, yTempsub, polyline->at(i-1).z()));			//将分割后的线闭合
					DivVector->push_back(SubLine);
					DivStart = false;
					ParentLine->push_back(QVector3D(m_fRValue, yTempParent, polyline->at(i-1).z()));
				}

				ParentLine->push_back(polyline->at(i));
			}else if(polyline->at(i).x() <m_fRValue)
			{
				if(!DivStart)
				{
					SubLine = new QVector3DArray;
					DivStart = true;
					x1 = polyline->at(i).x();
					y1=  polyline->at(i).y();

					if(i>=0)
						yTempsub = polyline->at(i-1).y();
					else
						yTempsub = 0 ; 
					if(HasParent)
					{
						SubLine->push_back(QVector3D(m_fLValue, yTempsub, polyline->at(i-1).z()));
					}
					if(ParentLine != NULL)
					{
						ParentLine->push_back(QVector3D(m_fRValue, yTempsub,polyline->at(i-1).z()));
					}
				}
				SubLine->push_back(polyline->at(i));
			}
		}

		if(NULL != ParentLine)
			DivVector->push_back(ParentLine);   return;
	}else
	{


		for(int i = 0 ; i< polyline->count(); i++)
		{
			if(polyline->at(i).x() <m_fRValue)
			{
				if(ParentLine == NULL)
				{
					ParentLine = new QVector3DArray;
					HasParent = true; 
				}

				if(DivStart)
				{
					x2 = polyline->at(i).x();
					y2= polyline->at(i).y();
					if(i >0) 
					{
						xLast = polyline->at(i-1).x();
						yTempParent = polyline->at(i-1).y();
					}else
					{
						xLast = 0 ; 
						yTempParent = 0 ;
					}
					float temp =  m_fRValue * (yTempParent- y2)/(xLast -x2) + (yTempParent * x2 - y2* xLast)/(x2- xLast);
					SubLine->push_back(QVector3D(m_fRValue, temp, polyline->at(i-1).z()));			//将分割后的线闭合
					SubLine->push_back(QVector3D(m_fRValue, yTempsub, polyline->at(i-1).z()));			//将分割后的线闭合
					DivVector->push_back(SubLine);							//将分割后线存于Vector
					DivStart = false;
					ParentLine->push_back(QVector3D(m_fLValue, temp, polyline->at(i-1).z()));
				}

				ParentLine->push_back(polyline->at(i));
			}else if(polyline->at(i).x() >=m_fRValue)
			{
				if(!DivStart)
				{
					SubLine = new QVector3DArray;
					DivStart = true;
					x1 = polyline->at(i).x();
					y1=  polyline->at(i).y();
					if(i >0) 
					{
						xLast = polyline->at(i-1).x();
						yLast = polyline->at(i-1).y();
						yTempsub = m_fRValue * (yLast- y1)/(xLast -x1) + (yLast * x1 - y1* xLast)/(x1- xLast);
					}else
					{
						xLast = 0 ; 
						yTempsub = 0 ;
					}

					if(HasParent)
					{
						SubLine->push_back(QVector3D(m_fRValue, yTempsub, polyline->at(i-1).z()));
					}

					if(ParentLine != NULL)
					{	
						ParentLine->push_back(QVector3D(m_fLValue, yTempsub, polyline->at(i-1).z()));
					}
				}
				SubLine->push_back(polyline->at(i));
			}
		}
	}


	if(NULL != ParentLine)				
	{
		DivVector->push_back(ParentLine);
	}


}

void CDivisionData::Division( QRenderable* res, const QRenderUnit* rsc, int iLineNum )
{
	for(int iLineIndex = 0; iLineIndex < iLineNum; iLineIndex++)
	{

	}
}


/************************************************************************/
/* 函数功能： 分割线							                        */
/* 参数说明:  NULL														*/
/* 返 回 值:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void  CDivisionData::DivisionLine(QVector<QVector3DArray*> *DivVector, QVector3DArray *polyline)
{
	QVector3DArray* ParentLine = NULL;
	QVector3DArray* SubLine = NULL;

	bool DivStart = false;				//记录是否需要分割
	bool HasParent = false;				//记录是经度为－77的值
	bool RStart	 = false;
	double x1 = 0 ; 
	double x2 = 0 ;
	double y1 = 0 ; 
	double y2 = 0 ;
	double yTempParent = 0 ;
	double yTempsub = 0 ;
	double xLast = 0 ; 
	double yLast = 0 ;

	if(polyline->at(0).x()>m_fRValue)
	{
		for(int i = 0 ; i< polyline->count(); i++)
		{
			if(polyline->at(i).x() >=m_fRValue)
			{
				if(ParentLine == NULL)
				{
					ParentLine = new QVector3DArray;
					HasParent = true; 
				}

				if(DivStart)
				{
					x2 = polyline->at(i).x();
					y2= polyline->at(i).y();
					if(i>=0)
					{
						xLast = polyline->at(i-1).x();
						yLast = polyline->at(i-1).y();
						yTempParent = polyline->at(i-1).y();
					}
					SubLine->push_back(QVector3D(m_fLValue, yTempParent, polyline->at(i-1).z()));		//将分割后的线闭合
					DivVector->push_back(SubLine);
					SubLine = NULL;
					DivStart = false;
					ParentLine->push_back(QVector3D(m_fRValue, yTempParent, polyline->at(i-1).z()));
				}

				ParentLine->push_back(polyline->at(i));
			}else if(polyline->at(i).x() <m_fRValue)
			{
				if(!DivStart)
				{
					SubLine = new QVector3DArray;
					DivStart = true;
					x1 = polyline->at(i).x();
					y1=  polyline->at(i).y();

					if(i>=0)
						yTempsub = polyline->at(i-1).y();
					else
						yTempsub = 0 ; 
					if(HasParent)
					{
						SubLine->push_back(QVector3D(m_fLValue, yTempsub, polyline->at(i-1).z()));
						ParentLine->push_back(QVector3D(m_fRValue, yTempsub,polyline->at(i-1).z()));
						DivVector->push_back(ParentLine); 
						HasParent = false;
						ParentLine = NULL;
					}
				}
				SubLine->push_back(polyline->at(i));
			}
			if(i == (polyline->count()-1))
			{
				if(ParentLine)
					DivVector->push_back(ParentLine);
				if(SubLine)
					DivVector->push_back(SubLine); 
				SubLine = NULL;
			}
		}
	}else
	{


		for(int i = 0 ; i< polyline->count(); i++)
		{
			if(polyline->at(i).x() <m_fRValue)
			{
				if(ParentLine == NULL)
				{
					ParentLine = new QVector3DArray;
					HasParent = true; 
				}

				if(DivStart)
				{
					x2 = polyline->at(i).x();
					y2= polyline->at(i).y();
					if(i >0) 
					{
						xLast = polyline->at(i-1).x();
						yTempParent = polyline->at(i-1).y();
					}else
					{
						xLast = 0 ; 
						yTempParent = 0 ;
					}
					float temp =  m_fRValue * (yTempParent- y2)/(xLast -x2) + (yTempParent * x2 - y2* xLast)/(x2- xLast);
					SubLine->push_back(QVector3D(m_fRValue, temp, polyline->at(i-1).z()));			//将分割后的线闭合
					DivVector->push_back(SubLine);							//将分割后线存于Vector
					SubLine = NULL;
					DivStart = false;
					ParentLine->push_back(QVector3D(m_fLValue, temp, polyline->at(i-1).z()));
				}

				ParentLine->push_back(polyline->at(i));
			}else if(polyline->at(i).x() >=m_fRValue)
			{
				if(!DivStart)
				{
					SubLine = new QVector3DArray;
					DivStart = true;
					x1 = polyline->at(i).x();
					y1=  polyline->at(i).y();
					if(i >0) 
					{
						xLast = polyline->at(i-1).x();
						yLast = polyline->at(i-1).y();
						yTempsub = m_fRValue * (yLast- y1)/(xLast -x1) + (yLast * x1 - y1* xLast)/(x1- xLast);
					}else
					{
						xLast = 0 ; 
						yTempsub = 0 ;
					}

					if(HasParent)
					{
						SubLine->push_back(QVector3D(m_fRValue, yTempsub, polyline->at(i-1).z()));
						ParentLine->push_back(QVector3D(m_fLValue, yTempsub, polyline->at(i-1).z()));
						DivVector->push_back(ParentLine); 
						HasParent = false;
						ParentLine = NULL;
					}

				}
				SubLine->push_back(polyline->at(i));
			}

			if(i == (polyline->count() -1))
			{
				if( ParentLine)				
				{
					DivVector->push_back(ParentLine);
				}
				if( SubLine)
					DivVector->push_back(SubLine); 
			}
			
		}
	}

}

/************************************************************************/
/* 函数功能： 用纬线分割多边形以及多线段     	                        */
/* 参数说明:  NULL														*/
/* 返 回 值:  NULL														*/
/* 作    者:  zhanglei														*/
/* 日    期:  2011 年 06 月 15 日										*/
/************************************************************************/
void CDivisionData::DivisionbyLat(QVector<QVector3DArray*> *DivVector, QVector3DArray *polyline,DIVISITON_TYPE divtype)
{
	QVector3DArray* ParentLine = NULL;
	QVector3DArray* SubLine = NULL;

	bool DivStart = false;				//记录是否需要分割
	bool HasParent = false;				//记录是经度为－77的值
	bool RStart	 = false;
	double x1 = 0 ; 
	double x2 = 0 ;
	double y1 = 0 ; 
	double y2 = 0 ;
	double xTempParent = 0 ;
	double xTempsub = 0 ;
	double xLast = 0 ; 
	double yLast = 0 ;	
	
	
	if(polyline->at(0).y()>0)
		{			
			for(int i = 0 ; i< polyline->count(); i++)
			{

				if(polyline->at(i).y() >=0)
				{
					if(ParentLine == NULL)
					{
						ParentLine = new QVector3DArray;
						HasParent = true; 
					}

					if(DivStart)
					{
						x1 = polyline->at(i).x();
						y1= polyline->at(i).y();

						float temp = x1;

						if(i>=0)
						{
							xLast = polyline->at(i-1).x();
							yLast = polyline->at(i-1).y();											

							if (yLast!= y1)
							{
								temp = (yLast *x1 - y1 *xLast) /(yLast - y1);
							}						
						}
						SubLine->push_back(QVector3D(temp, 0.0f, polyline->at(i-1).z()));	
						
						SubLine->push_back(SubLine->at(0));									

						if (divtype ==KEEP_SOUTH||divtype ==KEEP_ALL)
						{
							DivVector->push_back(SubLine);
						}

						DivStart = false;
						ParentLine->push_back(QVector3D(temp,0.0f, polyline->at(i-1).z()));
					}
					ParentLine->push_back(polyline->at(i));

				}else if(polyline->at(i).y() <0)
				{
					if(!DivStart)
					{
						SubLine = new QVector3DArray;
						DivStart = true;

						x1 = polyline->at(i).x();
						y1=  polyline->at(i).y();

						float temp = x1;

						if (i>0)
						{
							xLast = polyline->at(i-1).x();
							yLast = polyline->at(i-1).y();	

							if (yLast!=y1)
							{
								temp = (x1*yLast-y1*xLast)/(yLast-y1);
							}						
						}

						if(HasParent)
						{
							SubLine->push_back(QVector3D(temp, 0.0f, polyline->at(i-1).z()));
						}
						if(ParentLine != NULL)
						{
							ParentLine->push_back(QVector3D(temp, 0.0f,polyline->at(i-1).z()));
						}
					}
					SubLine->push_back(polyline->at(i));
				}			
			}

			if(NULL != ParentLine)
			{
				if (divtype==KEEP_NORTH||divtype ==KEEP_ALL)
				{
					DivVector->push_back(ParentLine); 			
				}
			}
		}else //polyline->at(0).y()<=0 
		{
			for(int i = 0 ; i< polyline->count(); i++)
			{
				if(polyline->at(i).y() <0)
				{
					if(ParentLine == NULL)
					{
						ParentLine = new QVector3DArray;
						HasParent = true; 
					}

					if(DivStart)
					{
						x2 = polyline->at(i).x();
						y2= polyline->at(i).y();

						float temp = x2;
						if(i >0) 
						{
							yLast = polyline->at(i-1).y();
							xLast = polyline->at(i-1).x();

							temp = (yLast*x2 - xLast*y2) /(yLast - y2);
						}

						SubLine->push_back(QVector3D(temp,0.0f, polyline->at(i-1).z()));
						
						SubLine->push_back(SubLine->at(0));	//将分割后的线闭合										

						if (divtype==KEEP_NORTH||divtype ==KEEP_ALL)
						{
							DivVector->push_back(SubLine);							//将分割后线存于Vector
						}

						DivStart = false;
						ParentLine->push_back(QVector3D(temp, 0.0f, polyline->at(i-1).z()));
					}

					ParentLine->push_back(polyline->at(i));

				}else if(polyline->at(i).y() >=0)
				{
					if(!DivStart)
					{
						SubLine = new QVector3DArray;
						DivStart = true;
						x2 = polyline->at(i).x();
						y2=  polyline->at(i).y();

						float temp = x2;

						if(i >0) 
						{
							xLast = polyline->at(i-1).x();
							yLast = polyline->at(i-1).y();						
							temp = (x2*yLast-y2*xLast)/(yLast-y2);
						}

						if(HasParent)
						{
							SubLine->push_back(QVector3D(temp, 0.0f, polyline->at(i-1).z()));
						}

						if(ParentLine != NULL)
						{	
							ParentLine->push_back(QVector3D(temp, 0.0f, polyline->at(i-1).z()));
						}
					}
					SubLine->push_back(polyline->at(i));
				}			
			}

			if(NULL != ParentLine)				
			{
				if (divtype==KEEP_SOUTH||divtype ==KEEP_ALL)
				{
					DivVector->push_back(ParentLine);			
				}			
			}
		}
}

void CDivisionData::DivisionLinebyLat(QVector<QVector3DArray*> *DivVector, QVector3DArray *polyline)
{
	QVector3DArray* NewLine = NULL;

	double x1 = 0 ; 
	double x2 = 0 ;
	double y1 = 0 ; 
	double y2 = 0 ;

	NewLine = new QVector3DArray;	

	if (polyline->count()>=2)
	{
		NewLine->push_back(polyline->at(0));

		for (int i = 1 ; i< polyline->count(); i++)
		{
			if (polyline->at(i).y()*polyline->at(i-1).y()<=0) //means cross the latitude
			{
				x1 = polyline->at(i).x();
				y1= polyline->at(i).y();
				x2 = polyline->at(i-1).x();
				y2= polyline->at(i-1).y();

				if (y1!=y2)
				{
					float tempX = (y2 *x1 - y1 *x2) /(y2 - y1); 
					NewLine->push_back(QVector3D(tempX,0,polyline->at(i-1).z()));
					DivVector->push_back(NewLine);

					//start a new line
					NewLine = NULL;
					NewLine = new QVector3DArray;
					NewLine->push_back(QVector3D(tempX,0,polyline->at(i-1).z()));
				}			
			}
			NewLine->push_back(polyline->at(i));
		}
		DivVector->push_back(NewLine);	
	}
	NewLine = NULL;

}
