#include "CPolygonAnalysis.h"
#include <QLineF>
#include "DrawOrder.h"
#include "CLineAnalysis.h"

#include <QMessageBox>

bool CPolygonAnalysis::TestPolygonIntersection(QVector3DArray* linestrip)
{
	if (linestrip->count()>2)
	{
		QPointF tempPoint;
		bool isintersect = false;
		for (int i=0;i<linestrip->count()-1;i++)
		{
			if(isintersect) break;
			QLineF line1 = QLineF(linestrip->at(i).x(),linestrip->at(i).y(),linestrip->at(i+1).x(),linestrip->at(i+1).y());
			for (int j=0;j<linestrip->count()-1;j++)
			{
				if (i<j-1)
				{
					QLineF line2  = QLineF(linestrip->at(j).x(),linestrip->at(j).y(),linestrip->at(j+1).x(),linestrip->at(j+1).y());
					if (QLineF::BoundedIntersection == line1.intersect(line2,&tempPoint))
					{
						isintersect = true;
						break;
					}	
				}
			}
		}
		return isintersect;
	}
	else
	{
		return false;
	}
}

/*
bool CPolygonAnalysis::TestLinestripIntersection(QVector3DArray* linestrip1,QVector3DArray* linestrip2)
{
	if(linestrip1->count()>2 && linestrip2->count()>2)
	{
		QPointF tempPoint;
		bool isintersect = false;
		for (int i=0;i<linestrip1->count()-1;i++)
		{
			if(isintersect) break;

			QLineF line1 = QLineF(linestrip1->at(i).x(),linestrip1->at(i).y(),linestrip1->at(i+1).x(),linestrip1->at(i+1).y());
			for (int j=0;j<linestrip2->count()-1;j++)
			{
				QLineF line2  = QLineF(linestrip2->at(j).x(),linestrip2->at(j).y(),linestrip2->at(j+1).x(),linestrip2->at(j+1).y());
				if (QLineF::BoundedIntersection == line1.intersect(line2,&tempPoint))
				{
					isintersect = true;
					break;
				}					
			}
		}
		return isintersect;
	}
	else
		return false;
}
*/

