#pragma once
#include "GLTransform.h"
class ViewModelTransform :
	public GLTransform
{
public:
	ViewModelTransform(void);
	~ViewModelTransform(void);
	virtual void StartApplyTransform();
	virtual void Transform(QMatrix4x4& tr);
	virtual void Transform();
	virtual void EndApplyTransform();
};
