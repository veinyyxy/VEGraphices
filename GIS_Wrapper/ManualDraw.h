/*============================================================ 
项目名： 人机交互平台
作 者： lugsh
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：ManualDraw.h ManualDraw.cpp 
类 名： ManualDraw
父 类： NULL
子 类： 无 
功能说明：手动绘图接口类
调用说明： 

-------------------------------------版本更新------------------------------------------------ 
V 1.2 
作 者：输入作者（或修改者）名字
完成日期：
更新说明：  
V 1.0 
原作者 ：lugsh
完成日期：2011年？月？日

V 1.1 
作 者：杨晓宇
完成日期：2011年4月25日
更新说明：加入撤销机制
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
	inline  void CoordTrans(RadProjTransform *pProj4);				//从当前投影坐标转换成为经纬度坐标
	QVector3D TransGLtoWGS84(QVector3D & glPostion);					//从经纬点转到当前投影坐标
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
	
	//Proj4Transform *m_Proj4Trans;		//用于将投影坐标转换成经纬坐标
	//Proj4Transform* coordTrans;
	GeoCoordTransform geotrans;
	LAYER_ATTRIBUTE *m_layerAttribute;
	Layer * m_ParentLayer;
    QOpenGLWindow * m_pOpenglView;			//用于记录当
	//QUndoStack* m_undoStack;
	bool  m_bFinished;	    	//判断是否结束画线
	Line *m_pLine;
	bool  m_bFirst;             //判断是否为初次绘制
signals:
	void DelButtonStatus(bool enable);                 //鼠标在图层上释放事件
private:
	QMouseEvent* mouseEvent;
	Map *m_pManageLay;								   //图层管理对象
};

