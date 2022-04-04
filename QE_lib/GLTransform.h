#pragma once
#include "QT_Include.h"

class GLTransform : public QObject
{
public:
	GLTransform(QMatrix4x4& tr, QMatrix4x4 ro, QMatrix4x4 sc);
	GLTransform(void);
	virtual ~GLTransform(void);
	void TranslateMatirx( const QMatrix4x4& tm4x4 );
	QMatrix4x4& TranslateMatirx();
	void RotateMatirx( const QMatrix4x4& rm4x4 );
	QMatrix4x4& RotateMatirx();
	void ScaleMatirx( const QMatrix4x4& sm4x4 );
	QMatrix4x4& ScaleMatirx();
	QMatrix4x4& GetTransform();
	void SetTransform(QMatrix4x4& tr);
	virtual void StartApplyTransform() = 0;
	virtual void Transform(QMatrix4x4& tr) = 0;
	virtual void EndApplyTransform() = 0;
	virtual void Transform() = 0;

public:
	friend class QRenderable;
	friend class QNode;
	friend class QScene;

protected:
	QMatrix4x4 m_TranslateMatirx;
	QMatrix4x4 m_RotateMatirx;
	QMatrix4x4 m_ScaleMatirx;
	QMatrix4x4 m_RenderableTransform;
};

