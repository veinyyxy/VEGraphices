#pragma once
#include "QDataArray.h"

class QFoundRenderUnit
{
public:
	virtual void Found(QDataArray* data) = 0;
	virtual void Found(int iIndex, QRenderable* pRenderable) = 0;
};