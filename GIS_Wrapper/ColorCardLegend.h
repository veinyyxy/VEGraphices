#pragma once
#include "GIS_WrapperConfig.h"
#include <QtCore/QPointer>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtGui/QPixmap>
#include <QtCore/QMap>
#include <QtGui/QPainter>
#include <QtCore/QRect>
#include <QtGui/QPainterPath>
#include "Legend.h"
#include "Layer.h"

class GIS_WRAPPER_EXPORT_IMPORT ColorCardLegend : public Legend
{
public:
	//typedef QMap<double,COLORITEM> VALUE_COLOR;
	ColorCardLegend(const COLORITEM_TABLE& vc, LEGEND_ARRANGE arrange = HORIZONTAL, int iCardWidht = 35, int iCardHeight = 15);
	virtual ~ColorCardLegend(void);
	void GeneralLegend();
	void UpdateLegend();
	inline void SetData(COLORITEM_TABLE& data){m_CardValue = data;}
	inline QPixmap* GetPixmap(){return m_pPixmap;}
	inline void SetTextColor(const QColor& qc){m_TextColor = qc;}
	inline void IsDrawText(bool dt){m_bDrawText = dt;}
private:
	COLORITEM_TABLE m_CardValue;
	QColor m_TextColor;
	bool m_bDrawText;
};

