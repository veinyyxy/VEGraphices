/*============================================================ 
�ļ�����DrawContourLine.cpp 
�� ���� DrawContourLine
�� �ࣺ ManualDraw
�� �ࣺ �� 
����˵��������ֵ�ߵ��㷨
����˵����
1����ȡ�û���PING�����������㣬�����²��㷨�����ؼ���õ��ĵ㼯��
2������Ļ��ת����OPENGL�����꣬��������Ӧ�Ļ�ͼ�ܵ���

-----------------------------�汾����-------------------------
============================================================*/
#include "Const.h"
#include "DrawContourArea.h"
#include <QtCore/QSettings>
#include <QtCore/QTextCodec>
#include "ILayerAttribute.h"
#include <qinputdialog.h>
#include "SaveColorTableToXML.h"
#include "ReadColorTableFromXML.h"
#include <QMessageBox>


typedef struct
{
	double dLon;
	double dLat;
	double dValue;
}ContourValue;

int DrawContourArea::m_nCount = 0;

DrawContourArea::DrawContourArea(Layer::ELayerType type):m_layertype(type)
{
	pline = new Line;
	m_bFinished = false;
	pArray = NULL;
	m_pContourLine = NULL;
	m_bFirst = true;
	m_pSpLine =  new CCardinalSpline;//����������������
	m_pSymbolData = new SymbolData;
	pOriginalArray = new QVector3DArray;
	m_fStep = 0.1f/100000;	
	m_LineColor.setRed(240.00f);
	m_LineColor.setGreen(0);
	m_LineColor.setBlue(10.00f);
	m_LineColor.setAlpha(255);
	if (colortablelist.isEmpty())
	{
		InitColorTable();		
	}
	if (m_layertype == Layer::ELayerType::Temperature_Layer) //�¶Ⱦ�ƽԤ��
	{ 
		m_fStep = 0.0001;
		SetCurrentColorTable(QString("0"));
	
	}
	if (m_layertype == Layer::ELayerType::Rain_Layer) //��ˮ��ƽԤ��
	{
		m_fStep = 0.0001;
		SetCurrentColorTable(QString("1"));	
	}	
	if (m_layertype == Layer::ELayerType::Rain_Yubao_Layer)//��ˮԤ��
	{
		m_fStep = 0.000001;
		SetCurrentColorTable(QString("2"));		
	}
	m_bCloseLine = true;
}

DrawContourArea::~DrawContourArea(void)
{
	//���ٻ�����������
	if(NULL != m_pSpLine)
	{
		delete m_pSpLine;
		m_pSpLine = NULL; 
	}
}

