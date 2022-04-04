/*============================================================
项目名： 人机交互平台
作 者： lugsh
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
-------------------------------------------------------------------------------------
文件名：ManualDraw.h ManualDraw.cpp
类 名： ManualDraw
父 类： NULL
子 类： 无
功能说明：手动绘图接口类
调用说明：

--------------------------版本更新----------------------------
V 1.1
作 者：输入作者（或修改者）名字
完成日期：
更新说明：
V 1.0
原作者 ：lugsh
完成日期：2011年？月？日
============================================================*/

#include "ManualDraw.h"
// #include <QtCore/QSettings>
// #include <QtCore/QTextCodec>

/************************************************************************/
/* 函数功能： 默认构造函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
ManualDraw::ManualDraw(void):m_pManageLay(0)
{
//	m_Proj4Trans = new Proj4Transform;
	m_layerAttribute = NULL;
	m_ParentLayer = NULL;
	m_pOpenglView = NULL;
	m_Symbol = NULL;
	m_symbolShade = NULL; //阴影符号
	m_bFinished = false;
	m_bFirst = false;
}

/************************************************************************/
/* 函数功能： 默认析构函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
ManualDraw::~ManualDraw(void)
{
// 	if(m_Proj4Trans!=NULL)
// 	{
// 		delete m_Proj4Trans;
// 		m_Proj4Trans = NULL;
// 	}
}

void ManualDraw::mousePressEvent( QMouseEvent *event )
{
	if(NULL != m_pManageLay)
	{
		Layer * currentLayer = m_pManageLay->CurrentLayer();
		if(NULL!= currentLayer )
		{
			if(currentLayer->isLayerHide())
				return;

			 if( currentLayer->LayerType()== Layer::ELayerType::Station_Layer)
			{
 				if(m_ParentLayer != currentLayer )               //判断两次图层是否相同
 				{
 					m_bFirst = true;
 					SetParentLayer(currentLayer);
 				}
				MousePress(event);
				return;
			}
 			if( currentLayer->LayerType()== Layer::ELayerType::Temperature_Layer)
 			{
 				if(m_ParentLayer != currentLayer )               //判断两次图层是否相同
 				{
 					m_bFirst = true;
 					SetParentLayer(currentLayer);
 				}
 				MousePress(event);
				return;
 			}
 			if( currentLayer->LayerType()== Layer::ELayerType::Rain_Layer)
 			{
 				if(m_ParentLayer != currentLayer )               //判断两次图层是否相同
 				{
 					m_bFirst = true;
 					SetParentLayer(currentLayer);
 				}
 				MousePress(event);
				return;
 			}
			if( currentLayer->LayerType()== Layer::ELayerType::Rain_Yubao_Layer)
			{
				if(m_ParentLayer != currentLayer )               //判断两次图层是否相同
				{
					m_bFirst = true;
					SetParentLayer(currentLayer);
				}
				MousePress(event);
				return;
			}
			if( currentLayer->LayerType()== Layer::ELayerType::Disaster_Layer)
			{
				if(m_ParentLayer != currentLayer )               //判断两次图层是否相同
				{
					m_bFirst = true;
					SetParentLayer(currentLayer);
				}
				MousePress(event);
				return;
			}
			if( currentLayer->LayerType()== Layer::ELayerType::Symbol_Layer)
			{
				if(m_ParentLayer != currentLayer )               //判断两次图层是否相同
				{
					m_bFirst = true;
					SetParentLayer(currentLayer);
				}
				MousePress(event);
				return;
			}
		}
	}
}

void ManualDraw::mouseReleaseEvent( QMouseEvent *event )
{
	if(NULL != m_pManageLay)
	{
		Layer * currentLayer = m_pManageLay->CurrentLayer();
		if(NULL!= currentLayer )
		{
			if( currentLayer->LayerType()== Layer::ELayerType::Symbol_Layer)
			{
				MouseRelease(event);
			}
			if( currentLayer->LayerType()== Layer::ELayerType::Temperature_Layer)
			{
				MouseRelease(event);
			}
			if( currentLayer->LayerType()== Layer::ELayerType::Rain_Layer)
			{
				MouseRelease(event);
			}
			if( currentLayer->LayerType()== Layer::ELayerType::Rain_Yubao_Layer)
			{
				MouseRelease(event);
			}
		}
	}
}

void ManualDraw::mouseMoveEvent( QMouseEvent *event )
{
	if(NULL != m_pManageLay)
	{
		Layer * currentLayer = m_pManageLay->CurrentLayer();
		if(NULL!= currentLayer )
		{
			if( currentLayer->LayerType()== Layer::ELayerType::Symbol_Layer)
			{
				if(m_ParentLayer != currentLayer )               //判断两次图层是否相同
				{
					m_bFirst = true;
					SetParentLayer(currentLayer);
				}
				MouseEvent(event);
			}
			if( currentLayer->LayerType()== Layer::ELayerType::Temperature_Layer)
			{
				if(m_ParentLayer != currentLayer )               //判断两次图层是否相同
				{
					m_bFirst = true;
					SetParentLayer(currentLayer);
				}
				MouseEvent(event);
			}
			if( currentLayer->LayerType()== Layer::ELayerType::Rain_Layer)
			{
				if(m_ParentLayer != currentLayer )               //判断两次图层是否相同
				{
					m_bFirst = true;
					SetParentLayer(currentLayer);
				}
				MouseEvent(event);
			}
			if( currentLayer->LayerType()== Layer::ELayerType::Rain_Yubao_Layer)
			{
				if(m_ParentLayer != currentLayer )               //判断两次图层是否相同
				{
					m_bFirst = true;
					SetParentLayer(currentLayer);
				}
				MouseEvent(event);
			}
		}
	}	
}

void ManualDraw::wheelEvent( QWheelEvent *event )
{
	/*qreal xyzScale = 0.0;
	static GLfloat v = 0.1f, v1 = 0.1f;
	m_isTrans = true;

	if(event->delta() > 0)
	{
		if(1)
		{
			//	m_pObserver->Camera()->Scale(0.9);
			m_curScale = 0.9f;
		}
		else
		{
			xyzScale *= 1.1;
		}
	}
	else
	{
		if(1)
		{
			//	m_pObserver->Camera()->Scale(1.1);
			m_curScale = 1.1f;
		}
		else
		{
			xyzScale /= 1.1;
		}
	}
	m_pGLWidget->update();
	*/
}

