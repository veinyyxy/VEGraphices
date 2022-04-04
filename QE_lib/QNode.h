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
�ڵ������������ݽṹ����辺���Ҷ�Ļ���
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
	// Qualifier: ����һ���ڵ�
	// Parameter: QNode * c
	//************************************
	virtual int InsertChild(QNode* c) = 0;
	//virtual int RemoveChild(const COMPOSE_KEY& key) = 0;
	//************************************
	// Method:    RemoveChild
	// FullName:  QNode::RemoveChild
	// Access:    virtual public 
	// Returns:   int
	// Qualifier: ɾ��һ���ڵ�
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
	// Qualifier: �ڵ㴦���������д������඼����ʵ�ִ˷�����
	//            ��ʵ�ֶ����д˽ڵ�Ĵ���
	//************************************
	virtual void NodeProcess() = 0;
	virtual bool DumpChild(QNode** node) = 0;
	virtual bool DumpChild(QRenderable** renderable) = 0;
	//************************************
	// Method:    Transform
	// FullName:  QNode::Transform
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: ���ú͵õ��ڵ�任����
	// Parameter: const QMatrix4x4 matrx
	//************************************
	virtual void Transform(const QMatrix4x4 matrx);
	virtual QMatrix4x4 Transform();
	//************************************
	// Method:    ApplyTransform
	// FullName:  QNode::ApplyTransform
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: ���ñ任����
	//************************************
	virtual void ApplyTransform();
	virtual void RemoveAllChild() = 0;
	virtual QLeaf* AsLeaf() = 0;
	virtual QFork* AsFork() = 0;

	virtual void SetTranslate(const QVector3D& pos);              //����ƽ�ƾ���
	virtual QMatrix4x4& NodeTranslate();
	virtual void SetRotation(qreal a, const QVector3D& aix);      //������ת����
	virtual QMatrix4x4& NodeRotate();
	virtual void SetScale(qreal s );                              //�������ž���
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
 	QMatrix4x4 nodeTranslate;           //ƽ�ƾ���
 	QMatrix4x4 nodeRotate;              //��ת����
 	QMatrix4x4 nodeScale;               //���ž���

	TransOrder* trans;                   //�����洢�����ñ任˳��

protected:
 	QObserver* m_observer;
 	QTransform3D* m_SceneTransform;
	QNodeCallBack* m_NodeCallBack;
	QERenderContext* m_pRenderContext;
};