/************************************************************************/
/* 函数功能： 采用某条曲线对多边形进行裁剪                              */                                           
/* 参数说明:                      						                */
/* 返 回 值:  无                 										*/
/* 作    者:  zhanglei													*/
/* 日    期:  2011 年 04 月 25 日										*/
/************************************************************************/
bool CPolygonAnalysis::ModifyPolygon(QVector3DArray& inputdata,QVector3DArray* modifylines)
{
//	Outputdata.Clear();
	Cutlinestrip.Clear();
	linestrip1.Clear();
	linestrip2.Clear();

	QVector3DArray Outputdata;
	bool isClosed;

	if (inputdata.at(0)==inputdata.at(inputdata.count()-1))
	{
		isClosed = true;
	}
	else
	{
		isClosed = false;
	}
	QVector3DArray polygon1,polygon2;
	int nCountStrip1,nCountStrip2;
	if(!modifylines) return false;
	if(inputdata.count() < 3 || modifylines->count() <3) return false ;   

	int nCount1= 0;
	int nCount2 = modifylines->count()-2;
	int nCountBegin=0;
	int nCountEnd = inputdata.count();	
	
	bool b_foundpos1 = false ; //是否找到交叉点
	bool b_foundpos2 = false;
	
	QPointF tempPoint,tempPoint1,tempPoint2;
	QVector<int> nCountArray; //collect index of intersection point of modify line
	QVector<int> nCount2Array; //collect index of intersection point of inputdata
	QVector<QPointF> tempPointArray;
	
	//STEP 1: Search for intersection points	
	for (int i=0;i<modifylines->count()-1;i++)
	{
		QLineF line1 = QLineF(modifylines->at(i).x(),modifylines->at(i).y(),modifylines->at(i+1).x(),modifylines->at(i+1).y());
			for (int j=0;j<inputdata.count()-1;j++)
			{
				if (QLineF::BoundedIntersection == line1.intersect(QLineF(inputdata.at(j).x(),inputdata.at(j).y(),inputdata.at(j+1).x(),inputdata.at(j+1).y()),&tempPoint))
				{
					nCountArray.push_back(i);
					nCount2Array.push_back(j);
					tempPointArray.push_back(tempPoint);

					if (nCountArray.size()>1 && i == nCountArray.at(nCountArray.size()-2) )
					{
						nCountArray.pop_back();
						nCount2Array.pop_back();
						tempPointArray.pop_back();
					}
				}				
			}
	}

   	if (nCountArray.count()>1)
	{
		//如果有两个或以上的交点，计算修改线相交点的索引
		int rangeMax = nCountArray.at(1)-nCountArray.at(0);
		int index = 0;
		for (int i=1;i<nCountArray.count();i++)
		{			
			if ((nCountArray.at(i)-nCountArray.at(i-1))>rangeMax)
			{
				rangeMax = nCountArray.at(i)-nCountArray.at(i-1);
				index = i-1;
			}
		}
		b_foundpos1 = true;
		b_foundpos2 = true;
		nCount1 = nCountArray.at(index);
		nCount2 = nCountArray.at(index+1);
		nCountBegin = nCount2Array.at(index);
		nCountEnd = nCount2Array.at(index+1);
		tempPoint1 = tempPointArray.at(index);      // intersection point
		tempPoint2 = tempPointArray.at(index+1);	// intersection point
	}
	else if(nCountArray.count()==1)
	{
		//如果只有一个交点，根据索引位置，判断该交点是头交点还是尾交点
		nCount1 = nCount2 = nCountArray.at(0);
		if ((2*nCount1)>(modifylines->count()-2)) 
		{
			b_foundpos1 = false;
			b_foundpos2 = true;
			nCountEnd = nCount2Array.at(0);		
			tempPoint2 = tempPointArray.at(0);
			nCount1 = 0;			
		}
		else if ((2*nCount1)<=(modifylines->count()-2)) 
		{
			b_foundpos1 = true;
			b_foundpos2 = false;
			nCountBegin = nCount2Array.at(0);
			tempPoint1 = tempPointArray.at(0);
			nCount2 = modifylines->count()-2;			
		}
	}	

	// STEP 1.1 input points to cut lines
	for (int i=nCount1+1;i<nCount2+1;i++)
	{
		Cutlinestrip.push_back(modifylines->at(i));
	}	
// 
	QVector3D pos1 = modifylines->at(nCount1);
	QVector3D pos2 = modifylines->at(nCount1+1);
	QVector3D pos3 = modifylines->at(nCount2);
	QVector3D pos4 = modifylines->at(nCount2+1);
	float zvalue = modifylines->at(0).z();

	//Get intersection point for unbounded intersection point
	if (!b_foundpos1) //如果没有BoundedIntersection的点，则取距离最近的点做相交运算，取交点
	{
		float dis1;
		float dis1Min = pos1.distanceToLine(inputdata.at(0),QVector3D(0.0f,0.0f,0.0f));
		QLineF linebegin = QLineF(pos1.x(),pos1.y(),pos2.x(),pos2.y());
		for (int i=0;i<inputdata.count()-1;i++)
		{
			dis1 = pos1.distanceToLine(inputdata.at(i),QVector3D(0.0f,0.0f,0.0f)); //计算距离修改线段Begin点最近的点
			if (dis1<dis1Min) 
			{
				dis1Min =dis1;
				nCountBegin = i;
			}
		}
		if (pos1.distanceToLine(inputdata.at(nCountBegin+1),QVector3D(0.0f,0.0f,0.0f))>pos1.distanceToLine(inputdata.at(nCountBegin-1),QVector3D(0.0f,0.0f,0.0f)))
		{
			nCountBegin--;
		}
		//取重心点
		//  		float xx = (pos1.x()+ inputdata.at(nCountBegin).x() + inputdata.at(nCountBegin+1).x())/3.0f;
		//  		float yy = (pos1.y()+ inputdata.at(nCountBegin).y() + inputdata.at(nCountBegin+1).y())/3.0f;
		//  		tempPoint1 = QPointF(xx,yy);
		linebegin.intersect(QLineF(inputdata.at(nCountBegin).x(),inputdata.at(nCountBegin).y(),inputdata.at(nCountBegin+1).x(),inputdata.at(nCountBegin+1).y()),&tempPoint1);
	}
	if (!b_foundpos2)
	{
		float dis2;	
		float dis2Min = pos4.distanceToLine(inputdata.at(0),QVector3D(0.0f,0.0f,0.0f));
		QLineF lineend = QLineF(pos3.x(),pos3.y(),pos4.x(),pos4.y());	
		for (int i=0;i<inputdata.count()-1;i++)
		{
			dis2 = pos4.distanceToLine(inputdata.at(i),QVector3D(0.0f,0.0f,0.0f)); //计算距离修改线段Begin点最近的点
			if (dis2<dis2Min) 
			{
				dis2Min =dis2;
				nCountEnd = i;
			}
		}
		if (pos4.distanceToLine(inputdata.at(nCountEnd+1),QVector3D(0.0f,0.0f,0.0f))>pos4.distanceToLine(inputdata.at(nCountEnd-1),QVector3D(0.0f,0.0f,0.0f)))
		{
			nCountEnd--;
		}
		//取重心点
		// 		float xx = (pos4.x()+ inputdata.at(nCountEnd).x() + inputdata.at(nCountEnd+1).x())/3.0f;
		// 		float yy = (pos4.y()+ inputdata.at(nCountEnd).y() + inputdata.at(nCountEnd+1).y())/3.0f;
		// 		tempPoint2 = QPointF(xx,yy);
		lineend.intersect(QLineF(inputdata.at(nCountEnd).x(),inputdata.at(nCountEnd).y(),inputdata.at(nCountEnd+1).x(),inputdata.at(nCountEnd+1).y()),&tempPoint2);
	}

	//STEP 2: Input points to stripline1 and stripline2 

	m_postemp1 = QVector3D(tempPoint1.x(),tempPoint1.y(),zvalue);
	m_postemp2 = QVector3D(tempPoint2.x(),tempPoint2.y(),zvalue);
	
	if (nCountBegin>nCountEnd)
	{
		if (isClosed)
		{
			//linestrip1
			linestrip1.push_back(m_postemp2);		
			for (int i=nCountEnd+1;i<nCountBegin;i++)
			{
				linestrip1.push_back(inputdata.at(i));
			}
			linestrip1.push_back(m_postemp1);

			//linestrip2
			linestrip2.push_back(m_postemp2);
			for (int i=nCountEnd;i!=nCountBegin;)
			{
				linestrip2.push_back(inputdata.at(i));			
				if (i>0) i--;	
				else if(i==0) i=inputdata.count()-1;        
			}
			//	linestrip2.push_back(inputdata.at(nCountEnd));	
			linestrip2.push_back(m_postemp1);
		}	
		else
		{
			for (int i=inputdata.count()-1;i>nCountBegin;i--)
			{
				Outputdata.push_back(inputdata.at(i));
			}
			Outputdata.push_back(m_postemp1);
			for (int i=nCount1+1;i<=nCount2;i++)
			{
				Outputdata.push_back(modifylines->at(i));
			}
			Outputdata.push_back(m_postemp2);
			for (int i=nCountEnd;i>=0;i--)
			{
				Outputdata.push_back(inputdata.at(i));
			}
		}
	}

	if (nCountBegin<=nCountEnd)
	{
		if (isClosed)
		{
			//linestrip1
			linestrip1.push_back(m_postemp2);		
			for (int i=nCountEnd;i>nCountBegin;i--)
			{
				linestrip1.push_back(inputdata.at(i));
			}
			linestrip1.push_back(m_postemp1);

			//linestrip2
			linestrip2.push_back(m_postemp2);
			for (int i=nCountEnd;i!=nCountBegin;)
			{
				linestrip2.push_back(inputdata.at(i));			
				if (i<inputdata.count()-1) i++;	
				else if(i==inputdata.count()-1) i= 0;        
			}
			linestrip2.push_back(inputdata.at(nCountBegin));	
			linestrip2.push_back(m_postemp1);
		}		
		else
		{
			for (int i=0;i<=nCountBegin;i++)
			{
				Outputdata.push_back(inputdata.at(i));
			}
			Outputdata.push_back(m_postemp1);
			for (int i=nCount1+1;i<=nCount2;i++)
			{
				Outputdata.push_back(modifylines->at(i));
			}
			Outputdata.push_back(m_postemp2);
			for (int i=nCountEnd+1;i<inputdata.count();i++)
			{
				Outputdata.push_back(inputdata.at(i));
			}
		}
	}

	if (linestrip1.count()<4 || linestrip2.count()<4)
	{
		return false;
	}

	//STEP 3 : Merge strip line and cut line to polygon
	if (isClosed)
	{
		MergeStripline(true,linestrip1,Cutlinestrip,polygon1);
		MergeStripline(true,linestrip2,Cutlinestrip,polygon2);
			    
		CDrawOrder  *bDrawOrder = new CDrawOrder();
		bool bIsConvert1 = bDrawOrder->isConvert(&polygon1,true);
		bool bIsConvert2= bDrawOrder->isConvert(&polygon2,true);

		if (bIsConvert1 && !bIsConvert2) 
		{
			Outputdata = polygon2;
		}
		else if (!bIsConvert1 && bIsConvert2) 
		{
			Outputdata = polygon1;
		}
		else ////都为逆时针或都为顺时针
		{
			int ReturnVaule = PickupStripline(linestrip1,linestrip2,Cutlinestrip);
			if ( ReturnVaule == 1)
			{
				Outputdata = polygon1;
			}
			else if ( ReturnVaule == 2)
			{
				Outputdata = polygon2;
			}
			else
			{
				return false; 
			}
		}
		//if (bIsConvert1 && bIsConvert2) //都为逆时针
		//{
		//	Outputdata = (linestrip1.count()<linestrip2.count())? polygon1:polygon2;
		//}
		//if (!bIsConvert1 && !bIsConvert2) //都为顺时针
		//{
		//	Outputdata = (linestrip1.count()>linestrip2.count())? polygon1:polygon2;
		//}		
		delete bDrawOrder;
		bDrawOrder = NULL;
	}	
    /*TEST CODE 
	m_linestrip.Clear();
	m_linestrip.push_back(m_postemp1);
	for (int i=nCount1+1;i<=nCount2;i++)
	{
		m_linestrip.push_back(modifylines->at(i)+ QVector3D(0.0f,0.0f,0.00001f));
	}
	m_linestrip.push_back(m_postemp2);
	**/

	//STEP 4: Test for intersection
	CLineAnalysis m_lineanalysis;
	if (m_lineanalysis.isPolygonintersect(Outputdata))
	{
		return false;
	}
	else
	{
		inputdata = Outputdata;	
		return true;
	}
	
}