void ManualDraw::leaveEvent( QEvent * even )
{
	LeaveEvent();
}
 void ManualDraw::mouseDoubleClickEvent (QMouseEvent * event)
 {
	 if(NULL != m_pManageLay)
	 {
		 Layer * currentLayer = m_pManageLay->CurrentLayer();
		 if(NULL!= currentLayer )
		 {
			 if( currentLayer->LayerType()== Layer::ELayerType::Symbol_Layer)
			 {
				 if(m_ParentLayer != currentLayer )               //判断两次图层是否相同
				 {
					 m_bFirst = true;
					 SetParentLayer(currentLayer);
				 }
				 MouseDoubleClick(event);
			 }
			 if( currentLayer->LayerType()== Layer::ELayerType::Temperature_Layer)
			 {
				 if(m_ParentLayer != currentLayer )               //判断两次图层是否相同
				 {
					 m_bFirst = true;
					 SetParentLayer(currentLayer);
				 }
				 MouseDoubleClick(event);
			 }
			 if( currentLayer->LayerType()== Layer::ELayerType::Rain_Layer)
			 {
				 if(m_ParentLayer != currentLayer )               //判断两次图层是否相同
				 {
					 m_bFirst = true;
					 SetParentLayer(currentLayer);
				 }
				 MouseDoubleClick(event);
			 }
			 if( currentLayer->LayerType()== Layer::ELayerType::Rain_Yubao_Layer)
			 {
				 if(m_ParentLayer != currentLayer )               //判断两次图层是否相同
				 {
					 m_bFirst = true;
					 SetParentLayer(currentLayer);
				 }
				 MouseDoubleClick(event);
			 }
		 }
	 }
 }

