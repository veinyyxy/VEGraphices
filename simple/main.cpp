#include <QWidget>
#include <QMainWindow>
#include <QApplication>

#include "CIPScene.h"

void AddPolygon(QNode* pN)
{
	
	GISPolygon* polygon = new GISPolygon;
	QVector3DArray* pV3Array = polygon->getVectexArray();

	pV3Array->push_back(QVector3D(0, 0, 0));
	pV3Array->push_back(QVector3D(0, 100, 0));
	pV3Array->push_back(QVector3D(100, 100, 0));
	pV3Array->push_back(QVector3D(100, 0, 0));

	QVector4DArray* pV3ColorArray = polygon->getColorArray();
	pV3ColorArray->push_back(QVector4D(0.5, 0.5, 0.5, 1.0));
	pV3ColorArray->push_back(QVector4D(0.5, 0.5, 0.5, 1.0));
	pV3ColorArray->push_back(QVector4D(0.5, 0.5, 0.5, 1.0));
	pV3ColorArray->push_back(QVector4D(0.5, 0.5, 0.5, 1.0));
	polygon->SetColorArray();
	pN->InsertRenderable(polygon);
}

void AddLine(QNode* pN)
{
	Line* pLine = new Line;
	pLine->Mode(GL_LINE_LOOP);
	QVector3DArray* pV3Array = pLine->getVectexArray();

	pV3Array->push_back(QVector3D(-20, 10, 0));
	pV3Array->push_back(QVector3D(-20, -110, 0));
	pV3Array->push_back(QVector3D(-120, -110, 0));
	pV3Array->push_back(QVector3D(-120, 10, 0));

	QVector4DArray* pV3ColorArray = pLine->getColorArray();
	pV3ColorArray->push_back(QVector4D(1.0, 0.0, 0.5, 1.0));
	pV3ColorArray->push_back(QVector4D(1.0, 0.0, 0.5, 1.0));
	pV3ColorArray->push_back(QVector4D(1.0, 0.0, 0.5, 1.0));
	pV3ColorArray->push_back(QVector4D(1.0, 0.0, 0.5, 1.0));
	pLine->SetColorArray();
	pN->InsertRenderable(pLine);
}

void AddCoordinates(QNode* pN, DrawSurface* pDS)
{
	double dZ = 0.5;
	Line* pLine = new Line;
	QVector3DArray* pV3Array = pLine->getVectexArray();

	double dWidth = pDS->Width();
	double dHeight = pDS->Height();

	pV3Array->push_back(QVector3D(-dWidth, 0, dZ));
	pV3Array->push_back(QVector3D(dWidth, 0, dZ));
	pN->InsertRenderable(pLine);
	pLine = new Line;
	pV3Array = pLine->getVectexArray();
	pV3Array->push_back(QVector3D(0, -dHeight, dZ));
	pV3Array->push_back(QVector3D(0, dHeight, dZ));
	pN->InsertRenderable(pLine);

	double iS = dWidth / 5;
	double iH = 5;

	for (double i = -dWidth; i < dWidth ; i+=5)
	{
		pLine = new Line;
		pV3Array = pLine->getVectexArray();
		pV3Array->push_back(QVector3D(i, 0, dZ));
		pV3Array->push_back(QVector3D(i, iH, dZ));
		pN->InsertRenderable(pLine);
	}

	for (double i = -dHeight; i < dHeight ; i+=5)
	{
		pLine = new Line;
		pV3Array = pLine->getVectexArray();
		pV3Array->push_back(QVector3D(0, i, dZ));
		pV3Array->push_back(QVector3D(iH, i, dZ));
		pN->InsertRenderable(pLine);
	}
}

void AddPoint(QNode* pN)
{
	Point* pPoint = new Point;
	
	QVector3DArray* pV3Array = pPoint->getVectexArray();

	pV3Array->push_back(QVector3D(-20, -10, 0));
	pV3Array->push_back(QVector3D(-20, -110, 0));
	pV3Array->push_back(QVector3D(-120, -110, 0));
	pV3Array->push_back(QVector3D(-120, -10, 0));

	QVector4DArray* pV3ColorArray = pPoint->getColorArray();
	pV3ColorArray->push_back(QVector4D(1.0, 1.0, 0.0, 1.0));
	pV3ColorArray->push_back(QVector4D(1.0, 1.0, 0.0, 1.0));
	pV3ColorArray->push_back(QVector4D(1.0, 1.0, 0.0, 1.0));
	pV3ColorArray->push_back(QVector4D(1.0, 1.0, 0.0, 1.0));
	pPoint->SetColorArray();
	pN->InsertRenderable(pPoint);
}
#include "Sphere.h"

void AddSphere(QNode* pN)
{
	Sphere* pSh = new Sphere;

	pN->InsertRenderable(pSh);
}

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	CIPScene cipScene;
	DrawSurface* pDrawSurface = cipScene.CreateDrawSurface();
	QGLWidget* pGLWidget = dynamic_cast<QGLWidget*>(pDrawSurface);
	QMainWindow mainWindow;
	mainWindow.setCentralWidget(pGLWidget);
	//////////////////////////////////////////////////////////////////////////
	cipScene.SetBackgroundColor(QColor(20, 80, 255));
	QLeaf* pLeaf = new QLeaf;
	
	//AddCoordinates(pLeaf, pDrawSurface);
	//AddPolygon(pLeaf);
	//AddLine(pLeaf);
	AddSphere(pLeaf);
	cipScene.SetNode(pLeaf);
	//////////////////////////////////////////////////////////////////////////
	mainWindow.show();
	app.exec();
}