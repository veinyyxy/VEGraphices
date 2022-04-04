#include "QRenderable.h"


QRenderable::~QRenderable(void)
{
	
}

QRenderable::QRenderable() : 
	ListName(0), 
	m_EnableDispList(false), 
	useCallList(false), 
	m_pParent(0), 
	parentNode(0), 
	pickPrecision(0),
	arrayStride(0),
	dataCallBack(0),
	unitCallBack(0),
	volumeCallBack(0),
	m_pRenderContext(0)
{
	/*m_TranslateMatirx.setToIdentity();
	m_RotateMatirx.setToIdentity();
	m_ScaleMatirx.setToIdentity();*/
}

NODE_VECTOR* QRenderable::ParentVecotr()
{
	return &parentVector;
}

QNode* QRenderable::Parent( int i )
{
	return 0;
}

QRenderable* QRenderable::ParentRenderable()
{
	return m_pParent;
}

void QRenderable::ParentRenderable( QRenderable* pP )
{
	m_pParent = pP;
}

void QRenderable::StateSet( const QStateSet& statset )
{
	stateSet = statset;
}

void QRenderable::AttribSet( const QAttributeSet& as )
{
	attriSet = as;
}

QAttributeSet* QRenderable::AttribSet()
{
	return &attriSet;
}

void QRenderable::EndaleState()
{
	stateSet.EnableAll();
}

void QRenderable::DisableState()
{
	stateSet.DisableAll();
}

void QRenderable::ApplayAttrib()
{
	attriSet.Applay();
}

bool QRenderable::DisplayList()
{
	return m_EnableDispList;
}

void QRenderable::DisplayList( bool yesno )
{
	m_EnableDispList = yesno;
}

GLuint QRenderable::GetListName()
{
	return ListName;
}

void QRenderable::DisplayListDirty()
{
	useCallList = false;
}

void QRenderable::CallDisplayList()
{
	glCallList(ListName);
}

QNode* QRenderable::ParentNode()
{
	return parentNode;
}

void QRenderable::ParentNode( QNode* node )
{
	parentNode = node;
	//setParent((QObject*)node);
}

void QRenderable::SetDataCallBack( QRenderDataCallBack* rdcb )
{
	dataCallBack = rdcb;
}

QRenderDataCallBack* QRenderable::GetDataCallBack()
{
	return dataCallBack;
}

void QRenderable::SetUnitCallBack( QFindedRenderUnit* ucb )
{
	unitCallBack = ucb;
}

QFindedRenderUnit* QRenderable::GetUnitCallBack()
{
	return unitCallBack;
}

void QRenderable::SetVolumeCallBack( QFindedRenderVolume* fvcb )
{
	volumeCallBack = fvcb;
}

QFindedRenderVolume* QRenderable::GetVolumeCallBack()
{
	return volumeCallBack;
}

bool QRenderable::CallListIsDirty()
{
	return useCallList;
}

bool QRenderable::PickUp( const QVector3D* pickPoint )
{
	return false;
}

void QRenderable::PickPrecision( float pp )
{
	pickPrecision = pp;
}

float QRenderable::PickPrecision()
{
	return pickPrecision;
}

void QRenderable::TransformDirty( bool lt )
{
	m_bTransformDirty = lt;
}

bool QRenderable::TransformDirty()
{
	return m_bTransformDirty;
}

void QRenderable::ModleTransform( QTransform3D* gltf )
{
	m_pModleTransform = *gltf;
}

QTransform3D* QRenderable::ModleTransform()
{
	return &m_pModleTransform;
}

QRenderable& QRenderable::operator=( const QRenderable& other )
{
	//ARRAY_KEY_TYPE keyType;
	/*this->arrayStride = other.arrayStride;
	this->stateSet = other.stateSet;
	this->attriSet = other.attriSet;
	this->m_EnableDispList = other.m_EnableDispList;
	this->ListName = other.ListName;
	this->parentVector = other.parentVector;
	this->pickPrecision = other.pickPrecision;
	this->m_pModleTransform = other.m_pModleTransform;
	this->m_bTransformDirty = other.m_bTransformDirty;*/
	memcpy(this, &other, sizeof(QRenderable));
	this->m_pParent = 0;
	this->parentNode = 0;
	this->useCallList = 0;
	this->dataCallBack = 0;
	this->unitCallBack = 0;
	this->volumeCallBack = 0;
	this->parentVector.clear();
	return *this;
}
