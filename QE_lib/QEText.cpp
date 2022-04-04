#include "QEText.h"
#if 1

QEText::QEText( void ) : QRenderUnit(SIMPLEX)
	, m_qrMargin(0)
	, m_pixmapDirty(true)
	, m_TextrueID(0)
	, m_textMode(STRING_MODE)
	, pGLContext(0)
{
	m_TextPositions.DataUsage(GL_VERTEX_ARRAY);
	SetArrayData(&m_TextPositions);
	m_OriPosition = RIGHT_BOTTOM;   //默认为右下角
	m_bHadPicked=false;
}

QEText::QEText( const QString& str, const QFont& ft ) : QRenderUnit(SIMPLEX)
	, m_font(ft)
	, m_String(str)
	, m_qrMargin(0)
	, m_pixmapDirty(true)
	, m_TextrueID(0)
	, m_textMode(STRING_MODE)
	, pGLContext(0)
{
	m_TextPositions.DataUsage(GL_VERTEX_ARRAY);
	SetArrayData(&m_TextPositions);
	m_OriPosition = RIGHT_BOTTOM;   //默认为右下角
	m_bHadPicked=false;
}

QEText::QEText( const QStringList& strList, const FONT_LIST fontList ) : QRenderUnit(SIMPLEX)
	, m_StringList(strList)
	, m_fontList(fontList)
	, m_qrMargin(0)
	, m_pixmapDirty(true)
	, m_TextrueID(0)
	, m_textMode(STRING_MODE)
	, pGLContext(0)
{
	m_TextPositions.DataUsage(GL_VERTEX_ARRAY);
	SetArrayData(&m_TextPositions);
	m_OriPosition = RIGHT_BOTTOM;   //默认为右下角
	m_bHadPicked=false;
}

QEText::~QEText(void)
{
	SetArrayData(false, &m_TextPositions);
	QGLContext* pTempGLContext = const_cast<QGLContext*>(QGLContext::currentContext());
	if(pTempGLContext)
	{
		//pTempGLContext->deleteTexture(m_TextrueID);
		for(int i = 0; i < m_TextrueList.size(); i++)
			pTempGLContext->deleteTexture(m_TextrueList[i].glTextrue);
	}
}

void QEText::Render()
{
	QEText::TEXTRUE_INFO textureinfo;
	GLuint textrue = 0;
	if(!m_pRenderContext) return;
	QGLContext* pTempGLContext = const_cast<QGLContext*>(QGLContext::currentContext());
	QMatrix4x4 rotMatrix = m_pRenderContext->Transform()->Rotate();
	if(pTempGLContext != pGLContext)
	{
		m_pixmapDirty = true;
		m_TextrueList.clear();
		/*DeleteTextModeTextrue(pGLContext);
		DeleteTextListModeTextrue(pGLContext);*/
	}
	if(m_textMode == STRING_MODE)
	{
		if(m_pixmapDirty)
		{
			if(m_String.length() != 0)
			{
				textureinfo = DrawTextToPixmap(m_String, m_font, pTempGLContext);
				if (m_TextrueList.isEmpty())
					m_TextrueList.push_back(textureinfo);
				else
					m_TextrueList[0] = textureinfo;
			}			
		}
		else
			textureinfo = m_TextrueList[0];
		
		if (!m_TextrueList.isEmpty())
		{
			QVector3D pos = m_TextPositions[0];
			pos = rotMatrix * pos;
			DrawOneString(pos, m_TextrueList[0], m_OriPosition);
		}		
	}
	if(m_textMode == STRING_LIST_MODE)
	{
		int iStringCount = m_StringList.length();
		if(iStringCount == 0 || m_StringList.length() != m_TextPositions.count()) return;
		if(m_pixmapDirty)
		{
			for(int iStrCount = 0; iStrCount < iStringCount; iStrCount++)
			{
				m_TextrueID = 0;
				m_pixmapDirty = true;
				textureinfo = DrawTextToPixmap(m_StringList[iStrCount], m_font, pTempGLContext);
				m_TextrueList.push_back(textureinfo);
			}
			m_pixmapDirty = false;
		}
		
		for(int iT = 0; iT < m_TextrueList.size(); iT++)
		{
			QVector3D pos = m_TextPositions[iT];
			pos =  rotMatrix * pos;
			DrawOneString(pos, m_TextrueList[iT], m_OriPosition);
		}
	}
}

