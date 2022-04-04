#include "LayerDisplayControl.h"


CLayerDisplayControl::CLayerDisplayControl(void) : m_toRender(0), m_notRender(0)
{
}


CLayerDisplayControl::~CLayerDisplayControl(void)
{
}

bool CLayerDisplayControl::IsRender()
{
	QObserver* pOb = m_pRenderContext->Observer();
	qreal qrScal = pOb->Camera()->GetCameraScale();
	if(qrScal - m_toRender <= 0)//5641.3806
		return true;
	else if(qrScal - m_notRender >= 0)//13302.083
		return false;
	else
		return true;
}

void CLayerDisplayControl::StartRender(QERenderContext* renderContext)
{
	m_pRenderContext = renderContext;
}

void CLayerDisplayControl::EndRender()
{

}

void CLayerDisplayControl::Apply( QNode& node )
{

}