bool CPolygonAnalysis::EditPolygon(QVector3DArray *pData,QVector3DArray *pPoints,int nPosStart,int nPosEnd )
{
	//编辑顶点
	QVector3DArray *pTempArray;
		//*pTempOriginalArray;
	pTempArray = new QVector3DArray;
	//pTempOriginalArray = new QVector3DArray;
	CDrawOrder  *bDrawOrder = new CDrawOrder();
	bool bIsConvert = bDrawOrder->isConvert(pData,true);
	int i;

	//如果等高线是逆时针绘制
	if(bIsConvert == true) 
	{
		if(nPosStart < nPosEnd  )//同方向
		{   
			bool bModify = bDrawOrder->isConvert(pPoints,false);
			if(bModify == bIsConvert)  
			{
				for(i=pData->size()-1; i>nPosStart; i--)	
				{
					if(i >= nPosEnd)
					{
						pTempArray->push_back(pData->at(i));//逆序存到临时数组中
					//	pTempOriginalArray->push_back(pOriginalData->at(i));
					}
					pData->pop_back();
				//	pOriginalData->pop_back();
				}
				pData->pop_back();
				//pOriginalData->pop_back();

				//加入新编辑的点
				for(i=0; i<pPoints->size(); i++)
				{
					QVector3D temp = pPoints->at(i);
					pData->push_back(temp);		
				//	pOriginalData->push_back(TransGLtoWGS84(temp));
				}

				//加入原等值线后半部分的点
				for(i=pTempArray->size()-2; i>=0; i--)
				{   
				//	QVector3D temp =pTempOriginalArray->at(i);
					pData->push_back(pTempArray->at(i));
					//	pOriginalData->push_back(TransGLtoWGS84(temp));
			//		pOriginalData->push_back(temp);
				}
			} 
			else
			{
				//弹出从0点到nposEnd的点
				for(i = pData->count()-1 ; i>nPosEnd; i--)
				{
					pData->pop_back();
				//	pOriginalData->pop_back();
				}

				//弹出从nposstart之后的点
				for(i = 0 ; i <nPosStart; i++)
				{
					pData->pop_front();
				//	pOriginalData->pop_front();
				}

				//重新生成线
				for(i=0; i< pPoints->count()-1; i++)
				{
					pData ->push_front(pPoints->at(i));
			//		QVector3D temp  = pPoints->at(i);
				//	pOriginalData->push_front(TransGLtoWGS84(temp));
				}
			}
		}
		else if (nPosStart > nPosEnd)    //方向相反
		{
			bool bModify = bDrawOrder->isConvert(pPoints,false);

			if(bModify == bIsConvert)     //开始点在结束点前面
			{
				//弹出从0点到nposEnd的点
				for(i = 0 ; i< nPosEnd; i++)
				{
					pData->pop_front();
			//		pOriginalData->pop_front();
				}

				//弹出从nposstart之后的点
				for(i = pData->count()-1 ; i >(nPosStart - nPosEnd); i--)
				{
					pData->pop_back();
				//	pOriginalData->pop_back();
				}

				//重新生成线
				for(i = 0; i< pPoints->count(); i++)
				{
					pData ->push_back(pPoints->at(i));
					QVector3D temp  = pPoints->at(i);
			//		pOriginalData->push_back(TransGLtoWGS84(temp));
				}
			}
			else
			{
				for(i=pData->size()-1; i>nPosEnd; i--)	
				{
					if(i >= nPosStart)
					{
						pTempArray->push_back(pData->at(i));//逆序存到临时数组中
					//	pTempOriginalArray->push_back(pOriginalData->at(i));
					}
					pData->pop_back();
			//		pOriginalData->pop_back();
				}
				pData->pop_back();
		//		pOriginalData->pop_back();

				//加入新编辑的点
				for(i=pPoints->size()-1; i>0; i--)
				{
					QVector3D temp = pPoints->at(i);
					pData->push_back(temp);		
				//	pOriginalData->push_back(TransGLtoWGS84(temp));
				}

				//加入原等值线后半部分的点
				for(i=pTempArray->size()-2; i>=0; i--)
				{
					pData->push_back(pTempArray->at(i));
			//		QVector3D temp = pTempOriginalArray->at(i);
					//	pOriginalData->push_back(TransGLtoWGS84(temp));
				//	pOriginalData->push_back(temp);
				}
			}
		}
	}
	else if(bIsConvert == false)  //如果等值线是顺时针绘制
	{
		if(nPosStart < nPosEnd  )//同方向
		{   
			bool bModify = bDrawOrder->isConvert(pPoints,false);

			if(bModify == bIsConvert)  
			{
				for(i=pData->size()-1; i>nPosStart; i--)	
				{
					if(i >= nPosEnd)
					{
						pTempArray->push_back(pData->at(i));//逆序存到临时数组中
				//		pTempOriginalArray->push_back(pOriginalData->at(i));
					}
					pData->pop_back();
				//	pOriginalData->pop_back();
				}
				pData->pop_back();
		//		pOriginalData->pop_back();

				//加入新编辑的点
				for(i=0; i<pPoints->size(); i++)
				{
					QVector3D temp = pPoints->at(i);
					pData->push_back(temp);		
			//		pOriginalData->push_back(TransGLtoWGS84(temp));
				}

				//加入原等值线后半部分的点
				for(i=pTempArray->size()-2; i>=0; i--)
				{
			//		QVector3D temp = pTempOriginalArray->at(i);
					pData->push_back(pTempArray->at(i));
					//	pOriginalData->push_back(TransGLtoWGS84(temp));
		//			pOriginalData->push_back(temp);
				}
			}
			else
			{
				//弹出从0点到nposEnd的点
				for(i = pData->count()-1 ; i>nPosEnd; i--)
				{
					pData->pop_back();
			//		pOriginalData->pop_back();
				}

				//弹出从nposstart之后的点
				for(i = 0 ; i <nPosStart; i++)
				{
					pData->pop_front();
				//	pOriginalData->pop_front();
				}

				//重新生成线
				for(i=0; i< pPoints->count()-1; i++)
				{
					pData ->push_front(pPoints->at(i));
					QVector3D temp  = pPoints->at(i);
		//			pOriginalData->push_front(TransGLtoWGS84(temp));
				}
			}
		}
		else if (nPosStart > nPosEnd)    //方向相反
		{
			bool bModify = bDrawOrder->isConvert(pPoints,false);

			if(bModify == bIsConvert)     //开始点在结束点前面
			{
				//弹出从0点到nposEnd的点
				for(i = 0 ; i< nPosEnd; i++)
				{
					pData->pop_front();
			//		pOriginalData->pop_front();
				}

				//弹出从nposstart之后的点
				for(i = pData->count()-1 ; i >(nPosStart - nPosEnd); i--)
				{
					pData->pop_back();
				//	pOriginalData->pop_back();
				}

				//重新生成线
				for(i = 0; i< pPoints->count(); i++)
				{
					pData ->push_back(pPoints->at(i));
					QVector3D temp  = pPoints->at(i);
				//	pOriginalData->push_back(TransGLtoWGS84(temp));
				}
			}
			else
			{
				for(i=pData->size()-1; i>nPosEnd; i--)	
				{
					if(i >= nPosStart)
					{
						pTempArray->push_back(pData->at(i));//逆序存到临时数组中
				//		pTempOriginalArray->push_back(pOriginalData->at(i));
					}
					pData->pop_back();
			//		pOriginalData->pop_back();
				}
				pData->pop_back();
			//	pOriginalData->pop_back();

				//加入新编辑的点
				for(i=pPoints->size()-1; i>0; i--)
				{
					QVector3D temp = pPoints->at(i);
					pData->push_back(temp);		
			//		pOriginalData->push_back(TransGLtoWGS84(temp));
				}

				//加入原等值线后半部分的点
				for(i=pTempArray->size()-2; i>=0; i--)
				{
					pData->push_back(pTempArray->at(i));
				//	QVector3D temp = pTempOriginalArray->at(i);
					//	pOriginalData->push_back(TransGLtoWGS84(temp));
				//	pOriginalData->push_back(temp);
				}
			}
		}
	}	

	return true;
}

