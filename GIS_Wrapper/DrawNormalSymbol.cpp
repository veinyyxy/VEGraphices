#include "DrawNormalSymbol.h"


DrawNormalSymbol::DrawNormalSymbol(void)
{
	symboldata		= new SymbolData;
	nsLayer			= new SymbolLayer;
	symbolpoint		= new NormalSymbol;
	plineSymbol		= new Line;
	m_symbolLine	= NULL;
	line			= new Line;
	bFinished		= false;
	pArray			= NULL;
	m_OutLineNum	= 0;	
	m_bFirst		= true;
	m_pSpLine		= new CCardinalSpline;
	b1 = true;
	b2 = true;
	m_CSlayerAttribute = NULL;
	initAttribute(1);
}


DrawNormalSymbol::~DrawNormalSymbol(void)
{
}
/************************************************************************/
// �������ܣ� ����¼�
// �� �� ֵ:  void
// ��    ��:  �		
// ��    ��:  2011 �� 4 �� 19 ��
/************************************************************************/
void DrawNormalSymbol::MouseEvent( QMouseEvent* event )
{

}
/************************************************************************/
// �������ܣ� ������¼�
// �� �� ֵ:  void
// ��    ��:  �		
// ��    ��:  2011 �� 4 �� 19 ��
/************************************************************************/
void DrawNormalSymbol::MousePress( QMouseEvent *event )
{
	int nIndex;
	m_CSlayerAttribute = getILayerAttrib();
	pAttri = new CSymbolAttribute;
	QColor tColor;

// 	m_nLayerAttr = m_CSlayerAttribute->value(QString("SymbolShape"));
// 	nIndex = m_nLayerAttr->Value().toInt();
	nIndex = 0 ;

	m_nLayerAttr = m_CSlayerAttribute->value(QString("borderColor"));
	tColor = m_nLayerAttr->Value().value<QColor>();
    pAttri->SetColor(QVector4D(tColor.red()/255.0, tColor.green()/255.0, tColor.blue()/255.0, 1.0f));

	m_nLayerAttr = m_CSlayerAttribute->value(QString("symbolSize"));
	pAttri->SetValue(m_nLayerAttr->Value().toFloat());


	QVector3D glPoint, scPoint(event->x(), event->y(), 0.0f);

	QVector3DArray *pOriginalArray = symbolpoint->OriginalArray();

	//CUndoAddPointCommand* undoAddPointCommand = new CUndoAddPointCommand;
	//undoAddPointCommand->SetSymbolPoint(symbolpoint);
	//undoAddPointCommand->SetGLWdiget(m_pOpenglView);

	if(m_Symbol)
	{
		//CSymbolAttribute *Attri = new CSymbolAttribute;
		pAttri->SetSymbolType(nIndex);
		//GetAttribute();
		ScreenToGL(scPoint, glPoint);
		//symbolpoint->SetPosition(glPoint, pAttri);

	
		//undoAddPointCommand->SetUndoPoint(glPoint);
		//undoAddPointCommand->SetRedoPoint(glPoint);
		//undoAddPointCommand->SetSymolAtrribute(pAttri);
		//m_undoStack->push(undoAddPointCommand);
		//undoAddPointCommand->SetIndex(symbolpoint->VertexArray());

		symbolpoint->SetAngle(0.0f);
		symbolpoint->SetPointStyle(m_Symbol);
		//����ԭʼ����
		QVector3D temp = TransGLtoWGS84(glPoint);
		pOriginalArray->push_back(temp);
	}

	if (m_bFirst)
	{
		m_ParentLayer->GetLayerData()->InsertRenderable(symbolpoint);
		m_bFirst= false;
		nsLayer = dynamic_cast<SymbolLayer*>(m_ParentLayer);
		if (nsLayer == NULL)
		{
			return;
		}
		if (symboldata)
		{
			symboldata = nsLayer->getSymbolData();
			symboldata->getNormalSymbol()->push_back(symbolpoint);
		}
	}


}
/************************************************************************/
// �������ܣ� ������뿪�¼�
// �� �� ֵ:  void
// ��    ��:  �		
// ��    ��:  2011 �� 4 �� 19 ��
/************************************************************************/
void DrawNormalSymbol::MouseRelease( QMouseEvent *event )
{
}

/************************************************************************/
// �������ܣ� ����뿪�¼�
// �� �� ֵ:  void
// ��    ��:  renxue		
// ��    ��:  2011 �� 6 �� 29 ��
/************************************************************************/
void DrawNormalSymbol::LeaveEvent()
{
}


void DrawNormalSymbol::setILayerAttrib( ILayerAttribute *Ilayer )
{

}
/************************************************************************/
// �������ܣ� ��ʼ������ֵ
// �� �� ֵ:  void
// ��    ��:  �		
// ��    ��:  2011 �� 4 �� 19 ��
// �޸����ߣ� ��ѩ
// �޸����ڣ� 2011 �� 5 �� 09 ��
/************************************************************************/
void DrawNormalSymbol::initAttribute( int nType )
{
	m_layerAttribute = new LAYER_ATTRIBUTE;
// 	QStringList strList;
// 	strList.append(QString("ʵ��Բ"));
// 	strList.append(QString("����Բ"));
// 	strList.append(QString("�Ǻ�"));
// 	strList.append(QString("����"));
// 	strList.append(QString("ʵ����"));
// 	strList.append(QString("����"));
// 	strList.append(QString("ʵ����"));
// 	ILayerAttribute *cValue = new ILayerAttribute("SymbolShape","SymbolShape",QString("������״"),QString("ʵ��Բ"), QVariant::StringList, 0, 0, strList);
// 	m_layerAttribute->insert(QString("SymbolShape"),cValue);

	//�߿���ɫ--renxue
	QStringList strColor;
	ILayerAttribute *borderColor = new ILayerAttribute("borderColor","borderColor",tr("�߿���ɫ"),QColor(0,0,255),QVariant::Color,0,0,strColor);
	m_layerAttribute->insert(QString("borderColor"),borderColor);
	//���Ŵ�С--renxue
	QStringList strSize;
	for (int i=1; i<=10; i++)
	{
		strSize.append(QString::number(i));
	}
	ILayerAttribute *symbolSize = new ILayerAttribute("symbolSize","symbolSize",tr("���Ŵ�С"),6,QVariant::Int,6,16,strSize);
	m_layerAttribute->insert(QString("symbolSize"),symbolSize);
}



/************************************************************************/
/* �������ܣ� ȡ������       			        					*/
/* ����˵��:  ��                 										*/
/* �� �� ֵ:  ��                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 05 �� 09 ��										*/
/************************************************************************/
void  DrawNormalSymbol::GetAttribute()
{
	LAYER_ATTRIBUTE *pLA = getILayerAttrib();
	if(!pLA) return;
	QColor tColor;

	for(int i=0; i<pLA->size(); i++)
	{
		ILayerAttribute *pLayAttribute = pLA->value(QString("borderColor"));

		tColor = pLayAttribute->Value().value<QColor>();
        pAttri->SetColor(QVector4D(tColor.red()/255.0, tColor.green()/255.0, tColor.blue()/255.0, 1.0f));


		pLayAttribute = pLA->value( QString("symbolSize"));
		pAttri->SetValue(pLayAttribute->Value().toFloat());

	}
}
