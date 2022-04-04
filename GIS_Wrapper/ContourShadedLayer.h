#pragma once
#include "GIS_WrapperConfig.h"
#include "layer.h"
#include "Polygon.h"
class GIS_WRAPPER_EXPORT_IMPORT ContourShadedLayer :
	public Layer
{
public:
	ContourShadedLayer(DataProvider* data = 0, CoordinateTransform* ct = 0);
	~ContourShadedLayer(void);
	virtual int InitLayer();
	virtual int CreateLayer(const QString& filename);
	virtual int CreateLayer(QNode* node);
	virtual int CreateLayer(const QStringList& namelist);
	virtual int ReloadLayer();
	void TransformLayer();
	QString GetProInfo(void);
	void MountLayer(QNode* root);
	void Clear();
	QRenderable* GetLayerData();
	void operator=(ContourShadedLayer &l);
public slots:
		void FindedData(QDataArray* data);
		void FindedUnit(GLenum type, QRenderable* data);
private:
	QRenderVolume* renderVol;
	QStringList openFileNameList;
};

