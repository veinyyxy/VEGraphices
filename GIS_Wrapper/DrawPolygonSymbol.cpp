/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�ļ����� DrawPolygonSymbol.cpp 
�� ����  CDrawPolygonSymbol
�� �ࣺ  ManulDraw
�� �ࣺ  �� 
����˵��������������ѩ���ȶ���������û���������������
����˵����

-------------------------------------�汾����------------------
V 1.1 
�޸����ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ����ѩ
������ڣ�2011��07��22��
============================================================*/

#include "DrawPolygonSymbol.h"
#include "ILayerAttribute.h"
#include "SymbolAttribute.h"

//************************************************************************/
// �������ܣ� ��ʼ��������Ա����
// �� �� ֵ:  void
// ��    ��:  renxue	
// ��    ��:  2011 �� 7 �� 22 ��
//************************************************************************/
CDrawPolygonSymbol::CDrawPolygonSymbol( int nType ) : m_nType(nType)
{
	m_pLine = new Line;
	m_pPolygonLine = NULL;
	m_bFinished = false;
	m_bFirst = true;
	m_pSymboldata = NULL;
	initAttribute(nType);
	m_nType = nType;
	m_PolygonAttribute.SetSymbolType(1038);
}

CDrawPolygonSymbol::~CDrawPolygonSymbol(void)
{
}

void CDrawPolygonSymbol::MouseEvent(QMouseEvent* event)
{
}

void CDrawPolygonSymbol::MouseRelease(QMouseEvent *event)
{
}

/************************************************************************/
/* �������ܣ� �������ʱ��ȡ���㣬�����Ҽ�����      					*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 07 �� 22 ��										*/
/************************************************************************/
void CDrawPolygonSymbol::MousePress(QMouseEvent *event)
{
	QVector3D glPoint, scPoint(event->x(), event->y(), 0.0f);
	QVector3DArray  *pArray = m_pLine->getVectexArray();
	QVector4DArray  *pColor = m_pLine->getColorArray();

	if(m_Symbol)
	{
		if (event->buttons() & Qt::LeftButton)
		{
			if(!m_bFinished)
			{
				m_InputData.Clear();
				m_bFinished = true;
				pArray->Clear();
				pColor->clear();
			}			
			
			ScreenToGL(scPoint, glPoint);
			pColor->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
			pArray->push_back(glPoint);			
			m_pLine->SetColorArray();

			if(m_bFirst)
			{
				m_ParentLayer->GetLayerData()->InsertRenderable(m_pLine);
				m_bFirst = false;
			}
		}
		else if(event->buttons() & Qt::RightButton)
		{
			//�����Ҽ�����ʼ��˫��
			if(m_bFinished)				
			{
				//ֻ��һ���㣬�޷���ɶ����
				if(pArray->count() < 2)  return;    

				m_bFinished = false;
				m_pPolygonLine = new Line;
				QVector3DArray * pPolygonArray = m_pPolygonLine->getVectexArray(); 
				QVector3DArray * pOriginalPolygonArray = m_pPolygonLine->OriginalArray();
				QVector4DArray * pPolygonColor = m_pPolygonLine->getColorArray();
				GetUIAttribute();

				//���û�����ɫ
				pPolygonColor->Clear();
				QVector4D tColor=m_PolygonAttribute.GetColor();
				if(tColor.x()>255)
				{
					tColor = QVector4D(0.0, 0.0, 1.0 , 1.0);
				}

				//���û��ƶ���
				QVector3D tPoint;
				pPolygonArray->Clear();
				pOriginalPolygonArray->Clear();
				for(int i = 0 ; i < pArray->size(); i++)
				{
					pPolygonArray->push_back (pArray->at(i));
					tPoint = pArray->at(i);
					pOriginalPolygonArray->push_back(TransformData(tPoint));
					pPolygonColor->push_back(tColor);
				}
				m_pPolygonLine->SetColorArray();
				pArray->Clear();

				//�������͡��߿�
				int  nLineWidth = m_PolygonAttribute.GetLineWidth();
				QString strLineType = m_PolygonAttribute.GetLineType();
				if("0" == strLineType)
				{
					m_pPolygonLine->SetLineStyle(nLineWidth, 1, 0xFFFF);
				}
				else if("1" == strLineType)
				{
					m_pPolygonLine->SetLineStyle(nLineWidth, 2, 0xAAAA);
				}
				else if("2" == strLineType)
				{
					m_pPolygonLine->SetLineStyle(nLineWidth, 1, 0x0101);
				}
				
				//�����������
				if(m_PolygonAttribute.GetFillType())
				{
					m_pPolygonLine->Mode(GL_POLYGON);	
				}
				else
				{
					m_pPolygonLine->Mode(GL_LINE_STRIP);	
				}
				
				//undo����
				CUndoAddDrawCommand* addCommand = new CUndoAddDrawCommand();
				addCommand->SetUndoLayer(m_ParentLayer);
				addCommand->SetUndoRenderable(m_pPolygonLine);
				addCommand->SetUndoGLWidget(m_pOpenglView);
				if(m_undoStack)
				{
					m_undoStack->push(addCommand);
				}
				SymbolLayer *sLayer = dynamic_cast<SymbolLayer*>(m_ParentLayer);
				if (sLayer == NULL) return;

				if(!m_bFirst)
				{
					m_pSymboldata = sLayer->getSymbolData();
					m_pSymboldata->getTroughLine()->push_back(m_pPolygonLine);
					m_ParentLayer->GetLayerData()->RemoveRenderable(m_pLine);
					m_bFirst = true;
				}
			}
		}
	}
}
/************************************************************************/
/* �������ܣ� ����뿪�����������ԭ��δ��ɵĶ���ε�         		*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 07 �� 22 ��										*/
/************************************************************************/
void CDrawPolygonSymbol::LeaveEvent()
{
	//û�а����Ҽ�����ʼ����켣��
	if(m_bFinished)				
	{
		m_bFinished = false;
		QVector3DArray *pArray = m_pLine->getVectexArray();
		pArray->Clear();
		if(!m_bFirst)
		{
			m_ParentLayer->GetLayerData()->RemoveRenderable(m_pLine);
			m_bFirst = true;
		}
	}
}

