/*============================================================ 
文件名：DrawContourLine.cpp 
类 名： DrawContourLine
父 类： ManualDraw
子 类： 无 
功能说明：画等值线的算法
调用说明：
1、获取用户的PING命令鼠标坐标点，调用下层算法，返回计算得到的点集；
2、将屏幕点转换成OPENGL的坐标，并传给相应的绘图管道；

-----------------------------版本更新-------------------------
V 0.1 
原作者 ：任雪
完成日期：2011年5月10日
============================================================*/

#include "DrawContourLine.h"
#include <QtCore/QSettings>
#include <QtCore/QTextCodec>
#include "ILayerAttribute.h"
#include <qinputdialog.h>
#include <QMessageBox>

typedef struct
{
	double dLon;
	double dLat;
	double dValue;
}ContourValue;

int DrawContourLine::m_nCount = 0;

DrawContourLine::DrawContourLine(void)
{
	pline = new Line;
	pArray = NULL;
	m_pContourLine = NULL;
	m_bFirst = true;
	m_pSpLine =  new CCardinalSpline;//创建基数样条对象
	m_pSymbolData = new SymbolData;
	pOriginalArray = new QVector3DArray;
	m_fStep = 0.01f/1000;
}

DrawContourLine::DrawContourLine(bool bClose) 
{
	pline = new Line;
	m_bFinished = false;
	pArray = NULL;
	m_pContourLine = NULL;
	m_bFirst = true;
	m_pSpLine =  new CCardinalSpline;
	m_bCloseLine = bClose;
	m_pSymbolData = new SymbolData;
	pOriginalArray = new QVector3DArray;
	m_fStep = 0.01f/1000;
}


DrawContourLine::~DrawContourLine(void)
{
	//销毁基数样条对象
	if(NULL != m_pSpLine)
	{
		delete m_pSpLine;
		m_pSpLine = NULL; 
	}
}

/************************************************************************/
/* 函数功能： 鼠标左键：收集点集，并绘制显示；鼠标右键：结束收集       	*/
/* 参数说明:  QMouseEvent 鼠标事件                 						*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 25 日										*/
/************************************************************************/
void DrawContourLine::MousePress(QMouseEvent *event)
{
	QVector3D glPoint, scPoint(event->x(), event->y(), 0.0f);

	if (event->buttons() & Qt::LeftButton)
	{
		if(!m_bFinished)
		{
			m_InputData.Clear();
			m_CrossPoints.clear();
			m_bFinished = true;
			pArray = NULL;
		}
		pArray = pline->getVectexArray();
		m_InputData.push_back(scPoint);

		//获得十字线的数据
		//GetCrossLineData(scPoint, i);
		if(m_InputData.size()<3)
		{
			m_OutputData.Clear();
			for(int i=0; i<m_InputData.size(); i++)
				m_OutputData<<QVector3D(m_InputData[i].x(), m_InputData[i].y(), m_InputData[i].z());
		}
		else
		{	//取得平滑后的点数组
			m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);

			if (m_lineanalysis.isPolygonintersect(m_OutputData))
			{
				QMessageBox::warning(NULL, QObject::tr("警告"),QObject::tr("多边形中出现线段相交！"),QMessageBox::Ok,QMessageBox::Ok);
				m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
				ClearCrossArray(m_InputData);
				return;
			}
		}

		QVector4DArray *pColor = pline->getColorArray();

		//清除上次的点数组
		pArray->Clear();			
		pColor->clear();
		for(int i = 0 ; i < m_OutputData.count(); i++)
		{
			ScreenToGL(m_OutputData[i], glPoint);
			pColor->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
			pArray->push_back(glPoint);
		}
		pline->SetColorArray();

		DrawCrossArray(m_InputData);

		if(pArray->count() <2)  return ;
		if(m_bFirst)
		{
			m_ParentLayer->GetLayerData()->InsertRenderable(pline);			
			m_bFirst = false;
		}
	}
	else if(event->buttons() & Qt::RightButton)
	{
		if(m_bFinished)				//按下右键，开始画双线
		{
			m_bFinished = false;
			if(pArray->count() <2)  return;          //只有一个点，退出

			m_pContourLine = new CSymbolContourLine;
			QVector3DArray * ArrayTemp = m_pContourLine->GetArray(); 
			pOriginalArray = m_pContourLine->OriginalArray();
			GetContourAttribute();
			m_pContourLine->SetCloseLine(m_bCloseLine);

			QVector3D temp;
			if(m_bCloseLine)
			{
				temp = m_InputData[0];
				m_InputData<<temp;

				//取得平滑后的点数组
				m_OutputData.Clear();
				m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);

				if (m_lineanalysis.isPolygonintersect(m_OutputData))
				{
					QMessageBox msg;
					msg.setText(tr("多边形中出现线段相交！"));
					msg.exec();
					m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
					ClearCrossArray(m_InputData);
					m_bFirst = true;
					return;
				}
				for(int i = 0 ; i < m_OutputData.count(); i++)
				{					
					ScreenToGL(m_OutputData[i], glPoint);
					glPoint.setZ(m_nCount * m_fStep);

					ArrayTemp->push_back(glPoint);;

					temp = TransGLtoWGS84(glPoint);
					temp.setZ(m_nCount * m_fStep);
					pOriginalArray->push_back(temp);
				}
			}
			else
			{
				for(int i = 0 ; i < pArray->size(); i++)
				{
					temp = pArray->at(i);
					temp.setZ(m_nCount * m_fStep);
					ArrayTemp->push_back (temp);
					temp = pArray->at(i);
					temp.setZ(m_nCount * m_fStep);
					pOriginalArray->push_back(TransGLtoWGS84(temp));
				}
			}
			m_nCount++; 

			bool ok;
			int m_value = QInputDialog::getInteger(NULL, tr("数值"),tr("选择数值:"), 0, -9999,9999, 1, &ok );
			if (ok) 
				m_pContourLine->GetAttribute()->SetValue(m_value);
			else 
			{
				pArray->Clear();
				m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
				m_bFirst = true;
				return;
			}

			ClearCrossArray(m_InputData);
			pArray->Clear();
			m_ParentLayer->GetLayerData()->InsertRenderable(m_pContourLine);
		//	m_pContourLine->SetColorArray();
			//undo机制
	/*		CUndoAddDrawCommand* addCommand = new CUndoAddDrawCommand();
			addCommand->SetUndoLayer(m_ParentLayer);
			addCommand->SetUndoRenderable(m_pContourLine);
			addCommand->SetUndoGLWidget(m_pOpenglView);*/
			//if(m_undoStack)
			//{
			//	m_undoStack->push(addCommand);
			//}

			//保存ContourLine到ParentLayer
			QHash<QString,CSymbolContourLine*> *HashLineList = m_ParentLayer->Get2DHashLineList();
			int ncount = HashLineList->size();			
			HashLineList->insert(QString::number(ncount,10),m_pContourLine);		
		    
			if(!m_bFirst)
			{
				m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
				m_bFirst = true;
			}
		}
	}
}
void DrawContourLine::ClearCrossArray(QVector3DArray&  InputData)
{
	if (InputData.count()>0)
	{
		for (int j=0;j<m_CrossArray.count();j++)
		{
			m_ParentLayer->GetLayerData()->RemoveRenderable(m_CrossArray.at(j));
			delete m_CrossArray.at(j);
		}
		m_CrossArray.clear();
	}
}

