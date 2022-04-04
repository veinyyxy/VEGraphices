#pragma once
#include "Polygon.h"

class ConcavePolygon : public GISPolygon
{
public:
	ConcavePolygon(void);
	~ConcavePolygon(void);
	void Render();

private:
	bool m_bTred;
	TessStruct *m_pTessData;
};