/************************************************************************/
/* 函数功能： 投影坐标对象						                        */
/* 参数说明:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void ManualDraw::CoordTrans(RadProjTransform *pProj4)
{
	//coordTrans = pProj4;
	geotrans.SetProj4Trans(pProj4);
}

/************************************************************************/
/* 函数功能： 将给定当前投影坐标点转换成经纬坐标点                      */
/* 参数说明:  绘定经纬坐标点											*/
/* 返 回 值:   NULL														*/
/* 作    者:  卢高昇													*/
/* 日    期:  2011 年 04 月 13 日										*/
/************************************************************************/
QVector3D ManualDraw::TransGLtoWGS84(QVector3D & glPostion)
{
	return geotrans.TransGLtoWGS84(glPostion);
}

/************************************************************************/
/* 函数功能： 将给定经纬坐标点转换成当前投影坐标点                      */
/* 参数说明:  绘定经纬坐标点											*/
/* 返 回 值:   NULL														*/
/* 作    者:  卢高昇													*/
/* 日    期:  2011 年 04 月 13 日										*/
/************************************************************************/
QVector3D ManualDraw::TransWGS84toGL(QVector3D & oriPostion)
{
	return geotrans.TransWGS84toGL(oriPostion);
}

/************************************************************************/
/* 函数功能： 得到属性容器						　                      */
/* 参数说明:  绘定经纬坐标点											*/
/* 返 回 值:   NULL														*/
/* 作    者:  卢高昇													*/
/* 日    期:  2011 年 04 月 13 日										*/
/************************************************************************/
LAYER_ATTRIBUTE * ManualDraw::getILayerAttrib()
{
	return m_layerAttribute;
}
/************************************************************************/
/* 函数功能： 初始化等值线属性					                        */
/* 参数说明:  是否是闭合等值线											*/
/* 返 回 值:   NULL														*/
/* 作    者:  杨东														*/
/* 日    期:  2011 年 04 月 13 日										*/
/************************************************************************/
void ManualDraw::InitAttribute(bool b)
{
	m_layerAttribute = new LAYER_ATTRIBUTE;
	QStringList n;

//	ILayerAttribute *cValue = new ILayerAttribute("cValue","cValue",tr("等值线数值"),0,QVariant::Double,-10000.000000,10000.000000,n);
//	m_layerAttribute->insert(QString("cValue"),cValue);

	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor",tr("线色"),QColor(255,0,0),QVariant::Color,0,0,n);
	m_layerAttribute->insert(QString("cColor"), cColor);

	if (b)
	{
		QStringList strFill;
		strFill.append(tr("否"));
		strFill.append(tr("是"));
		ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill",tr("填充"),0,QVariant::Bool,0,0,strFill);
		m_layerAttribute->insert(QString("cFill"),cFill);
		ILayerAttribute *cFillColor = new ILayerAttribute("cFillColor","cFillColor",tr("填充颜色"),QColor(0,0,255),QVariant::Color,0,0,n);
		m_layerAttribute->insert(QString("cFillColor"), cFillColor);
	}

	QStringList strKinds;
	strKinds.append(QString(tr("实线")));
	strKinds.append(QString(tr("短划线")));
	strKinds.append(QString(tr("点线")));
	ILayerAttribute *cKinds = new ILayerAttribute("cKinds","cKinds",tr("线型"),0,QVariant::StringList,0,0,strKinds);
	m_layerAttribute->insert(QString("cKinds"), cKinds);

	QStringList strWidth;
	for (int i=1; i<=10; i++)
	{
		strWidth.append(QString::number(i));
	}
	ILayerAttribute *cWidth = new ILayerAttribute("cWidth","cWidth",tr("线宽"),2,QVariant::Int,1,10,strWidth);
	m_layerAttribute->insert(QString("cWidth"),cWidth);
}

//void ManualDraw::SetUndoStack( QUndoStack* undoStack )
//{
//	m_undoStack = undoStack;
//}

//QUndoStack* ManualDraw::GetUndoStack()
//{
//	return m_undoStack;
//}