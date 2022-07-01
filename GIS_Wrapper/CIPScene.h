#pragma once
#include "GIS_WrapperConfig.h"

//#include "tabpage.h"
#include <QtCore/QSettings>
#include <QtWidgets/QStatusBar>
#include <QtGui/QUndoCommand>
#include <QtGui/QUndoStack>
#include <QtGui/QPainter>
#include "QRenderUnit.h"
#include "QRenderVolume.h"
#include "QCamera.h"
#include "QViewport.h"
#include "QFork.h"
#include "QLeaf.h"
#include "QScene.h"
//#include "Proj4Transform.h"
//#include "MeteToolButton.h"
#include "ManualDraw.h"
#include "Layer.h"
#include "Legend.h"
#include "Map.h"
//#include "MeteToolBox.h"
#include <QtCore/QThread>
#include <QtCore/QThreadPool>
#include <QtWidgets/QToolButton>
//#include "QTextureFont.h"
#include "GeoCoordTransform.h"
//#include "UndoStack.h"
//#include "QMouseManipulator.h"
#include "QDefaultMouseManipulator.h"
#include "QMouseManipulatorExtand.h"
//#include "MouseUndoExtand.h"
class MouseUndoExtand;
//class QMouseManipulatorExtand;

class GIS_WRAPPER_EXPORT_IMPORT CIPScene : public QScene
{
	//Q_OBJECT
public:
	CIPScene();
    CIPScene(QOpenGLContext* glContext);
	~CIPScene();
	void InitScene();
	void ReadShpFile(const char* filename);
	void SetTranslatAndScale(double fMaxX, double fMinX, double fMaxY, double fMinY);
	//void ScreenToGL(const QVector3D& screenPoint, QVector3D& glPoint);
	void ScaleScene(qreal sc);
	void setOpengl(Proj4Transform *p);
	void SetManualDraw(ManualDraw* md);
	
	ManualDraw* GetManualDraw();
	int test();
	bool SavePict(QString fileName, int quality=-1);//if no format,it will guess  fileName' suffix

	void TranslateLens(const GLfloat tr);
	//void ResizeViewport(QSize s);
	//设置地图背景色 吴恩平
	bool SetViewBackgroundColor(QColor color);	

	QVector3D& ScreenBeginPoint(){return m_IniScreenPoint;};
	QVector3D& ScreenEndPoint(){return m_EndScreeniPoint;};
	
	void ClearPoint(){m_IniScreenPoint = QVector3D(0,0,0);m_EndScreeniPoint = QVector3D(0,0,0);};
	//inline void OldMouseMainpulator(QMouseManipulator* pMmani){
	//	if(pMmani)
	//		m_pOldMouseManipulator = pMmani;
	//	m_pOldMouseManipulator = m_pMouseManipulator;
	//}
	//inline QMouseManipulator* OldMouseMainpulator(){return m_pOldMouseManipulator;}
	inline void ResetManipulator() { MouseManipulator((QMouseManipulator*)m_pDefalutMainpultorExtand/*&m_defalutMainpulator*/);}
	inline QVector3D GetCurScreenPos();
	/*inline QColor BackgroundColor(){return m_BackgroundColor;}
	inline void   backgroundColor(QColor color){m_BackgroundColor = color;}*/
	inline QMouseManipulatorExtand* GetMouseManipulatorExpand(){return (QMouseManipulatorExtand*)m_pDefalutMainpultorExtand;}
	//inline QMouseManipulator* GetMouseManipulator(){return &m_defalutMainpulator;}
	inline void UpdateViewport(int iW, int iH){GetDrawSurface()->Resize(iW, iH);}	
	/*inline void SetTrans(bool b){isTrans = b;};*/
public:
	inline void MoveX(int ScreenLen);
	inline void MoveY(int ScreenLen);
	inline void Scale(float scale);
	inline qreal GetCameraScale();
	inline void SetCameraScale(qreal scale);
	inline void FullMap();
	inline void usingDedaultTransform(bool b){m_bUsingDedaultTransform=b;}
//	void CurrentLyaer(Layer* cl) { currentLayer = cl;}
public:

	//QToolButton* m_r;
	//QToolButton* m_u;
	//QToolButton* m_rview;
	//QToolButton* m_uview;
	//MeteToolButton* p;
	QVector3D glPoint;
	Legend *legend;

	QFork* m_pFork;
	QLeaf* m_pLeaf;
	Line* m_pLine;
	bool m_bExportpic;
	QVector3D  m_IniPoint;													//选框左上角点
	QVector3D  m_EndniPoint;												//选框右下角点

	QVector3D  m_IniScreenPoint;													//选框左上角点
	QVector3D  m_EndScreeniPoint;												//选框右下角点

	void Exportpic(bool exportpic){m_bExportpic = exportpic;}; //设置是否导出图片
protected:
	bool m_bInitGL;
	bool m_bRenderThread;
	//QTextureFont textureText;
	//QDefaultMouseManipulator m_defalutMainpulator;
	MouseUndoExtand* m_pDefalutMainpultorExtand;

private:
	//QColor    m_BackgroundColor;
	QObserver m_observer;
	ManualDraw* manualDraw;
	Layer* currentLayer;
	bool m_bUsingDedaultTransform;
	QVector3D m_WGS84Pos; //当前视图中心点的地理坐标
	bool isTrans;
};
