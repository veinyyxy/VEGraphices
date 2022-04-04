#include "DrawRectangle.h"
#include "ILayerAttribute.h"
#include "SymbolAttribute.h"
DrawRectangle::DrawRectangle( int nType ,CoordinateTransform* proj,bool dr) : m_nType(nType)
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
	OnlydrawTP = dr;
}

DrawRectangle::DrawRectangle( int nType ) : m_nType(nType)
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
	m_proj = new RadProjTransform;
	OnlydrawTP = true;
}



DrawRectangle::~DrawRectangle(void)
{
}

/************************************************************************/
/* �������ܣ� ��Ӧ����ƶ��¼�					                        */
/* ����˵��:  QRenderable* volume  : ����								*/
/*			  QMouseEvent* event   : ����								*/	
/* �� �� ֵ:  NULL														*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 07 �� 28 ��										*/
/************************************************************************/
void DrawRectangle::MouseEvent(QMouseEvent* event)
{
	QVector3D  glPoint, scPoint(event->x(), event->y(), 0);
	if(m_InputData.size()<1) return;
	//ȡ��ƽ����ĵ�����
	m_InputData.push_back(scPoint);

	{
		QVector3D rup,ldp;
		QVector3D temp_lup = m_InputData.at(0);
		QVector3D temp_rdp = m_InputData.at(1);
		rup.setX(temp_rdp.x());
		rup.setY(temp_lup.y());
		ldp.setX(temp_lup.x());
		ldp.setY(temp_rdp.y());
		m_InputData.Clear();
		m_InputData.push_back(temp_lup);
		m_InputData.push_back(rup);
		m_InputData.push_back(temp_rdp);
		m_InputData.push_back(ldp);
		m_InputData.push_back(temp_lup);
	}

//	m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);
	m_OutputData.Clear();
	for(int i=0; i<m_InputData.size(); i++)
		m_OutputData<<QVector3D(m_InputData[i].x(), m_InputData[i].y(), m_InputData[i].z());
	QVector4DArray *pColor = m_pDisplayLine->getColorArray();
	QVector3DArray *pArray = m_pDisplayLine->getVectexArray();
	//����ϴεĵ�����
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

	m_InputData.pop_back();
	m_InputData.pop_back();
	m_InputData.pop_back();
	m_InputData.pop_back();
	m_OutputData.Clear();
}

