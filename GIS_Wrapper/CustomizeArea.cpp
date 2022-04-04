/*============================================================ 
项目名： 人机交互平台
作 者： 任雪
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：CustomizeArea.h CustomizeArea.cpp 
类 名： CCustomizeArea
父 类： ManualDraw
子 类： 无 
功能说明：自选区域
调用说明：
1、目前是用矩形的方式在屏幕上选定一定的区域；
2、；

-------------------------------------版本更新------------------------------------------------ 
V 1.2 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：任雪
完成日期：2011年？月？日
============================================================*/

#include "CustomizeArea.h"


CCustomizeArea::CCustomizeArea(void)
{
	pLine = new Line;
	bFinished = false;
}


CCustomizeArea::~CCustomizeArea(void)
{
}


void CCustomizeArea::MouseEvent(QMouseEvent* event)
{
}

void CCustomizeArea::MousePress(QMouseEvent *event)
{
	QVector3D glPoint, glTopLeftPoint, scPoint(event->x(), event->y(), 0.0f);
	
	if (event->buttons() & Qt::LeftButton)
	{
		if(!bFinished)
		{				
			bFinished = true;	
			pArray = NULL;
			topLeftPos = scPoint;
			nNum = 0;
		}
		else
		{
			nNum++;
			QVector3D topRightPos(event->x(), topLeftPos.y(), 0.0f), bottomLeftPos(topLeftPos.x(), event->y(), 0.0f);
			bottomRightPos = scPoint;
		    if(nNum>1)
			{
				pArray->clear();
			}
			pArray = pLine->getVectexArray();
			ScreenToGL(topLeftPos, glTopLeftPoint);		
			pArray->push_back(glTopLeftPoint);
			ScreenToGL(topRightPos, glPoint);		
			pArray->push_back(glPoint);
			ScreenToGL(bottomRightPos, glPoint);		
			pArray->push_back(glPoint);
			ScreenToGL(bottomLeftPos, glPoint);		
			pArray->push_back(glPoint);
			pArray->push_back(glTopLeftPoint);

			//set color
			QVector4DArray *pColor = pLine->getColorArray();
			for(int i=0; i<4; i++)
				pColor->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));

			m_ParentLayer->GetLayerData()->InsertRenderable(pLine);
		}		
	}
	else if(event->buttons() & Qt::RightButton)
	{
		if(bFinished)				//按下右键，确定区域选择完毕
		{
			bFinished = false;
			nNum = 0;
		}
	}
}
void CCustomizeArea::MouseRelease(QMouseEvent *event)
{
}

void CCustomizeArea::setILayerAttrib( ILayerAttribute *Ilayer )
{

}