void CDrawPolygonSymbol::setILayerAttrib( ILayerAttribute *Ilayer )
{
}

/************************************************************************/
// �������ܣ� ��ʼ������ε����Դ�
// �� �� ֵ:  void
// ��    ��:  renxue	
// ��    ��:  2011 �� 7 �� 22 ��
/************************************************************************/
void CDrawPolygonSymbol::initAttribute( int nType )
{
	m_layerAttribute = new LAYER_ATTRIBUTE;
	QStringList colorNum, strRegion;

	strRegion.append(QString("����"));
	strRegion.append(QString("ѩ��"));
	strRegion.append(QString("����"));
	strRegion.append(QString("�����"));
	strRegion.append(QString("ɳ����"));
	strRegion.append(QString("�ױ���"));
	ILayerAttribute *cValue = new ILayerAttribute("cValue","cValue","��������",0,QVariant::StringList,-0,0,strRegion);
	m_layerAttribute->insert(QString("cValue"),cValue);
	
	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor","��ɫ",QColor(0,0,255),QVariant::Color,0,0,colorNum);
	m_layerAttribute->insert(QString("cColor"),cColor);	

	QStringList strKinds;
	strKinds.append(QString("ʵ��"));
	strKinds.append(QString("�̻���"));
	strKinds.append(QString("����"));
	ILayerAttribute *cKinds = new ILayerAttribute("cKinds","cKinds","����",0,QVariant::StringList,0,0,strKinds);
	m_layerAttribute->insert(QString("cKinds"), cKinds);

	QStringList strWidth;
	for (int i=1; i<=10; i++)
	{
		strWidth.append(QString::number(i));
	}
	ILayerAttribute *cWidth = new ILayerAttribute("cWidth","cWidth","�߿�",1,QVariant::Int,1,50,strWidth);
	m_layerAttribute->insert(QString("cWidth"),cWidth);

	QStringList strFill;
	strFill.append("��");
	strFill.append("��");
	ILayerAttribute *cFill = new ILayerAttribute("cFill","cFill","�Ƿ����",0,QVariant::Bool,0,0,strFill);
	m_layerAttribute->insert(QString("cFill"),cFill);

}

//************************************************************************/
//* �������ܣ� ��ȡ�����ϵĶ��������         			        		*/
//* ����˵��:  ��                 										*/
//* �� �� ֵ:  ��                 										*/
//* ��    ��:  renxue													*/
//* ��    ��:  2011 �� 07 �� 22 ��										*/
//************************************************************************/
void  CDrawPolygonSymbol::GetUIAttribute()
{
	LAYER_ATTRIBUTE *pLA = getILayerAttrib();
	if(!pLA) return;
	QColor tColor;

	//����ֵ
	ILayerAttribute *pLayAttribute = pLA->value(QString("cValue"));
	pLayAttribute = pLA->value(QString("cValue"));
	m_PolygonAttribute.SetValue(pLayAttribute->Value().toString());

	//��ɫֵ
	pLayAttribute = pLA->value(QString("cColor"));
	tColor = pLayAttribute->Value().value<QColor>();
	m_PolygonAttribute.SetColor(QVector4D(tColor.redF(), tColor.greenF(), tColor.blueF(),tColor.alphaF()));

	//�߿�����
	pLayAttribute = pLA->value(QString("cWidth"));
	m_PolygonAttribute.SetLineWidth(pLayAttribute->Value().toInt());

	//��������
	pLayAttribute = pLA->value(QString("cKinds"));
	m_PolygonAttribute.SetLineType(pLayAttribute->Value().toString());

	//���
	pLayAttribute = pLA->value(QString("cFill"));
	if(NULL != pLayAttribute)
	{
		m_PolygonAttribute.SetFillType(pLayAttribute->Value().toBool());
	}
}


