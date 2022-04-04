/*============================================================
��Ŀ���� �˻�����ƽ̨
�� �ߣ� lugsh
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
-------------------------------------------------------------------------------------
�ļ�����ManualDraw.h ManualDraw.cpp
�� ���� ManualDraw
�� �ࣺ NULL
�� �ࣺ ��
����˵�����ֶ���ͼ�ӿ���
����˵����

--------------------------�汾����----------------------------
V 1.1
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵����
V 1.0
ԭ���� ��lugsh
������ڣ�2011�ꣿ�£���
============================================================*/

#include "ManualDraw.h"
// #include <QtCore/QSettings>
// #include <QtCore/QTextCodec>

/************************************************************************/
/* �������ܣ� Ĭ�Ϲ��캯��						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
ManualDraw::ManualDraw(void):m_pManageLay(0)
{
//	m_Proj4Trans = new Proj4Transform;
	m_layerAttribute = NULL;
	m_ParentLayer = NULL;
	m_pOpenglView = NULL;
	m_Symbol = NULL;
	m_symbolShade = NULL; //��Ӱ����
	m_bFinished = false;
	m_bFirst = false;
}

/************************************************************************/
/* �������ܣ� Ĭ����������						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
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
 				if(m_ParentLayer != currentLayer )               //�ж�����ͼ���Ƿ���ͬ
 				{
 					m_bFirst = true;
 					SetParentLayer(currentLayer);
 				}
				MousePress(event);
				return;
			}
 			if( currentLayer->LayerType()== Layer::ELayerType::Temperature_Layer)
 			{
 				if(m_ParentLayer != currentLayer )               //�ж�����ͼ���Ƿ���ͬ
 				{
 					m_bFirst = true;
 					SetParentLayer(currentLayer);
 				}
 				MousePress(event);
				return;
 			}
 			if( currentLayer->LayerType()== Layer::ELayerType::Rain_Layer)
 			{
 				if(m_ParentLayer != currentLayer )               //�ж�����ͼ���Ƿ���ͬ
 				{
 					m_bFirst = true;
 					SetParentLayer(currentLayer);
 				}
 				MousePress(event);
				return;
 			}
			if( currentLayer->LayerType()== Layer::ELayerType::Rain_Yubao_Layer)
			{
				if(m_ParentLayer != currentLayer )               //�ж�����ͼ���Ƿ���ͬ
				{
					m_bFirst = true;
					SetParentLayer(currentLayer);
				}
				MousePress(event);
				return;
			}
			if( currentLayer->LayerType()== Layer::ELayerType::Disaster_Layer)
			{
				if(m_ParentLayer != currentLayer )               //�ж�����ͼ���Ƿ���ͬ
				{
					m_bFirst = true;
					SetParentLayer(currentLayer);
				}
				MousePress(event);
				return;
			}
			if( currentLayer->LayerType()== Layer::ELayerType::Symbol_Layer)
			{
				if(m_ParentLayer != currentLayer )               //�ж�����ͼ���Ƿ���ͬ
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
				if(m_ParentLayer != currentLayer )               //�ж�����ͼ���Ƿ���ͬ
				{
					m_bFirst = true;
					SetParentLayer(currentLayer);
				}
				MouseEvent(event);
			}
			if( currentLayer->LayerType()== Layer::ELayerType::Temperature_Layer)
			{
				if(m_ParentLayer != currentLayer )               //�ж�����ͼ���Ƿ���ͬ
				{
					m_bFirst = true;
					SetParentLayer(currentLayer);
				}
				MouseEvent(event);
			}
			if( currentLayer->LayerType()== Layer::ELayerType::Rain_Layer)
			{
				if(m_ParentLayer != currentLayer )               //�ж�����ͼ���Ƿ���ͬ
				{
					m_bFirst = true;
					SetParentLayer(currentLayer);
				}
				MouseEvent(event);
			}
			if( currentLayer->LayerType()== Layer::ELayerType::Rain_Yubao_Layer)
			{
				if(m_ParentLayer != currentLayer )               //�ж�����ͼ���Ƿ���ͬ
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
				 if(m_ParentLayer != currentLayer )               //�ж�����ͼ���Ƿ���ͬ
				 {
					 m_bFirst = true;
					 SetParentLayer(currentLayer);
				 }
				 MouseDoubleClick(event);
			 }
			 if( currentLayer->LayerType()== Layer::ELayerType::Temperature_Layer)
			 {
				 if(m_ParentLayer != currentLayer )               //�ж�����ͼ���Ƿ���ͬ
				 {
					 m_bFirst = true;
					 SetParentLayer(currentLayer);
				 }
				 MouseDoubleClick(event);
			 }
			 if( currentLayer->LayerType()== Layer::ELayerType::Rain_Layer)
			 {
				 if(m_ParentLayer != currentLayer )               //�ж�����ͼ���Ƿ���ͬ
				 {
					 m_bFirst = true;
					 SetParentLayer(currentLayer);
				 }
				 MouseDoubleClick(event);
			 }
			 if( currentLayer->LayerType()== Layer::ELayerType::Rain_Yubao_Layer)
			 {
				 if(m_ParentLayer != currentLayer )               //�ж�����ͼ���Ƿ���ͬ
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
/* �������ܣ� ͶӰ�������						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void ManualDraw::CoordTrans(RadProjTransform *pProj4)
{
	//coordTrans = pProj4;
	geotrans.SetProj4Trans(pProj4);
}

/************************************************************************/
/* �������ܣ� ��������ǰͶӰ�����ת���ɾ�γ�����                      */
/* ����˵��:  �涨��γ�����											*/
/* �� �� ֵ:   NULL														*/
/* ��    ��:  ¬�ߕN													*/
/* ��    ��:  2011 �� 04 �� 13 ��										*/
/************************************************************************/
QVector3D ManualDraw::TransGLtoWGS84(QVector3D & glPostion)
{
	return geotrans.TransGLtoWGS84(glPostion);
}

