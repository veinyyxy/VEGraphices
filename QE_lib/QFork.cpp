#include "QFork.h"


QFork::QFork(QObject* parent) : QNode(parent), childMapIter(childSet)
{
}

QFork::~QFork(void)
{
	/*NODE_SET_ITERATOR locIter(childSet);
	while(locIter.hasNext())
	{
	QNode* node = locIter.next();
	if(node)
	delete node;
	}

	if(parentNode)
	parentNode->RemoveChild(this);*/
}

//QNode* QFork::GetChild(const COMPOSE_KEY& key)
//{
//	//return childSet[key];
//}

int QFork::InsertChild(QNode* c )
{
	childSet.insert(c);
	c->Parent(this);
	//InitIter();
	return childSet.size();
}

//QRenderable* QFork::Renderable( const COMPOSE_KEY& key )
//{
//	return 0;
//}

int QFork::InsertRenderable(QRenderable* r)
{
	return 0;
}

void QFork::ApplyState()
{

}

QNode* QFork::DumpNode()
{
	if(childMapIter.hasNext())
		return childMapIter.next();
	else
		return 0;
}

QRenderable* QFork::DumpRenderable()
{
	return 0;
}

void QFork::IteratorHome()
{
	childMapIter.toFront();
}

void QFork::IteratorEnd()
{
	childMapIter.toBack();
}

void QFork::InitIter()
{
	childMapIter = childSet;
}

int QFork::RemoveChild(const QNode* c)
{
	((QNode*)c)->setParent(0);
	int ir = childSet.remove((QNode*)c);
	return ir;
}

void QFork::NodeProcess()
{
	ApplyState();
	NODE_SET_ITERATOR locIter(childSet);
	while(locIter.hasNext())
	{
		QNode* node = locIter.next();
		if(m_NodeCallBack)
			m_NodeCallBack->StartRender(m_pRenderContext);

		if (!m_NodeCallBack)
		{
			node->RenderContext(m_pRenderContext);
			node->NodeProcess();
		}
		else if(m_NodeCallBack->IsRender())
		{
			node->RenderContext(m_pRenderContext);
			node->NodeProcess();
		}

		if(m_NodeCallBack)
			m_NodeCallBack->EndRender();
	}
}

bool QFork::DumpChild( QNode** node )
{
	InitIter();
	IteratorHome();
	if(childMapIter.hasNext())
	{
		*node = childMapIter.next();
		return true;
	}
	else
	{
		*node = 0;
		return false;
	}
}

bool QFork::DumpChild( QRenderable** renderable )
{
	*renderable = 0;
	return false;
}

int QFork::RemoveRenderable( const QRenderable* r )
{
	return 0;
}

void QFork::RemoveAllChild()
{
	NODE_SET_ITERATOR locIter(childSet);
	while(locIter.hasNext())
	{
		QNode* node = locIter.next();
		node->RemoveAllChild();
	}
	childSet.clear();
}

QLeaf* QFork::AsLeaf()
{return 0;}

QFork* QFork::AsFork()
{return this;}

bool QFork::RemoveChild(const QNode* c, bool del)
{
	int ir = 0;
	if(del)
	{
		ir = RemoveChild(c);
		if(c) delete c;
	}
	else
		ir = childSet.remove((QNode*)c);

	return ir;
}

void QFork::run()
{
	ApplyState();
	ApplyTransform();
	NODE_SET_ITERATOR locIter(childSet);
	while(locIter.hasNext())
	{
		QNode* node = locIter.next();
		node->ApplyState();
		node->ApplyTransform();
		node->start();
	}
}
