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
��Ҷ���󣬴˶�����Ҫ����������Ⱦ���󣬴˶��������Ⱦ������˵�Ǹ�����
����������˵�Ǹ���һ�Ķ���
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
	// Qualifier: �����ӽڵ�
	// Parameter: QNode * c
	//************************************
	virtual int InsertChild(QNode* c);
	//************************************
	// Method:    RemoveChild
	// FullName:  QLeaf::RemoveChild
	// Access:    virtual public 
	// Returns:   int
	// Qualifier: ɾ���ӽڵ�
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

