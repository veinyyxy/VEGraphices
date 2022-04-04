
/*============================================================*/ 

#pragma once

#include "SymbolLine.h"
#include "SymbolData.h"
#include "SymbolLayer.h"

#include "ManualDraw.h"
#include <stdlib.h>
#include "SymbolPoint.h"
#include "ColorAttribute.h"
//#include "UndoAddPointCommand.h"

//#include "QTextureFont.h"
#include "TextBox.h"


class GIS_WRAPPER_EXPORT_IMPORT DrawTexSymbol :public ManualDraw
{
	Q_OBJECT
public:
	DrawTexSymbol(CoordinateTransform* proj);
	~DrawTexSymbol(void);

	void MouseEvent(QMouseEvent* event);
	void MousePress(QMouseEvent *event);
	void MouseRelease(QMouseEvent *event);
	void LeaveEvent();
	void SetSymbol(Symbol *pSymbol) {m_Symbol = pSymbol;}
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void initAttribute();
	void SetParentLayer(Layer * Layer){m_ParentLayer = Layer;}
	void SetCoordinateTransform(CoordinateTransform* proj){m_proj = dynamic_cast<RadProjTransform*> (proj);}
private:
	
	bool  bFinished ;			//判断是否
	//bool m_bFirst;
	SymbolData *m_pSymbolData;
	
	QVector3D m_TextPos;
	QString m_strFont;
	int m_FontSize;
	QVector4D m_FontColor;
	QStringList strFontTypes;
	int m_nFontIndex;
	CTextBox *pTextBox;
	bool m_bStart;
	SymbolData *m_nSymbolData;
	QFont m_font;
	RadProjTransform* m_proj;
public slots:
	void DrawText(QString strContext);                 //鼠标在图层上释放事件
	
};