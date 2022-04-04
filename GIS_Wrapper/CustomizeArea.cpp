/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ��ѩ
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����CustomizeArea.h CustomizeArea.cpp 
�� ���� CCustomizeArea
�� �ࣺ ManualDraw
�� �ࣺ �� 
����˵������ѡ����
����˵����
1��Ŀǰ���þ��εķ�ʽ����Ļ��ѡ��һ��������
2����

-------------------------------------�汾����------------------------------------------------ 
V 1.2 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ����ѩ
������ڣ�2011�ꣿ�£���
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
		if(bFinished)				//�����Ҽ���ȷ������ѡ�����
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
