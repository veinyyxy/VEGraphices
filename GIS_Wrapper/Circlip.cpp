#include "Circlip.h"


Circlip::Circlip(void)
{
}


Circlip::~Circlip(void)
{
}

void Circlip::Render()
{
	if(m_bBlend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
	}
	QRenderUnit::Render();
	if(m_bBlend) glDisable(GL_BLEND);
}

void Circlip::ConcrateCirclip(float radius, GLint slices, float fZ, float fO, float fI)
{
	if(slices < 3) return;

	const float cfPI = 3.141592653589793;
	float x, y, xO, yO, xI, yI;
	int iCount = 360 / slices;
	float fCount = 360 /slices;
	vertex.resize(iCount * (3 + 3));
	
	for(int i = 0; i < iCount; i++)
	{
		x = sin(fCount * cfPI / 180.0);
		y = cos(fCount * cfPI / 180.0);
		xO += fO;
		yO += fO;

		/*xI -= fI;
		yI == fI;*/
		vertex[i * 6 + 0] = QVector3D(x, y, fZ);
		//vertex[i * 6 + 1] = QVector3D(xO, fO, fZ);
		vertex[i * 6 + 2] = QVector3D(xO, fO, fZ);

		vertex[i * 6 + 3] = QVector3D(x, y, fZ);
		//vertex[i * 6 + 4] = QVector3D(x, y, fZ);
		//vertex[i * 6 + 5] = QVector3D(x, y, fZ);
	}

	for(int i = 0; i < iCount; i++)
	{
		x = sin(fCount * cfPI / 180.0);
		y = cos(fCount * cfPI / 180.0);
		xO += fO;
		yO += fO;

		/*xI -= fI;
		yI == fI;*/
		//vertex[i * 6 + 0] = QVector3D(x, y, fZ);
		vertex[i * 6 + 1] = QVector3D(xO, fO, fZ);
		//vertex[i * 6 + 2] = QVector3D(xO, fO, fZ);

		//vertex[i * 6 + 3] = QVector3D(x, y, fZ);
		vertex[i * 6 + 4] = QVector3D(x, y, fZ);
		vertex[i * 6 + 5] = QVector3D(x, y, fZ);
	}
}
