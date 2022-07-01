#include "QRenderWindow.h"
#include "QScene.h"

QScene::QScene(void)
{
	m_bIsRun = false;
	root = 0;
	currentObserver = 0;
	m_SceneCallBack = 0;
	m_NodeCallBack = 0;
	m_glContext = 0;
	m_pMouseManipulator = 0;
	m_EnableThread = false;
}

QScene::QScene( QOpenGLContext* glContext )/* : QGLWidget(glContext)*/
{
	m_bIsRun = false;
	root = 0;
	currentObserver = 0;
	m_SceneCallBack = 0;
	m_NodeCallBack = 0;
	m_glContext = 0;
	m_pMouseManipulator = 0;
	m_EnableThread = false;
}

QScene::~QScene(void)
{
	if(root) delete root;
}

void QScene::SetErgodic(QNode* n)
{
	QRenderable* renderable = 0;
	QNode* node = 0;
	if(n)
	{
		n->IteratorHome();
		while(node = n->DumpNode())
		{
			node->ApplyState();
			node->ApplyTransform();
			SetErgodic(node);
		}

		while(renderable = n->DumpRenderable())
		{
			//glPushMatrix();
			renderable->ApplayAttrib();
			renderable->Render();
			//glPopMatrix();
		}
	}
}

void QScene::Render()
{
	m_pDrawSurface->MakeGLContext();
	InitScene();
	RenderFrame();
	m_pDrawSurface->DoneGLContext();
}

//int QScene::RegisterAcessor(const QString&  extName, const QString& filename)
//{
//	QLibrary* pLib = new QLibrary(filename);
//	QAccessor* accessor = (QAccessor*)pLib->resolve("GetAccessor");
//
//	if(accessor)
//		accessors.insert(extName, accessor);
//
//	return accessors.size();
//}
//
//QNode* QScene::ReadFile(const QString&  accessorKey, const QString& modFile)
//{
//	QAccessor* accessor = accessors[accessorKey];
//	if(!accessor)
//		return 0;
//	accessor->Read(modFile);
//	return accessor->Get();
//}

void QScene::SetNode( const QNode* n )
{
	root = (QNode*)n;
}

QNode* QScene::GetNode()
{
	return root;
}

QVector3D QScene::SceneColor()
{
	return backgroundColor;
}

void QScene::SceneColor( const QVector3D& color )
{
	backgroundColor = color;
}

void QScene::RenderFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	QObserver* ob;
	QMapIterator<QString, QObserver*> iter(m_ObserverMap);
	while(iter.hasNext())
	{
		m_SceneTransform.EnableTransforms();
		if(m_SceneCallBack)
		{
			m_SceneCallBack->ApplyTransform(&m_SceneTransform);
		}
		ob = iter.next().value();

		if(m_SceneCallBack)
		{
			m_SceneCallBack->ApplyObserver(iter.next().key(), ob);
		}
		ob->ApplyObserver();
		if(root)
		{
			m_RenderContext.Observer(ob);
            m_RenderContext.RenderWindow((QOpenGLWidget*)this);
			m_RenderContext.Transform(&m_SceneTransform);

			root->SetObserver(ob);
			root->RenderContext(&m_RenderContext);
			root->SetTransform(&m_SceneTransform);
			root->SetNodeCallBack(m_NodeCallBack);
			root->NodeProcess();
		}
		m_SceneTransform.UnableTransforms();
	}
}

void QScene::InitScene()
{
}

DrawSurface* QScene::GetDrawSurface()

{
        return m_pDrawSurface;
}