/************************************************************************/
/* 函数功能： 挑选需要跟那根曲线线条进行合并                            */ 
/* 参数说明:                      						                */
/* 返 回 值:  无                 										*/
/* 作    者:  zhanglei													*/
/* 日    期:  2011 年 04 月 25 日										*/
/************************************************************************/
int CPolygonAnalysis::PickupStripline(QVector3DArray& stripline1,QVector3DArray& stripline2,QVector3DArray& modifyline) 
{
	if(stripline1.count()<3 || stripline2.count()<3 || modifyline.count()<3) return 0;
	if(stripline1.at(0)!=stripline2.at(0))  return 0;
	if(stripline1.at(stripline1.count()-1)!=stripline2.at(stripline2.count()-1)) return 0;

	QVector3D posbegin = stripline1.at(stripline1.count()-1);
	QVector3D posEnd =  stripline1.at(0);
	QVector3D  vectorbase = posEnd - posbegin ;
	vectorbase.normalize();

	QVector3D vector1,vector1max,vector2,vector2max,vector3,vector3max,vector13,vector23;
	vector1max = QVector3D(0.0f,0.0f,0.0f);
	vector2max = QVector3D(0.0f,0.0f,0.0f);
	vector3max = QVector3D(0.0f,0.0f,0.0f);
	//calculate the max vector in stripline1
	for (int i=1;i<stripline1.count()-1;i++)
	{
		vector1 = QVector3D(stripline1.at(i)-posbegin);
		vector1.normalize();		
		vector1 = vector1-vectorbase;

		if (vector1.length()>vector1max.length())
		{
			vector1max = vector1;
		}
	}	
	//calculate the max vector in stripline2	
	for (int i=1;i<stripline2.count()-1;i++)
	{
		vector2 = QVector3D(stripline2.at(i)-posbegin);
		vector2.normalize();		
		vector2 = vector2-vectorbase;

		if (vector2.length()>vector2max.length())
		{
			vector2max = vector2;
		}
	}
	//calculate the max vector in modifyline
	for (int i=0;i<modifyline.count();i++)
	{
		vector3 = QVector3D(modifyline.at(i)-posbegin);
		vector3.normalize();		
		vector3 = vector3-vectorbase;

		if (vector3.length()>vector3max.length())
		{
			vector3max = vector3;
		}
	}
	vector13 = vector3max - vector1max;
	vector23 = vector3max - vector2max;
	if (vector23.length()>vector13.length())
	{
		return 2; //choose line2
	}
	else
	{
		return 1; // choose line1
	}
	//if(vector3max)
}

