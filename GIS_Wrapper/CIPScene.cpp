#include"CIPScene.h"

CIPScene::CIPScene() : m_bInitGL(true)
	, m_bRenderThread(false)
	, m_bUsingDedaultTransform(true)
{
	m_observer.Camera()->CameraType(QCamera::ORTHO);
	m_observer.Camera()->Ortho(0,0,0,0,-300,300);
	InsertObserver("MainObserver", &m_observer);
	//m_defalutMainpulator.UpDataManipulator(0, &m_observer, &m_SceneTransform);
	//QScene::MouseManipulator(&m_defalutMainpulator);
	m_pFork = NULL;
	m_pLeaf = NULL;
	m_pLine = NULL;
	m_pDefalutMainpultorExtand = NULL;
	m_bExportpic = false; //正常模式，非导出图片
	//m_backgroundColor = QColor(100, 170, 255);	
}

CIPScene::CIPScene( QOpenGLContext* glContext ) :
	QScene(glContext)
	, m_bInitGL(true)
	, m_bRenderThread(false)
	, m_bUsingDedaultTransform(true)
{
	m_observer.Camera()->CameraType(QCamera::ORTHO);
	m_observer.Camera()->Ortho(0,0,0,0,-300,300);
	InsertObserver("MainObserver", &m_observer);
	//m_defalutMainpulator.UpDataManipulator(0, &m_observer, &m_SceneTransform);
	//QScene::MouseManipulator(&m_defalutMainpulator);
	m_pFork = NULL;
	m_pLeaf = NULL;
	m_pLine = NULL;
	m_pDefalutMainpultorExtand = NULL;
	m_bExportpic = false; //正常模式，非导出图片
	//m_backgroundColor = QColor(100, 170, 255);	
	isTrans = true;
}

CIPScene::~CIPScene()
{
	//setAutoBufferSwap(false);
	if (m_pLine != NULL)
	{
		delete m_pLine;
		m_pLine = NULL;
	}
	if (m_pLeaf != NULL)
	{
		delete m_pLeaf;
		m_pLeaf = NULL;
	}
	if (m_pDefalutMainpultorExtand)
	{
		delete m_pDefalutMainpultorExtand;
		m_pDefalutMainpultorExtand = NULL;
	}
}

bool CIPScene::SetViewBackgroundColor(QColor color)
{
	SetBackgroundColor(color);
	return true;
}

void CIPScene::ReadShpFile(const char* filename)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CIPScene::SetTranslatAndScale(double fMaxX, double fMinX, double fMaxY, double fMinY)
{
	double mapWidth = fMaxX - fMinX;
	double mapHeight = fMaxY - fMinY;
	double dWindowMinValue, dMapMinValue;
	qreal w = GetDrawSurface()->Width();
	qreal h = GetDrawSurface()->Height();

	w - h >0.0 ? dWindowMinValue = h : dWindowMinValue = w;
	mapWidth - mapHeight > 0.0 ? dMapMinValue = mapHeight : dMapMinValue = mapWidth;

	double scale = dWindowMinValue / dMapMinValue;

	/*mapScale.setX(scale);
	mapScale.setY(scale);
	mapScale.setZ(scale);*/

	qreal xm = mapWidth / 2;
	qreal ym = mapHeight / 2;
	qreal x, y;
	/*if(fMinX < 0)
		x = -(fMinX - xm);
	else
		x = -(fMinX + xm);

	if(fMinY < 0)
		y = -(fMinY - ym);
	else
		y = -(fMinY + ym);*/
	x = 0.0 - xm;
	y = 0.0 - ym;

	/*m_mapCurrentTranslate.setX(x*scale);
	m_mapCurrentTranslate.setY(y*scale);*/
}

void CIPScene::ScaleScene( qreal sc )
{
	//xyzScale *= sc;
}

void CIPScene::setOpengl( Proj4Transform *p )
{
	//MokatoTrans = p;
}

void CIPScene::SetManualDraw( ManualDraw* md )
{
	manualDraw = md;
}

ManualDraw* CIPScene::GetManualDraw()
{
	return manualDraw;
}


bool CIPScene::SavePict(QString fileName,int quality)
{
	QImage img= GetDrawSurface()->ToImage();
	if (fileName.isEmpty()) 
	{
		return false;
	}
	else
	{
		bool bl;
		bl = img.save(fileName,0,-1);
		return bl;
	}
}

int CIPScene::test()
{
	return 0;
}
#define GL_MULTISAMPLE  0x809D
void CIPScene::InitScene()
{
	glClearColor(m_backgroundColor.redF(), m_backgroundColor.greenF(), 
		m_backgroundColor.blueF(), m_backgroundColor.alphaF());
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glEnable(GL_LINE_STIPPLE);
	glAlphaFunc(GL_GREATER, 0.1f);                        // 设 Alpha 测试
	glEnable(GL_ALPHA_TEST);                            // 打开 Alpha 测试
	glPointSize(3);

	glEnable(GL_POINT_SMOOTH);   
	glEnable(GL_LINE_SMOOTH);   
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points   
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Antialias the lines   
	glEnable(GL_BLEND);   
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    
	glEnable(GL_MULTISAMPLE);
	/*if (isTrans)
	{		
	GetObserver("MainObserver")->Camera()->SetCameraScale(1500.0138171976105);
	}*/
    QOpenGLWindow* qglWdiget = dynamic_cast<QOpenGLWindow*>(GetDrawSurface());
}

inline void CIPScene::MoveX(int ScreenLen)
{
	QVector3D temp1(0, 0, 0), glPoint1;
	QVector3D temp2(ScreenLen, 0, 0),glPoint2;
	ScreenToGL(temp1, glPoint1);
	ScreenToGL(temp2, glPoint2);
	float factor =glPoint2.x() - glPoint1.x();

	this->GetSceneTransform()->Tranlate(QVector3D(factor,0,0));
	FlushScene();
}

inline void CIPScene::MoveY(int ScreenLen)
{
	QVector3D temp1(0, 0, 0), glPoint1;
	QVector3D temp2(ScreenLen, 0, 0),glPoint2;
	ScreenToGL(temp1, glPoint1);
	ScreenToGL(temp2, glPoint2);
	float factor =glPoint2.x() - glPoint1.x();

	this->GetSceneTransform()->Tranlate(QVector3D(0,factor,0));
	FlushScene();
}

void CIPScene::SetCameraScale(qreal scale)
{
	this->GetObserver("MainObserver")->Camera()->SetCameraScale(scale);
}

qreal CIPScene::GetCameraScale()
{
	return this->GetObserver("MainObserver")->Camera()->GetCameraScale();	
}

void CIPScene::Scale(float scale)
{
	this->GetObserver("MainObserver")->Camera()->Scale(scale);
	FlushScene();
}

void CIPScene::FullMap()
{
	QMatrix4x4 matrix;
	matrix.translate(QVector3D(28440, -228952, 0));
	this->GetSceneTransform()->Tranlate(matrix);

	this->GetObserver("MainObserver")->Camera()->SetCameraScale(37197);
	FlushScene();
}

 
