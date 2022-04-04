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
�ļ�����ModifySymbol.h ModifySymbol.cpp 
�� ���� CModifySymbol
�� �ࣺ NULL
�� �ࣺ �� 
����˵���� ʰȡ�޸���
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
//#include "UndoSymbolMarkMoveCommand.h"
#include "CardinalSpline.h"
#include "SymbolAttribute.h"
//#include "UndoSymbolLineMoveCommand.h"
//#include "UndoDeleteDrawCommand.h"
//#include "UndoDelPointCommand.h"


#define BUFSIZE 1024						//�趨ѡ��ģʽ�»�������С
class GIS_WRAPPER_EXPORT_IMPORT CModifySymbol :
	public ManualDraw
{
	Q_OBJECT
public:
	CModifySymbol(void);
	virtual ~CModifySymbol(void);
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
	void MoveSymbol(QRenderable *pPickObj,QVector3D &scPoint);        //�ƶ�����	
	void EditSymbol(QRenderable *pPickObj,QVector3DArray *pPoint, int nPosStart, int nPosEnd); //�༭����

signals:
	void UpdateAttribute(ManualDraw *);                 //�����ͼ�����ͷ��¼�

private:
	Line *m_pShowLine;
	QVector3DArray *pArray;                             //Line�Ķ���ָ��
	QVector4DArray *pColor;                             //Line����ɫָ��
	QRenderable* m_pPickObj, *m_pEditObj;				//�õ���ǰʰȡ���󣬱༭����
	bool m_bCanMove;									//�����Ƿ��ƶ���
	int  m_iFlag;										//���������ʽ 1 Ϊ�����϶�
	bool m_bEdit;										//���Ա༭����
	bool m_bEditStart;                                  //�༭״̬�ĵ�һ�����־
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

};

