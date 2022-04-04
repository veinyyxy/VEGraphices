/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� lugsh
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����DelSymbol.h DelSymbol.cpp 
�� ���� CDelSymbol
�� �ࣺ NULL
�� �ࣺ �� 
����˵���� ʰȡ��ɾ���༭����
����˵���� 

-------------------------------------�汾����------------------------------------------------ 
V 1.2 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.1 
�� �ߣ�renxue
������ڣ�2011��04��28��
����˵���� 
V 1.0 
ԭ���� ��lugsh
������ڣ�2011�ꣿ�£���
============================================================*/

#pragma once
#include "manualdraw.h"
//#include "UndoSymbolPointMoveCommand.h"
//#include "UndoSymbolLineMoveCommand.h"
//#include "UndoDeleteDrawCommand.h"
//#include "UndoDelPointCommand.h"


#define BUFSIZE 1024						//�趨ѡ��ģʽ�»�������С
class GIS_WRAPPER_EXPORT_IMPORT CDelSymbol :public ManualDraw
{
	Q_OBJECT
public:
	CDelSymbol(void);
	virtual ~CDelSymbol(void);
	void MouseEvent(QMouseEvent* event);				//����ƶ��¼�
	void MousePress(QMouseEvent *event);				//��갴���¼�
	void MouseRelease(QMouseEvent *event);				//����ͷ��¼�
	void MouseDoubleClick(QMouseEvent *event);			//���˫���¼�
	void LeaveEvent();
    void keyPressEvent(QKeyEvent * event );				//���¼����¼�
	void SetSymbol(Symbol *pSymbol);
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void SelectSymbol(QRenderable *pPickObj);           //ѡ�з���
	void ReleaseSymbol(QRenderable *pPickObj);          //�ͷ�ѡ�еķ���
	void DeleteSymbol(QRenderable *pPickObj);           //ɾ������ 
	void DelSymbolImmediately(QVector3D delPoint, Layer* parentLayer);
signals:
	void UpdateAttribute(ManualDraw *);                 //�����ͼ�����ͷ��¼�

private:
	QRenderable* m_pPickObj;				            //�õ���ǰʰȡ����
	//CUndoSymbolPointMoveCommand* m_pUndoSymbolPointEdit;
	//CUndoSymbolLineMoveCommand* m_pUndoLineMoveEdit;
	float m_fXMoveDistance, m_fYMoveDistance;
	QVector4D  SourceColor;
};

