#include "QGISProvider.h"
#include "Line.h"
#include "Polygon.h"
#include "Point.h"
#include "Geometry.h"

QGISProvider::QGISProvider(void)
{
	node = 0;
	node = new QLeaf;
	root = new QObject;
	renderVol = new QRenderVolume;
	lz = 0.0f;
}

QGISProvider::~QGISProvider(void)
{
	if(renderVol)
		delete renderVol;
}

int QGISProvider::GetExtent(double* maxX,double* minX,double* maxY,double* minY)
{
	*maxX=Trans->Transform(boundingBox.MaxX,boundingBox.MaxY,0).x();
	*minX=Trans->Transform(boundingBox.MinX,boundingBox.MinY,0).x();
	*maxY=Trans->Transform(boundingBox.MaxX,boundingBox.MaxY,0).y();
	*minY=Trans->Transform(boundingBox.MinX,boundingBox.MinY,0).y();
	return 1;
}

int QGISProvider::LoadPlug( const QString filename )
{
	return 0;
}

int QGISProvider::InitDataProvider()
{
	QSettings setting("MapPath.ini", QSettings::IniFormat);
	setting.setIniCodec(QTextCodec::codecForName("UTF-8"));

	QgsApplication::setPkgDataPath(setting.value("/PLUG_CONFIG/PRODB_IN").toString());
	QgsApplication::setPluginPath(setting.value("/PLUG_CONFIG/PLUG_IN").toString());
	QgsApplication::initQgis();
	//lz = parentLayer->LayerZ();
	return 0;
}

bool QGISProvider::DumpPoint( Point& point )
{
	QGis::GeometryType geoType = qgsLayer->geometryType();
	if(QGis::Point == geoType)
	{
		return false;
	}
	else
		return false;
}

bool QGISProvider::DumpLine(Line& Lines)
{
	QGis::GeometryType geoType = qgsLayer->geometryType();
	static int i = 0;

	//Line 
	if(QGis::Line && i < featureCount)
	{
		qgsLayer->featureAtId(i,*feature,true,true);
		QgsGeometry * geometry = feature->geometry();
		QgsPolyline qgsPolyline = geometry->asPolyline();
		for (j = 0; j<qgsPolyline.count(); j++)
		{
			QVector3DArray* pArray = Lines.getVectexArray();
			pArray->push_back(QVector3D(qgsPolyline.at(j).x(), qgsPolyline.at(j).y(), 0.0));
		}
		i++;
		return true;
	}
	else
	{
		i = 0;
		return false;
	}
}

bool QGISProvider::DumpLine( GISMultiLine* lineSet )
{
	QGis::GeometryType geoType = qgsLayer->geometryType();
	int i = 0;

	//Line 
	while(i < featureCount)
	{
		Line* line = new Line;
		qgsLayer->featureAtId(i,*feature,true,true);
		QgsGeometry * geometry = feature->geometry();
		QgsPolyline qgsPolyline = geometry->asPolyline();
		for (j = 0; j<qgsPolyline.count(); j++)
		{
			QVector3DArray* pArray = line->getVectexArray();
			pArray->push_back(QVector3D(qgsPolyline.at(j).x(), qgsPolyline.at(j).y(), 0.0));
		}
		lineSet->InsertRenderable(line);
		i++;
	}
	return true;
}

