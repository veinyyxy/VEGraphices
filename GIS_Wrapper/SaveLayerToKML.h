/*============================================================ 
文件名：SaveLayerToKML.h  
类  名： SaveLayerToKML
父  类： QWidget
子  类： 无 
功能说明：将矢量涂层数据保存成KML格式
调用说明：

----------------------------版本更新----------------------------
V 1.0 
原作者 ：chenzhixue
完成日期：2011年12月16日
============================================================*/


#pragma once

#include "GIS_WrapperConfig.h"
#include <QtCore/QHash>
#include <Qtcore/QFile>
#include <QtCore/QTextStream>
#include <QtWidgets//QFileDialog>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QWidget>
#include "SymbolData.h"
#include "Layer.h"
#include "VectorLayer.h"
#include "CIPASMessageBox.h"
//#include "gdal.h"

#include "ogr_api.h"
#include "SaveLayerToShape.h"
//extern "C"
//{
//
//#include "gpc.h"
//};
class GIS_WRAPPER_EXPORT_IMPORT SaveLayerToKML :
	public QWidget
{
public:
	SaveLayerToKML(void);
	~SaveLayerToKML(void);
public:
	/*************************************************************
	功能说明：将多要素层的数据保存成KML格式的文件
	参数说明：fileName是包含路径和文件名的字符串
	          返回值为bool型，正确执行返回true，错误或异常返回false
	**************************************************************/
	void SaveMultiFeatureToKML(QString outfileName, VectorLayer *pLay,int type);
		/*************************************************************
	功能说明：将单要素层的数据保存成KML格式的文件
	参数说明：fileName是包含路径和文件名的字符串
	          返回值为bool型，正确执行返回true，错误或异常返回false
	**************************************************************/
	void SaveSimpleFeatureToKML(QString outfileName, VectorLayer *pLay,int type);
private:
	bool SavePoint(const char* fileName, VectorLayer *lay);
	bool SaveArc(const char* fileName, VectorLayer *lay);
	bool SavePolygon(const char* fileName, VectorLayer *lay);
private:
	OGRGeometryH mGeom;
	OGRDataSourceH mDS;
};

