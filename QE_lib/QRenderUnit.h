#pragma once

#include "QDataArray.h"
#include "QRenderable.h"

/*********************************
单一渲染对象
**********************************/
class QE_LIB_EXPORT_IMPORT QRenderUnit : public QRenderable
{
	Q_OBJECT

public:
	QRenderUnit(DATA_COMPOSE compos);
	virtual ~QRenderUnit(void);

	virtual void Render();
	void Render(QSet<QRenderable*> rs);
	//virtual void Render(const COMPOSE_KEY& subkey);
	virtual void SetArrayData(QDataArray* arry);
	void SetArrayData(bool state, QDataArray* arry);

	void Clear();
	inline QDataArray* VertexArray();
	//void VertexArray(QDataArray* arry){vertexArray = arry;}
	inline QDataArray* NormalArray();
	//void NormalArray(QDataArray* arry){normalArray = arry;}
	inline QDataArray* ColorArray();
	//void ColorArray(QDataArray* arry){colorArray = arry;}
	inline QDataArray* TextureArray();
	//void TextureArray(QDataArray* arry){textureArray = arry;}
	//QDataArray* MaterialArray(){return materialArray;}
	inline GLsizei VertexCount();
	inline void VertexCount(GLsizei count);
	inline GLenum Mode();
	inline void Mode(GLenum m);
	
	//void EnableArray(GLenum arrayFlag);
	virtual int InsertRenderable(const QRenderable* unit);
	virtual int RemoveRenderable(QRenderable* unit);
	bool DumpRenderable(QRenderable** renderable);
	int FindDataUseSignal(GLenum type);
	void FindUnitUseSignal();
	inline void CallBack(QRenderDataCallBack* cb);
	inline QRenderDataCallBack* CallBack();
	bool IsContiner();
	inline QRenderUnit* AsRenderUnit();
	inline QRenderVolume* AsRenderVolume();
	void FindDataUseCallBack(GLenum type);
	void FindUnitUseCallBack();
	void FindVolumeUseCallBack();
	void CallListDirty(bool bv);
	void SetTexture( GLuint & texture);
	inline QVector3DArray* OriginalArray();
	inline void SetBakData();			//备份原始数据
	virtual bool PickUp(const QVector3D* pickPoint);
	virtual int ChildCount();
	void sTOog(const QVector3D& src, QVector3D& des);
	void ogTOs(const QVector3D& src, QVector3D& des);
	void NeedCalculateA() { bCalculateA = true;}
   
	void SetRenderNodeMask(bool bv);
	bool GetRenderNodeMask() {return m_NodeMask;}
	QRenderUnit& operator=(const QRenderUnit& other);
signals:
	void signal_Finded(QDataArray* data);
protected:
		bool bCalculateA;			//标志是否重新计算角度
		bool m_NodeMask;
private:
	virtual void ApplyData();
	QSet<GLenum> typeSet;
	typedef QSetIterator<GLenum> ITER;
	//GLenum type;
	DATA_COMPOSE compose;
	GLenum mode;
	GLsizei vertexCount;
	QDataArray* vertexArray;
	QDataArray* normalArray;
	QDataArray* colorArray;
	QDataArray* textureCoordArray0;
	QDataArray* textureCoordArray1;
	QDataArray* textureCoordArray2;
	QDataArray* textureCoordArray3;
	QDataArray* textureCoordArray4;
	QDataArray* mixinArray;
	//QDataArray* materialArray;
	bool bColorArray;
	bool bTextureArray;
	GLuint *pTexture;
	QVector3DArray m_OriginalArray;
};

typedef QMap<COMPOSE_KEY, QRenderUnit*> UNIT_MAP;
typedef QMapIterator<COMPOSE_KEY, QRenderUnit*> UNIT_MAP_ITERATOR;

typedef QSet<QRenderable*> RENDER_UNIT_SET, *P_RENDER_UNIT_SET;
typedef QSetIterator<QRenderable*> RENDER_UNIT_ITERATOR, *P_RENDER_UNIT_ITERATOR;