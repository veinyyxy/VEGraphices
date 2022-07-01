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
�ļ�����ManualDraw.h ManualDraw.cpp 
�� ���� ManualDraw
�� �ࣺ NULL
�� �ࣺ �� 
����˵�����ֶ���ͼ�ӿ���
����˵���� 

-------------------------------------�汾����------------------------------------------------ 
V 1.2 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵����  
V 1.0 
ԭ���� ��lugsh
������ڣ�2011�ꣿ�£���

V 1.1 
�� �ߣ�������
������ڣ�2011��4��25��
����˵�������볷������
============================================================*/

#pragma once
#include <QtOpenGL/QOpenGLWindow>
#include <QtGui/QUndoStack>
#include "GIS_WrapperConfig.h"
#include "QT_Include.h"
#include "QRenderVolume.h"
#include "Layer.h"
#include  <QtGui/QMouseEvent>
#include "CoordinateTransform.h"
#include "ILayerAttribute.h"
#include "QMouseManipulator.h"
#include "Map.h"
#include "GeoCoordTransform.h"

class GIS_WRAPPER_EXPORT_IMPORT ManualDraw : public QMouseManipulator     //QObject
{
	Q_OBJECT
public:
	ManualDraw(void);
	virtual ~ManualDraw(void);
	virtual void MouseEvent(QMouseEvent* event) = 0 ;
	virtual void MousePress(QMouseEvent *event)= 0 ;
	virtual void MouseRelease(QMouseEvent *event)= 0;
	virtual void LeaveEvent()= 0;
	virtual void SetSymbol(Symbol *pSymbol) = 0 ;
	virtual void setILayerAttrib(ILayerAttribute *Ilayer) = 0;

	virtual void SetShadeSymbol(Symbol *pSymbol) {m_symbolShade = pSymbol;}
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
	virtual void leaveEvent(QEvent * even);
	virtual void mouseDoubleClickEvent (QMouseEvent * event);
	virtual void MouseDoubleClick(QMouseEvent *event) { return;}
	virtual void keyPressEvent(QKeyEvent * event )	{ return;}
	virtual void keyReleaseEvent(QKeyEvent *event){}
	virtual void InitAttribute(bool b);

	virtual void SetParentLayer(Layer * Layer){m_ParentLayer = Layer;}
    virtual void SetViewWidget(QOpenGLWindow * pView) { m_pOpenglView = pView;}
	virtual Layer* GetParentLayer() {return m_ParentLayer;}
	virtual QVector3DArray* getPoint(){return 0;}
	inline  void CoordTrans(RadProjTransform *pProj4);				//�ӵ�ǰͶӰ����ת����Ϊ��γ������
	QVector3D TransGLtoWGS84(QVector3D & glPostion);					//�Ӿ�γ��ת����ǰͶӰ����
	QVector3D TransWGS84toGL(QVector3D & oriPostion);
	LAYER_ATTRIBUTE *getILayerAttrib();
	//void SetUndoStack(QUndoStack* undoStack);
	//QUndoStack* GetUndoStack();
	void SetManageLayer(Map * pManageLay) {  m_pManageLay = pManageLay;};	
	virtual void DelSymbolImmediately(QVector3D delPoint, Layer* parentLayer){return;}
	/*inline Line* GetPathLine(){return m_pLine;}
	inline void SetFirstFlag(bool bf){m_bFirst = bf;}
	inline bool GetFirstFlag(){return m_bFirst;}*/
public:
     QString name;
protected:
	QRenderVolume * pVolume;
	Symbol * m_Symbol;
	Symbol* m_symbolShade;
	
	//Proj4Transform *m_Proj4Trans;		//���ڽ�ͶӰ����ת���ɾ�γ����
	//Proj4Transform* coordTrans;
	GeoCoordTransform geotrans;
	LAYER_ATTRIBUTE *m_layerAttribute;
	Layer * m_ParentLayer;
    QOpenGLWindow * m_pOpenglView;			//���ڼ�¼��
	//QUndoStack* m_undoStack;
	bool  m_bFinished;	    	//�ж��Ƿ��������
	Line *m_pLine;
	bool  m_bFirst;             //�ж��Ƿ�Ϊ���λ���
signals:
	void DelButtonStatus(bool enable);                 //�����ͼ�����ͷ��¼�
private:
	QMouseEvent* mouseEvent;
	Map *m_pManageLay;								   //ͼ��������
};

