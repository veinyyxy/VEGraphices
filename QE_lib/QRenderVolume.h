#pragma once
#include "QNode.h"
#include "QRenderable.h"
#include "QRenderUnit.h"

/*********************************
������Ⱦ���󣬿��԰�������͵�һ��Ⱦ����
*********************************/
class QE_LIB_EXPORT_IMPORT QRenderVolume : public QRenderable
{
	Q_OBJECT

public:
	QRenderVolume(void);
	~QRenderVolume(void);
	virtual int InsertRenderable(const QRenderable* unit);
	virtual int InsertRenderableNotDel(const QRenderable* unit);
	int RemoveRenderable(QRenderable* unit);
	//QRenderUnit* GetUnit(const COMPOSE_KEY& key){return unitSet[key];}
	virtual void Render();
	void Render(QSet<QRenderable*> rs);
	//virtual void Render(const QRenderable* unit);
	virtual void CallDisplayList();
	GLuint GetListName(const COMPOSE_KEY& key);
	int Size(){return unitSet.size();}
	void Clear();
	void ClearNotDel();
	virtual bool DumpRenderable(QRenderable** renderable);
	int FindDataUseSignal(GLenum type);
	bool IsContiner();
	inline QRenderUnit* AsRenderUnit();
	inline QRenderVolume* AsRenderVolume();
	void FindUnitUseSignal();
	void FindDataUseCallBack(GLenum type);
	void FindUnitUseCallBack();
	void FindVolumeUseCallBack();
	void CallListDirty(bool bv);
	virtual int ChildCount();
	virtual bool PickUp(const QVector3D* pickPoint);

signals:
	void signal_Finded(GLenum type, QRenderable* data);

private:
	RENDER_UNIT_SET unitSet;
};

