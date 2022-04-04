#include "QMouseManipulatorExtand.h"


QMouseManipulatorExtand::QMouseManipulatorExtand(void)
{
	m_isExportingPicture = false;
}


QMouseManipulatorExtand::~QMouseManipulatorExtand(void)
{
}

void QMouseManipulatorExtand::mousePressEvent( QMouseEvent *event )
{
	m_pMouseManipulator->SetAssociScene(m_pAssociatedScene);
	m_pMouseManipulator->mousePressEvent(event);
}

void QMouseManipulatorExtand::mouseReleaseEvent( QMouseEvent *event )
{
	m_pMouseManipulator->SetAssociScene(m_pAssociatedScene);
	m_pMouseManipulator->mouseReleaseEvent(event);
}

void QMouseManipulatorExtand::mouseMoveEvent( QMouseEvent *event )
{
	m_pMouseManipulator->SetAssociScene(m_pAssociatedScene);
	m_pMouseManipulator->mouseMoveEvent(event);	
}

void QMouseManipulatorExtand::wheelEvent( QWheelEvent *event )
{
	m_pMouseManipulator->SetAssociScene(m_pAssociatedScene);
	m_pMouseManipulator->wheelEvent(event);
}

void QMouseManipulatorExtand::mouseDoubleClickEvent( QMouseEvent * event )
{
	m_pMouseManipulator->SetAssociScene(m_pAssociatedScene);
	m_pMouseManipulator->mouseDoubleClickEvent(event);
}

void QMouseManipulatorExtand::leaveEvent( QEvent * even )
{
	m_pMouseManipulator->SetAssociScene(m_pAssociatedScene);
	m_pMouseManipulator->leaveEvent(even);
}
