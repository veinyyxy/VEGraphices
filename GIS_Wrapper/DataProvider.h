#pragma once
#include "GIS_WrapperConfig.h"
#include "QT_Include.h"
#include "QNode.h"
#include "Line.h"
#include "Polygon.h"
#include "Point.h"
#include "Proj4Transform.h"
#include "ILayerAttribute.h"

#include <vector>
using namespace std;

typedef struct{
	float x;
	float y; 
	float z;
}Point3d;

class Layer;

typedef enum _VECTOR_DATA_TYPE
{
	VECTOR_POINT = 1,
	VECTOR_LINE,
	VECTOR_POLYGON
} VECTOR_DATA_TYPE, *P_VECTOR_DATA_TYPE;

typedef enum _FILE_TYPE
{
	BASE_MAP = 1,
	PRODUCT_DATA,
	STRUCT_DATA,
	NOTYPE
} FILE_TYPE, *P_FILE_TYPE;

class GIS_WRAPPER_EXPORT_IMPORT DataProvider : public QObject
{
public:
	DataProvider(void);
	virtual ~DataProvider(void);
	virtual int InitDataProvider() = 0;
	virtual int OpenFile(const QString& filename) = 0;
	virtual int LoadPlug(const QString filename) = 0;

	virtual bool DumpPoint(Point& point) = 0;
	virtual bool DumpPoint(GISMultiPoint* pointSet) = 0;

	virtual bool DumpLine(Line& linePoint) = 0;
	virtual bool DumpLine(GISMultiLine* lineSet) = 0;

	virtual bool DumpPolygon(GISPolygon& polygPoint) = 0;
	virtual bool DumpPolygon(GISMultiPolygon* polygonSet) = 0;

	virtual bool DumpAll(QRenderable* container) = 0;

	virtual QNode* GetFileContent() = 0;
	virtual QString GetProInfo() = 0;
	virtual VECTOR_DATA_TYPE GetDataType() = 0;
	virtual QString  GetFileName();//��ȡ�ļ���
	virtual void  SetFileName(QString vfilename);//�����ļ���
	virtual void addProj(CoordinateTransform *coorTrans) = 0;
	virtual vector<Point3d> GetPoint3d(void) = 0;
	inline Layer* ParentLayer();
	inline void ParentLayer(Layer* pl);
	virtual int UpdateAttribute(ILayerAttribute* iLA);

	virtual void SetBShowData(int nshow);
	virtual void SetBShowData(QString strshow);
	virtual QStringList GetFileDeclaration();
	virtual void SetFileDeclaration(QStringList &ProInfoList);
	virtual QHash<QString,QVector4D>  *GetColorHash();
	virtual int GraphicType();
	virtual void GraphicType(int iType);
	virtual FILE_TYPE GetFileType();
	virtual float GetProductRadius(){return m_radius;};
	
protected:
	Layer* parentLayer;
	int m_graphicType;				// ��ͼ����
	int m_nBshowdata;				//��ʾ����
	QString m_StrShow;				//��ʾ����	
	QString     m_fileName;
	QStringList m_fileDeclaration;  //�ļ�������Ϣ
	FILE_TYPE m_fileType;
	QHash<QString,QVector4D>     m_pColorValueHash;
	float m_radius;
	int m_requestLayer;
};

