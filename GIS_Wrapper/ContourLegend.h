#pragma once
#include "ColorCardLegend.h"

class ContourLegend : public ColorCardLegend
{
public:
	ContourLegend(const COLORITEM_TABLE& vc, int iCardWidht = 35, int iCardHeight = 15);
	virtual ~ContourLegend(void);
	void GeneralLegend();
};