void QScene::PickUp( int x, int y )
{
	//GLint dep;
	//glGetIntegerv(GL_MAX_NAME_STACK_DEPTH, &dep);
	//GLint glviewport[4] = {0};
	//GLint hits;
	//GLuint selectBuf[512] = {0};
	//GLenum getMode = 0;
	//glSelectBuffer (512, selectBuf);
	//glRenderMode(GL_SELECT);

	//glInitNames();
	//glPushName(5);

	//glGetIntegerv(GL_RENDER_MODE, (GLint*)&getMode);
	//glGetIntegerv (GL_VIEWPORT, glviewport);
	//

	//glMatrixMode (GL_PROJECTION);
	//glPushMatrix ();
	//glLoadIdentity ();

	//gluPickMatrix ((GLdouble) x, (GLdouble) (glviewport[3] - y),
	//	0.5, 0.5, glviewport);

	//glLoadName(5);
	//glPushName(5);
	//Render();
	//glPopName();

	//glMatrixMode (GL_PROJECTION);
	//glPopMatrix ();

	////glFlush ();
	//hits = glRenderMode (GL_RENDER);
	//qDebug("%f, %f, %f, %f, %f", selectBuf[0], selectBuf[1], selectBuf[2], selectBuf[3], selectBuf[4]);
	PickRay(QPoint(x, y));
	QVector3D xj;
	QVector3D v1(-20, 0, 0);
	QVector3D v2(20, 0, 0);
	QVector3D v3(0, 20, 0);
	bool res = Intersect(v1, v2, v3, &xj);
}
void QScene::PickRay(const QPoint& screenPoint)
{
	QMatrix4x4 mv;
	QMatrix4x4 prj;
	double o1x, o1y, o1z;
	double o2x, o2y, o2z;

	//WGL.GetCursorPos(ref mpos);
	/*Point p = this.PointToClient(new Point(mpos.x, mpos.y));
	mpos.x = p.X; mpos.y = p.Y;*/
	glGetFloatv(GL_MODELVIEW_MATRIX , mv.data());
	glGetFloatv(GL_PROJECTION_MATRIX, prj.data());

	int viewPort[4] = {0};
	glGetIntegerv(GL_VIEWPORT, viewPort);

	GLdouble modelMatrix[16] = { 0 };
	GLdouble projMatrix[16] = { 0 };
	for (int i = 0; i < sizeof modelMatrix; i++)
	{
		modelMatrix[i] = mv.data()[i];
	}
	for (int i = 0; i < sizeof modelMatrix; i++)
	{
		projMatrix[i] = prj.data()[i];
	}
	gluUnProject(screenPoint.x(), (viewPort[3] - screenPoint.y() - 1), 0.0, modelMatrix, projMatrix, viewPort, &o1x, &o1y, &o1z);
	gluUnProject(screenPoint.x(), (viewPort[3] - screenPoint.y() - 1), 1.0, modelMatrix, projMatrix, viewPort, &o2x, &o2y, &o2z);

	rayStart.setX(o1x);rayStart.setY(o1y);rayStart.setZ(/*o1y*/0);
	rayEnd.setX(o2x);rayEnd.setY(o2y);rayEnd.setZ(/*o2z*/0);
}

