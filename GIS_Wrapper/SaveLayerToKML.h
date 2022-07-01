/*============================================================ 
�ļ�����SaveLayerToKML.h  
��  ���� SaveLayerToKML
��  �ࣺ QWidget
��  �ࣺ �� 
����˵������ʸ��Ϳ�����ݱ����KML��ʽ
����˵����

----------------------------�汾����----------------------------
V 1.0 
ԭ���� ��chenzhixue
������ڣ�2011��12��16��
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
	����˵��������Ҫ�ز�����ݱ����KML��ʽ���ļ�
	����˵����fileName�ǰ���·�����ļ������ַ���
	          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
	**************************************************************/
	void SaveMultiFeatureToKML(QString outfileName, VectorLayer *pLay,int type);
		/*************************************************************
	����˵��������Ҫ�ز�����ݱ����KML��ʽ���ļ�
	����˵����fileName�ǰ���·�����ļ������ַ���
	          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
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

