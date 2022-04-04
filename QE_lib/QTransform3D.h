#pragma once

#include "QT_Include.h"
#include "GL_Include.h"

class QE_LIB_EXPORT_IMPORT QTransform3D : public QObject
{
public:
	QTransform3D(void);
	~QTransform3D(void);
	inline void Mode(GLenum mode){m_TransforMode = mode;}
	inline GLenum Mode(){return m_TransforMode;}
	inline void Tranlate(const QVector3D& pos){m_Translate.translate(pos);}
	inline QMatrix4x4& Tranlate(){return m_Translate;}
	inline void Tranlate(const QMatrix4x4& transMaix){m_Translate = transMaix;}
	inline void Rotate(GLfloat angle, const QVector3D& aix){m_Rotate.rotate(angle, aix);}
	inline QMatrix4x4& Rotate(){return m_Rotate;}
	inline void Rotate(const QMatrix4x4& rotMaix){m_Rotate = rotMaix;}
	inline void Scale(const QVector3D& scal){m_Scale.scale(scal);}
	inline QMatrix4x4& Scale(){return m_Scale;}
	inline void Scale(const QMatrix4x4& scaMaix){m_Scale = scaMaix;}
	QTransform3D& operator=(const QTransform3D& other);
	void EnableTransforms();
	void UnableTransforms();
	void ToIdentity();
	inline QMatrix4x4 GetMultTransform(){return m_AllInTransform;}
private:
	inline void MatrixTransform(const QMatrix4x4& maix){m_AllInTransform = maix;}
	QMatrix4x4 m_Translate;
	QMatrix4x4 m_Rotate;
	QMatrix4x4 m_Scale;
	QMatrix4x4 m_AllInTransform;
	QMatrix4x4 m_temp;
	GLenum m_TransforMode;
};

