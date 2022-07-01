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
	void StartProfile(QVector3D & point);				//��ʼ���Ʋ�����
	void ProcessProfile(QVector3D & point);				//����������
	void EndProfile(QVector3D & point);					//����������
	QVector3D TransformData(QVector3D & glPostion);	   //��opengl����ת��Ϊ��γ����

	void sTOog(const QVector3D& src, QVector3D& des);
	void ogTOs(const QVector3D& src, QVector3D& des);
	void isTransForm(bool b){isTrans = b;};
	void DumpGraphicsAttribute(GEOMETRY_SET& resultSet);

signals:
	 void VectorMouse_Down(QVector3D & point);//�����ͼ�����ͷ��¼�
	 void VectorMouse_Up(QVector3D & point);//�����ͼ�����ͷ��¼�
	 void IdentiferEnd(StationValues* vLayer);//��ʶʰȡ��� ���ص�ǰͼ��
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

	Line  * m_pProfileLine;				//���ڻ���������
	bool  m_bStart;						//��ʶΪ��ʼ��������
	QVector3D m_IniPoint;			    //�����߳�ʼ��
	Proj4Transform *m_Proj4Trans;		//���ڽ�ͶӰ����ת���ɾ�γ����
	bool  isTrans;

	QRenderable* m_pPickObj, *m_pEditObj;				//�õ���ǰʰȡ���󣬱༭����
};