/************************************************************************/
/* �������ܣ� ���������ռ��㼯����������ʾ������Ҽ��������ռ�       	*/
/* ����˵��:  QMouseEvent ����¼�                 						*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 04 �� 25 ��										*/
/************************************************************************/
void DrawContourArea::MousePress(QMouseEvent *event)
{
	QVector3D glPoint, scPoint(event->x(), event->y(), 0.0f);	

	Layer::ELayerType type = GetParentLayer()->LayerType();

	if(type == Layer::ELayerType::Station_Layer)				//վ�������Ǵ���
	{
		GetParentLayer()->LayerWarning();
		return;
	}

	if(type != Layer::ELayerType::Symbol_Layer && type != m_layertype)
	{
		GetParentLayer()->LayerWarning();
		return;
	}	
	else
	{
		//����ͼ������
		if (m_layertype == Layer::ELayerType::Temperature_Layer)
		{
			GetParentLayer()->LayerType(Layer::ELayerType::Temperature_Layer);
			bool bDraw= GetParentLayer()->GetIsDraw();
			if(!bDraw)
			{
				AddTempCountorLine();	
				GetParentLayer()->SetIsDraw(true);
			}
		}
		if (m_layertype == Layer::ELayerType::Rain_Layer)
		{
			GetParentLayer()->LayerType(Layer::ELayerType::Rain_Layer);
			bool bDraw= GetParentLayer()->GetIsDraw();
			if(!bDraw)
			{
				AddRainCountorLine();		
				GetParentLayer()->SetIsDraw(true);
			}
		}		
		if (m_layertype == Layer::ELayerType::Rain_Yubao_Layer)
		{
			GetParentLayer()->LayerType(Layer::ELayerType::Rain_Yubao_Layer);
			bool bDraw= GetParentLayer()->GetIsDraw();
			if(!bDraw)
			{
				AddRainYUBAOCountorLine();		
				GetParentLayer()->SetIsDraw(true);
			}
		}	
	}

	///��ʼ��������
	if (event->buttons() & Qt::LeftButton)
	{
		//�������
		if(!m_bFinished)
		{
			m_InputData.Clear();
			m_CrossPoints.clear();
			m_bFinished = true;
			pArray = NULL;
		}

		pArray = pline->getVectexArray();
		m_InputData.push_back(scPoint);

		if(m_InputData.size()<3)
		{
			m_OutputData.Clear();
			for(int i=0; i<m_InputData.size(); i++)
				m_OutputData<<QVector3D(m_InputData[i].x(), m_InputData[i].y(), m_InputData[i].z());
		}
		else
		{	//ȡ��ƽ����ĵ�����
			m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);
		}

		QVector4DArray *pColor = pline->getColorArray();

		//����ϴεĵ�����
		pArray->Clear();
		pColor->clear();
		for(int i = 0 ; i < m_OutputData.count(); i++)
		{
			ScreenToGL(m_OutputData[i], glPoint);
			pColor->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
			pArray->push_back(glPoint);
		}
		pline->SetColorArray();

		if(m_bFirst)
		{
			m_ParentLayer->GetLayerData()->InsertRenderable(pline);
			m_bFirst = false;
		}

		//���ƿ��Ƶ�
		DrawCrossArray(m_InputData);
	}
	else if(event->buttons() & Qt::RightButton)
	{
		if(m_bFinished)				//�����Ҽ�����ʼ��˫��
		{
			m_bFinished = false;
			if(pArray->count() <2)  return;          //ֻ��һ���㣬�˳�

			m_pContourLine = new CSymbolContourLine;
			QVector3DArray * ArrayTemp = m_pContourLine->GetArray(); 
			pOriginalArray = m_pContourLine->OriginalArray();
			m_pContourLine->IsMark(true);
			m_pContourLine->HideBound(false);
			m_pContourLine->SetContourType((int)m_layertype);

			bool ok;
			double valueDouble = 0;
			QString strvalue;
		
			strvalue = QInputDialog::getItem(NULL, tr("��ֵ"),	tr("ѡ����ֵ:"), m_stringitems, 0, false, &ok);
			
			if (ok && !strvalue.isEmpty())
			{
				if (m_currentmap.contains(strvalue))
				{
                   valueDouble = m_currentmap.value(strvalue);
				}
			}

			if (!GetContourAttribute(valueDouble)) //���û�л�����������򷵻�,�����ʱ����
			{
				m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
				return;
			}

			m_ParentLayer->SetColorIndex(m_currentcolorindex);
			m_pContourLine->SetCloseLine(m_bCloseLine);

			QVector3D temp;
			if(m_bCloseLine)
			{
				temp = m_InputData[0];
				m_InputData<<temp;
				
				//ȡ��ƽ����ĵ�����
				m_OutputData.Clear();
				m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);

				if (m_lineanalysis.isPolygonintersect(m_OutputData))
				{
					QMessageBox msg;
					msg.setText(tr("������г����߶��ཻ��"));
					msg.exec();
					ClearCrossArray(m_InputData);
					m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
					m_bFirst = true;
					return;
				}

				float zValue = m_CoutourZArray.value(valueDouble);

				for(int i = 0 ; i < m_OutputData.count(); i++)
				{					
					ScreenToGL(m_OutputData[i], glPoint);
					glPoint.setZ(zValue);
					ArrayTemp->push_back(glPoint);
					temp = TransGLtoWGS84(glPoint);
					temp.setZ(zValue);
					pOriginalArray->push_back(temp);		
				}
			}
			pArray->Clear();

			ClearCrossArray(m_InputData);
			//undo����
			//CUndoAddDrawCommand* addCommand = new CUndoAddDrawCommand();
			//addCommand->SetUndoLayer(m_ParentLayer);		
			//addCommand->SetUndoRenderable(m_pContourLine);
			//addCommand->SetUndoGLWidget(m_pOpenglView);
			//if(m_undoStack)
			//{
			//	m_undoStack->push(addCommand);
			//}

			//�����Ƶ�ֵ�ߵĶ��󱣴��ڷ��Ų���
			SymbolLayer *sl = dynamic_cast<SymbolLayer*>(m_ParentLayer);
			if (sl == NULL)
			{
				return;
			}

			if(!m_bFirst)
			{
				m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
				m_bFirst = true;
			}
		}
	}	
	m_ParentLayer->TransformLayer();
}