void DrawContourLine::DrawCrossArray(QVector3DArray&  InputData)
{
	if (InputData.count()>0)
	{
		QVector3D glPos1,glPos2,glPos3,glPos4;
		QVector3D srPos1,srPos2,srPos3,srPos4;
		int iCount = InputData.count() -1;
		Line* temp1 = new Line;
		Line* temp2 = new Line;
		srPos1 = QVector3D(InputData[iCount].x()-3.0f,InputData[iCount].y()-3.0f,InputData[iCount].z()) ;
		srPos2 = QVector3D(InputData[iCount].x()-3.0f,InputData[iCount].y()+3.0f,InputData[iCount].z()) ;
		srPos3 = QVector3D(InputData[iCount].x()+3.0f,InputData[iCount].y()-3.0f,InputData[iCount].z()) ;
		srPos4 = QVector3D(InputData[iCount].x()+3.0f,InputData[iCount].y()+3.0f,InputData[iCount].z()) ;

		ScreenToGL(srPos1,glPos1);
		ScreenToGL(srPos2,glPos2);
		ScreenToGL(srPos3,glPos3);
		ScreenToGL(srPos4,glPos4);	


		temp1->getColorArray()->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
		temp1->getVectexArray()->push_back(glPos1);
		temp1->getColorArray()->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
		temp1->getVectexArray()->push_back(glPos4);
		temp1->SetColorArray();

		temp2->getColorArray()->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
		temp2->getVectexArray()->push_back(glPos2);
		temp2->getColorArray()->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
		temp2->getVectexArray()->push_back(glPos3);
		temp2->SetColorArray();


		m_CrossArray.push_back(temp1);
		m_CrossArray.push_back(temp2);

		m_ParentLayer->GetLayerData()->InsertRenderable(temp1);
		m_ParentLayer->GetLayerData()->InsertRenderable(temp2);
	}
	

}

