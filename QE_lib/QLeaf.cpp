#include "QLeaf.h"


QLeaf::QLeaf(QObject* parent) : QNode(parent), rendableIter(renderSet)
{
	
}


QLeaf::~QLeaf(void)
{
	/*RENDERABLE_SET_ITERATOR locIter(renderSet);
	while(locIter.hasNext())
	{
		QRenderable* renderable = locIter.next();
		if(renderable) delete renderable;
	}

	if(parentNode)
		parentNode->RemoveChild(this);*/
}
//
//QNode* QLeaf::GetChild(const COMPOSE_KEY& key)
//{
//	return 0;
//}

int QLeaf::InsertChild(QNode* c)
{
	return 0;
}

//QRenderable* QLeaf::Renderable(const COMPOSE_KEY& key)
//{
//	return renderSet[key];
//}

int QLeaf::InsertRenderable(QRenderable* r)
{
	//renderSet.push_back(r);
	renderSet.insert(r);
	r->ParentNode(this);
	r->setParent(this);
	//Parent(this);
	InitIter();
	return renderSet.size();
}

void QLeaf::ApplyState()
{

}

void QLeaf::IteratorHome()
{
	rendableIter.toFront();
}

QNode* QLeaf::DumpNode()
{
	return 0;
}

QRenderable* QLeaf::DumpRenderable()
{
	if(rendableIter.hasNext())
		return rendableIter.next();
	else
		return 0;
}

void QLeaf::IteratorEnd()
{
	rendableIter.toBack();
}

void QLeaf::InitIter()
{
	rendableIter = renderSet;
}

int QLeaf::RemoveChild(const QNode* c)
{
	return 0;
}

void QLeaf::NodeProcess()
{
	RENDERABLE_SET_ITERATOR locIter(renderSet);
	
	while(locIter.hasNext())
	{
	   QRenderable* renderable = locIter.next();
		
		if(m_NodeCallBack)
			m_NodeCallBack->StartRender(m_pRenderContext);
	
		//glPushAttrib(GL_ALL_ATTRIB_BITS);
		//renderable->ApplayAttrib();		
		if (!m_NodeCallBack)
		{
			renderable->RenderContext(m_pRenderContext);
			renderable->Render();
		}
		else if (m_NodeCallBack->IsRender())
		{
			renderable->RenderContext(m_pRenderContext);
			renderable->Render();
		}

		if(m_NodeCallBack)
			m_NodeCallBack->EndRender();
		//glPopAttrib();		
	}
}

bool QLeaf::DumpChild( QNode** node )
{
	return false;
}

bool QLeaf::DumpChild( QRenderable** renderable )
{
	return false;
}

int QLeaf::RemoveRenderable( const QRenderable* r )
{
	//if(r) delete r;
	((QRenderable*)r)->setParent(0);
	renderSet.remove((QRenderable*)r);
	return true;
}

void QLeaf::RemoveAllChild()
{
	RENDERABLE_SET_ITERATOR locIter(renderSet);

	while(locIter.hasNext())
	{
		QRenderable* renderable = locIter.next();
		if(renderable)
			delete renderable;
	}
	renderSet.clear();
}

QLeaf* QLeaf::AsLeaf()
{return this;}

QFork* QLeaf::AsFork()
{return 0;}

bool QLeaf::RemoveChild(const QNode* c, bool del)
{
	return false;
}

void QLeaf::run()
{
	NodeProcess();
	glFinish();
}

int QLeaf::InsertRenderable2( QRenderable* r )
{
	renderSet.insert(r);
	return true;
}

void QLeaf::RemoveAllChild2()
{
	renderSet.clear();
}
