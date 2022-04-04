#pragma once
//#include "QNode.h"
#include "QStateSet.h"
#include "QDataArray.h"
#include "QAttributeSet.h"
#include "GLTransform.h"
#include "QTransform3D.h"
#include "QERenderContext.h"

class QNode;
typedef QVector<QNode*> NODE_VECTOR;
typedef QPair<int, QString> COMPOSE_KEY;
typedef QMap<COMPOSE_KEY, QDataArray*> ARRAY_MAP;
class QRenderable;

typedef enum _DATA_FORM_TYPE
{
	CONTAINER = 1,
	SINGLE
};

enum ARRAY_KEY_TYPE
{
	STRING_KEY = 1,
	UINT_KEY
};

class QRenderDataCallBack
{
public:
	GLenum dataType;
	virtual void FindedData(QDataArray* data) = 0;
};

class QFindedRenderUnit
{
public:
	virtual int FindedUnit(QRenderable* unit) = 0;
};

class QFindedRenderVolume
{
public:
	virtual int FindedVolume(QRenderable* volume) = 0;
};

#define APPLY_PARENT_TRANSFORM_START(h)/* glPushMatrix();\
glLoadIdentity();\
if(h)\
{\
if(h->TransformDirty())\
{\
m_TranslateMatirx = h->Transform() * this->m_TranslateMatirx;\
glLoadMatrixd(m_TranslateMatirx.data());\
}\
}*/

#define APPLY_PARENT_TRANSFORM_END// glPopMatrix();

class QE_LIB_EXPORT_IMPORT QRenderUnit;
class QE_LIB_EXPORT_IMPORT QRenderVolume;
class QE_LIB_EXPORT_IMPORT QObject;
/**********************���пɻ��ƶ���Ļ��࣬�������б���ʵ�֣�
virtual void Render() = 0;
virtual int InsertRenderable(const QRenderable* unit) = 0;
virtual int RemoveRenderable(QRenderable* unit) = 0;
virtual bool DumpRenderable(QRenderable** renderable) = 0;
virtual int FindDataUseSignal(GLenum type) = 0;
//virtual int FindDataUseCallback() = 0;
virtual void Clear() = 0;
**********************************************************/
class QE_LIB_EXPORT_IMPORT QRenderable : virtual public QObject
{
public:
	typedef QMap<int, QVariant> ATTRIBUTE_MAP, *P_ATTRIBUTE_MAP;

