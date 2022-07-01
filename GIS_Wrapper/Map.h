/*=================================================================================== 
项目名： 人机交互平台
作 者： 杨晓宇
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年4月18日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：Map.h Map.cpp 
类 名： Map
父 类： QObject
子 类： 无 
功能说明：传递符号线算法数据数据，收集符号线算法输出数据
调用说明：
------------------------------------------------------------------------------------*/
#pragma once
#include <QtCore/QMap>
#include <QtGui/QUndoStack>
#include <QtGui/QUndoCommand>
#include "GIS_WrapperConfig.h"
#include "QT_Include.h"
#include "VectorLayer.h"
#include "QFork.h"

class GIS_WRAPPER_EXPORT_IMPORT Map : public QObject
{
public:
	Map(void);
    Map(const Map& otherMap);
	virtual ~Map(void);
	Layer* CreateVectorLayer(const QString& name);
	Layer* CreateVectorLayer(const QStringList& namelist);

	Layer* GetLayer(const QString& name);
	int AddLayer(const QString& name, Layer* mapLayer);
	int RemoveLayer(const QString& name, bool bIsDel = true);
	inline double GetMapScale();
	inline void SetMapScale(double scale);
	bool LinkMap(const Map* pMap);
	bool SeparateMap(const Map* pMap);
	void TramsformMap();
	inline QNode* DrawMap();
	inline void SetDataProvider(DataProvider* dp);
	inline DataProvider* GetDataProvider();
	inline void SetCoordinateTransform(CoordinateTransform* ct);
	inline CoordinateTransform* GetCoordinateTransform();
	void MoveMap(const QVector3D& pos);
	void RotateMap(qreal a, const QVector3D& aix);
	void ScaleMap(qreal s);
	bool HideLayer(const QString& name, bool bl);
	bool isCurrentLayerHide();
	inline Layer* CurrentLayer();
	inline void CurrentLayer(Layer* l);
	void Draw();
	void UndoSatck(QUndoStack* udos);
	QUndoStack* UndoSatck();
	QStringList GetLayerNameList();
	QMap<QString, Layer*>* GetLayerMap(){return &layerMap;};
	void CopyTo(Map* pOther);
	Map& operator+=(const Map& other);
	Map& operator-=(const Map& other);
	Map& operator=(const Map& other);

protected:
	Layer* currentLayer;
	DataProvider* dataProvider;
	CoordinateTransform* coordTrans;
	QNode* mapRoot;
	double mapScale;
	QVector3D mapTransf;
	QVector3D rouAix;
	qreal rouAngle;
	QMap<QString, Layer*> layerMap;
	QUndoStack* m_pMapUndoStack;
};