/************************************************************************/
/* �������ܣ� ��Ӧ����ƶ��¼�					                        */
/* ����˵��:  QRenderable* volume  : ����								*/
/*			  QMouseEvent* event   : ����								*/	
/* �� �� ֵ:  NULL														*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 05 �� 10 ��										*/
/************************************************************************/
void DrawContourArea::MouseEvent(QMouseEvent* event)
{
	QVector3D  glPoint, scPoint(event->x(), event->y(), 1.0f);

	if(m_bFinished)
	{
		//ȡ��ƽ����ĵ�����

		m_InputData<<scPoint;
		m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);
		QVector4DArray *pColor = pline->getColorArray();
		pArray = pline->getVectexArray();
		//����ϴεĵ�����
		pArray->Clear();			
		pColor->clear();
		int j=0;
		QVector3D topLefPoint, topRightPoint, bottomLeftPoint, bottomRightPoint;
		for(int i = 0 ; i < m_OutputData.count(); i++)
		{
			ScreenToGL(m_OutputData[i], glPoint);
			glPoint.setZ(1.0f);
			pColor->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
			pArray->push_back(glPoint);
		}
		pline->SetColorArray();

		if(m_bFirst)
		{
			m_ParentLayer->GetLayerData()->InsertRenderable(pline);
		}
		m_InputData.pop_back();
		m_OutputData.Clear();
	}
}

/************************************************************************/
/* �������ܣ� ��Ӧ��굯���¼�					                        */
/* ����˵��:  QRenderable* volume  : ����								*/
/*			  QMouseEvent* event   : ����								*/	
/* �� �� ֵ:   NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 08 ��										*/
/************************************************************************/
void DrawContourArea::MouseRelease(QMouseEvent *event)
{
	return;
}

/************************************************************************/
/* �������ܣ� ����뿪�¼�					                            */
/* ����˵��:  QRenderable* volume  : ����								*/
/*			  QMouseEvent* event   : ����								*/	
/* �� �� ֵ:   NULL														*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 06 �� 29 ��										*/
/************************************************************************/
void DrawContourArea::LeaveEvent()
{
	if(m_bFinished)				//û�а����Ҽ�������ƿ�
	{
		m_bFinished = false;

		//�����¼
		if(pArray != NULL)
			pArray->Clear();

		ClearCrossArray(m_InputData);
		if(!m_bFirst)
		{
			m_ParentLayer->GetLayerData()->RemoveRenderable(pline);
			m_bFirst = true;
		}
	}
	return;
}


void DrawContourArea::setILayerAttrib( ILayerAttribute *Ilayer )
{
//	ILayerAttribute *Ilayer = m_layerAttribute->value(QString("cFill"));

}

