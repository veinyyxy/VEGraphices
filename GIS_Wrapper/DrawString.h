#pragma once
#include "ManualDraw.h"
#include "TextBox.h"

class GIS_WRAPPER_EXPORT_IMPORT CDrawString:public ManualDraw
{
public:
	CDrawString(int nType);
	~CDrawString(void);
	void MouseEvent(QMouseEvent* event);
	void MousePress(QMouseEvent *event);
	void MouseRelease(QMouseEvent *event);
	void LeaveEvent();
	void SetSymbol(Symbol *pSymbol) {m_Symbol = pSymbol;}
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void initAttribute();
	void SetParentLayer(Layer * Layer){m_ParentLayer = Layer;}

private:

	bool  bFinished ;			//≈–∂œ «∑Ò
    QColor m_FontColor;
	int m_FontSize;
	QVector3D m_TextPos;
	QString m_strDraw;
};

