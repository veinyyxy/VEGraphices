#include "DrawLineDistance.h"
#include "ILayerAttribute.h"
#include "SymbolAttribute.h"

DrawLineDistance::DrawLineDistance( int nType ,CoordinateTransform* proj)// : m_nType(nType)
{
	m_pDisplayLine = new Line;
	pText = new QEText();
	m_bFinished = false;
	m_pArray = NULL;
	m_pSymbolLine = NULL;
	m_bFirst = true;
	m_pSpLine =  new CCardinalSpline;
	m_pSymboldata = new SymbolData;
	InitAttribute(nType);
	m_nType = nType;
	m_proj = dynamic_cast<RadProjTransform*> (proj);
}




DrawLineDistance::~DrawLineDistance(void)
{
	if(NULL != m_pSpLine)
	{
		delete m_pSpLine;
		m_pSpLine = NULL;
	}
}

/************************************************************************/
/* �������ܣ� ��Ӧ����ƶ��¼�					                        */
/* ����˵��:  QRenderable* volume  : ����								*/
/*			  QMouseEvent* event   : ����								*/	
/* �� �� ֵ:  NULL														*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 07 �� 28 ��										*/
/************************************************************************/
void DrawLineDistance::MouseEvent(QMouseEvent* event)
{
	QVector3D  glPoint, scPoint(event->x(), event->y(), 0.52);
	if(m_InputData.size()<1) return;
	m_InputData.push_back(scPoint);
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
	QVector3DArray * pOriginalArray = new QVector3DArray;
	for(int i = 0 ; i < pArray->size(); i++)
	{
		QVector3D temp = pArray->at(i);
		CoordTrans(m_proj);
		pOriginalArray->push_back(TransGLtoWGS84(temp));
	}
	QVector3D p1,p2;
	p1 = m_proj->xyWorld2DC(pOriginalArray->at(0).x(),pOriginalArray->at(0).y());
	p2 = m_proj->xyWorld2DC(pOriginalArray->at(1).x(),pOriginalArray->at(1).y());
	double diac = sqrt((p1.x()-p2.x())*(p1.x()-p2.x())+(p1.y()-p2.y())*(p1.y()-p2.y()));
	diac = diac/1000.0;
	QColor m_FontColor = QColor(255,0,0, 255);
	pText->GetPositionArray()->clear();
	pText->SetFont(QFont(tr("����"), 10));
	pText->SetColor(m_FontColor);
	pText->SetOriPosition(QEText::CENTER);
	pText->GetPositionArray()->push_back(glPoint);
	
	pText->SetString(QString("%1(KM)").arg(diac));
	m_ParentLayer->GetLayerData()->InsertRenderable(pText);

 	m_ParentLayer->GetLayerData()->InsertRenderable(m_pDisplayLine);   
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
void DrawLineDistance::MousePress(QMouseEvent *event)
{
 	if(!m_Symbol) return;
 	QVector3D glPoint, scPoint(event->x(), event->y(), 0.52f);
 
  	if (event->buttons() & Qt::LeftButton && m_InputData.size()==0)
  	{	
  		    m_InputData.push_back(scPoint);
	}
	else
		m_InputData.clear();
}

/************************************************************************/
/* �������ܣ� ����ͷ�         			        		    			*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 04 �� 02 ��										*/
/************************************************************************/
void DrawLineDistance::MouseRelease(QMouseEvent *event)
{
	if(NULL != m_pSpLine)
	{
		delete m_pSpLine;
		m_pSpLine = NULL;
	}
	m_ParentLayer->GetLayerData()->RemoveRenderable(m_pDisplayLine);
	m_ParentLayer->GetLayerData()->RemoveRenderable(pText);
	m_InputData.clear();
}

/************************************************************************/
/* �������ܣ� ��ȡ���㣬û�а����Ҽ�����ʼ����켣��         			*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 04 �� 02 ��										*/
/************************************************************************/
void DrawLineDistance::LeaveEvent()
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

void DrawLineDistance::setILayerAttrib( ILayerAttribute *Ilayer )
{

}
/************************************************************************/
// �������ܣ� ��ʼ����ͷ�����Դ�
// �� �� ֵ:  void
// ��    ��:  �		
// ��    ��:  2011 �� 4 �� 13 ��
/************************************************************************/
void DrawLineDistance::InitAttribute( int nType )
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
void  DrawLineDistance::GetSymbolAttribute()
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
    m_pSymbolLine->m_LineAttribute.SetColor(QVector4D(tColor.red()/255.0, tColor.green()/255.0, tColor.blue()/255.0, 1.0f));

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
