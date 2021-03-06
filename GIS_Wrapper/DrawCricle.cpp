#include "DrawCricle.h"


DrawCricle::DrawCricle(int nType ,CoordinateTransform* proj)
{
	m_pDisplayLine = new Line;
	m_bFinished = false;
	m_pArray = NULL;
	m_pSymbolLine = NULL;
	m_bFirst = true;
	m_pSymboldata = new SymbolData;
	InitAttribute(nType);
	m_nType = nType;
	m_pressNum = 0;
	m_proj = dynamic_cast<RadProjTransform*> (proj);
}


DrawCricle::~DrawCricle(void)
{
}

/************************************************************************/
/* 函数功能： 响应鼠标移动事件					                        */
/* 参数说明:  QRenderable* volume  : 绘制								*/
/*			  QMouseEvent* event   : 鼠标点								*/	
/* 返 回 值:  NULL														*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 07 月 28 日										*/
/************************************************************************/
void DrawCricle::MouseEvent(QMouseEvent* event)
{
	QVector3D  glPoint, scPoint(event->x(), event->y(), 0);
	if(m_InputData.size()<1) return;
	//取得平滑后的点数组
	m_InputData.push_back(scPoint);

	QVector3D temp_centrl = m_InputData.at(0);
	QVector3D temp_cricle = m_InputData.at(1);
	float r = abs(temp_centrl.x()-temp_cricle.x());
	m_InputData.Clear();
	for(int i = 0;i<360;i++)
	{
		QVector3D tp;
		tp.setY(temp_centrl.y()+r*sin(i*M_PI/180));
		tp.setX(temp_centrl.x()+r*cos(i*M_PI/180));
		m_InputData.push_back(tp);
	}
	m_OutputData.Clear();
	for(int i=0; i<m_InputData.size(); i++)
		m_OutputData<<QVector3D(m_InputData[i].x(), m_InputData[i].y(), m_InputData[i].z());
	QVector4DArray *pColor = m_pDisplayLine->getColorArray();
	QVector3DArray *pArray = m_pDisplayLine->getVectexArray();
	//清除上次的点数组
	pArray->Clear();			
	pColor->clear();

	for(int i = 0 ; i < m_OutputData.count(); i++)
	{
		ScreenToGL(m_OutputData[i], glPoint);
		pColor->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
		glPoint.setZ(0.52f);
		pArray->push_back(glPoint);
	}
	m_pDisplayLine->SetColorArray();
	
	m_InputData.clear();
	m_InputData.push_back(temp_centrl);
	m_OutputData.Clear();
}

/************************************************************************/
/* 函数功能： 获取鼠标点         			        					*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 02 日										*/
/************************************************************************/
void DrawCricle::MousePress(QMouseEvent *event)
{
	if(!m_Symbol) return;
	QVector3D glPoint, scPoint(event->x(), event->y(), 0.52f);

	if (event->buttons() & Qt::LeftButton && m_pressNum == 0)
	{
		if(!m_bFinished)
		{
			m_InputData.Clear();
			m_bFinished = true;
			m_pArray = NULL;
		}		
		m_InputData.push_back(scPoint);

		if(m_InputData.size()<3)
		{
			m_OutputData.Clear();
			for(int i=0; i<m_InputData.size(); i++)
				m_OutputData<<QVector3D(m_InputData[i].x(), m_InputData[i].y(), m_InputData[i].z());
		}
		QVector4DArray *pColor = m_pDisplayLine->getColorArray();
		m_pArray = m_pDisplayLine->getVectexArray();		
		m_pArray->Clear();	//清除上次的点数组		
		pColor->clear();

		for(int i = 0 ; i < m_OutputData.count(); i++)
		{
			ScreenToGL(m_OutputData[i], glPoint);
			pColor->push_back(QVector4D(1.0, 0.0, 0.0, 255.0));
			m_pArray->push_back(glPoint);
		}

		m_pDisplayLine->SetColorArray();

		if(m_bFirst)
		{
			m_ParentLayer->GetLayerData()->InsertRenderable(m_pDisplayLine);
			m_bFirst = false;
		}
		m_pressNum = 1;
	}
}

