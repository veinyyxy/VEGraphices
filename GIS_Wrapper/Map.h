/*=================================================================================== 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ������
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011��4��18��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����Map.h Map.cpp 
�� ���� Map
�� �ࣺ QObject
�� �ࣺ �� 
����˵�������ݷ������㷨�������ݣ��ռ��������㷨�������
����˵����
------------------------------------------------------------------------------------*/
#pragma once
#include <QtGui/QUndoStack>
#include <QtGui/QUndoCommand>
#include "GIS_WrapperConfig.h"
#include "QT_Include.h"
#include "VectorLayer.h"

class GIS_WRAPPER_EXPORT_IMPORT Map : public QObject
{
public:
	Map(void);
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