/************************************************************************/
/* 函数功能： 合并两条曲线段，可选择是否需要平滑                        */ 
/* 参数说明:                      						                */
/* 返 回 值:  无                 										*/
/* 作    者:  zhanglei													*/
/* 日    期:  2011 年 04 月 25 日										*/
/************************************************************************/
void CPolygonAnalysis::MergeStripline(bool smooth,QVector3DArray& stripline1,QVector3DArray& stripline2,QVector3DArray& outputline)
{
	outputline.Clear();
	for (int i=0;i<stripline1.count();i++)
	{
		outputline.push_back(stripline1.at(i));
	}
	for (int i=0;i<stripline2.count();i++)
	{
		outputline.push_back(stripline2.at(i));
	}
// 	int midindex = stripline2.count()/2;
// 	//QVector3D point1,point2;
// 	int num = 4;
// 
// 	for (int i=midindex;i<stripline2.count();i++)
// 	{
// 		outputline.push_back(stripline2.at(i));
// 	}
// 	for (int i=0;i<stripline1.count();i++)
// 	{
// 		outputline.push_back(stripline1.at(i));
// 	}
// 	for (int i=0;i<midindex;i++)
// 	{
// 		outputline.push_back(stripline2.at(i));
// 	}
// 
// 	point1 = stripline1.at(0);
// 	point2 = stripline1.back();
	
	if (smooth)
	{
		//SmoothPolygon(4,outputline);
	//	SmoothPolygon(outputline,stripline1.count());
		SmoothPolygon(outputline,0);
	}
}


