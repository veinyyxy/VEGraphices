/*============================================================ 
�ļ�����SaveLayerToShape.h  
��  ���� SaveLayerToShape
��  �ࣺ QWidget
��  �ࣺ �� 
����˵������ʸ��Ϳ�����ݱ����Shape��ʽ
����˵����

----------------------------�汾����----------------------------
V 1.0 
ԭ���� ��chenzhixue
������ڣ�2011��11��20��
============================================================*/


#pragma once

#include "GIS_WrapperConfig.h"
#include <Qtcore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QFileDialog>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>
#include "SymbolData.h"
#include "Layer.h"
#include "VectorLayer.h"
#include "CIPASMessageBox.h"


extern "C"
{
#include "shapefil.h"
#include "gpc.h"
};
class GIS_WRAPPER_EXPORT_IMPORT CSaveLayerToShape :
	public QWidget
{
public:
	CSaveLayerToShape(void);
	~CSaveLayerToShape(void);
public:
	/*************************************************************
	����˵��������Ҫ�ز�����ݱ����shape��ʽ���ļ�
	����˵����fileName�ǰ���·�����ļ������ַ���
	          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
	**************************************************************/
	void SaveMultiFeatureToShape(QString outfileName, VectorLayer *pLay,int type);
		/*************************************************************
	����˵��������Ҫ�ز�����ݱ����shape��ʽ���ļ�
	����˵����fileName�ǰ���·�����ļ������ַ���
	          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
	**************************************************************/
	void SaveSimpleFeatureToShape(QString outfileName, VectorLayer *pLay,int type);

	static gpc_polygon *convertToGPC_Poly(GISPolygon *poly);
	static gpc_polygon *combinePolygon(QList<GISPolygon *> p);
	static QHash<QString,gpc_polygon*> *differencePolygon(QHash<QString,gpc_polygon*> *hp);
private:
	bool SavePoint(const char* fileName, VectorLayer *lay);
	bool SaveArc(const char* fileName, VectorLayer *lay);
	bool SavePolygon(const char* fileName, VectorLayer *lay);

};