/************************************************************************/
/* 函数功能： 响应鼠标移动事件					                        */
/* 参数说明:  QRenderable* volume  : 绘制								*/
/*			  QMouseEvent* event   : 鼠标点								*/	
/* 返 回 值:  NULL														*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 05 月 10 日										*/
/************************************************************************/
void DrawContourLine::MouseEvent(QMouseEvent* event)
{
	QVector3D  glPoint, scPoint(event->x(), event->y(), 0.0f);

	if(m_bFinished)
	{
		if(m_InputData.size()<3) return;
		//取得平滑后的点数组
		m_InputData<<scPoint;
		m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);
		QVector4DArray *pColor = pline->getColorArray();
		pArray = pline->getVectexArray();
		//清除上次的点数组
		pArray->Clear();			
		pColor->clear();
		int j=0;
		QVector3D topLefPoint, topRightPoint, bottomLeftPoint, bottomRightPoint;
		for(int i = 0 ; i < m_OutputData.count(); i++)
		{
			ScreenToGL(m_OutputData[i], glPoint);
			glPoint.setZ(0.0f);
			pColor->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
			pArray->push_back(glPoint);
		}
		pline->SetColorArray();

		if(pArray->count() <2)  return ;
		if(m_bFirst)
		{
			m_ParentLayer->GetLayerData()->InsertRenderable(pline);
		}
		m_InputData.pop_back();
		m_OutputData.Clear();
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
void DrawContourLine::MouseRelease(QMouseEvent *event)
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
void DrawContourLine::LeaveEvent()
{
	if(m_bFinished)				//没有按下右键，鼠标移开
	{
		m_bFinished = false;
		ClearCrossArray(m_InputData);
		pArray->Clear();		
		if(!m_bFirst)
		{
			m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
			m_bFirst = true;
		}
	}
	return;
}


void DrawContourLine::setILayerAttrib( ILayerAttribute *Ilayer )
{
	
}

/************************************************************************/
/* 函数功能： 取得等值线属性       			        					*/
/* 参数说明:  无                 										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 25 日										*/
/************************************************************************/
void  DrawContourLine::GetContourAttribute()
{
	LAYER_ATTRIBUTE *pLA = getILayerAttrib();
	if(!pLA) return;
	CSymbolAttribute * pAttr = m_pContourLine->GetAttribute();

// 	//等值线值
 	ILayerAttribute *pLayAttribute = pLA->value(QString("cValue"));
// 	pAttr->SetValue(pLayAttribute->Value().toDouble());

	//颜色值
	QColor tColor;
	pLayAttribute = pLA->value(QString("cColor"));
	tColor = pLayAttribute->Value().value<QColor>();
	pAttr->SetColor(QVector4D(tColor.redF(), tColor.greenF(), tColor.blueF(),tColor.alphaF()));

	//是否填充等值线
	pLayAttribute = pLA->value(QString("cFill"));
	bool bFilled;
	if(NULL != pLayAttribute)
	{
		bFilled = pLayAttribute->Value().toBool();
		pAttr->SetFillType(bFilled);
		if(m_bCloseLine) 
		{
			pLayAttribute = pLA->value(QString("cFillColor"));
			tColor = pLayAttribute->Value().value<QColor>();
			pAttr->SetFillColor(QVector4D(tColor.redF(), tColor.greenF(), tColor.blueF(),tColor.alphaF()));
		}
	}

	//线宽设置
	pLayAttribute = pLA->value(QString("cWidth"));
	pAttr->SetLineWidth(pLayAttribute->Value().toInt());

	//线型设置
	pLayAttribute = pLA->value(QString("cKinds"));
	pAttr->SetLineType(pLayAttribute->Value().toString());

	pAttr->SetLayerZ(0);
}

/************************************************************************/
/* 函数功能： 在输入的控制点上画十字		        					*/
/* 参数说明:  无                 										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 05 月 10 日										*/
/************************************************************************/
void DrawContourLine::GetCrossLineData(QVector3D scPoint, int i)
{
	QVector3D topLeftPoint, topRightPoint, bottomLeftPoint, bottomRightPoint;

	topLeftPoint.setX(scPoint.x()-6);
	topLeftPoint.setY(scPoint.y()+6);
	topLeftPoint.setZ(0.0f);
	
	topRightPoint.setX(scPoint.x()+6);
	topRightPoint.setY(scPoint.y()+6);
	topRightPoint.setZ(0.0f);
		
	bottomLeftPoint.setX(scPoint.x()-6);
	bottomLeftPoint.setY(scPoint.y()-6);
	bottomLeftPoint.setZ(0.0f);
	
	bottomRightPoint.setX(scPoint.x()+6);
	bottomRightPoint.setY(scPoint.y()-6);
	bottomRightPoint.setZ(0.0f);
	m_CrossPoints[i]<<topLeftPoint<<topRightPoint<<bottomLeftPoint<<bottomRightPoint;
}