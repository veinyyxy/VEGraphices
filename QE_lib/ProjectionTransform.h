#pragma once
#include "GLTransform.h"
class ProjectionTransform :
	public GLTransform
{
public:
	ProjectionTransform(void);
	~ProjectionTransform(void);
	virtual void StartApplyTransform();
	virtual void Transform(QMatrix4x4& tr);
	virtual void Transform();
	virtual void EndApplyTransform();
};