/************************************************************************/
/* �������ܣ� ��������γ�����ת���ɵ�ǰͶӰ�����                      */
/* ����˵��:  �涨��γ�����											*/
/* �� �� ֵ:   NULL														*/
/* ��    ��:  ¬�ߕN													*/
/* ��    ��:  2011 �� 04 �� 13 ��										*/
/************************************************************************/
QVector3D ManualDraw::TransWGS84toGL(QVector3D & oriPostion)
{
	return geotrans.TransWGS84toGL(oriPostion);
}

/************************************************************************/
/* �������ܣ� �õ���������						��                      */
/* ����˵��:  �涨��γ�����											*/
/* �� �� ֵ:   NULL														*/
/* ��    ��:  ¬�ߕN													*/
/* ��    ��:  2011 �� 04 �� 13 ��										*/
/************************************************************************/
LAYER_ATTRIBUTE * ManualDraw::getILayerAttrib()
{
	return m_layerAttribute;
}
/************************************************************************/
/* �������ܣ� ��ʼ����ֵ������					                        */
/* ����˵��:  �Ƿ��Ǳպϵ�ֵ��											*/
/* �� �� ֵ:   NULL														*/
/* ��    ��:  �														*/
/* ��    ��:  2011 �� 04 �� 13 ��										*/
/************************************************************************/
void ManualDraw::InitAttribute(bool b)
{
	m_layerAttribute = new LAYER_ATTRIBUTE;
	QStringList n;

//	ILayerAttribute *cValue = new ILayerAttribute("cValue","cValue",tr("��ֵ����ֵ"),0,QVariant::Double,-10000.000000,10000.000000,n);
//	m_layerAttribute->insert(QString("cValue"),cValue);

	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor",tr("��ɫ"),QColor(255,0,0),QVariant::Color,0,0,n);
	m_layerAttribute->insert(QString("cColor"), cColor);

	if (b)
	{
		QStringList strFill;
		strFill.append(tr("��"));
		strFill.append(tr("��"));
		ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill",tr("���"),0,QVariant::Bool,0,0,strFill);
		m_layerAttribute->insert(QString("cFill"),cFill);
		ILayerAttribute *cFillColor = new ILayerAttribute("cFillColor","cFillColor",tr("�����ɫ"),QColor(0,0,255),QVariant::Color,0,0,n);
		m_layerAttribute->insert(QString("cFillColor"), cFillColor);
	}

	QStringList strKinds;
	strKinds.append(QString(tr("ʵ��")));
	strKinds.append(QString(tr("�̻���")));
	strKinds.append(QString(tr("����")));
	ILayerAttribute *cKinds = new ILayerAttribute("cKinds","cKinds",tr("����"),0,QVariant::StringList,0,0,strKinds);
	m_layerAttribute->insert(QString("cKinds"), cKinds);

	QStringList strWidth;
	for (int i=1; i<=10; i++)
	{
		strWidth.append(QString::number(i));
	}
	ILayerAttribute *cWidth = new ILayerAttribute("cWidth","cWidth",tr("�߿�"),2,QVariant::Int,1,10,strWidth);
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