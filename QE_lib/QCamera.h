/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ������
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011��4��15��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����QCamera.h QCamera.cpp 
�� ���� QCamera
�� �ࣺ QObject
�� �ࣺ ��
����˵���� ���ڹ���OpenGL��ͶӰ�任
����˵���� 
================================================================*/ 
#pragma once

#include "QT_Include.h"
#include "GL_Include.h"

class QE_LIB_EXPORT_IMPORT QCamera : virtual public QObject
{
public:
	typedef enum _CAMERA_TYPE
	{
		ORTHO = 1,
		FRUSTRUM
	}CAMERA_TYPE, *P_CAMERA_TYPE;

	QCamera(CAMERA_TYPE t = ORTHO);
	QCamera(GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble zN, GLdouble zF);
	QCamera(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	~QCamera(void);
	void SetLookAt(const QVector3D& pos, const QVector3D& cen, const QVector3D& up);
	void SetPerspectiveMatrix(const QMatrix4x4& maix);
	void SetPerspectiveMatrix(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	QMatrix4x4 GetProjectionMatix();
	void Frustum(GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble zN, GLdouble zF);
	void Ortho(GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble zN, GLdouble zF);
	void Move(TRANSFORM_MODE mode, const QVector3D& pos);
	void Move(const QMatrix4x4& maix);
	void Rotate(TRANSFORM_MODE mode, GLfloat degree, const QVector3D& aix);
	void Rotate(const QMatrix4x4& maix);
	void Scale(const GLfloat s);
	void Scale(const QMatrix4x4& maix);
	void ApplyCamera();
	qreal GetCameraScale();
	void SetCameraScale(qreal sc);
        inline const QVector3D& Position(){return position;}
        inline const QVector3D& Center(){return center;}
        inline const QVector3D& Up(){return up;}
        inline GLdouble Left(){return left;}

        inline GLdouble Right(){return right;}

        inline GLdouble Bottom(){return bottom;}

        inline GLdouble Top(){return top;}

        inline GLdouble Near(){return zNear;}

        inline GLdouble Far(){return zFar;}

        inline const QMatrix4x4& ViewMatrix()
        {return viewPointMatrix;}

        inline const QMatrix4x4& ProjecMatrix(){return lensMatrix;}

        inline CAMERA_TYPE CameraType()
        {
                return cameraType;
        }

        inline void CameraType( CAMERA_TYPE t )
        {
                cameraType = t;
        }
	const QCamera& operator=(const QCamera& it);
private:
	//
	QVector3D position;
	QVector3D center;
	QVector3D up;

	//
	GLdouble left;
	GLdouble right;
	GLdouble bottom;
	GLdouble top;
	GLdouble zNear;
	GLdouble zFar;
	
	//
	QMatrix4x4 viewPointMatrix;
	QMatrix4x4 lensMatrix;

	CAMERA_TYPE cameraType;
	qreal m_cameraScal;
};