/************************************************************************/
/* 函数功能： 平滑多边形某索引处（index）的曲线，采用相邻点求平均       */ 
/* 参数说明:                      						                */
/* 返 回 值:  无                 										*/
/* 作    者:  zhanglei													*/
/* 日    期:  2011 年 04 月 25 日										*/
/************************************************************************/
void CPolygonAnalysis::PolygonSmooth(QVector3DArray& polygon,int index)
{
	if (polygon.count()<3 || index<0 || index>polygon.count()-1) return;
	
	int nCount = polygon.count();
//	QVector3D pointtemp = polygon.at(index);
//	QVector3D pointtemp2 = (polygon.at(Recapindex(index-1,nCount))+polygon.at(Recapindex(index+1,nCount)))/2;
//	QVector3D pointtemp3 = (pointtemp+ pointtemp2)/2;
//	polygon.replace(index,pointtemp3);
}

/************************************************************************/
/* 函数功能： 采用平滑模板，对多边形某索引处（index）的点进行平滑       */ 
/* 参数说明:                      						                */
/* 返 回 值:  无                 										*/
/* 作    者:  zhanglei													*/
/* 日    期:  2011 年 04 月 25 日										*/
/************************************************************************/
void CPolygonAnalysis::PolygonSmooth(QVector3DArray& polygon,int index,int radius)
{
	if (polygon.count()<3 || index<0 || index>polygon.count()-1 || radius>polygon.count()/2-1) return;

	int nCount = polygon.count();
	QVector3D PointTemp;
	QVector<float> weightArray;
	float weight;
	QVector3DArray tempdata;
	float sum = radius*radius;
	for (int i=0;i<radius*2+1;i++)
	{
        weight = (radius-abs(i-radius))/sum;
		weightArray.push_back(weight);
	}
	 
 	for (int i=-radius;i<=radius;i++)
 	{
 		tempdata.push_back(polygon.at(Recapindex(i+index,nCount)));
 	}
	
	for (int i=-radius;i<=radius;i++)
	{
		PointTemp.setX(0.0f);
		PointTemp.setY(0.0f);
		for (int j=-radius;j<=radius;j++)
		{
			PointTemp += polygon.at(Recapindex(j+index,nCount))*weightArray.at(j+radius);
		//	PointTemp += tempdata.at(j);
		}
	//	PointTemp =  PointTemp/(2*radius+1);
	//	polygon.replace(Recapindex(i+index,nCount),PointTemp);
		tempdata.replace(i+radius,PointTemp);
	}

 	for (int i=-radius;i<=radius;i++)
 	{
 		polygon.replace(Recapindex(i+index,nCount),tempdata.at(i+radius));
 	}
 	
	tempdata.Clear();
	//QVector3D pointtemp = polygon.at(index);
	//QVector3D pointtemp2 = (polygon.at(Recapindex(index-1,nCount))+polygon.at(Recapindex(index+1,nCount)))/2;
	//QVector3D pointtemp3 = (pointtemp+ pointtemp2)/2;
	//polygon.replace(index,pointtemp3);
}

