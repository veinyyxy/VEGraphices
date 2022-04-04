#pragma once
#include "Config.h"
#include "QRenderUnit.h"
#include "ColorAttribute.h"
#include "QTextrueInterface.h"
#include <QtGui/QStaticText>
#include "TextureAttribute.h"
#include "QPixmapTextrue.h"
#include "StationValues.h"

class QE_LIB_EXPORT_IMPORT QEText : public QRenderUnit 
{
public:
	typedef struct _textrue_info{
		GLuint glTextrue;
		qreal glTextrueWidth;
		qreal glTextrueHeight;
	} TEXTRUE_INFO;
	typedef QVector<QFont> FONT_LIST;
	typedef QVector<QPixmap> PIXMAP_LIST;
	typedef QVector<QColor> COLOR_LIST;
	typedef QVector<TEXTRUE_INFO> TEXTRUE_LIST;
	typedef enum {LEFT_TOP=0, LEFT_BOTTOM=1, RIGHT_TOP=2, RIGHT_BOTTOM=3, TOP_CENTRE=4,BOTTOM_CENTER=5,RIGHT_CENTER=6,LEFT_CENTER=7,CENTER=8} POSITION_ORIGIN_POINT;
	typedef enum {STRING_MODE, STRING_LIST_MODE} TEXT_MODE;
	inline QEText(void);
	inline QEText(const QString& str, const QFont& ft);
	inline QEText(const QStringList& strList, const FONT_LIST fontList);
	~QEText(void);
	inline void SetMargin(qreal ma){m_qrMargin = ma;}
	inline qreal Margin(){return m_qrMargin;}
	inline QVector3DArray* GetPositionArray(){return &m_TextPositions;}
	inline void SetTextMode(TEXT_MODE m){m_textMode = m;}
	inline TEXT_MODE TextMode(){return m_textMode;}
	bool PickUp( const QVector3D* pickPoint );
	inline int GetPickUpIndex(){return m_iPickUpIndex;}
	inline QVector3D GetPosition(){return position;}
	inline void SetSelected(bool bHadPicked) {m_bHadPicked = bHadPicked;}
	bool GetSelectFlag(){return m_bHadPicked;}
	//inline qreal SetPixmapDirty(bool pd){m_pixmapDirty = pd;}
	//inline bool PixmapDirty(){return m_pixmapDirty;}
	//////////////////////////////////////////////////////////////////////////
	inline void SetDirty(bool b = true){m_pixmapDirty = b;}
	inline void SetString(const QString& str){m_String = str;m_pixmapDirty = true;}
	inline QString String(){return m_String;}
	inline void SetFont(const QFont& ft){m_font = ft;}
	inline QFont Font(){return m_font;}
	inline void SetColor(const QColor& color){m_Color = color;m_ColorAttribute.Color(QVector4D(m_Color.redF(), m_Color.greenF(), m_Color.blueF(), m_Color.alphaF()));}
	inline QColor GetColor(){return m_Color;}
	//////////////////////////////////////////////////////////////////////////
	inline void SetStringList(const QStringList& strList){m_StringList = strList;m_pixmapDirty = true;}
	inline QStringList& StringList(){return m_StringList;}
	inline void SetFontList(const FONT_LIST& fontList){m_fontList = fontList;}
	inline FONT_LIST FontList(){return m_fontList;}
	inline void SetColorList(const COLOR_LIST& colorList){m_ColorList = colorList;}
	inline COLOR_LIST ColorList(){return m_ColorList;}
	inline void SetOriPosition(POSITION_ORIGIN_POINT flag) {m_OriPosition = flag;}
	inline POSITION_ORIGIN_POINT GetOriPosition() {return m_OriPosition;}
	inline QVector<StationValues>* StationValueArray();
	//////////////////////////////////////////////////////////////////////////
	void Render();
protected:
	QVector<StationValues> m_SValuesArray;//站点数据数组
private:
	TEXT_MODE m_textMode;
	QVector3DArray m_TextPositions;
	//QVector2DArray m_TextureCoord;
	ColorAttribute m_ColorAttribute;
	//TextureAttribute m_TextrueAttribute;
	//QPixmapTextrue m_FontTextrue;
	qreal m_qrMargin;
	bool m_pixmapDirty;
	QEText::TEXTRUE_INFO DrawTextToPixmap(const QString& str, const QFont& font, QGLContext* pTempGLContext);
	void DrawOneString(const QVector3D& pos, TEXTRUE_INFO textrueinfo, POSITION_ORIGIN_POINT oripoint);
	void  ScreenToGL( const QVector3D& screenPoint, QVector3D& glPoint );
	bool GetRectPoint(POSITION_ORIGIN_POINT ogrPosition, const QVector3D& ogrPoint, bool bChange
		, qreal stringWidth, qreal stringHeight
		, QVector3D& point1, QVector3D& point2
		, QVector3D& point3, QVector3D& point4);
	void DeleteTextModeTextrue(QGLContext* pGlContext);
	void DeleteTextListModeTextrue(QGLContext* pGlContext);
	qreal m_stringHeight;
	qreal m_stringWidth;
	QGLContext* pGLContext;

	GLuint m_TextrueID;
	QPixmap m_pixmap;
	QString m_String;
	QFont m_font;
	QColor m_Color;
	
	TEXTRUE_LIST m_TextrueList;
	PIXMAP_LIST m_pixmapList;
	QStringList m_StringList;
	FONT_LIST m_fontList;
	COLOR_LIST m_ColorList;
	POSITION_ORIGIN_POINT m_OriPosition;		//标注原点的标识
	QVector3D position;
	int m_iPickUpIndex;
	bool m_bHadPicked;
};