void DrawContourArea::InitAttribute(bool b)
{
	m_layerAttribute = new LAYER_ATTRIBUTE;	

	if (b)
	{
		QStringList strFill;
		strFill.append(tr("��"));
		strFill.append(tr("��"));
		ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill",tr("���"),0,QVariant::Bool,0,0,strFill);
		m_layerAttribute->insert(QString("cFill"),cFill);
	}

}
/************************************************************************/
/* �������ܣ� ��������ֵ������ɫ����       			        					*/
/* ����˵��:  ��                 										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  zhanglei													*/
/* ��    ��:  2011 �� 04 �� 25 ��										*/
/************************************************************************/
bool  DrawContourArea::GetContourAttribute(double index)
{
	CSymbolAttribute * pAttr = m_pContourLine->GetAttribute();	
	if (m_currentColorTable.contains(index))
	{
		QColor tempcolor = m_currentColorTable.value(index)._color;
		int value = index;
		if(m_layertype ==  Layer::ELayerType::Rain_Layer)
		{
			switch(value)
			{
			case 50:
				index = 50;
				break;
			case 20:
				index = 20;
				break;
			case 0:
				index = 10;
				break;
			case -10:
				index = -10;
				break;
			case -20:
				index = -20;
				break;
			case -50:
				index = -50;
				break;
			default:
				break;
			}
		}

		pAttr->SetValue(index);
		pAttr->SetColor(QVector4D(m_LineColor.redF(),m_LineColor.greenF(),m_LineColor.blueF(),m_LineColor.alphaF()));
		pAttr->SetFillType(false);	
		pAttr->SetFillColor(QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),tempcolor.alphaF()));
		pAttr->SetLineWidth(2.0f);	
		pAttr->SetLineType(QString(tr("0")));
		pAttr->SetLayerZ(0);
		return true;
	}
	else
	{
		return false;
	}
}


void DrawContourArea::InitColorTable()
{
	ReadColorTableFromXML reader;	
	reader.readFile(CIPASApplication::ConfigPath()+ "BasicInfo/CipasColorTable.xml",colortablelist);
}


void DrawContourArea::SetCurrentColorTable(QString str)
{
	if (colortablelist.contains(str))
	{
		m_currentColorTable = colortablelist.value(str);
		
		m_currentcolorindex.clear();
		m_stringitems.clear();
		m_currentmap.clear();
		m_CoutourZArray.clear();
		
		COLORTABLE::Iterator it;
		for (it=m_currentColorTable.begin();it!=m_currentColorTable.end();++it)
		{
			m_currentcolorindex.insert(it.key(),COLORITEM(it.value()._color,QString("")));
			m_currentmap.insert(it.value()._Name,it.key());
			
			if (it.value()._Zcount!=0)
			{
				m_stringitems.push_back(it.value()._Name);	
			}			
			m_CoutourZArray.insert(it.key(),it.value()._Zcount*m_fStep);
		}
		
	}
}

void DrawContourArea::AddRainCountorLine()
{

	CSymbolContourLine *pContourLine = new CSymbolContourLine;
	QVector3DArray * ArrayTemp = pContourLine->GetArray();
	QVector3DArray * pOriginalArray = pContourLine->OriginalArray();
	pContourLine->SetCloseLine(true);
	pContourLine->IsMark(false);

	//��������ֵ
	CSymbolAttribute * pAttr = pContourLine->GetAttribute();
	pAttr->SetValue(-10);
	QColor tempcolor =QColor(255.0f,255.0f,64.0f,255.0f);
	pAttr->SetColor(QVector4D(m_LineColor.redF(),m_LineColor.greenF(),m_LineColor.blueF(),m_LineColor.alphaF()));
	pAttr->SetFillType(false);
	pAttr->SetFillColor( QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),tempcolor.alphaF()));
	pAttr->SetLineWidth(2.0f);
	pAttr->SetLineType(QString(tr("0")));
	pAttr->SetLayerZ(0);

	for(int i = 0 ; i < 221; i++)
	{
		QVector3D temp(contourArray[i][0], contourArray[i][1],0);
		ArrayTemp->push_back(temp);
		pOriginalArray->push_back(temp);
	}

	Layer* layer = GetParentLayer();
	SymbolLayer * sl = dynamic_cast<SymbolLayer*> (layer);

	if (sl!=NULL)
	{
 		SymbolData * pSymbolData = sl->getSymbolData();	// ȡ�÷��Ų�Ķ���
 		if(pSymbolData)
 		{
 			pSymbolData->getSymbolContourLine()->push_back(pContourLine);
 		}

		GetParentLayer()->GetLayerData()->InsertRenderable(pContourLine);
		GetParentLayer()->TransformLayer();
	}
}