bool QScene::IsInTrg(QVector3D vtxA, QVector3D vtxB, QVector3D vtxC, TriEqaResult& pHit)
{
	LineEqv l1(vtxA, vtxB - vtxA), l2(vtxC, vtxA - vtxC), l3(vtxB, vtxC - vtxB);

	double res1[3] = {0};
	double res2[3] = {0};

	QVector3D pos(pHit.x, pHit.y, pHit.z);

	l1.TestPoint(pos, res1, res2);
	l2.TestPoint(pos, res1 + 1, res2 + 1);
	l3.TestPoint(pos, res1 + 2, res2 + 2);

	int i;
	int ca = 0, cm = 0;
	for (i = 0; i <= 2; i++)
	{
		if (res1[i] < 0) { cm++; }
		if (res1[i] >= 0) { ca++; }
	}

	if ((ca == 3) | (cm == 3))
	{
		ca = 0;
		cm = 0;
		for (i = 0; i <= 2; i++)
		{
			if (res2[i] < 0) { cm++; }
			if (res2[i] >= 0) { ca++; }
		}

		if ((ca == 3) | (cm == 3))
		{
			return true;
		}
		else
		{
			ca = 0; cm = 0;
			for (i = 0; i <= 2; i++)
			{
				if (res2[i] <= 0) { cm++; }
				if (res2[i] > 0) { ca++; }
			}
			if ((ca == 3) | (cm == 3))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		ca = 0; cm = 0;
		for (i = 0; i <= 2; i++)
		{
			if (res1[i] <= 0) { cm++; }
			if (res1[i] > 0) { ca++; }
		}

		if ((ca == 3) | (cm == 3))
		{
			ca = 0;
			cm = 0;
			for (i = 0; i <= 2; i++)
			{
				if (res2[i] < 0) { cm++; }
				if (res2[i] >= 0) { ca++; }
			}

			if ((ca == 3) | (cm == 3))
			{
				return true;
			}
			else
			{
				ca = 0; cm = 0;
				for (i = 0; i <= 2; i++)
				{
					if (res2[i] <= 0) { cm++; }
					if (res2[i] > 0) { ca++; }
				}
				if ((ca == 3) | (cm == 3))
				{
					return true;
				}
				else
				{ return false; }
			}
		}
		else
		{ return false; }
	}
}

void QScene::CalcPlaneNormal(const QVector3D& P1, const QVector3D& P2, const QVector3D& P3, QVector3D& res)
{
	QVector3D V1, V2;
	//QVector3D result;

	V1 = P2 - P1;
	V2 = P3 - P1;
	res = QVector3D::crossProduct(V1, V2);

	res.normalize();
}

bool QScene::Intersect(QVector3D& vtxA, QVector3D& vtxB, QVector3D& vtxC, QVector3D* pos)
{
	QVector3D tempVer;
	CalcPlaneNormal(vtxA, vtxB, vtxC, tempVer);
        //PlaneEqv
	PlaneEqv curFace(vtxA, tempVer);

	QVector3D ori = rayEnd - rayStart;
	LineEqv curSpd(rayStart, ori);

        //
	TriEqaResult pHit(curSpd, curFace);
        //
	if (!pHit.hasSolution) { return false; }

	pos = new QVector3D(pHit.x, pHit.y, pHit.z);

        //
	QVector3D re = (*pos - rayStart) * ori;
	if (re.x() > 0 && re.y() && re.z())
	{
		return IsInTrg(vtxA, vtxB, vtxC, pHit);
	}
	else { return false; }
}

void QScene::ClearSceneTransform()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool QScene::SetViewBackgroundColor(QColor color)
{
	return true;
}

void QScene::run()
{
	while(m_bIsRun)
	{
		Render();
	}
}

//void QScene::initializeGL()
//{
//	InitScene();
//}
//
//void QScene::paintGL()
//{
//	/*m_pDrawSurface->MakeGLContext();
//	InitScene();
//	int iWidth = m_pDrawSurface->width();
//	int iHeight = m_pDrawSurface->height();
//	resizeGL(iWidth, iHeight);*/
//	RenderFrame();
//}

void QScene::AdjustObserver( int width, int height )
{
	double dNear, dFar;
	QMapIterator<QString, QObserver*> iter(m_ObserverMap);
	if(m_ObserverMap.size() == 1)
	{
		QObserver* ob = iter.next().value();
		dNear = ob->Camera()->Near();
		dFar = ob->Camera()->Far();
		ob->Camera()->Ortho(-width/2, width/2, -height/2, height/2, dNear, dFar);
		ob->Viewport()->SetViewport(0, 0, width, height);
	}
	else
	{
		ResizeMultViewport(width, height);
	}
}

double PointDistance(const QVector3D& a, const QVector3D& b)
{
	return sqrt((pow(b.x() - a.x(), 2) + pow(b.y() - a.y(), 2)) + pow(b.z() - a.z(), 2));
}

void QScene::ScreenToGL( const QVector3D& screenPoint, QVector3D& glPoint )
{
	m_pDrawSurface->MakeGLContext();
	GLint    viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ = 0.0f;
	GLdouble posX, posY, posZ;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	winX = screenPoint.x();
	winY = viewport[3] - screenPoint.y();
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	glPoint.setX(posX);
	glPoint.setY(posY);
	glPoint.setZ(posZ);
}

void QScene::FlushScene()
{
	if(!m_EnableThread)
	{
		m_pDrawSurface->FlushSurface();
  }
}

void QScene::ResizeMultViewport( int MainWindowWidth, int MainWindowHeight )
{
}

void QScene::RenderToPixmap( QPixmap* pPixmap )
{
        //*pPixmap = m_DrawFace->renderPixmap(m_DrawFace->width(), m_DrawFace->height(), true);
}

void QScene::GLToScreen( const QVector3D& glPoint, QVector3D& screenPoint )
{
	m_pDrawSurface->MakeGLContext();
	qreal x(0), y(0), z(0), x1(0), y1(0), z1(0);
	GLint    viewport[4] = {0};
	GLdouble modelview[16] = {0};
	GLdouble projection[16] = {0};
	GLfloat winX = 0.0f, winY = 0.0f, winZ = 0.0f;
	GLdouble posX = 0.0, posY = 0.0, posZ = 0.0;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	//glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluProject(glPoint.x(), glPoint.y(), glPoint.z(), modelview, projection, viewport, &x, &y, &z);
	screenPoint.setX(x);
	screenPoint.setY(y);
	screenPoint.setZ(z);
}

DrawSurface* QScene::CreateDrawSurface(SURFACE_TYPE surType)
{
    QSurfaceFormat format;
	format.setVersion(4,0);  
    format.setProfile(QSurfaceFormat::CoreProfile);
    //format.setSampleBuffers(true);
	format.setSamples(4);

	switch(surType)
	{
	case WINDOW_TYPE:
		//m_pDrawSurface = new QRenderWindow;
		m_pDrawSurface = new QRenderWindow(format);
		break;
	case OFF_SCREEN:
        m_pDrawSurface = nullptr;//new PixelBufferSurface;
		break;
	default:
		return 0;
	}
	m_pDrawSurface->SetAssociatedScene(this);
	return m_pDrawSurface;
}

void QScene::SwitchGLState( bool ss, GLenum stateid )
{
	if (ss)
	{
		glEnable(stateid);
	}
	else
	{
		glDisable(stateid);
	}
	FlushScene();
}

void QScene::SetBackgroundColor( const QColor& c )
{
	glClearColor(c.redF(), c.greenF(), c.blueF(), c.alphaF());
	m_backgroundColor = c;
	FlushScene();
}