/************************************************************************/
/* 函数功能： 平滑多边形某索引处（index）的曲线，采用曲线拟合方法       */ 
/* 参数说明:                      						                */
/* 返 回 值:  无                 										*/
/* 作    者:  zhanglei													*/
/* 日    期:  2011 年 04 月 25 日										*/
/************************************************************************/
void CPolygonAnalysis::SmoothPolygon(QVector3DArray& polygon,int index)
{
	//Get input data, control points
	QVector3DArray inputdata,outputdata;
	int ibegin,iend;
	GetRangeSection(polygon,0,ibegin,iend);
	QVector3D pos1,pos2;

	//算出来的间隔偏小，拟合出来的效果不明显
	//ibegin -= 10;
	//iend += 10;
	
	//计算另外两个控制点，对删除的曲线段的点去平均
	for (int i=ibegin+1;i<polygon.count();i++)
	{
		pos1 +=polygon.at(i);
	}
	for (int i=1;i<iend+1;i++)
	{
		pos2 +=polygon.at(i);
	}
	pos1 = pos1/(polygon.count()-ibegin-1);
	pos2 = pos2/(iend);

	//输入控制点
	inputdata.push_back(polygon.at(ibegin));
	//inputdata.push_back(polygon.at(ibegin+1));
	//inputdata.push_back(polygon.at(iend));
	inputdata.push_back(pos1);
	inputdata.push_back(pos2);
	inputdata.push_back(polygon.at(iend+1));
//	CutStripinPolygon(polygon,inputdata,ibegin,iend+1);

	//Get smooth strip with the inputdata
	bool isSuccess = GetSmoothStrip(inputdata,outputdata);
	//replace smooth strip in polygon
	if (isSuccess)
	{
		m_smoothangle = outputdata;
		ReplaceStrip(polygon,ibegin,iend+1,outputdata);
	}	
}



/************************************************************************/
/* 函数功能： 计算多边形某索引（index）两侧相邻线段斜率相差最小的两点的 */ 
/*             索引                                                     */
/* 参数说明:                      						                */
/* 返 回 值:  无                 										*/
/* 作    者:  zhanglei													*/
/* 日    期:  2011 年 04 月 25 日										*/
/************************************************************************/
void CPolygonAnalysis::GetRangeSection(QVector3DArray& polygon,int index,int& indexbegin,int& indexend)
{
	if(polygon.count()<3 || index<0 || index>=polygon.count()) return;
	
	QVector3D vector1,vector2,dvector;
	float minlength = 2.0f;
	int nCount = polygon.count();
	indexbegin = indexend = index;

	if(polygon.at(indexbegin)!=polygon.at(Recapindex(indexbegin-1,nCount)) && polygon.at(indexend)!=polygon.at(Recapindex(indexend+1,nCount)))
	{
		dvector = QVector3D(0.0f,0.0f,0.0f);
		for(;;indexbegin--,indexend++)
		{			
			if(indexbegin==-1) indexbegin = polygon.count()-2;
			if(indexend ==polygon.count()-1) indexend = 1;
			vector1 = polygon.at(indexbegin)-polygon.at(Recapindex(indexbegin-1,nCount));
			vector2 = polygon.at(Recapindex(indexend+1,nCount))-polygon.at(indexend);
			dvector = vector2.normalized() - vector1.normalized();			

			if(minlength>=dvector.length()) 
				minlength = dvector.length();
			else 
				break;
		}		
	}
}
/************************************************************************/
/* 函数功能： 根据给定的点inputdata,输出平滑的点output       	*/
/* 参数说明:                                  						    */
/* 返 回 值:  返回TRUE 表示成功得出平滑曲线，没有内部线段相交                 										*/
/* 作    者:  zhanglei													*/
/* 日    期:  2011 年 04 月 25 日										*/
/************************************************************************/
bool CPolygonAnalysis::GetSmoothStrip(QVector3DArray& inputdata,QVector3DArray& outputdata)
{
	if (inputdata.count()<3) return false;
	
	m_splineline.GetSmoothPoints(inputdata,outputdata);
	
	bool isintersect = m_lineanalysis.isPolygonintersect(outputdata);

	return !isintersect;
}