/************************************************************************/
/* �������ܣ� ��ȡ����         			        					*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 04 �� 02 ��										*/
/************************************************************************/
void DrawRectangle::MousePress(QMouseEvent *event)
{
	QVector3D ldp,rup;
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
		m_pArray->Clear();	//����ϴεĵ�����		
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
/* �������ܣ� ����ͷ�         			        		    			*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 04 �� 02 ��										*/
/************************************************************************/
void DrawRectangle::MouseRelease(QMouseEvent *event)
{
	QVector3D ldp,rup;
	QVector3D glPoint, scPoint(event->x(), event->y(), 0.52f);
	if(/*event->buttons() & */ Qt::LeftButton &&m_pressNum == 1)
	{
		if(!m_bFinished)
		{
			m_pressNum = 0;
			m_bFinished = false;
			m_InputData.clear();
			return;
		}		
		m_InputData.push_back(scPoint);
		p = new QVector3DArray;
		p->push_back(m_InputData.at(0));
		p->push_back(m_InputData.at(1));
		{
			QVector3D temp_lup = m_InputData.at(0);
			QVector3D temp_rdp = m_InputData.at(1);
			rup.setX(temp_rdp.x());
			rup.setY(temp_lup.y());
			ldp.setX(temp_lup.x());
			ldp.setY(temp_rdp.y());
			m_InputData.Clear();
			m_InputData.push_back(temp_lup);
			m_InputData.push_back(rup);
			m_InputData.push_back(temp_rdp);
			m_InputData.push_back(ldp);
			m_InputData.push_back(temp_lup);
		}

		{
			m_OutputData.Clear();
			for(int i=0; i<m_InputData.size(); i++)
				m_OutputData<<QVector3D(m_InputData[i].x(), m_InputData[i].y(), m_InputData[i].z());
		}
		m_InputData.clear();
		QVector4DArray *pColor = m_pDisplayLine->getColorArray();
		m_pArray = m_pDisplayLine->getVectexArray();		
		m_pArray->Clear();	//����ϴεĵ�����		
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
		if(m_bFinished)				//�ڶ��ΰ����������ʼ��˫��
		{
			m_bFinished = false;
			if(m_pArray->count() <2)  return;          //ֻ��һ���㣬�˳�

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

			p1 = m_proj->xyWorld2DC(pOriginalArray->at(0).x(),pOriginalArray->at(0).y());
			p2 = m_proj->xyWorld2DC(pOriginalArray->at(2).x(),pOriginalArray->at(2).y());
			//central = m_proj->xyWorld2DC(m_proj->m_central.x(),m_proj->m_central.y());
			p1.setX(p1.x()/1000.0);
			p1.setY(p1.y()/1000.0);
			p2.setX(p2.x()/1000.0);
			p2.setY(p2.y()/1000.0);
			
			p->push_back(p1);
			p->push_back(p2);
			p->push_back(pOriginalArray->at(0));
			p->push_back(pOriginalArray->at(2));
			m_pArray->Clear();
			m_pSymbolLine->SetSymbolLineStyle(m_Symbol);
			if (OnlydrawTP == false)
			{
				emit DrawAction();
			}

			m_pressNum = 0;

			m_ParentLayer->GetLayerData()->InsertRenderable(m_pSymbolLine);
			m_pSymbolLine->SetColorArray();
			//undo����
 /*			CUndoAddDrawCommand* addCommand = new CUndoAddDrawCommand();
 			addCommand->SetUndoLayer(m_ParentLayer);
 			addCommand->SetUndoRenderable(m_pSymbolLine);
 			addCommand->SetUndoGLWidget(m_pOpenglView);*/
 /*			if(m_undoStack)
 			{
 				m_undoStack->push(addCommand);
 			}*/
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
/* �������ܣ� ��ȡ���㣬û�а����Ҽ�����ʼ�����켣��         			*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 04 �� 02 ��										*/
/************************************************************************/
void DrawRectangle::LeaveEvent()
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

void DrawRectangle::setILayerAttrib( ILayerAttribute *Ilayer )
{

}
/************************************************************************/
// �������ܣ� ��ʼ����ͷ�����Դ�
// �� �� ֵ:  void
// ��    ��:  �		
// ��    ��:  2011 �� 4 �� 13 ��
/************************************************************************/
void DrawRectangle::InitAttribute( int nType )
{
	m_layerAttribute = new LAYER_ATTRIBUTE;

	//��ɫ
	QStringList nColorNum;
	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor",tr("��ɫ"),QColor(0,0,255),QVariant::Color,0,0,nColorNum);
	m_layerAttribute->insert(QString("cColor"),cColor);

	//����
	QStringList strKinds;
	strKinds.append(QString(tr("ʵ��")));
	strKinds.append(QString(tr("�̻���")));
	strKinds.append(QString(tr("����")));
	ILayerAttribute *cKinds = new ILayerAttribute("cKinds","cKinds",tr("����"),0,QVariant::StringList,0,0,strKinds);
	m_layerAttribute->insert(QString("cKinds"), cKinds);

	//�߿�
	QStringList strWidth;
	for (int i=1; i<=10; i++)
	{
		strWidth.append(QString::number(i));
	}
	ILayerAttribute *cWidth = new ILayerAttribute("cWidth","cWidth",tr("�߿�"),1,QVariant::Int,1,50,strWidth);
	m_layerAttribute->insert(QString("cWidth"),cWidth);


	if(nType == 1101)//��ֵ�ߵı�ֵ
	{
		QStringList nContourValue;
		ILayerAttribute *cValue = new ILayerAttribute("cValue","cValue",tr("��ֵ"),0,QVariant::Double,-10000.000000,10000.000000,nContourValue);
		m_layerAttribute->insert(QString("cValue"),cValue);
	}	
	else if(nType == 1308)//����ε������ֵ
	{
		QStringList strRegion;
		strRegion.append(QString(tr("����")));
		strRegion.append(QString(tr("ѩ��")));
		strRegion.append(QString(tr("����")));
		strRegion.append(QString(tr("�����")));
		strRegion.append(QString(tr("ɳ����")));
		strRegion.append(QString(tr("�ױ���")));
		ILayerAttribute *cValue = new ILayerAttribute("cValue","cValue",tr("��������"),0,QVariant::StringList,-0,0,strRegion);
		m_layerAttribute->insert(QString("cValue"),cValue);
	}

	if (nType==1101 || nType==1308)//1101��ʾ��ֵ�ߣ�1308��ʾ�����
	{
		QStringList strFill;
		strFill.append(tr("��"));
		strFill.append(tr("��"));
		ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill",tr("���"),0,QVariant::Bool,0,0,strFill);
		m_layerAttribute->insert(QString("cFill"),cFill);
	}
}

/************************************************************************/
/* �������ܣ� ��ȡ��������         			        					*/
/* ����˵��:  ��                 										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 04 �� 19 ��										*/
/************************************************************************/
void  DrawRectangle::GetSymbolAttribute()
{
	LAYER_ATTRIBUTE *pLA = getILayerAttrib();
	if(!pLA) return;

	//�������ͱ��
	ILayerAttribute *pLayAttribute;	
	m_pSymbolLine->m_LineAttribute.SetSymbolType(m_nType);

	//������ɫ
	pLayAttribute = pLA->value(QString("cColor"));
	if(!pLayAttribute) return;
	QColor tColor = pLayAttribute->Value().value<QColor>();;
	m_pSymbolLine->m_LineAttribute.SetColor(QVector3D(tColor.red()/255.0, tColor.green()/255.0, tColor.blue()/255.0));

	//��������
	pLayAttribute = pLA->value(QString("cKinds"));
	if(!pLayAttribute) return;
	m_pSymbolLine->m_LineAttribute.SetLineType(pLayAttribute->Value().toString());

	//�����߿�
	pLayAttribute = pLA->value(QString("cWidth"));
	if(!pLayAttribute) return;
	m_pSymbolLine->m_LineAttribute.SetLineWidth(pLayAttribute->Value().toInt());


	if(m_nType == 1101)//��ֵ�ߵı�ֵ
	{
		pLayAttribute = pLA->value(QString("cValue"));
		if(!pLayAttribute) return;
		m_pSymbolLine->m_LineAttribute.SetValue(pLayAttribute->Value().toDouble());
	}	
	else if(m_nType == 1308)//����ε������ʶ
	{
		pLayAttribute = pLA->value(QString("cValue"));
		if(!pLayAttribute) return;
		int nRegion = pLayAttribute->Value().toInt();
		QString strRegion;
		switch(nRegion)
		{
		case 0:
			strRegion = tr("����");
			break;
		case 1:
			strRegion = tr("ѩ��");
			break;
		case 2:
			strRegion = tr("����");
			break;
		case 3:
			strRegion = tr("�����");
			break;
		case 4:
			strRegion = tr("ɳ����");
			break;
		case 5:
			strRegion = tr("�ױ���");
			break;
		}
		m_pSymbolLine->m_LineAttribute.SetValue(strRegion);
	}	

	//�����������
	if(m_nType == 1003 || m_nType ==1005)//���ļ�ͷ
	{
		m_pSymbolLine->m_LineAttribute.SetFillType(true);
	}
	else if(m_nType == 1101 || m_nType ==1308)//��ֵ�� �� �����
	{
		pLayAttribute = pLA->value(QString("cFill"));
		if(!pLayAttribute) return;
		m_pSymbolLine->m_LineAttribute.SetFillType(pLayAttribute->Value().toBool());
	}
}