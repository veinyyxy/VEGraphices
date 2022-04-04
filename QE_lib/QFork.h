#pragma once

#include "QNode.h"
#include "QRenderable.h"

typedef QMap<COMPOSE_KEY, QNode*> NODE_MAP;
typedef QMapIterator<COMPOSE_KEY, QNode*> NODE_MAP_ITERATOR;

typedef QSet<QNode*> NODE_SET, P_NODE_SET;
typedef QSetIterator<QNode*> NODE_SET_ITERATOR, *P_NODE_SET_ITERATOR;

class QE_LIB_EXPORT_IMPORT QFork : public QNode
{
public:
	QFork(QObject* parent = 0);
	~QFork(void);

	//virtual QNode* GetChild(const COMPOSE_KEY& key);
	virtual int InsertChild(QNode* c);
	virtual int RemoveChild(const QNode* c);
	//virtual QRenderable* Renderable(const COMPOSE_KEY& key);
	virtual int InsertRenderable(QRenderable* r);
	virtual QNode* DumpNode();
	virtual QRenderable* DumpRenderable();
	virtual void IteratorHome();
	virtual void IteratorEnd();
	void InitIter();
	void ApplyState();
	void NodeProcess();
	bool DumpChild(QNode** node);
	bool DumpChild(QRenderable** renderable);
	int RemoveRenderable(const QRenderable* r);
	void RemoveAllChild();
	QLeaf* AsLeaf();
	QFork* AsFork();
	bool RemoveChild(const QNode* c, bool del);
	void run();

protected:
	NODE_SET childSet;
	NODE_SET_ITERATOR childMapIter;
};