/************************************************************************/
/* 函数功能： 鼠标释放         			        		    			*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 02 日										*/
/************************************************************************/
void DrawCricle::MouseRelease(QMouseEvent *event)
{
	QVector3D glPoint, scPoint(event->x(), event->y(), 0.52f);
	if(/*event->buttons() &*/ Qt::LeftButton && m_pressNum == 1)
	{
		if(!m_bFinished)
		{
			m_pressNum = 0;
			m_bFinished = false;
			m_InputData.clear();
			return;
		}		
		m_InputData.push_back(scPoint);
		QVector3D temp_centrl = m_InputData.at(0);
		QVector3D temp_cricle = m_InputData.at(1);
		float r = abs(temp_centrl.x()-temp_cricle.x());
		m_InputData.Clear();
		for(int i = 0;i<360;i++)
		{
			QVector3D tp;
			tp.setY(temp_centrl.y()+r*sin(i*M_PI/180));
			tp.setX(temp_centrl.x()+r*cos(i*M_PI/180));
			m_InputData.push_back(tp);
		}
		{
			m_OutputData.Clear();
			for(int i=0; i<m_InputData.size(); i++)
				m_OutputData<<QVector3D(m_InputData[i].x(), m_InputData[i].y(), m_InputData[i].z());
		}
		m_InputData.clear();
		QVector4DArray *pColor = m_pDisplayLine->getColorArray();
		m_pArray = m_pDisplayLine->getVectexArray();		
		m_pArray->Clear();	//清除上次的点数组		
		pColor->clear();

		for(int i = 0 ; i < m_OutputData.count(); i++)
		{
			ScreenToGL(m_OutputData[i], glPoint);
			pColor->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
			m_pArray->push_back(glPoint);
		}

		m_pDisplayLine->SetColorArray();

		if(m_bFirst)
		{
			m_ParentLayer->GetLayerData()->InsertRenderable(m_pDisplayLine);
			m_bFirst = false;
		}
		////////////////////////////////////////////////////////
		if(m_bFinished)				//第二次按下左键，开始画双线
		{
			m_bFinished = false;
			if(m_pArray->count() <2)  return;          //只有一个点，退出

			m_pSymbolLine = new CSymbolLine;
			QVector3DArray * ArrayTemp = m_pSymbolLine->GetArray(); 
			QVector3DArray * pOriginalArray = m_pSymbolLine->OriginalArray();
			GetSymbolAttribute();
			for(int i = 0 ; i < m_pArray->size(); i++)
			{
				ArrayTemp->push_back (m_pArray->at(i));
				QVector3D temp = m_pArray->at(i);
				CoordTrans(m_proj);
				pOriginalArray->push_back(TransGLtoWGS84(temp));
			}

			m_pArray->Clear();
			m_pSymbolLine->SetSymbolLineStyle(m_Symbol);
			m_pressNum = 0;

			m_ParentLayer->GetLayerData()->InsertRenderable(m_pSymbolLine);
			m_pSymbolLine->SetColorArray();
			//undo机制
			//CUndoAddDrawCommand* addCommand = new CUndoAddDrawCommand();
			//addCommand->SetUndoLayer(m_ParentLayer);
			//addCommand->SetUndoRenderable(m_pSymbolLine);
			//addCommand->SetUndoGLWidget(m_pOpenglView);
			//if(m_undoStack)
			//{
			//	m_undoStack->push(addCommand);
			//}
			SymbolLayer *sLayer = dynamic_cast<SymbolLayer*>(m_ParentLayer);
			if (sLayer == NULL) return;
			//SymbolData *m_pSymboldata = sLayer->getSymbolData();
			m_pSymboldata = sLayer->getSymbolData();
			m_pSymboldata->getSymbolLine()->push_back(m_pSymbolLine);

			if(!m_bFirst)
			{
				m_ParentLayer->GetLayerData()->RemoveRenderable(m_pDisplayLine);
				m_bFirst = true;
			}
		}
	}
	else
	{
		m_InputData.Clear();
		m_pressNum = 0;
	}
}

/************************************************************************/
/* 函数功能： 获取鼠标点，没有按下右键，开始清理轨迹线         			*/
/* 参数说明:  鼠标事件           										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 02 日										*/
/************************************************************************/
void DrawCricle::LeaveEvent()
{
	if(m_bFinished)				
	{
		m_bFinished = false;
		m_pArray->Clear();
		if(!m_bFirst)
		{
			m_ParentLayer->GetLayerData()->RemoveRenderable(m_pDisplayLine);
			m_bFirst = true;
		}
	}
}