	QRenderable();
	virtual ~QRenderable(void);
	inline virtual NODE_VECTOR* ParentVecotr();
	inline virtual QNode* Parent(int i);
	//************************************
	// Method:    StateSet
	// FullName:  QRenderable::StateSet
	// Access:    public 
	// Returns:   void
	// Qualifier: ���ô˶����״̬���ϣ�״̬���Ͽ��԰������е�OpenGL״̬
	// Parameter: const QStateSet & statset
	//************************************
	inline void StateSet(const QStateSet& statset);
	//************************************
	// Method:    AttribSet
	// FullName:  QRenderable::AttribSet
	// Access:    public 
	// Returns:   void
	// Qualifier: ���ö�������Լ��ϣ����Լ����п�����ӣ���ɫ�����Ρ����С���߿�ȵ�����
	// Parameter: const QAttributeSet & as
	//************************************
	inline void AttribSet(const QAttributeSet& as);
	inline QAttributeSet* AttribSet();
	//QStateSet* StateSet(){return &stateSet;}
	//************************************
	// Method:    EndaleState
	// FullName:  QRenderable::EndaleState
	// Access:    public 
	// Returns:   void
	// Qualifier: �����͹ر�״̬�����е�״̬
	//************************************
	void EndaleState();
	void DisableState();
	void ApplayAttrib();
	//************************************
	// Method:    Boundary
	// FullName:  QRenderable::Boundary
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: ����ı߽���Ϣ
	//************************************
	virtual void Boundary(){}
	//************************************
	// Method:    DisplayList
	// FullName:  QRenderable::DisplayList
	// Access:    public 
	// Returns:   bool
	// Qualifier: �����͹ر���Ⱦ��
	//************************************
	inline bool DisplayList();
	inline void DisplayList(bool yesno);
	//************************************
	// Method:    GetListName
	// FullName:  QRenderable::GetListName
	// Access:    public 
	// Returns:   GLuint
	// Qualifier: �õ���Ⱦ������
	//************************************
	inline GLuint GetListName();
	//************************************
	// Method:    DisplayListDirty
	// FullName:  QRenderable::DisplayListDirty
	// Access:    public 
	// Returns:   void
	// Qualifier: ��־��Ⱦ��Ϊ�ɵģ���Ҫ������Ⱦ
	//************************************
	inline void DisplayListDirty();
	//************************************
	// Method:    CallDisplayList
	// FullName:  QRenderable::CallDisplayList
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: ������Ⱦ�н�����Ⱦ
	//************************************
	inline virtual void CallDisplayList();
	//************************************
	// Method:    Transform
	// FullName:  QRenderable::Transform
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: ���ú͵õ�����ı任����
	// Parameter: const QMatrix4x4 trans
	//************************************
	/*inline virtual void Transform(const QMatrix4x4 trans);
	inline virtual QMatrix4x4 Transform();*/
	//************************************
	// Method:    ApplyTransform
	// FullName:  QRenderable::ApplyTransform
	// Access:    public 
	// Returns:   void
	// Qualifier: ���ñ任����
	//************************************
	inline void ApplyTransform();
	//virtual void Render(const COMPOSE_KEY& subkey) = 0;
	//************************************
	// Method:    Render
	// FullName:  QRenderable::Render
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: �ϲ������Զ����ô˷������Ա���������Ⱦ����
	//************************************
	virtual void Render() = 0;
	virtual void Render(QSet<QRenderable*> rs) = 0;
	//************************************
	// Method:    InsertRenderable
	// FullName:  QRenderable::InsertRenderable
	// Access:    virtual public 
	// Returns:   int
	// Qualifier: ����Ⱦ����Ϊ����ʱ��ʵ�ִ˷���
	// Parameter: const QRenderable * unit
	//************************************
	virtual int InsertRenderable(const QRenderable* unit) = 0;
	virtual int InsertRenderableNotDel(const QRenderable* unit) { return 0;};
	//************************************
	// Method:    RemoveRenderable
	// FullName:  QRenderable::RemoveRenderable
	// Access:    virtual public 
	// Returns:   int
	// Qualifier: ����Ⱦ����Ϊ����ʱ��ʵ�ִ˷���
	// Parameter: QRenderable * unit
	//************************************
	virtual int RemoveRenderable(QRenderable* unit) = 0;
	//************************************
	// Method:    DumpRenderable
	// FullName:  QRenderable::DumpRenderable
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier: ����
	// Parameter: QRenderable * * renderable
	//************************************
	virtual bool DumpRenderable(QRenderable** renderable) = 0;
	//************************************
	// Method:    FindDataUseSignal
	// FullName:  QRenderable::FindDataUseSignal
	// Access:    virtual public 
	// Returns:   int
	// Qualifier: �̳���ʵ�ִ˷�����
	//            �Ա�ʹ��QT���źŻ��ƶԴ˶���������������ݽ��м���
	// Parameter: GLenum type
	//************************************
	virtual int FindDataUseSignal(GLenum type) = 0;
	virtual void FindUnitUseSignal() = 0;
	virtual void FindDataUseCallBack(GLenum type) = 0;
	virtual void FindUnitUseCallBack() = 0;
	virtual void FindVolumeUseCallBack() = 0;
	//virtual int FindDataUseCallback() = 0;
	//************************************
	// Method:    Clear
	// FullName:  QRenderable::Clear
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: ����˶��������
	//************************************
	virtual void Clear() = 0;
	//************************************
	// Method:    ParentNode
	// FullName:  QRenderable::ParentNode
	// Access:    public 
	// Returns:   QNode*
	// Qualifier: �õ������ô˶���ĸ�����
	//************************************
	QNode* ParentNode();
	void ParentNode(QNode* node);
	virtual bool IsContiner() = 0;
	virtual QRenderUnit* AsRenderUnit() = 0;
	virtual QRenderVolume* AsRenderVolume() = 0;
	virtual void CallListDirty(bool bv) = 0;

	virtual bool PickUp(const QVector3D* pickPoint);
	virtual int ChildCount() = 0; 
	bool CallListIsDirty();
	inline virtual void SetDataCallBack(QRenderDataCallBack* rdcb);
	inline virtual QRenderDataCallBack* GetDataCallBack();
	inline virtual void SetUnitCallBack(QFindedRenderUnit* ucb);
	inline virtual QFindedRenderUnit* GetUnitCallBack();
	inline virtual void SetVolumeCallBack(QFindedRenderVolume* fvcb);
	inline virtual QFindedRenderVolume* GetVolumeCallBack();
	inline void PickPrecision(float pp);
	inline float PickPrecision();
	
	void TransformRenderObject();
	QRenderable* ParentRenderable();
	void ParentRenderable(QRenderable* pP);
	void TransformDirty(bool lt);
	bool TransformDirty();
	inline void ModleTransform(QTransform3D* gltf);
	inline QTransform3D* ModleTransform();
	QRenderable& operator=(const QRenderable& other);
	inline void RenderContext(QERenderContext* pRC){m_pRenderContext = pRC;}
	inline QERenderContext* RenderContext(){return m_pRenderContext;}
	inline void SetID(int id){m_iRenderID = id;}
	inline int GetID(){return m_iRenderID;}
	inline void SetAttributeMap(const P_ATTRIBUTE_MAP pMap){m_attributeMap = *pMap;}
	inline P_ATTRIBUTE_MAP GetAttributeMap(){return &m_attributeMap;}

protected:
	//ARRAY_KEY_TYPE keyType;
	QRenderable* m_pParent;
	QNode* parentNode;
	int arrayStride;
	QStateSet stateSet;
	QAttributeSet attriSet;
	bool m_EnableDispList;
	GLuint ListName;
	NODE_VECTOR parentVector;
	bool useCallList;
	QRenderDataCallBack* dataCallBack;
	QFindedRenderUnit* unitCallBack;
	QFindedRenderVolume* volumeCallBack;
	float pickPrecision;
	QTransform3D m_pModleTransform;
	bool m_bTransformDirty;
	QERenderContext* m_pRenderContext;
	int m_iRenderID;
	ATTRIBUTE_MAP m_attributeMap;
};
