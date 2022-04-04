#include "DrawTroughLine.h"


DrawTroughLine::DrawTroughLine(int type)
{
	plineSymbol		= new Line;
	m_symbolLine	= NULL;
	line			= new Line;
	bFinished		= false;
	pArray			= NULL;
	m_OutLineNum	= 0;	
	m_bFirst		= true;
	m_pSpLine		= new CCardinalSpline;
	m_pSymbolData = new SymbolData;
	initAttribute(1);
}

DrawTroughLine::~DrawTroughLine(void)
{
	if(plineSymbol)
	{
		delete plineSymbol;
		plineSymbol = NULL;
	}
	
	/*delete m_symbolLine;
	m_symbolLine = NULL;*/
	if(line)
	{
		delete line;
		line = NULL;
	}
	
	if(m_pSpLine)
	{
		delete m_pSpLine;
		m_pSpLine = NULL;
	}	
}
/************************************************************************/
// �������ܣ� ����¼�
// �� �� ֵ:  void
// ��    ��:  �		
// ��    ��:  2011 �� 4 �� 19 ��
/************************************************************************/
void DrawTroughLine::MouseEvent( QMouseEvent* event )
{

}
/************************************************************************/
// �������ܣ� ������¼�
// �� �� ֵ:  void
// ��    ��:  �		
// ��    ��:  2011 �� 4 �� 19 ��
/************************************************************************/
void DrawTroughLine::MousePress( QMouseEvent *event )
{
	QVector3D glPoint, scPoint(event->x(), event->y(), 0.0f);

	LAYER_ATTRIBUTE *pLA = getILayerAttrib();
	if(!pLA) return;
	QColor tColor;


	ILayerAttribute *pILayer = pLA->value(QString("cColor"));
	if(pILayer)
	{
		tColor = pILayer->Value().value<QColor>();
	}
	QVector4D v4D(tColor.redF(), tColor.blueF(),tColor.yellowF() , 1.0);

	if (event->buttons() & Qt::LeftButton)
	{
		if(!bFinished)
		{
			m_InputData.Clear();
			bFinished = true;
			pArray = NULL;
		}
		pArray = line->getVectexArray();
		m_InputData.push_back(scPoint);

		if(m_InputData.size()<3)
		{
			m_OutputData.Clear();
			for(int i=0; i<m_InputData.size(); i++)
				m_OutputData<<QVector3D(m_InputData[i].x(), m_InputData[i].y(), m_InputData[i].z());
		}
		else
		{
			//ȡ��ƽ����ĵ�����
			m_pSpLine->GetSmoothPoints(m_InputData, m_OutputData);
		}
		QVector4DArray *pColor = line->getColorArray();

		//����ϴεĵ�����
		pArray->Clear();			
		pColor->clear();
		for(int n = 0 ; n < m_OutputData.count(); n++)
		{
			ScreenToGL(m_OutputData[n], glPoint);
			pColor->push_back(v4D/*QVector4D(1.0, 0.0, 0.0, 1.0)*/);
			pArray->push_back(glPoint);
		}

		line->SetColorArray();
		if(pArray->count() <2)  return ;

		if(m_bFirst)
		{
			m_ParentLayer->GetLayerData()->InsertRenderable(line);
			m_bFirst = false;
		}
	}
	else if(event->buttons() & Qt::RightButton)
	{
		if(bFinished)				//�����Ҽ�����ʼ��˫��
		{
			bFinished = false;
			if(pArray->count() <2)  
				return;          //ֻ��һ���㣬�˳�
			m_symbolLine = new Line;
			
			QVector3DArray * ArrayTemp = m_symbolLine->getVectexArray(); 
			QVector3DArray * pOriginalArray = m_symbolLine->OriginalArray();
			for(int i = 0 ; i < pArray->size(); i++)
			{
				ArrayTemp->push_back (pArray->at(i));
				QVector3D temp = pArray->at(i);
				pOriginalArray->push_back(TransformData(temp));
			}

			QVector4DArray *pColor = m_symbolLine->getColorArray();

			//����ϴεĵ�����
			pArray->Clear();			
			pColor->clear();


			for(int n = 0 ; n < m_OutputData.count(); n++)
			{
				ScreenToGL(m_OutputData[n], glPoint);
				//pColor->push_back(QVector4D(1.0, 0.0, 0.0, 1.0));
				pColor->push_back(QVector4D(tColor.redF(), tColor.greenF(),tColor.blueF() , 1.0));
				pArray->push_back(glPoint);
			}
			m_symbolLine->SetColorArray();
			m_ParentLayer->GetLayerData()->InsertRenderable(m_symbolLine);

			SymbolLayer *smybolLayer = dynamic_cast<SymbolLayer*>(m_ParentLayer);
			m_pSymbolData = smybolLayer->getSymbolData();
			m_pSymbolData->getTroughLine()->push_back(m_symbolLine);

			if(!m_bFirst)
			{
				m_ParentLayer->GetLayerData()->RemoveRenderable(line);
				m_bFirst = true;
			}
		}
	}
}
/************************************************************************/
// �������ܣ� ������뿪�¼�
// �� �� ֵ:  void
// ��    ��:  �		
// ��    ��:  2011 �� 4 �� 19 ��
/************************************************************************/
void DrawTroughLine::MouseRelease( QMouseEvent *event )
{

}

/************************************************************************/
/* �������ܣ� ����뿪�¼�         			        					*/
/* ����˵��:  ����¼�           										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 06 �� 29 ��										*/
/************************************************************************/
void DrawTroughLine::LeaveEvent()
{
	if(bFinished)				//�����Ҽ�����ʼ��˫��
	{
		bFinished = false;
		m_ParentLayer->GetLayerData()->RemoveRenderable(line);
	}
}

void DrawTroughLine::setILayerAttrib( ILayerAttribute *Ilayer )
{

}
/************************************************************************/
// �������ܣ� ��ʼ������ֵ
// �� �� ֵ:  void
// ��    ��:  �		
// ��    ��:  2011 �� 4 �� 19 ��
/************************************************************************/
void DrawTroughLine::initAttribute( int nType )
{
	m_layerAttribute = new LAYER_ATTRIBUTE;
	QStringList n;

	QStringList strWidth;
	for (int i=1; i<=20; i++)
	{
		strWidth.append(QString::number(i));
	}
// 	ILayerAttribute *cValue = new ILayerAttribute("cValue","�߿�",1,QVariant::String,1,20,strWidth);
// 	m_layerAttribute->push_back(cValue);

	ILayerAttribute *cColor = new ILayerAttribute("cColor","cColor","��ɫ",QColor(255,0,0),QVariant::Color,0,0,n);
	m_layerAttribute->insert(QString("cColor"),cColor);

// 	QStringList strKinds;
// 	strKinds.append(QString("ʵ��"));
// 	strKinds.append(QString("����"));
// 	strKinds.append(QString("�㻮��"));
// 	strKinds.append(QString("˫�㻮��"));
// 	strKinds.append(QString("����"));
// 	ILayerAttribute *cKinds = new ILayerAttribute("cKinds","��������",QString("ʵ��"),QVariant::String,0,0,strKinds);
// 	m_layerAttribute->push_back(cKinds);
}