void DrawCricle::setILayerAttrib( ILayerAttribute *Ilayer )
{

}
/************************************************************************/
// 函数功能： 初始化箭头线属性窗
// 返 回 值:  void
// 作    者:  杨东		
// 日    期:  2011 年 4 月 13 日
/************************************************************************/
void DrawCricle::InitAttribute( int nType )
{
	m_layerAttribute = new LAYER_ATTRIBUTE;

	//颜色
	QStringList nColorNum;
	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor",tr("颜色"),QColor(0,0,255),QVariant::Color,0,0,nColorNum);
	m_layerAttribute->insert(QString("cColor"),cColor);

	//线型
	QStringList strKinds;
	strKinds.append(QString(tr("实线")));
	strKinds.append(QString(tr("短划线")));
	strKinds.append(QString(tr("点线")));
	ILayerAttribute *cKinds = new ILayerAttribute("cKinds","cKinds",tr("线型"),0,QVariant::StringList,0,0,strKinds);
	m_layerAttribute->insert(QString("cKinds"), cKinds);

	//线宽
	QStringList strWidth;
	for (int i=1; i<=10; i++)
	{
		strWidth.append(QString::number(i));
	}
	ILayerAttribute *cWidth = new ILayerAttribute("cWidth","cWidth",tr("线宽"),1,QVariant::Int,1,50,strWidth);
	m_layerAttribute->insert(QString("cWidth"),cWidth);


	if(nType == 1101)//等值线的标值
	{
		QStringList nContourValue;
		ILayerAttribute *cValue = new ILayerAttribute("cValue","cValue",tr("标值"),0,QVariant::Double,-10000.000000,10000.000000,nContourValue);
		m_layerAttribute->insert(QString("cValue"),cValue);
	}	
	else if(nType == 1308)//多边形的区域标值
	{
		QStringList strRegion;
		strRegion.append(QString(tr("雨区")));
		strRegion.append(QString(tr("雪区")));
		strRegion.append(QString(tr("雾区")));
		strRegion.append(QString(tr("大风区")));
		strRegion.append(QString(tr("沙暴区")));
		strRegion.append(QString(tr("雷暴区")));
		ILayerAttribute *cValue = new ILayerAttribute("cValue","cValue",tr("区域类型"),0,QVariant::StringList,-0,0,strRegion);
		m_layerAttribute->insert(QString("cValue"),cValue);
	}

	if (nType==1101 || nType==1308)//1101表示等值线，1308表示多边形
	{
		QStringList strFill;
		strFill.append(tr("是"));
		strFill.append(tr("否"));
		ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill",tr("填充"),0,QVariant::Bool,0,0,strFill);
		m_layerAttribute->insert(QString("cFill"),cFill);
	}
}

/************************************************************************/
/* 函数功能： 获取符号属性         			        					*/
/* 参数说明:  无                 										*/
/* 返 回 值:  无                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 19 日										*/
/************************************************************************/
void  DrawCricle::GetSymbolAttribute()
{
	LAYER_ATTRIBUTE *pLA = getILayerAttrib();
	if(!pLA) return;

	//设置类型编号
	ILayerAttribute *pLayAttribute;	
	m_pSymbolLine->m_LineAttribute.SetSymbolType(m_nType);

	//设置颜色
	pLayAttribute = pLA->value(QString("cColor"));
	if(!pLayAttribute) return;
	QColor tColor = pLayAttribute->Value().value<QColor>();;
    m_pSymbolLine->m_LineAttribute.SetColor(QVector4D(tColor.red()/255.0, tColor.green()/255.0, tColor.blue()/255.0, 1.0f));

	//设置线型
	pLayAttribute = pLA->value(QString("cKinds"));
	if(!pLayAttribute) return;
	m_pSymbolLine->m_LineAttribute.SetLineType(pLayAttribute->Value().toString());

	//设置线宽
	pLayAttribute = pLA->value(QString("cWidth"));
	if(!pLayAttribute) return;
	m_pSymbolLine->m_LineAttribute.SetLineWidth(pLayAttribute->Value().toInt());


	if(m_nType == 1101)//等值线的标值
	{
		pLayAttribute = pLA->value(QString("cValue"));
		if(!pLayAttribute) return;
		m_pSymbolLine->m_LineAttribute.SetValue(pLayAttribute->Value().toDouble());
	}	
	else if(m_nType == 1308)//多边形的区域标识
	{
		pLayAttribute = pLA->value(QString("cValue"));
		if(!pLayAttribute) return;
		int nRegion = pLayAttribute->Value().toInt();
		QString strRegion;
		switch(nRegion)
		{
		case 0:
			strRegion = tr("雨区");
			break;
		case 1:
			strRegion = tr("雪区");
			break;
		case 2:
			strRegion = tr("雾区");
			break;
		case 3:
			strRegion = tr("大风区");
			break;
		case 4:
			strRegion = tr("沙暴区");
			break;
		case 5:
			strRegion = tr("雷暴区");
			break;
		}
		m_pSymbolLine->m_LineAttribute.SetValue(strRegion);
	}	

	//设置填充设置
	if(m_nType == 1003 || m_nType ==1005)//填充的箭头
	{
		m_pSymbolLine->m_LineAttribute.SetFillType(true);
	}
	else if(m_nType == 1101 || m_nType ==1308)//等值线 ， 多边形
	{
		pLayAttribute = pLA->value(QString("cFill"));
		if(!pLayAttribute) return;
		m_pSymbolLine->m_LineAttribute.SetFillType(pLayAttribute->Value().toBool());
	}
}
