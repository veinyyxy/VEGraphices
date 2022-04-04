#pragma once

#include "QNode.h"
#include "QRenderable.h"
#include "QNodeCallBack.h"

typedef QMap<COMPOSE_KEY, QRenderable*> RENDERABLE_MAP;
typedef QMapIterator<COMPOSE_KEY, QRenderable*> RENDERABLE_MAP_ITERATOR;

typedef QSet<QRenderable*> RENDERABLE_SET, *P_RENDERABLE_SET;
typedef QSetIterator<QRenderable*> RENDERABLE_SET_ITERATOR, *P_RENDERABLE_SET_ITERATOR;

class QFork;
/***************************************************************
树叶对象，此对象主要用于容纳渲染对象，此对象对于渲染对象来说是个容器
，对于树来说是个单一的对象
***************************************************************/
class QE_LIB_EXPORT_IMPORT QLeaf : public QNode
{
public:
	QLeaf(QObject* parent = 0);
	~QLeaf(void);

	//virtual QNode* GetChild(const COMPOSE_KEY& key);
	//************************************
	// Method:    InsertChild
	// FullName:  QLeaf::InsertChild
	// Access:    public 
	// Returns:   int
	// Qualifier: 插入子节点
	// Parameter: QNode * c
	//************************************
	virtual int InsertChild(QNode* c);
	//************************************
	// Method:    RemoveChild
	// FullName:  QLeaf::RemoveChild
	// Access:    virtual public 
	// Returns:   int
	// Qualifier: 删除子节点
	// Parameter: const QNode * c
	//************************************
	virtual int RemoveChild(const QNode* c);
	//virtual QRenderable* Renderable(const COMPOSE_KEY& key);
	//************************************
	// Method:    InsertRenderable
	// FullName:  QLeaf::InsertRenderable
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: QRenderable * r
	//************************************
	virtual int InsertRenderable(QRenderable* r);
	int InsertRenderable2(QRenderable* r);
	virtual QNode* DumpNode();
	virtual QRenderable* DumpRenderable();
	virtual void IteratorHome();
	virtual void IteratorEnd();
	void ApplyState();
	void InitIter();
	void NodeProcess();
	bool DumpChild(QNode** node);
	bool DumpChild(QRenderable** renderable);
	int RemoveRenderable(const QRenderable* r);
	void RemoveAllChild();
	void RemoveAllChild2();
	QLeaf* AsLeaf();
	QFork* AsFork();
	bool RemoveChild(const QNode* c, bool del);
	void run();

private:
	RENDERABLE_SET renderSet;
	RENDERABLE_SET_ITERATOR rendableIter;
};