QEText::TEXTRUE_INFO QEText::DrawTextToPixmap(const QString& str, const QFont& font, QGLContext* pTempGLContext)
{
	TEXTRUE_INFO textureinfo = {0};
	GLuint texture = 0;
	if(str.length() != 0)
	{
		if(m_pixmapDirty)
		{
			QFontMetricsF fontMerics(font);
			textureinfo.glTextrueHeight = fontMerics.height();
			textureinfo.glTextrueWidth = fontMerics.width(str);

			QPixmap* pPixmap = new QPixmap(textureinfo.glTextrueWidth + m_qrMargin * 2, textureinfo.glTextrueHeight + m_qrMargin * 2);
			pPixmap->fill(QColor(255, 255, 255, 0));
			QPainter painter;
			painter.begin(pPixmap);
			painter.setPen(QColor(255,255,255,255));
			painter.setFont(font);

			painter.drawStaticText(0, 0, QStaticText(str));
			painter.end();
			
			if(!pTempGLContext) return textureinfo;

			textureinfo.glTextrue = pTempGLContext->bindTexture(*pPixmap);
			if(pPixmap) delete pPixmap;
			m_pixmapDirty = false;
			pGLContext = pTempGLContext;
			return textureinfo;
		}
		return textureinfo;
	}
	return textureinfo;
}

