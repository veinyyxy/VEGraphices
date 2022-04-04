#include "QDefaultMouseManipulator.h"


QDefaultMouseManipulator::QDefaultMouseManipulator(void) : lastPos(0, 0)
{
	
}


QDefaultMouseManipulator::~QDefaultMouseManipulator(void)
{
}

void QDefaultMouseManipulator::mousePressEvent( QMouseEvent *event )
{
	lastPos = event->pos();
}

void QDefaultMouseManipulator::mouseReleaseEvent( QMouseEvent *event )
{
	
}

void QDefaultMouseManipulator::mouseDoubleClickEvent (QMouseEvent * event)
{

}

void QDefaultMouseManipulator::mouseMoveEvent( QMouseEvent *event )
{
	float xTrans = 0, yTrans = 0;

	int ex = event->x();
	int ey = event->y();

	int dx = ex - lastPos.x();
	int dy = ey - lastPos.y();

	if (1/*event->buttons() & Qt::LeftButton*/)
	{		
		//QMatrix4x4 currentTrans, sceneTrans;
		xTrans += dx * m_pObserver->Camera()->GetCameraScale();
		yTrans -= dy * m_pObserver->Camera()->GetCameraScale();
		/*currentTrans.translate(xTrans, yTrans, 0);
		sceneTrans = m_pSceneTransform->Tranlate();
		sceneTrans *= currentTrans;		

		m_pSceneTransform->Tranlate(sceneTrans);*/	
	}
	m_pSceneTransform->Tranlate(QVector3D(xTrans, yTrans, 0));
	lastPos = event->pos();
}

void QDefaultMouseManipulator::wheelEvent( QWheelEvent *event )
{
	qreal xyzScale = 0.0;
	static GLfloat v = 0.1f, v1 = 0.1f;

	if(event->delta() > 0)
	{
		m_pObserver->Camera()->Scale(0.9);
	}
	else
	{
		m_pObserver->Camera()->Scale(1.1);
	}
}

void QDefaultMouseManipulator::keyPressEvent( QKeyEvent *event )
{

}

void QDefaultMouseManipulator::keyReleaseEvent( QKeyEvent *event )
{

}

void QDefaultMouseManipulator::leaveEvent( QEvent * even )
{

}
