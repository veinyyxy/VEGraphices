#include "SaveLayerToKML.h"
#include <QDebug>
//#include "gpc.cpp"

SaveLayerToKML::SaveLayerToKML(void)
{
}


SaveLayerToKML::~SaveLayerToKML(void)
{
	if ( mGeom )
	{
		OGR_G_DestroyGeometry( mGeom );
	}

	if ( mDS )
	{
		OGR_DS_Destroy( mDS );
	}
}

void SaveLayerToKML::SaveMultiFeatureToKML( QString outfileName, VectorLayer *pLay,int type )
{

}

void SaveLayerToKML::SaveSimpleFeatureToKML( QString outfileName, VectorLayer *pLay,int type )
{
	if (pLay == NULL)
	{
		CCIPASMessageBox::CIPASWaring(this, CCIPASMessageBox::CIPAS_OK, tr("警告"), tr("请选择所存图层"));
		return;
	}
	if(outfileName.isEmpty())
	{
		CCIPASMessageBox::CIPASWaring(this, CCIPASMessageBox::CIPAS_OK, tr("警告"), tr("请输入文件名"));
		return;
	}
	std::string stdPath = outfileName.toStdString();
	const char * path = stdPath.data();

	if ( type == 8)
	{
		return;
	}else if (type == 3)
	{
		SaveArc(path,pLay);
	}else if (type == 4)
	{
		SavePolygon(path,pLay);
	}

}

bool SaveLayerToKML::SavePoint( const char* fileName, VectorLayer *lay )
{
	return true;
}

bool SaveLayerToKML::SaveArc( const char* fileName, VectorLayer *lay )
{
	//注册驱动
	OGRRegisterAll();
	OGRSFDriverH poDriver;
	poDriver = OGRGetDriverByName(QString("KML").toLocal8Bit().data());

	if (poDriver == NULL)
	{
		return false;
	}
	 
	mDS = OGR_Dr_CreateDataSource(poDriver,fileName,NULL);

	if ( mDS == NULL )
	{
		return false;
	}
	QTextCodec *mCodec;
	mCodec = QTextCodec::codecForName("UTF-8");

	OGRSpatialReferenceH ogrRef = NULL;
	OGRwkbGeometryType wkbType = wkbLineString;
	//OGRSpatialReferenceH hSRS = NULL;
	//const char * proj = lay->GetProInfo().toLocal8Bit().data();
	//OSRImportFromProj4(hSRS,proj);

	QString myWkt = "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],TOWGS84[0,0,0,0,0,0,0],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.0174532925199433,AUTHORITY[\"EPSG\",\"9108\"]],AUTHORITY[\"EPSG\",\"4326\"]]";
	//char* Wkt;
	//if ( OSRExportToWkt( hSRS, &Wkt ) == OGRERR_NONE )
	//{
	//	myWkt = Wkt;
	//	OGRFree( Wkt );
	//}
	ogrRef = OSRNewSpatialReference(myWkt.toLocal8Bit().data());

	QString layerName = QFileInfo( fileName ).baseName();
	// OGR_DS_GetLayerByName(mDS,QFile::encodeName(layerName).data()); 
	OGRLayerH mLayer = OGR_DS_CreateLayer(mDS,QFile::encodeName(layerName).data(),ogrRef,wkbType,NULL);
	 		
	//OSRDestroySpatialReference( ogrRef );
	 
	if ( mLayer == NULL )
	{
		return false;
	}
	
	OGRFieldType ogrType = OFTString; //default to string 
	int ogrWidth =255 ;
	int ogrPrecision = 0;

	// create field definition
	OGRFieldDefnH fld = OGR_Fld_Create( mCodec->fromUnicode( "Name" ), ogrType );
	if ( ogrWidth > 0 )
	{
		OGR_Fld_SetWidth( fld, ogrWidth );
	}

	if ( ogrPrecision >= 0 )
	{
		OGR_Fld_SetPrecision( fld, ogrPrecision );
	}
	 
	if ( OGR_L_CreateField( mLayer, fld, true ) != OGRERR_NONE )
	{
		return false;
	}

	// build geometry from WKB

	QHash<QString,CSymbolContourLine*> *HashLineList = lay->Get2DHashLineList();
	int arcNum = 0 ;
	QHash<QString,CSymbolContourLine*>::iterator i ;
	for (i = HashLineList->begin(); i != HashLineList->end(); ++i)
	{
		OGRFeatureH poFeature = OGR_F_Create( OGR_L_GetLayerDefn( mLayer ) );
		mGeom = OGR_G_CreateGeometry(wkbType);
		OGR_F_SetFieldString( poFeature, 0, mCodec->fromUnicode( i.key()));

		CSymbolContourLine *poly = i.value();
		QVector3DArray* vecterArray = poly->OriginalArray(); 

		for (int vertex = 0 ;vertex<vecterArray->count();vertex++)
		{
			QVector3D tempV = vecterArray->at(vertex);
			double x = tempV.x();
			double y = tempV.y();
			OGR_G_AddPoint(mGeom,x,y,i.key().toDouble());
		}
		OGR_F_SetGeometry(poFeature,mGeom);

		OGR_L_CreateFeature(mLayer,poFeature);
		OGR_F_Destroy(poFeature);
	}
	 	 
	return true;

}

