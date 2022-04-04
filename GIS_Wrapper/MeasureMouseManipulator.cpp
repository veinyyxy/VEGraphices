/*============================================================ 
文件名：MeasureMouseManipulator.cpp 
类 名： MeasureMouseManipulator
父 类： ManualDraw
子 类： 无 
功能说明： 量测操纵器

-----------------------------版本更新-------------------------
V 0.1 
原作者 ：任雪
完成日期：2011年5月10日
============================================================*/

#include "MeasureMouseManipulator.h"
#include <QtCore/QSettings>
#include <QtCore/QTextCodec>
#include "ILayerAttribute.h"

//int DrawContourLine::m_nCount = 0;

MeasureMouseManipulator::MeasureMouseManipulator(void)
{
	pline = new Line;
	pArray = NULL;

	m_bFinished = false;
	m_bFirst = true;
	pOriginalArray = new QVector3DArray;
}

MeasureMouseManipulator::MeasureMouseManipulator(bool bClose) 
{
	pline = new Line;
	m_bFinished = false;
	pArray = NULL;
	m_bFirst = true;
	m_bCloseLine = bClose;
	pOriginalArray = new QVector3DArray;

}


MeasureMouseManipulator::~MeasureMouseManipulator(void)
{
	//销毁基数样条对象
// 	if(NULL != m_pSpLine)
// 	{
// 		delete m_pSpLine;
// 		m_pSpLine = NULL; 
// 	}
	if (pline!=NULL)
	{
		m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
		m_bFirst= true;
	}	
}

/************************************************************************/
/* 函数功能： 鼠标左键：收集点集，并绘制显示；鼠标右键：结束收集       	*/
/* 参数说明:  QMouseEvent 鼠标事件                 						*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 25 日										*/
/************************************************************************/
void MeasureMouseManipulator::MousePress(QMouseEvent *event)
{
	QVector3D glPoint, scPoint(event->x(), event->y(), 0.0f);	

	if (event->buttons() & Qt::LeftButton)
	{
		if (m_bFirst)
		{				
			pline->getVectexArray()->clear();
			pline->getColorArray()->clear();

			ScreenToGL(scPoint,glPosOld);
			glPosCurrent  = glPosOld;
			GeoPosOld = TransGLtoWGS84(glPosOld);
			m_bFirst = false;
			m_ParentLayer->GetLayerData()->InsertRenderable(pline);
		}  
		else
		{
			m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
			m_bFirst= true;

			QString str;
			QString str2 = tr("大圆距离:");
			str.setNum(m_distance);
			str2 += str;
			msgBox.setText(str2+tr("公里"));
			msgBox.exec();
		}
	}
	else if(event->buttons() & Qt::RightButton)
	{
		if (!m_bFirst)
		{
			m_bFirst= true;
			m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
			m_bFinished = true; 

			QString str;
			QString str2 = tr("大圆距离:");
			str.setNum(m_distance);
			str2 += str;
			msgBox.setText(str2+tr("公里"));
			msgBox.exec();

			//QString str;
			//str.setNum(m_distance);
			//msgBox.setText(str+ tr("公里"));
			//msgBox.exec();
		}
	}
}

double MeasureMouseManipulator::CalGeoDistance(QVector3D &pos1,QVector3D &pos2)
{
	double EarthRadius = 6378.137;
	
	double x1,x2,y1,y2;
	x1 = pos1.x()*MPI/180;
	x2 = pos2.x()*MPI/180;
	y1 = pos1.y()*MPI/180;
	y2 = pos2.y()*MPI/180;
	
	//double dis = EarthRadius*acosf(sinf(x1)*sinf(x2)+cosf(x1)*cosf(x2)*cosf(y1-y2));
	double dis = EarthRadius*acosf(cosf(y2)*cosf(y1)*cosf(x2-x1)+sinf(y2)*sinf(y1));
	return dis;
	//double x = sqrtf(pow((sinf((y1 - y2)/2.0f)), 2) +cosf(y1) * cosf(y2) * pow(sinf((x1-x2)/2.0f), 2));
	//return  2 * asinf(x) * EarthRadius;

}
/************************************************************************/
/* 函数功能： 响应鼠标移动事件					                        */
/* 参数说明:  QRenderable* volume  : 绘制								*/
/*			  QMouseEvent* event   : 鼠标点								*/	
/* 返 回 值:  NULL														*/
/* 作    者:  													    */
/* 日    期:  2011 年 05 月 10 日										*/
/************************************************************************/
void MeasureMouseManipulator::MouseEvent(QMouseEvent* event)
{
	QVector3D  glPoint, scPoint(event->x(), event->y(), 1.0f);
	

	if(!m_bFirst) //如果已经开始量测
	{
		ScreenToGL(scPoint,glPosCurrent);
		
		//动态绘制量测线 
		m_ParentLayer->GetLayerData()->RemoveRenderable(pline);

		pline->getVectexArray()->clear();
		pline->getColorArray()->clear();
		
		pline->getColorArray()->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
		pline->getColorArray()->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
		pline->getVectexArray()->push_back(glPosOld);
		pline->getVectexArray()->push_back(glPosCurrent);
		pline->SetColorArray();
		GeoPosCurrent = TransGLtoWGS84(glPosCurrent);
	//	m_distance = sqrtf((GeoPosOld.x()-GeoPosCurrent.x())*(GeoPosOld.x()-GeoPosCurrent.x())+(GeoPosOld.y()-GeoPosCurrent.y())*(GeoPosOld.y()-GeoPosCurrent.y()));
	//	m_distance = sqrtf((glPosOld.x()-glPosCurrent.x())*(glPosOld.x()-glPosCurrent.x())+(glPosOld.y()-glPosCurrent.y())*(glPosOld.y()-glPosCurrent.y()));
	//	m_distance = glPosOld.distanceToLine(glPosCurrent);
		m_distance = CalGeoDistance(GeoPosOld,GeoPosCurrent);
		m_ParentLayer->GetLayerData()->InsertRenderable(pline);
	}
}

/************************************************************************/
/* 函数功能： 响应鼠标弹起事件					                        */
/* 参数说明:  QRenderable* volume  : 绘制								*/
/*			  QMouseEvent* event   : 鼠标点								*/	
/* 返 回 值:   NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
void MeasureMouseManipulator::MouseRelease(QMouseEvent *event)
{
	return;
}

/************************************************************************/
/* 函数功能： 鼠标离开事件					                            */
/* 参数说明:  QRenderable* volume  : 绘制								*/
/*			  QMouseEvent* event   : 鼠标点								*/	
/* 返 回 值:   NULL														*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 06 月 29 日										*/
/************************************************************************/
void MeasureMouseManipulator::LeaveEvent()
{
	if(m_bFinished)				//没有按下右键，鼠标移开
	{
		m_bFinished = false;		
		if(!m_bFirst)
		{
			m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
			m_bFirst = true;
		}
	}
	else
	{
		if(!m_bFirst)
		{
			m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
			m_bFirst = true;
		}
	}
	return;
}


void MeasureMouseManipulator::setILayerAttrib( ILayerAttribute *Ilayer )
{
	
}

void  MeasureMouseManipulator::GetContourAttribute()
{
	
}