void QEText::DrawOneString( const QVector3D& pos, TEXTRUE_INFO textrueinfo, POSITION_ORIGIN_POINT oripoint)
{
	qreal tempCameraScale = 1.0;

	glEnable(GL_TEXTURE_2D);
  	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);  
  	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,   GL_REPEAT);  
 	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);  
 	glTexParameterf(GL_TEXTURE_2D,   GL_TEXTURE_MAG_FILTER,GL_NEAREST);  

	glBindTexture(GL_TEXTURE_2D, textrueinfo.glTextrue);

	QVector3D pos1, pos2, pos3, pos4;
	if(!GetRectPoint(m_OriPosition, pos, true, textrueinfo.glTextrueWidth, textrueinfo.glTextrueHeight, pos1, pos2, pos3, pos4)) return;

	glBegin(GL_QUADS);

	GLfloat backColor[4] = {0};
	glGetFloatv(GL_CURRENT_COLOR, backColor);
	glColor4f(m_Color.redF(), m_Color.greenF(), m_Color.blueF(), m_Color.alphaF());
	if (m_bHadPicked)
	{
		glColor4f(0.5f, 0.0f, 1.0f, 1.0f);
	}

	//glVertex3d(x, y, z);
	glVertex3d(pos1.x(), pos1.y(), pos1.z());
	glTexCoord2d(1.0, 1.0);

	glVertex3d(pos2.x(), pos2.y(), pos2.z());
	glTexCoord2d(0, 1.0);

	glVertex3d(pos3.x(), pos3.y(), pos3.z());
	glTexCoord2d(0, 0);

	glVertex3d(pos4.x(), pos4.y(), pos4.z());
	glTexCoord2d(1.0, 0);

	glColor4f(backColor[0], backColor[1], backColor[2], backColor[3]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


void  QEText::ScreenToGL( const QVector3D& screenPoint, QVector3D& glPoint )
{
	GLint    viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ = 0.0f;
	GLdouble posX, posY, posZ;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	winX = screenPoint.x();
	winY = viewport[3] - screenPoint.y();
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	glPoint.setX(posX);
	glPoint.setY(posY);
	glPoint.setZ(0.0);
}

QVector<StationValues>* QEText::StationValueArray()
{
	return &m_SValuesArray;
}

bool QEText::PickUp( const QVector3D* pickPoint )
{
	bool bResult = false;
	QVector3D screenPoint, scPointUp, scPointMid;
	pickPrecision = 15;
	//float* point = 0;
	QVector3D pickPointGL = *pickPoint, point1, point2, point3, point4, screenPoint1, /*screenPoint2, */screenPoint3/*, screenPoint4*/;

	//qreal* dataPoint = 0, x(0), y(0), z(0), x1(0), y1(0), z1(0);
	qreal dis, dis2, dis3;
	//QVector3D startPoint, EndPoint, tempPoint;

	for(int i = 0; i < m_TextPositions.size(); i++)
	{
		QVector3D ogirPoint = m_TextPositions[i];
		
		if(!GetRectPoint(m_OriPosition, ogirPoint, true, m_TextrueList[i].glTextrueWidth, m_TextrueList[i].glTextrueHeight,point1, point2, point3, point4)) return false;
		ogTOs(point1, screenPoint1);
		//ogTOs(point2, screenPoint2);
		ogTOs(point3, screenPoint3);
		//ogTOs(point4, screenPoint4);

		QRectF textRect(QPointF(screenPoint3.x(), screenPoint3.y()), QPointF(screenPoint1.x(), screenPoint1.y()));
		bResult = textRect.contains(QPointF(pickPoint->x(), pickPoint->y()));
		if(bResult)
		{
			position = m_TextPositions[i];
			m_iPickUpIndex = i;
			return true;
		}
	}
	return false;
}

bool QEText::GetRectPoint( POSITION_ORIGIN_POINT ogrPosition, const QVector3D& ogrPoint, bool bChange
	, qreal stringWidth, qreal stringHeight
	, QVector3D& point1, QVector3D& point2
	, QVector3D& point3, QVector3D& point4 )
{
	if(!m_pRenderContext) return false;
	qreal tempCameraScale = 1.0;
	QMatrix4x4 rotMatrix;
	qreal x, y, z;
	x = ogrPoint.x();
	y = ogrPoint.y();
	z = ogrPoint.z();
	if(bChange)
	{
		tempCameraScale = m_pRenderContext->Observer()->Camera()->GetCameraScale();
		rotMatrix = m_pRenderContext->Transform()->Rotate();
		rotMatrix = rotMatrix.transposed();
	}
	switch(ogrPosition)
	{
	case LEFT_TOP :
		point1.setX(x + stringWidth * tempCameraScale);
		point1.setY(y - stringHeight * tempCameraScale);
		point1.setZ(z);

		point2.setX(x + stringWidth * tempCameraScale);
		point2.setY(y );
		point2.setZ(z);

		point3.setX(x );
		point3.setY(y);
		point3.setZ(z);

		point4.setX(x);
		point4.setY(y - stringHeight * tempCameraScale);
		point4.setZ(z);

		break;
	case LEFT_BOTTOM:
		point1.setX(x + stringWidth * tempCameraScale );
		point1.setY(y);
		point1.setZ(z);

		point2.setX(x + stringWidth * tempCameraScale);
		point2.setY(y + stringHeight * tempCameraScale);
		point2.setZ(z);

		point3.setX(x );
		point3.setY(y -stringHeight * tempCameraScale);
		point3.setZ(z);

		point4.setX(x);
		point4.setY(y);
		point4.setZ(z);
		break;
	case  RIGHT_TOP:
		point1.setX(x);
		point1.setY(y - stringHeight * tempCameraScale);
		point1.setZ(z);

		point2.setX(x );
		point2.setY(y );
		point2.setZ(z);

		point3.setX(x - stringWidth * tempCameraScale);
		point3.setY(y);
		point3.setZ(z);

		point4.setX(x - stringWidth * tempCameraScale);
		point4.setY(y - stringHeight * tempCameraScale);
		point4.setZ(z);
		break;
	case RIGHT_BOTTOM:
		point1.setX(x);
		point1.setY(y);
		point1.setZ(z);

		point2.setX(x );
		point2.setY(y + stringHeight * tempCameraScale );
		point2.setZ(z);

		point3.setX(x - stringWidth * tempCameraScale);
		point3.setY(y + stringHeight * tempCameraScale);
		point3.setZ(z);

		point4.setX(x -stringWidth * tempCameraScale);
		point4.setY(y );
		point4.setZ(z);
		break;
	case TOP_CENTRE:
		point1.setX(x + stringWidth * tempCameraScale /  2);
		point1.setY(y - stringHeight * tempCameraScale );
		point1.setZ(z);

		point2.setX(x +  stringWidth * tempCameraScale /  2);
		point2.setY(y);
		point2.setZ(z);

		point3.setX(x - stringWidth * tempCameraScale /2);
		point3.setY(y );
		point3.setZ(z);

		point4.setX(x - stringWidth * tempCameraScale /2);
		point4.setY(y - stringHeight * tempCameraScale );
		point4.setZ(z);
		break;
	case BOTTOM_CENTER :
		point1.setX(x + stringWidth * tempCameraScale / 2);
		point1.setY(y);
		point1.setZ(z);

		point2.setX(x +  stringWidth * tempCameraScale /  2);
		point2.setY(y + stringHeight * tempCameraScale );
		point2.setZ(z);

		point3.setX(x - stringWidth * tempCameraScale /2);
		point3.setY(y + stringHeight * tempCameraScale);
		point3.setZ(z);

		point4.setX(x - stringWidth * tempCameraScale /2);
		point4.setY(y );
		point4.setZ(z);
		break;
	case LEFT_CENTER :
		point1.setX(x + stringWidth * tempCameraScale);
		point1.setY(y - stringHeight * tempCameraScale /2);
		point1.setZ(z);

		point2.setX(x +  stringWidth * tempCameraScale );
		point2.setY(y + stringHeight * tempCameraScale / 2 );
		point2.setZ(z);

		point3.setX(x);
		point3.setY(y + stringHeight * tempCameraScale / 2);
		point3.setZ(z);

		point4.setX(x);
		point4.setY(y  - stringHeight * tempCameraScale /2 );
		point4.setZ(z);
		break;
	case RIGHT_CENTER:
		point1.setX(x);
		point1.setY(y - stringHeight * tempCameraScale /2);
		point1.setZ(z);

		point2.setX(x );
		point2.setY(y + stringHeight * tempCameraScale / 2 );
		point2.setZ(z);

		point3.setX(x -  stringWidth * tempCameraScale);
		point3.setY(y + stringHeight * tempCameraScale /2);
		point3.setZ(z);

		point4.setX(x -  stringWidth * tempCameraScale);
		point4.setY(y  - stringHeight * tempCameraScale /2 );
		point4.setZ(z);
		break;
	case CENTER:
		point1.setX(x + stringWidth * tempCameraScale / 2);
		point1.setY(y - stringHeight * tempCameraScale /2);
		point1.setZ(z);

		point2.setX(x +  stringWidth * tempCameraScale /  2);
		point2.setY(y + stringHeight * tempCameraScale /2 );
		point2.setZ(z);

		point3.setX(x - stringWidth * tempCameraScale /2);
		point3.setY(y + stringHeight * tempCameraScale /2);
		point3.setZ(z);

		point4.setX(x - stringWidth * tempCameraScale /2);
		point4.setY(y - stringHeight * tempCameraScale /2);
		point4.setZ(z);
		break;
	default:
		break;
	}
	point1 = rotMatrix * point1;
	point2 = rotMatrix * point2;
	point3 = rotMatrix * point3;
	point4 = rotMatrix * point4;

	return true;
}

void QEText::DeleteTextModeTextrue(QGLContext* pGlContext)
{
	if(pGlContext)
	{
		pGlContext->deleteTexture(m_TextrueID);
		m_TextrueID = 0;
	}
}

void QEText::DeleteTextListModeTextrue(QGLContext* pGlContext)
{
	if(pGlContext)
	{
		for(int i = 0; i < m_TextrueList.size(); i++)
			pGlContext->deleteTexture(m_TextrueList[i].glTextrue);
		m_TextrueList.clear();
	}
}

#else
QEText::QEText( void ) : QRenderUnit(SIMPLEX)
	, m_qrMargin(0)
	, m_pixmapDirty(true)
	, m_TextrueID(0)
	, m_textMode(STRING_MODE)
	, pGLContext(0)
{
	m_TextPositions.DataUsage(GL_VERTEX_ARRAY);
	SetArrayData(&m_TextPositions);
	m_OriPosition = RIGHT_BOTTOM;   //默认为右下角
	
}

QEText::QEText( const QString& str, const QFont& ft ) : QRenderUnit(SIMPLEX)
	, m_font(ft)
	, m_String(str)
	, m_qrMargin(0)
	, m_pixmapDirty(true)
	, m_TextrueID(0)
	, m_textMode(STRING_MODE)
	, pGLContext(0)
{
	m_TextPositions.DataUsage(GL_VERTEX_ARRAY);
	SetArrayData(&m_TextPositions);
	m_OriPosition = RIGHT_BOTTOM;   //默认为右下角
}

QEText::QEText( const QStringList& strList, const FONT_LIST fontList ) : QRenderUnit(SIMPLEX)
	, m_StringList(strList)
	, m_fontList(fontList)
	, m_qrMargin(0)
	, m_pixmapDirty(true)
	, m_TextrueID(0)
	, m_textMode(STRING_MODE)
	, pGLContext(0)
{
	m_TextPositions.DataUsage(GL_VERTEX_ARRAY);
	SetArrayData(&m_TextPositions);
	m_OriPosition = RIGHT_BOTTOM;   //默认为右下角
}

QEText::~QEText(void)
{
	SetArrayData(false, &m_TextPositions);
	QGLContext* pTempGLContext = const_cast<QGLContext*>(QGLContext::currentContext());
	if(pTempGLContext)
	{
		//pTempGLContext->deleteTexture(m_TextrueID);
		for(int i = 0; i < m_TextrueList.size(); i++)
			pTempGLContext->deleteTexture(m_TextrueList[i].glTextrue);
	}
}

void QEText::Render()
{
	QMatrix4x4 rotMatrix = m_pRenderContext->Transform()->Rotate();
	QEText::TEXTRUE_INFO textureinfo;
	GLuint textrue = 0;
	if(!m_pRenderContext) return;
	QGLContext* pTempGLContext = const_cast<QGLContext*>(QGLContext::currentContext());
	if(pTempGLContext != pGLContext)
	{
		m_pixmapDirty = true;
		m_TextrueList.clear();
		/*DeleteTextModeTextrue(pGLContext);
		DeleteTextListModeTextrue(pGLContext);*/
	}
	if(m_textMode == STRING_MODE)
	{
		if(m_pixmapDirty)
		{
			if(m_String.length() != 0)
			{
			  textureinfo = DrawTextToPixmap(m_String, m_font, pTempGLContext);
			  m_TextrueList.push_back(textureinfo);
			}			
		}
		else
			textureinfo = m_TextrueList[0];

		//if(textrue) m_TextrueID = textrue;
		
		if (!m_TextrueList.isEmpty())
		{
			QVector3D pos = m_TextPositions[0];
			pos =  rotMatrix * pos;
			DrawOneString(pos, m_TextrueList[0], m_OriPosition);
		}
	}
	if(m_textMode == STRING_LIST_MODE)
	{
		int iStringCount = m_StringList.length();
		if(iStringCount == 0 || m_StringList.length() != m_TextPositions.count()) return;
		if(m_pixmapDirty)
		{
			for(int iStrCount = 0; iStrCount < iStringCount; iStrCount++)
			{
				m_TextrueID = 0;
				m_pixmapDirty = true;
				textureinfo = DrawTextToPixmap(m_StringList[iStrCount], m_font, pTempGLContext);
				m_TextrueList.push_back(textureinfo);
			}
			m_pixmapDirty = false;
		}
		
		for(int iT = 0; iT < m_TextrueList.size(); iT++)
		{
			QVector3D pos = m_TextPositions[iT];
			pos =  rotMatrix * pos;
			DrawOneString(pos, m_TextrueList[iT], m_OriPosition);
		}
	}
}

QEText::TEXTRUE_INFO QEText::DrawTextToPixmap(const QString& str, const QFont& font, QGLContext* pTempGLContext)
{
	TEXTRUE_INFO textureinfo;
	GLuint texture = 0;
	if(str.length() != 0)
	{
		if(m_pixmapDirty)
		{
			QFontMetricsF fontMerics(font);
			textureinfo.glTextrueHeight = fontMerics.height();
			textureinfo.glTextrueWidth = fontMerics.width(str);

			QPixmap* pPixmap = new QPixmap(textureinfo.glTextrueWidth + m_qrMargin * 2, textureinfo.glTextrueHeight + m_qrMargin * 2);
			pPixmap->fill(QColor(255, 255, 255, 0));
			QPainter painter;
			painter.begin(pPixmap);
			painter.setPen(QColor(255,255,255,255));
			painter.setFont(font);
			//painter.drawText(0 + m_qrMargin, m_stringHeight + m_qrMargin, m_String);
			painter.drawStaticText(0, 0, QStaticText(str));
			painter.end();
			//m_pixmap = *pPixmap;
			//QImage image = m_pixmap.toImage();
			//image.save("f://test.png");
			//m_FontTextrue.LoadTextrueFromPixmap(&m_pixmap);
			
			if(!pTempGLContext) return textureinfo;

			textureinfo.glTextrue = pTempGLContext->bindTexture(*pPixmap);
			if(pPixmap) delete pPixmap;
			m_pixmapDirty = false;
			pGLContext = pTempGLContext;
			return textureinfo;
		}
		return textureinfo;
	}
}

void QEText::DrawOneString( const QVector3D& pos, TEXTRUE_INFO textrueinfo, POSITION_ORIGIN_POINT oripoint)
{
	qreal tempCameraScale = 1.0;

	glEnable(GL_TEXTURE_2D);
  	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);  
  	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,   GL_REPEAT);  
 	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);  
 	glTexParameterf(GL_TEXTURE_2D,   GL_TEXTURE_MAG_FILTER,GL_NEAREST);  

	glBindTexture(GL_TEXTURE_2D, textrueinfo.glTextrue);

	QVector3D pos1, pos2, pos3, pos4;
	if(!GetRectPoint(m_OriPosition, pos, true, textrueinfo.glTextrueWidth, textrueinfo.glTextrueHeight, pos1, pos2, pos3, pos4)) return;

	glBegin(GL_QUADS);

	GLfloat backColor[4] = {0};
	glGetFloatv(GL_CURRENT_COLOR, backColor);
	glColor4f(m_Color.redF(), m_Color.greenF(), m_Color.blueF(), m_Color.alphaF());

	//glVertex3d(x, y, z);
	glVertex3d(pos1.x(), pos1.y(), pos1.z());
	glTexCoord2d(1.0, 1.0);

	glVertex3d(pos2.x(), pos2.y(), pos2.z());
	glTexCoord2d(0, 1.0);

	glVertex3d(pos3.x(), pos3.y(), pos3.z());
	glTexCoord2d(0, 0);

	glVertex3d(pos4.x(), pos4.y(), pos4.z());
	glTexCoord2d(1.0, 0);

	glColor4f(backColor[0], backColor[1], backColor[2], backColor[3]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


void  QEText::ScreenToGL( const QVector3D& screenPoint, QVector3D& glPoint )
{
	GLint    viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ = 0.0f;
	GLdouble posX, posY, posZ;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	winX = screenPoint.x();
	winY = viewport[3] - screenPoint.y();
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	glPoint.setX(posX);
	glPoint.setY(posY);
	glPoint.setZ(0.0);
}

QVector<StationValues>* QEText::StationValueArray()
{
	return &m_SValuesArray;
}

bool QEText::PickUp( const QVector3D* pickPoint )
{
	bool bResult = false;
	QVector3D screenPoint, scPointUp, scPointMid;
	pickPrecision = 15;
	//float* point = 0;
	QVector3D pickPointGL = *pickPoint, point1, point2, point3, point4, screenPoint1, /*screenPoint2, */screenPoint3/*, screenPoint4*/;

	//qreal* dataPoint = 0, x(0), y(0), z(0), x1(0), y1(0), z1(0);
	qreal dis, dis2, dis3;
	//QVector3D startPoint, EndPoint, tempPoint;

	for(int i = 0; i < m_TextPositions.size(); i++)
	{
		QVector3D ogirPoint = m_TextPositions[i];
		
		if(!GetRectPoint(m_OriPosition, ogirPoint, true, m_TextrueList[i].glTextrueWidth, m_TextrueList[i].glTextrueHeight,point1, point2, point3, point4)) return false;
		ogTOs(point1, screenPoint1);
		//ogTOs(point2, screenPoint2);
		ogTOs(point3, screenPoint3);
		//ogTOs(point4, screenPoint4);

		QRectF textRect(QPointF(screenPoint3.x(), screenPoint3.y()), QPointF(screenPoint1.x(), screenPoint1.y()));
		bResult = textRect.contains(QPointF(pickPoint->x(), pickPoint->y()));
		if(bResult)
		{
			position = m_TextPositions[i];
			m_iPickUpIndex = i;
			return true;
		}
	}
	return false;
}

bool QEText::GetRectPoint( POSITION_ORIGIN_POINT ogrPosition, const QVector3D& ogrPoint, bool bChange
	, qreal stringWidth, qreal stringHeight
	, QVector3D& point1, QVector3D& point2
	, QVector3D& point3, QVector3D& point4 )
{
	if(!m_pRenderContext) return false;
	qreal tempCameraScale = 1.0;
	QMatrix4x4 rotMatrix;
	qreal x, y, z;
	x = ogrPoint.x();
	y = ogrPoint.y();
	z = ogrPoint.z();
	if(bChange)
	{
		tempCameraScale = m_pRenderContext->Observer()->Camera()->GetCameraScale();
		rotMatrix = m_pRenderContext->Transform()->Rotate();
		rotMatrix = rotMatrix.transposed();
	}
	switch(ogrPosition)
	{
	case LEFT_TOP :
		point1.setX(x + stringWidth * tempCameraScale);
		point1.setY(y - stringHeight * tempCameraScale);
		point1.setZ(z);

		point2.setX(x + stringWidth * tempCameraScale);
		point2.setY(y );
		point2.setZ(z);

		point3.setX(x );
		point3.setY(y);
		point3.setZ(z);

		point4.setX(x);
		point4.setY(y - stringHeight * tempCameraScale);
		point4.setZ(z);

		break;
	case LEFT_BOTTOM:
		point1.setX(x + stringWidth * tempCameraScale );
		point1.setY(y);
		point1.setZ(z);

		point2.setX(x + stringWidth * tempCameraScale);
		point2.setY(y + stringHeight * tempCameraScale);
		point2.setZ(z);

		point3.setX(x );
		point3.setY(y -stringHeight * tempCameraScale);
		point3.setZ(z);

		point4.setX(x);
		point4.setY(y);
		point4.setZ(z);
		break;
	case  RIGHT_TOP:
		point1.setX(x);
		point1.setY(y - stringHeight * tempCameraScale);
		point1.setZ(z);

		point2.setX(x );
		point2.setY(y );
		point2.setZ(z);

		point3.setX(x - stringWidth * tempCameraScale);
		point3.setY(y);
		point3.setZ(z);

		point4.setX(x - stringWidth * tempCameraScale);
		point4.setY(y - stringHeight * tempCameraScale);
		point4.setZ(z);
		break;
	case RIGHT_BOTTOM:
		point1.setX(x);
		point1.setY(y);
		point1.setZ(z);

		point2.setX(x );
		point2.setY(y + stringHeight * tempCameraScale );
		point2.setZ(z);

		point3.setX(x - stringWidth * tempCameraScale);
		point3.setY(y + stringHeight * tempCameraScale);
		point3.setZ(z);

		point4.setX(x -stringWidth * tempCameraScale);
		point4.setY(y );
		point4.setZ(z);
		break;
	case TOP_CENTRE:
		point1.setX(x + stringWidth * tempCameraScale /  2);
		point1.setY(y - stringHeight * tempCameraScale );
		point1.setZ(z);

		point2.setX(x +  stringWidth * tempCameraScale /  2);
		point2.setY(y);
		point2.setZ(z);

		point3.setX(x - stringWidth * tempCameraScale /2);
		point3.setY(y );
		point3.setZ(z);

		point4.setX(x - stringWidth * tempCameraScale /2);
		point4.setY(y - stringHeight * tempCameraScale );
		point4.setZ(z);
		break;
	case BOTTOM_CENTER :
		point1.setX(x + stringWidth * tempCameraScale / 2);
		point1.setY(y);
		point1.setZ(z);

		point2.setX(x +  stringWidth * tempCameraScale /  2);
		point2.setY(y + stringHeight * tempCameraScale );
		point2.setZ(z);

		point3.setX(x - stringWidth * tempCameraScale /2);
		point3.setY(y + stringHeight * tempCameraScale);
		point3.setZ(z);

		point4.setX(x - stringWidth * tempCameraScale /2);
		point4.setY(y );
		point4.setZ(z);
		break;
	case LEFT_CENTER :
		point1.setX(x + stringWidth * tempCameraScale);
		point1.setY(y - stringHeight * tempCameraScale /2);
		point1.setZ(z);

		point2.setX(x +  stringWidth * tempCameraScale );
		point2.setY(y + stringHeight * tempCameraScale / 2 );
		point2.setZ(z);

		point3.setX(x);
		point3.setY(y + stringHeight * tempCameraScale / 2);
		point3.setZ(z);

		point4.setX(x);
		point4.setY(y  - stringHeight * tempCameraScale /2 );
		point4.setZ(z);
		break;
	case RIGHT_CENTER:
		point1.setX(x);
		point1.setY(y - stringHeight * tempCameraScale /2);
		point1.setZ(z);

		point2.setX(x );
		point2.setY(y + stringHeight * tempCameraScale / 2 );
		point2.setZ(z);

		point3.setX(x -  stringWidth * tempCameraScale);
		point3.setY(y + stringHeight * tempCameraScale /2);
		point3.setZ(z);

		point4.setX(x -  stringWidth * tempCameraScale);
		point4.setY(y  - stringHeight * tempCameraScale /2 );
		point4.setZ(z);
		break;
	case CENTER:
		point1.setX(x + stringWidth * tempCameraScale / 2);
		point1.setY(y - stringHeight * tempCameraScale /2);
		point1.setZ(z);

		point2.setX(x +  stringWidth * tempCameraScale /  2);
		point2.setY(y + stringHeight * tempCameraScale /2 );
		point2.setZ(z);

		point3.setX(x - stringWidth * tempCameraScale /2);
		point3.setY(y + stringHeight * tempCameraScale /2);
		point3.setZ(z);

		point4.setX(x - stringWidth * tempCameraScale /2);
		point4.setY(y - stringHeight * tempCameraScale /2);
		point4.setZ(z);
		break;
	default:
		break;
	}
	point1 = rotMatrix * point1;
	point2 = rotMatrix * point2;
	point3 = rotMatrix * point3;
	point4 = rotMatrix * point4;

	return true;
}

void QEText::DeleteTextModeTextrue(QGLContext* pGlContext)
{
	if(pGlContext)
	{
		pGlContext->deleteTexture(m_TextrueID);
		m_TextrueID = 0;
	}
}

void QEText::DeleteTextListModeTextrue(QGLContext* pGlContext)
{
	if(pGlContext)
	{
		for(int i = 0; i < m_TextrueList.size(); i++)
			pGlContext->deleteTexture(m_TextrueList[i].glTextrue);
		m_TextrueList.clear();
	}
}

#endif