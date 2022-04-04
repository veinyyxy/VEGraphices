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
����˵���� λͼ���ֵ�ʵ�֣�������OpenGL��դ��ʱ��������ӵ���Ⱦ���棬
		   Ч�ʽϵͣ�����������Ⱦ�ж�����Ⱦ�������Ч��
����˵���� 
================================================================*/ 
#pragma once
#include "QRenderUnit.h"
#include "StationValues.h"

class QE_LIB_EXPORT_IMPORT QBitmapText :
	public QRenderUnit
{
public:
	QBitmapText(void);
	virtual ~QBitmapText(void);
	void Render();
	inline int InsertRenderable(const QRenderable* unit);
	inline int RemoveRenderable(QRenderable* unit);
	inline QVector<QString>* StringArray();
	inline QVector3DArray* Pointion();
	inline void* FontSize();
	inline void FontSize(void* fs);
	inline void SetColor(const QAttribute* color);
	inline void SetXOffset(float);
	inline void SetYOffset(float);
	inline QVector<StationValues>* StationValueArray();
	bool PickUp(const QVector3D* pickPoint);//chenzhixue--20110622
	int GetPickIndex();                    //chenzhixue--20110622
	void SetPickIndex();                  //chenzhixue--20110622
	void CallDisplayList(int i);
protected:
	void TextRender();
	void TextRenderUseCallList();
	QVector<QString> stringArray;
	QVector3DArray postion;
	void* size;
	//QAttributeSet attribSet;
	float m_fXAlpha;    // OFFSET OF X
	float m_fYAlpha;    // OFFSET OF Y
	QVector<StationValues> m_SValuesArray;//վ����������
	int m_iPickIndex;
	GLsizei m_iListRang;
	QVector<int> m_ListVector;
	GLuint m_uiList;
	GLuint m_uiBitmapTextList;
};