bool SaveLayerToKML::SavePolygon( const char* fileName, VectorLayer *lay )
{
	//注册驱动
	OGRRegisterAll();
	OGRSFDriverH poDriver;
	poDriver = OGRGetDriverByName(QString("KML").toLocal8Bit().data());

	if (poDriver == NULL)
	{
		return false;
	}

	mDS = OGR_Dr_CreateDataSource(poDriver,fileName,NULL);

	if ( mDS == NULL )
	{
		return false;
	}
	QTextCodec *mCodec;
	mCodec = QTextCodec::codecForName("UTF-8");

	OGRSpatialReferenceH ogrRef = NULL;
	OGRwkbGeometryType wkbType = wkbMultiPolygon;
	//OGRSpatialReferenceH hSRS = NULL;
	//const char * proj = lay->GetProInfo().toLocal8Bit().data();
	//OSRImportFromProj4(hSRS,proj);

	QString myWkt = "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],TOWGS84[0,0,0,0,0,0,0],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.0174532925199433,AUTHORITY[\"EPSG\",\"9108\"]],AUTHORITY[\"EPSG\",\"4326\"]]";
	//char* Wkt;
	//if ( OSRExportToWkt( hSRS, &Wkt ) == OGRERR_NONE )
	//{
	//	myWkt = Wkt;
	//	OGRFree( Wkt );
	//}
	ogrRef = OSRNewSpatialReference(myWkt.toLocal8Bit().data());

	QString layerName = QFileInfo( fileName ).baseName();
	// OGR_DS_GetLayerByName(mDS,QFile::encodeName(layerName).data()); 
	OGRLayerH mLayer = OGR_DS_CreateLayer(mDS,QFile::encodeName(layerName).data(),ogrRef,wkbType,NULL);

	//OSRDestroySpatialReference( ogrRef );

	if ( mLayer == NULL )
	{
		return false;
	}

	OGRFieldType ogrType = OFTString; //default to string 
	int ogrWidth =255 ;
	int ogrPrecision = 0;

	// create field definition
	OGRFieldDefnH fld = OGR_Fld_Create( mCodec->fromUnicode( "Name" ), ogrType );
	if ( ogrWidth > 0 )
	{
		OGR_Fld_SetWidth( fld, ogrWidth );
	}

	if ( ogrPrecision >= 0 )
	{
		OGR_Fld_SetPrecision( fld, ogrPrecision );
	}

	if ( OGR_L_CreateField( mLayer, fld, true ) != OGRERR_NONE )
	{
		return false;
	}

	// build geometry from WKB
	QHash<QString,GISPolygon*> *HashPolygon = lay->Get2DFDnoTessel();
	QHash<QString,gpc_polygon*> hashPolyCombine;
	QStringList keylist = HashPolygon->uniqueKeys();

	//多边形合并
	for (int i = 0 ;i< keylist.count();i++)
	{
		QList<GISPolygon *> valuelist = HashPolygon->values(keylist.at(i));
		gpc_polygon * tempPoly = CSaveLayerToShape::combinePolygon(valuelist);
		hashPolyCombine.insert(keylist.at(i),tempPoly);
	}
	//多边形求差
	QHash<QString,gpc_polygon*> *hp = CSaveLayerToShape::differencePolygon(&hashPolyCombine);
	QHash<QString,gpc_polygon*>::iterator iter;

	int *panParts;
	for (iter= hp->begin() ; iter != hp->end(); ++iter  )
	{
		OGRFeatureH poFeature = OGR_F_Create( OGR_L_GetLayerDefn( mLayer ) );
		mGeom = OGR_G_CreateGeometry(wkbType);
		OGR_F_SetFieldString( poFeature, 0, mCodec->fromUnicode( iter.key()));
		gpc_polygon *poly  = iter.value();
		panParts = (int *) malloc(sizeof(int) * (poly->num_contours)) ;
		int v; 
		for (v = 0; v < poly->num_contours; v++)
		{
			OGRGeometryH tempGeom = OGR_G_CreateGeometry(wkbPolygon );
			OGRGeometryH linering = OGR_G_CreateGeometry(wkbLinearRing);
			for (int index= 0; index < poly->contour[v].num_vertices; index++)
			{
				double x = poly->contour[v].vertex[index].x;
				double y =  poly->contour[v].vertex[index].y;
				 OGR_G_AddPoint(linering,x,y,iter.key().toDouble());
			}
			OGRErr  tt =OGR_G_AddGeometry(tempGeom,linering);
			OGRErr  pp =OGR_G_AddGeometry(mGeom,tempGeom); 
		//	OGR_G_DestroyGeometry(tempGeom);

		}

		OGR_F_SetGeometry(poFeature,mGeom);
		OGR_L_CreateFeature(mLayer,poFeature);
		OGR_F_Destroy(poFeature);
		gpc_free_polygon(poly);
	}

	return true;

}
