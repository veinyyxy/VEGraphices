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
�ļ�����ResizeSymbol.h ResizeSymbol.cpp 
�� ���� CResizeSymbol
�� �ࣺ NULL
�� �ࣺ �� 
����˵���� ʰȡ�޸���
����˵���� 

-------------------------------------�汾����------------------------------------------------ 
V 1.0 
ԭ���� �� 
������ڣ�2011�ꣿ�£���
============================================================*/

#pragma once
#include "manualdraw.h"
//#include "UndoSymbolPointMoveCommand.h"
//#include "UndoSymbolMarkMoveCommand.h"
#include "CardinalSpline.h"
#include "SymbolAttribute.h"
//#include "UndoSymbolLineMoveCommand.h"
//#include "UndoDeleteDrawCommand.h"
//#include "UndoDelPointCommand.h"


#define BUFSIZE 1024						//�趨ѡ��ģʽ�»�������С
class GIS_WRAPPER_EXPORT_IMPORT CScaleSymbolLine :
	public ManualDraw
{
	Q_OBJECT
public:
	CScaleSymbolLine(void);
	virtual ~CScaleSymbolLine(void);
	void MouseEvent(QMouseEvent* event);				//����ƶ��¼�
	void MousePress(QMouseEvent *event);				//��갴���¼�
	void MouseRelease(QMouseEvent *event);				//����ͷ��¼�
	void MouseDoubleClick(QMouseEvent *event);			//���˫���¼�
	void LeaveEvent();
    void keyPressEvent(QKeyEvent * event );				//���¼����¼�
	void SetSymbol(Symbol *pSymbol);
	void setILayerAttrib(ILayerAttribute *Ilayer);
	void SetContourAttrib(CSymbolAttribute *pAttribute);//��������ֵ
	//void GetContourAttribute();                         //��õ�ֵ�ߵ�����ֵ
	void SelectSymbol(QRenderable *pPickObj);                         //ѡ�з���
	void ReleaseSymbol(QRenderable *pPickObj);                        //�ͷ�ѡ�еķ���
	void DeleteSymbol(QRenderable *pPickObj);                         //ɾ������ 	
private:
	inline void DrawRect(Line* rect,QVector3D& glpos,int srcD,QVector4D color);
	inline void DrawSymbolLineBound(QRenderable* m_pPickObj,QVector4D color);
	
private:
	bool SelectSymbolBound(QRenderable *pPickObj);
	void ResizeSymbol(QRenderable *pPickObj,float scale);
	inline QVector3D GetScalePoint(QVector3D glCenter,QVector3D glPosOrigin,float scale);
signals:
	void UpdateAttribute(ManualDraw *);                 //�����ͼ�����ͷ��¼�

private:
	Line *m_pShowLine;
	QVector3DArray *pArray;                             //Line�Ķ���ָ��
	QVector4DArray *pColor;                             //Line����ɫָ��
	QRenderable* m_pPickObj, *m_pEditObj;				//�õ���ǰʰȡ���󣬱༭����
	QRenderable* m_pPickObjShade;
	bool m_bCanMove;									//�����Ƿ��ƶ���
	int  m_iFlag;										//���������ʽ 1 Ϊ�����϶�
	bool m_bEdit;										//���Ա༭����
	
	QVector3DArray m_PressPoints, m_SmoothPoints;       //�ռ��ı༭��
	CCardinalSpline m_SpLine;                           //ƽ���㷨
	int nPosStart, nPosEnd;
	QVector3DArray *pTempArray, *pTempOriginalArray;
	CSymbolAttribute *m_pAttribute;		                //��������ֵ
	QVector4D  SourceColor;

	//CUndoSymbolPointMoveCommand* m_pUndoSymbolPointEdit;
	//CUndoSymbolLineMoveCommand* m_pUndoLineMoveEdit;
	//CUndoSymbolMarkMoveCommand* m_pundoSymbolMarkEdit;
	float m_fXMoveDistance, m_fYMoveDistance;

private:
	QVector3D m_ScOldPoint,m_ScCurrentPoint;
	bool isLmousePressing;
	Line *m_pRectTemp;	
	Line *m_rect1;
	Line *m_rect2;
	Line *m_rect3;
	Line *m_rect4;
	Line *m_rect5;
	Line *m_rect6;
	Line *m_rect7;
	Line *m_rect8;

	
};

