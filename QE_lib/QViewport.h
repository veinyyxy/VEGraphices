#pragma once

#include "QT_Include.h"
#include "GL_Include.h"
/**********************************
用于管理OpenGL视口变换
**********************************/
class QE_LIB_EXPORT_IMPORT QViewport : virtual public QObject
{
public:
	QViewport(void);
	QViewport(GLfloat x, GLfloat y, GLfloat w, GLfloat h);
	~QViewport(void);
	void SetViewport(GLfloat x, GLfloat y, GLfloat w, GLfloat h);
	void CreateViewport();
	inline GLfloat X();
	inline GLfloat Y();
	inline GLfloat Width();
	inline GLfloat Height();
	GLfloat GetViewportPix();
	void ScreenToGL( const QVector3D& screenPoint, QVector3D& glPoint );
	void ApplyViewport();
	const QViewport& operator=(const QViewport& it);

private:
	GLfloat x;
	GLfloat y;
	GLfloat width;
	GLfloat height;
};