void CPolygonAnalysis::ReplaceStrip(QVector3DArray& polygon,int indexbegin,int indexend,QVector3DArray& insertStrip)
{
	if(polygon.count()<=indexbegin || polygon.count()<=indexend) return;
	if(indexbegin<0 || indexend<0 ) return;
	if(insertStrip.count()<2) return;
	
	QVector3DArray outputdata;
	if (indexbegin<indexend && indexbegin>0)
	{
		for (int i=0;i<indexbegin;i++)
		{
			outputdata.push_back(polygon.at(i));
		}	
		for (int i=0;i<insertStrip.count();i++)
		{
			outputdata.push_back(insertStrip.at(i));
		}
		for (int i= indexend+1;i<polygon.count();i++)
		{
			outputdata.push_back(polygon.at(i));
		}			
	}
	else if (indexbegin>indexend)
	{
		for (int i=indexend+1;i<indexbegin;i++)
		{
			outputdata.push_back(polygon.at(i));
		}
		for(int i=0;i<insertStrip.count();i++)
		{
			outputdata.push_back(insertStrip.at(i));
		}
	}

	polygon.Clear();
	polygon = outputdata;
}

/*
Line* CPolygonAnalysis::Drawlinestrip1()
{
	QVector4D m_color2 = QVector4D(1.0f,1.0f,0.0f,1.0f);	
	//	QVector4D m_color2 = polygonanalysis.m_color2;
	int ncount2 = linestrip1.count();
	if (ncount2>1)
	{
		Line* line2 = new Line;
		for (int i=0;i<ncount2;i++)
		{
			line2->getVectexArray()->push_back(linestrip1.at(i));
			line2->getColorArray()->push_back(m_color2);
		}
		line2->SetColorArray(true);
		//	line2->SetLineStyle(3.0f,1,0xFFFF);
		//	m_ParentLayer->GetLayerData()->InsertRenderable(line1);
	//	m_ParentLayer->GetLayerData()->InsertRenderable(line2);
		return line2;
	}	
	else 
		return NULL;

}

Line* CPolygonAnalysis::Drawlinestrip2()
{
	QVector4D m_color3 = QVector4D(0.0f,1.0f,0.0f,1.0f);	
	//	QVector4D m_color2 = polygonanalysis.m_color2;
	int ncount3 = linestrip2.count();
	if (ncount3>1)
	{
		Line* line3 = new Line;
		for (int i=0;i<ncount3;i++)
		{
			line3->getVectexArray()->push_back(linestrip2.at(i));
			line3->getColorArray()->push_back(m_color3);
		}
		line3->SetColorArray(true);
		//	line2->SetLineStyle(3.0f,1,0xFFFF);
		//	m_ParentLayer->GetLayerData()->InsertRenderable(line1);
	//	m_ParentLayer->GetLayerData()->InsertRenderable(line3);
		return line3;
	}	
	else
		return NULL;
}
*/
/*
Line* CPolygonAnalysis::DrawSmooth()
{
	QVector4D m_color1 = QVector4D(1.0f,1.0f,0.0f,1.0f);	
	//	QVector4D m_color2 = polygonanalysis.m_color2;
	int ncount1 = m_smoothangle.count();
	if (ncount1>1)
	{
		Line* line1 = new Line;
		for (int i=0;i<ncount1;i++)
		{
			line1->getVectexArray()->push_back(m_smoothangle.at(i)+QVector3D(1000.0f,1000.0f,0.00001f));
			line1->getColorArray()->push_back(m_color1);
		}
		line1->SetColorArray(true);
		//line1->SetLineStyle(3.0f,1,0xFFFF);
		//	line2->SetLineStyle(3.0f,1,0xFFFF);
		//	m_ParentLayer->GetLayerData()->InsertRenderable(line1);
		//	m_ParentLayer->GetLayerData()->InsertRenderable(line1);
		return line1;
	}	
	else
		return NULL;
}
*/
/*
Line* CPolygonAnalysis::DrawCutline()
{
	QVector4D m_color1 = QVector4D(1.0f,0.0f,1.0f,1.0f);	
	//	QVector4D m_color2 = polygonanalysis.m_color2;
	int ncount1 = Cutlinestrip.count();
	if (ncount1>1)
	{
		Line* line1 = new Line;
		for (int i=0;i<ncount1;i++)
		{
			line1->getVectexArray()->push_back(Cutlinestrip.at(i));
			line1->getColorArray()->push_back(m_color1);
		}
		line1->SetColorArray(true);
		//	line2->SetLineStyle(3.0f,1,0xFFFF);
		//	m_ParentLayer->GetLayerData()->InsertRenderable(line1);
	//	m_ParentLayer->GetLayerData()->InsertRenderable(line1);
		return line1;
	}	
	else
		return NULL;
}
*/