#pragma once
#include "GIS_WrapperConfig.h"
#define CORE_EXPORT _declspec(dllimport)
#define GUI_EXPORT _declspec(dllimport)
#include <QObject>
#include <QString>
#include <QLineF>

#include "QRenderUnit.h"
#include "QRenderVolume.h"
#include "QCamera.h"
#include "QViewport.h"
#include "QNode.h"
#include "Polygon.h"
#include "QLeaf.h"
#include "Layer.h"
#include "Proj4Transform.h"
#include "Line.h"
#include "QBitmapText.h"
//#include "QTextureFont.h"

typedef QPair< qreal, qreal > LONG_LAT_POINT;
typedef QPair< LONG_LAT_POINT, LONG_LAT_POINT > LONG_LAT_LINE;
typedef QMap< LONG_LAT_LINE, Line* > LONG_LAT_LINE_MAP;
//typedef QMap< LONG_LAT_POINT, QTextureFont* > LONG_LAT_MARK_MAP;
typedef QVector<Line*> LONG_LAT_LINE_VECTOR;
//typedef QMap< LONG_LAT_POINT, QTextureFont* > ANNOTATION;
typedef struct longlatrect
{
	LONG_LAT_POINT left_top, right_top;
	LONG_LAT_POINT left_bottom, right_bottom;
} LONG_LAT_RECT;

typedef struct LongLatLineData
{
	 LONG_LAT_LINE_MAP longitude_line;
	 qreal longitude_line_interval;
	 LONG_LAT_LINE_MAP latitude_line;
	 qreal latitude_line_interval;
	 //ANNOTATION line_make;
	 int mark_font;
	 void clear(){
		 longitude_line.clear(); 
		 //line_make.clear();
		 latitude_line.clear();
	 }
} LONGLAT_LINE_DATA;

class GIS_WRAPPER_EXPORT_IMPORT QGraticuleCoordinates : public Layer
{
	Q_OBJECT

public:
	QGraticuleCoordinates(DataProvider* data = 0, CoordinateTransform* ct = 0);
	~QGraticuleCoordinates(void);
	//void ReadGraticule(QString projInfo);
	//QLeaf* get(){return node;}
	inline QRenderable* GetMap();
	int CreateLayer(const QString& filename);
	int CreateLayer(QNode* node);   // 创建层并绘制经纬网
	int CreateLayer(const QStringList& namelist);
	int CreateLayer(double X,double Y,double h,double R,double Res);
	int ReloadLayer();
	QString GetProInfo(void);
	void Clear(void);
	void TransformLayer();
	QRenderable* GetLayerData();
	void OldMakeLongLatLine();
	void MakeLongLatLine();
	void MakeLongLatLineData();
	void MakeLongLatLineStartAndEnd();
	void MakeAnnotation(const QString& fontName, int iWidth, int iHeight);
	void MakeLongLineData(const GLdouble* pLineData, int iLongSize);
	void MakeLatLineData(const GLdouble* pLineData,  int iLatSize);
	inline void SetGridInterval(qreal longItvl, qreal latItvl){
		m_LongLatLineData.longitude_line_interval = longItvl;
		m_LongLatLineData.latitude_line_interval = latItvl;
	}
	void SetGridLineColor(const LONG_LAT_LINE& line, QColor color);
	void SetGridColor(QColor color);
	inline LONG_LAT_LINE_MAP* GetLongLineData(){return &(m_LongLatLineData.longitude_line);}
	inline LONG_LAT_LINE_MAP* GetLatLineData(){return &(m_LongLatLineData.latitude_line);}
	inline qreal GridXInterval(){return m_LongLatLineData.longitude_line_interval;}
	inline qreal GridYInterval(){return m_LongLatLineData.latitude_line_interval;}
	void clearLine(const LONG_LAT_LINE& line);
	void clearLine();
	void clearAnnotation();
	void DrawGrid();								 //绘制网格线
	QVector3DArray * DrawCircle(double X,double Y,double h,double R);
public slots:
	void FindedUnit(GLenum type, QRenderable* data);  //用于投影变换

private:
	LONGLAT_LINE_DATA m_LongLatLineData;
	QRenderVolume* renderVol;
	QPointer<QObject> root;
	bool m_constantInterval;
	LONG_LAT_LINE_VECTOR m_LongLine;
	LONG_LAT_LINE_VECTOR m_LatLine;
	//LONG_LAT_MARK_MAP m_LongLatMark;
	LONG_LAT_RECT m_LongLatRect;
	QVector4D m_GridColor;
	QAttributeSet m_gridAttribute;
	ColorAttribute* m_colorAttribute;
	QVector<Line*> m_deleteLine;
	Line* pLongLineData;
	Line* pLatLineData;
	//QTextureFont* pAllAnntation;
	//QPointer<QLeaf> node;
	//Proj4Transform trans;
};