bool QGISProvider::DumpPolygon(GISPolygon& polygPoint )
{
	QGis::GeometryType geoType = qgsLayer->geometryType();
	static int i = 0, i1 = 0, i2 = 0;
	//Line 
	if(/*QGis::Polygon && */i < featureCount)
	{
		qgsLayer->featureAtId(i,*feature,true,true);
		QgsGeometry * geometry = feature->geometry();
		QgsPolygon qgsPolygon = geometry->asPolygon();
		if(qgsPolygon.count() != 0)
		{
			if(i1 < qgsPolygon.size())
			{
				QgsPolyline qgsPolyline = qgsPolygon.at(i1);

				for (j = 0; j<qgsPolyline.count(); j++)
				{
					QVector3DArray* pArray = polygPoint.getVectexArray();
					pArray->push_back(QVector3D(qgsPolyline.at(j).x(), qgsPolyline.at(j).y(), 0.0));
				}
				i1++;
				return true;
			}
			else
				i1 = 0;
		}
		else
		{
			QgsMultiPolygon  qgsMPolygon = geometry->asMultiPolygon ();

			if(i1 < qgsMPolygon.size())
			{			
				QgsPolygon qgsPolygon = qgsMPolygon.at(i1);
				if(i2 < qgsPolygon.size())
				{
					QgsPolyline qgsPolyline = qgsPolygon.at(i2);
					for (j = 0; j<qgsPolyline.count(); j++)
					{
						QVector3DArray* pArray = polygPoint.getVectexArray();
						pArray->push_back(QVector3D(qgsPolyline.at(j).x(), qgsPolyline.at(j).y(), 0.0));
					}
					i2++;
					return true;
				}
				else
					i2 = 0;

				i1++;
				return true;
			}
			else
				i1 = 0;
		}
		i++;
		return true;
	}
	else
	{
		i = 0;
		return false;
	}

}

bool QGISProvider::DumpPolygon( GISMultiPolygon* polygonSet )
{
	for(int i = 0; i < featureCount; i++)
	{
		qgsLayer->featureAtId(i,*feature,true,true);
		QgsGeometry * geometry = feature->geometry();
		QgsPolygon qgsPolygon = geometry->asPolygon();
		if(qgsPolygon.count() != 0)
		{
			for(int i1 = 0; i1 < qgsPolygon.size(); i1++)
			{
				GISPolygon* pol = new GISPolygon;
				pol->Mode(GL_LINE_STRIP);
				QgsPolyline qgsPolyline = qgsPolygon.at(i1);

				for (j = 0; j<qgsPolyline.count(); j++)
				{
					QVector3DArray* pArray = pol->getVectexArray();
					pArray->push_back(QVector3D(qgsPolyline.at(j).x(), qgsPolyline.at(j).y(), 0.0));
				}
				polygonSet->InsertRenderable(pol);
				i1++;
			}
		}
		else
		{
			QgsMultiPolygon  qgsMPolygon = geometry->asMultiPolygon ();

			for(int i3 = 0; i3 < qgsMPolygon.size(); i3++)
			{
				QgsPolygon qgsPolygon = qgsMPolygon.at(i3);
				//GISMultiPolygon* mulPol = new GISMultiPolygon;
				
				for(int i2 = 0; i2 < qgsPolygon.size(); i2++)
				{
					QgsPolyline qgsPolyline = qgsPolygon.at(i2);
					GISPolygon* pol = new GISPolygon;
					pol->Mode(GL_LINE_STRIP);

					for (j = 0; j<qgsPolyline.count(); j++)
					{
						QVector3DArray* pArray = pol->getVectexArray();
						pArray->push_back(QVector3D(qgsPolyline.at(j).x(), qgsPolyline.at(j).y(), 0.0));
					}
					polygonSet->InsertRenderable(pol);
				}
				//polygonSet->InsertRenderable(mulPol);
			}
		}
	}
	
	return true;
}

int QGISProvider::OpenFile( const QString& filename )
{
	QString path = filename.trimmed();
	QFileInfo fi( path );
	myLayerBaseName = fi.completeBaseName();

	qgsLayer = new QgsVectorLayer(path, myLayerBaseName, "ogr");
	QgsVectorDataProvider* vectorProvider = qgsLayer->dataProvider();
	if(!vectorProvider)
		return false;
	QStringList sublayers = vectorProvider->subLayers();
	//获得投影
	QgsCoordinateReferenceSystem qgsCRS = qgsLayer->srs();
	mCRS = qgsCRS.toProj4();
	//
	feature = new QgsFeature();
	featureCount = qgsLayer->featureCount();
	return false;
}

VECTOR_DATA_TYPE QGISProvider::GetDataType()
{
	QGis::GeometryType geoType = qgsLayer->geometryType();
	switch(geoType)
	{
	case QGis::Point:
		return VECTOR_POINT;
	case QGis::Line:
		return VECTOR_LINE;
	case QGis::Polygon:
		return VECTOR_POLYGON;
	default:
		return (VECTOR_DATA_TYPE)0;
	}
}
void QGISProvider::addProj(CoordinateTransform *coorTrans)
{	
	Trans = coorTrans;
}
