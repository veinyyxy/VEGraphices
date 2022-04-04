#include "DrawPointSymbol.h"


CDrawPointSymbol::CDrawPointSymbol(void)
{
	line = new Line;
	bFinished = false;
	pArray = NULL;
	//symbolpoint = NULL;
	symbolpoint  = NULL;
	m_bFirst = true;
	pOriginalArray = new QVector3DArray;
	m_pSymbolData = new SymbolData;
}


CDrawPointSymbol::~CDrawPointSymbol(void)
{

}

void CDrawPointSymbol::MouseEvent(QMouseEvent* event)
{

}

void CDrawPointSymbol::MousePress(QMouseEvent *event)
{
	QVector3D glPoint, scPoint(event->x(), event->y(), 0.0f);

	if(m_bFirst)
	{
		symbolpoint  = new SymbolPoint;
	}

	//CUndoAddPointCommand* undoAddPointCommand = new CUndoAddPointCommand;
	//undoAddPointCommand->SetSymbolPoint(symbolpoint);
	//undoAddPointCommand->SetGLWdiget(m_pOpenglView);
	int iIndex = -1;

	pOriginalArray = symbolpoint->OriginalArray();
	if(m_Symbol)
	{
		ScreenToGL(scPoint, glPoint);
		//symbolpoint->SetPosition(glPoint);

		//undo����
		//undoAddPointCommand->SetUndoPoint(glPoint);
		//undoAddPointCommand->SetRedoPoint(glPoint);
		//m_undoStack->push(undoAddPointCommand);
		//undoAddPointCommand->SetIndex(symbolpoint->VertexArray());

		symbolpoint->SetPointStyle(m_Symbol);
		symbolpoint->SetAngle(0.0f);

		//����ԭʼ����
		QVector3D temp = TransGLtoWGS84(glPoint);
		pOriginalArray->push_back(temp);
	}

	if(	m_bFirst)
	{
		m_ParentLayer->GetLayerData()->InsertRenderable(symbolpoint);
		m_bFirst= false;
		SymbolLayer *sLayer = dynamic_cast<SymbolLayer*>(m_ParentLayer);
		if (sLayer == NULL)
		{
			return;
		}
		m_pSymbolData = sLayer->getSymbolData();
		m_pSymbolData->getSymbolPoint()->push_back(symbolpoint);
	}

}

void CDrawPointSymbol::MouseRelease(QMouseEvent *event)
{
}

/************************************************************************/
// �������ܣ� ����뿪�¼�
// �� �� ֵ:  void
// ��    ��:  renxue		
// ��    ��:  2011 �� 06 �� 29 ��
/************************************************************************/
void CDrawPointSymbol::LeaveEvent()
{
}


void CDrawPointSymbol::setILayerAttrib( ILayerAttribute *Ilayer )
{

}
