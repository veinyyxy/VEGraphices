#include "QNode.h"



QNode::~QNode(void)
{
}

QNode::QNode( QObject* parent /*= 0*/ ) : QThread(parent)
{
	m_pRenderContext = 0;
	m_NodeCallBack = 0;
	this->parentNode = 0;
	stateSet=0;
	nodeTranslate.setToIdentity();
	nodeRotate.setToIdentity();
	nodeScale.setToIdentity();
	 m_SceneTransform = 0;
	 m_observer = 0;
}

void QNode::Parent(QNode* p)
{
		setParent(p);
		parentNode = p;
}

void QNode::Transform( const QMatrix4x4 matrx )
{
	nodeTransform = matrx;
}

QMatrix4x4 QNode::Transform()
{
	return nodeTransform;
}

void QNode::SetTranslate(const QVector3D& pos)
{
	nodeTranslate.translate(pos);
	trans->push_back("TRANSLATE");
}

void QNode::SetRotation(qreal a, const QVector3D& aix)
{
	nodeRotate.rotate(a,aix);
	trans->push_back("ROTATION");
}

void QNode::SetScale(qreal s )
{
	nodeScale.scale(s);
	trans->push_back("SCALE");
}

 void QNode::SetTransform(QTransform3D* tran)
 {
 	m_SceneTransform = tran;
 }
 
 void QNode::SetObserver(QObserver* observer)
 {
 	m_observer = observer;
 }

void QNode::ApplyTransform()
{
	nodeTransform = nodeTranslate * nodeRotate * nodeScale;
	/*if (trans->size() == 0)
	{
		nodeTransform.setToIdentity();
	}
	else
	{
		for(int i = 0; i < trans->size();)
		{
			if (trans->at(i) == "TRANSLATE")
			{
				i++;
				nodeTransform *= nodeTranslate;
			}

			else if(trans->at(i) == "ROTATION")
			{
				i++;
				nodeTransform *= nodeRotate;
			}

			else if(trans->at(i) == "ROTATION")
			{
				i++;
				nodeTransform *= nodeScale;
			}

		}
	}*/
	//glMultMatrixd(nodeTransform.data());
}

QMatrix4x4& QNode::NodeTranslate()
{
	return nodeTranslate;
}

QMatrix4x4& QNode::NodeRotate()
{
	return nodeRotate;
}

QMatrix4x4& QNode::NodeScale()
{
	return nodeScale;
}

 void QNode::AcceptCallBack(QNodeCallBack& pncb)
 {
 	pncb.Apply(*this);
 }