void DrawContourArea::AddRainYUBAOCountorLine()
{
	CSymbolContourLine *pContourLine = new CSymbolContourLine;
	QVector3DArray * ArrayTemp = pContourLine->GetArray();
	QVector3DArray * pOriginalArray = pContourLine->OriginalArray();
	pContourLine->SetCloseLine(true);
		pContourLine->IsMark(true);
	//��������ֵ
	CSymbolAttribute * pAttr = pContourLine->GetAttribute();
	pAttr->SetValue(10);
	QColor tempcolor =QColor(255.0f,255.0f,0.0f,255.0f);
	pAttr->SetColor(QVector4D(m_LineColor.redF(),m_LineColor.greenF(),m_LineColor.blueF(),m_LineColor.alphaF()));
	pAttr->SetFillType(false);
	pAttr->SetFillColor( QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),tempcolor.alphaF()));
	pAttr->SetLineWidth(2.0f);
	pAttr->SetLineType(QString("0"));
	pAttr->SetLayerZ(0);

	for(int i = 0 ; i < 221; i++)
	{
		QVector3D temp(contourArray[i][0], contourArray[i][1],0);
		ArrayTemp->push_back(temp);
		pOriginalArray->push_back(temp);
	}

	Layer* layer = GetParentLayer();
	SymbolLayer * sl = dynamic_cast<SymbolLayer*> (layer);

	if (sl!=NULL)
	{
 		SymbolData * pSymbolData = sl->getSymbolData();	// ȡ�÷��Ų�Ķ���
 		if(pSymbolData)
 		{
 			pSymbolData->getSymbolContourLine()->push_back(pContourLine);
 		}

		GetParentLayer()->GetLayerData()->InsertRenderable(pContourLine);
		GetParentLayer()->TransformLayer();
	}
}

void DrawContourArea::AddTempCountorLine()
{
	CSymbolContourLine *pContourLine = new CSymbolContourLine;
	QVector3DArray * ArrayTemp = pContourLine->GetArray();
	QVector3DArray * pOriginalArray = pContourLine->OriginalArray();
	pContourLine->SetCloseLine(true);
	pContourLine->HideBound(false);			
	pContourLine->IsMark(true);
	//��������ֵ
	CSymbolAttribute * pAttr = pContourLine->GetAttribute();
	pAttr->SetValue(0);
	QColor tempcolor =QColor(255.0f,255.0f,0.0f,255.0f);
	pAttr->SetColor(QVector4D(m_LineColor.redF(),m_LineColor.greenF(),m_LineColor.blueF(),m_LineColor.alphaF()));
	pAttr->SetFillType(false);
	pAttr->SetFillColor( QVector4D(tempcolor.redF(),tempcolor.greenF(),tempcolor.blueF(),tempcolor.alphaF()));
	pAttr->SetLineWidth(2.0f);
	pAttr->SetLineType(QString(tr("ʵ��")));
	pAttr->SetLayerZ(0);

	for(int i = 0 ; i < 221; i++)
	{
		QVector3D temp(contourArray[i][0], contourArray[i][1],0);
		ArrayTemp->push_back(temp);
		pOriginalArray->push_back(temp);
	}

	Layer* layer = GetParentLayer();
	SymbolLayer * sl = dynamic_cast<SymbolLayer*> (layer);
	
	if (sl!=NULL)
	{
 		SymbolData * pSymbolData = sl->getSymbolData();	// ȡ�÷��Ų�Ķ���
 		if(pSymbolData)
 		{
 			pSymbolData->getSymbolContourLine()->push_back(pContourLine);
 		}

		GetParentLayer()->GetLayerData()->InsertRenderable(pContourLine);
		GetParentLayer()->TransformLayer();		
	}
}

/************************************************************************/
/*����ʮ���߹���                                                        */
/************************************************************************/
void DrawContourArea::DrawCrossArray(QVector3DArray&  InputData)
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

/************************************************************************/
/* ���ʮ���߹���                                                                     */
/************************************************************************/
void DrawContourArea::ClearCrossArray(QVector3DArray&  InputData)
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