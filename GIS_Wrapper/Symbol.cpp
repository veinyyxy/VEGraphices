#include "Symbol.h"
#include "PixmapTexture.h"
// Bitmap of camp fire
#if 0
Symbol::Symbol(void) 
{
	m_pBitmap = NULL;
}


Symbol::~Symbol(void)
{

}


void Symbol::Render()
{
	glRasterPos2i(0, 32.0);

	glBitmap (32, 32, 100.0, 0.0, 32.0, 0.0, fire);
}

QVector4DArray* Symbol::getColorArray()
{
	return NULL;
}
QVector2DArray* Symbol::getTextureAttray()
{
	return NULL;
}
QVector3DArray* Symbol::getNormalAttray()
{
	return NULL;
}
Line* Symbol::AsLine()
{
	return NULL;
}
Plane* Symbol::AsPlane()
{
	return NULL;
}
GISPolygon* Symbol::AsPolygon()
{
	return NULL;
}
Point* Symbol::AsPoint()
{
	return NULL;
}

void Symbol::SetSymbolMode(int nMode)
{
	 m_nMode = nMode;
 };

 void Symbol::SetBitmap(GLubyte * pBitmap, BITMAP_INFO *pBmpInfo)
 {
// 	 if(NULL != pBitmap )
// 		// m_pBitmap = pBitmap;
// 	 else
// 	 {
// 		// m_pBitmap = fire;
// 	 }

	 if(NULL!= pBmpInfo)
	 {
// 		 m_nBmpWidth = pBmpInfo->BmpWidth;
// 		 m_nBmpHeight = pBmpInfo->BmpHeight;
// 		 m_fBmpOrigX = pBmpInfo->BmpOrigX;
// 		 m_fBmpOrigY = pBmpInfo->BmpOrigY;
// 		 m_fBmpMoveX = pBmpInfo->BmpMoveX;
// 		 m_fBmpMoveY = pBmpInfo->BmpMoveY;
	 }else
	 {
		 m_nBmpWidth = 10;
		 m_nBmpHeight = 12;
		 m_fBmpOrigX = 0.0;
		 m_fBmpOrigY = 0.0;
		 m_fBmpMoveX = 13.0;
		 m_fBmpMoveY = 0.0;
	 }
		
 }
#else
Symbol::Symbol(void)
{
	m_bSelected = false;
	m_nCount = 0 ;
	m_pAttribute = NULL;
	m_SelectColor.setX(0.5f);
	m_SelectColor.setY(1.0f);
	m_SelectColor.setZ(0.0f);
	m_SelectColor.setW(1.0f);
	m_nType = 0;
}

void Symbol::SetNormalColor(QVector4D normalcolor)
{
	m_NormalColor.setX(normalcolor.x());
	m_NormalColor.setY(normalcolor.y());
	m_NormalColor.setZ(normalcolor.z());
	m_NormalColor.setW(normalcolor.w());
}

bool Symbol::SetTextureTGA( QString fileName )
{
	CTGATexture* pTexture = new CTGATexture;
	//pTexture = m_TextureAttrib.CreateTexture();
	pTexture->TargetTextrue(GL_TEXTURE_2D);
	if(!pTexture) return false;
	pTexture->LoadTextrueFromFile(fileName);
	m_TextureAttrib.SetTexture(pTexture);
	//m_TextureAttrib.TargetTexture(GL_TEXTURE_2D);
	AttribSet()->insert(QE_ATTRI_TEXTURE0, &m_TextureAttrib);
	SetArrayData(&texture);
	return true;
}


void Symbol::ScreenToGL( const QVector3D& screenPoint, QVector3D& glPoint )
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

bool Symbol::SetTextureFromPixmap( const QPixmap* pPixmap, QGLWidget* pContext)
{
	CPixmapTexture* pTexture = new CPixmapTexture(pContext);
	pTexture->TargetTextrue(GL_TEXTURE_2D);
	if(!pTexture) return false;
	pTexture->LoadTextrueFromPixmap(pPixmap);
	m_TextureAttrib.SetTexture(pTexture);
	//m_TextureAttrib.TargetTexture(GL_TEXTURE_2D);
	AttribSet()->insert(QE_ATTRI_TEXTURE0, &m_TextureAttrib);
	SetArrayData(&texture);
}

void Symbol::setSymbolSize(int nsize)
{
	m_nSymbolSize = nsize;
}

#endif

