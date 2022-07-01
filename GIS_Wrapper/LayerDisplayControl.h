#pragma once
#include "GIS_WrapperConfig.h"
#include "QNodeCallBack.h"

class GIS_WRAPPER_EXPORT_IMPORT CLayerDisplayControl : public QNodeCallBack
{
public:
	CLayerDisplayControl(void);
	~CLayerDisplayControl(void);
	bool IsRender();
	void StartRender(QERenderContext* renderContext);
	void EndRender();
	void Apply(QNode& node);
    inline void SetWhetherRender(qreal fto, qreal fnot){m_toRender = fto; m_notRender = fnot;}

private:
	qreal m_toRender;
	qreal m_notRender;
};

