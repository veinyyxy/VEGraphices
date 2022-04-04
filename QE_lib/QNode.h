#pragma once

#include <QtCore/QThread>
#include "QStateSet.h"
#include "QObserver.h"
#include "QTransform3D.h"
#include "QNodeCallBack.h"
#include "QERenderContext.h"

class QRenderable;
typedef QPair<int, QString> COMPOSE_KEY;
/*******************************************************
节点容器，树数据结构中树杈和树叶的基类
*******************************************************/
class Node;
class QLeaf;
class QFork;

typedef QVector <QString>  TransOrder;

class QE_LIB_EXPORT_IMPORT QNode : public QThread
{
public:
	QNode(QObject* parent = 0);
	virtual ~QNode(void);
	virtual QNode* Parent(){return parentNode;}
	virtual void Parent(QNode* p);
	
	//virtual QNode* GetChild(const COMPOSE_KEY& key) = 0;
	//virtual int SetChild(const COMPOSE_KEY& key, QNode* c) = 0;
	//************************************
	// Method:    InsertChild
	// FullName:  QNode::InsertChild
	// Access:    virtual public 
	// Returns:   int
	// Qualifier: 插入一个节点
	// Parameter: QNode * c
	//************************************
	virtual int InsertChild(QNode* c) = 0;
	//virtual int RemoveChild(const COMPOSE_KEY& key) = 0;
	//************************************
	// Method:    RemoveChild
	// FullName:  QNode::RemoveChild
	// Access:    virtual public 
	// Returns:   int
	// Qualifier: 删除一个节点
	// Parameter: const QNode * c
	//************************************
	virtual int RemoveChild(const QNode* c) = 0;
	QStateSet* StateSet(){return stateSet;};
	void StateSet(QStateSet* ss){stateSet = ss;};
	virtual void ApplyState(){}
	//virtual QRenderable* Renderable(const COMPOSE_KEY& key) = 0;
	virtual int InsertRenderable(QRenderable* r) = 0;
	virtual int RemoveRenderable(const QRenderable* r) = 0;
	virtual QNode* DumpNode() = 0;
	virtual QRenderable* DumpRenderable() = 0;
	virtual void IteratorHome() = 0;
	virtual void IteratorEnd() = 0;
	//************************************
	// Method:    NodeProcess
	// FullName:  QNode::NodeProcess
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: 节点处理方法，所有从派生类都必须实现此方法，
	//            以实现对树中此节点的处理
	//************************************
	virtual void NodeProcess() = 0;
	virtual bool DumpChild(QNode** node) = 0;
	virtual bool DumpChild(QRenderable** renderable) = 0;
	//************************************
	// Method:    Transform
	// FullName:  QNode::Transform
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: 设置和得到节点变换矩阵
	// Parameter: const QMatrix4x4 matrx
	//************************************
	virtual void Transform(const QMatrix4x4 matrx);
	virtual QMatrix4x4 Transform();
	//************************************
	// Method:    ApplyTransform
	// FullName:  QNode::ApplyTransform
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: 设置变换矩阵
	//************************************
	virtual void ApplyTransform();
	virtual void RemoveAllChild() = 0;
	virtual QLeaf* AsLeaf() = 0;
	virtual QFork* AsFork() = 0;

	virtual void SetTranslate(const QVector3D& pos);              //设置平移矩阵
	virtual QMatrix4x4& NodeTranslate();
	virtual void SetRotation(qreal a, const QVector3D& aix);      //设置旋转矩阵
	virtual QMatrix4x4& NodeRotate();
	virtual void SetScale(qreal s );                              //设置缩放矩阵
	virtual QMatrix4x4& NodeScale();
	virtual bool RemoveChild(const QNode* c, bool del) = 0;

	/** Set update node callback, called during update traversal. */
	void SetNodeCallBack(QNodeCallBack* pncb) { m_NodeCallBack = pncb;}
 	QNodeCallBack* GetNodeCallBack() {return m_NodeCallBack;}
 	void SetTransform(QTransform3D* tran);
 	void SetObserver(QObserver* observer);	
	void AcceptCallBack(QNodeCallBack& pncb);
	inline void RenderContext(QERenderContext* pRC){m_pRenderContext = pRC;}
	inline QERenderContext* RenderContext(){return m_pRenderContext;}

protected:
	QStateSet* stateSet;
	QNode* parentNode;
 	QMatrix4x4 nodeTransform;
 	QMatrix4x4 nodeTranslate;           //平移矩阵
 	QMatrix4x4 nodeRotate;              //旋转矩阵
 	QMatrix4x4 nodeScale;               //缩放矩阵

	TransOrder* trans;                   //用来存储和设置变换顺序

protected:
 	QObserver* m_observer;
 	QTransform3D* m_SceneTransform;
	QNodeCallBack* m_NodeCallBack;
	QERenderContext* m_pRenderContext;
};
