#pragma once
#include "GIS_WrapperConfig.h"
#include "layer.h"
#include "Polygon.h"
#include <QtGui/QMouseEvent>
//#include <QtOpenGL/QGLWidget>
#include "QDataArray.h"

class GIS_WRAPPER_EXPORT_IMPORT VectorLayer :
	public Layer
{
	Q_OBJECT

public:
	VectorLayer(DataProvider* data = 0, CoordinateTransform* ct = 0);
	~VectorLayer(void);
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
	void operator=(VectorLayer &l);

	QRenderable* PickUp(const QVector3D& point);
	void SaveToShape();
	void StartProfile(QVector3D & point);				//开始绘制部面线
	void ProcessProfile(QVector3D & point);				//绘制剖面线
	void EndProfile(QVector3D & point);					//结束剖面线
	QVector3D TransformData(QVector3D & glPostion);	   //将opengl坐标转换为经纬坐标

	void sTOog(const QVector3D& src, QVector3D& des);
	void ogTOs(const QVector3D& src, QVector3D& des);
	void isTransForm(bool b){isTrans = b;};
	void DumpGraphicsAttribute(GEOMETRY_SET& resultSet);

signals:
	 void VectorMouse_Down(QVector3D & point);//鼠标在图层上释放事件
	 void VectorMouse_Up(QVector3D & point);//鼠标在图层上释放事件
	 void IdentiferEnd(StationValues* vLayer);//标识拾取完成 返回当前图层
public slots:
	void FindedData(QDataArray* data);
	void FindedUnit(GLenum type, QRenderable* data);
	void PickUnit(GLenum type, QRenderable* data);
	void SaveUnit(GLenum type, QRenderable* data);
	void DumpUnitAttribute(GLenum type, QRenderable* data);

private:

	QStringList openFileNameList;
	GEOMETRY_SET m_tempGeometrySet;
	QRenderable* m_pickUpObject;
	QVector3D pickPoint;

	Line  * m_pProfileLine;				//用于绘制剖面线
	bool  m_bStart;						//标识为开始做剖面线
	QVector3D m_IniPoint;			    //剖面线初始点
	Proj4Transform *m_Proj4Trans;		//用于将投影坐标转换成经纬坐标
	bool  isTrans;

	QRenderable* m_pPickObj, *m_pEditObj;				//得到当前拾取对象，编辑对象
};
