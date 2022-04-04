#include "SaveLayerToShape.h"
#include <QDebug>
#include "SymbolLayer.h"

//extern "C"
//{
#include "gpc.cpp"
//};


CSaveLayerToShape::CSaveLayerToShape(void)
{
}


CSaveLayerToShape::~CSaveLayerToShape(void)
{
}

void CSaveLayerToShape::SaveMultiFeatureToShape( QString outfileName,  VectorLayer *pLay,int type )
{

}

void CSaveLayerToShape::SaveSimpleFeatureToShape( QString outfileName,  VectorLayer *pLay,int type )
{
	if (pLay == NULL)
	{
		CCIPASMessageBox::CIPASWaring(this, CCIPASMessageBox::CIPAS_OK, QString(tr("警告")), QString(tr("请选择所存图层")));
		return;
	}
	if(outfileName.isEmpty())
	{
		CCIPASMessageBox::CIPASWaring(this, CCIPASMessageBox::CIPAS_OK, QString(tr("警告")), QString(tr("请输入文件名")));
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

bool CSaveLayerToShape::SavePoint( const char* fileName, VectorLayer *lay)
{
	//const char * shpfilename = *fileName + "_point";

	//int		nShapeType,nVMax;
	//double	*padfZ = NULL, *padfM = NULL;
	//SHPObject	*psObject;
	//SHPHandle	hSHP;
	//nShapeType = SHPT_POINT;
	//hSHP = SHPCreate(shpfilename, nShapeType );
	//SHPClose( hSHP );

	///**************************SHP&SHX********************************/
	//GradsProvider *gDP = dynamic_cast<GradsProvider *>(lay->Provider());
	//QHash<QString, CElement*> *elementHash = gDP->GetMicapsHashData();

	//CElement* stationlat = elementHash->value("站点纬度");
	//CElement* stationlon = elementHash->value("站点经度");

	//float* lat = stationlat->Data().GetDataArray(0);
	//float* lon = stationlon->Data().GetDataArray(0);

	//hSHP = SHPOpen( shpfilename, "r+b" );

	//nVMax = stationlat->Data().DataBlockDimensionSize(0,0);

	//for (int i = 0 ;i<nVMax;i++)
	//{
	//	double padfX = lon[i];
	//	double padfY = lat[i];
	//	psObject = SHPCreateObject( nShapeType, i, 0, NULL, NULL,
	//		1,&padfX , &padfY, padfZ, padfM );
	//	SHPWriteObject( hSHP, -1, psObject );
	//	SHPDestroyObject( psObject );
	//}

	//SHPClose( hSHP );

	/**************************DBF********************************/
	//DBFHandle	hDBF;
	//hDBF = DBFCreate(shpfilename);
	//if( hDBF == NULL )
	//{
	//	qDebug( "DBFCreate failed.");
	//	return false;
	//}

	
	//if( DBFAddField( hDBF, "NAME", FTString, 20, 0 ) == -1 )
	//{
	//	qDebug( "DBFAddField  failed.\n");
	//}
	//DBFAddField( hDBF, "AREA", FTDouble, 9, 3 );
	//DBFAddField( hDBF, "VALUE ", FTDouble, 9, 2 );

	//int 	iRecord = DBFGetRecordCount( hDBF );

	//DBFWriteStringAttribute(hDBF, iRecord,0,"REGION1" );
	//DBFWriteDoubleAttribute(hDBF, iRecord,1, 25.656  );
	//DBFWriteDoubleAttribute(hDBF, iRecord, 2, 150.22  );



	//int i = DBFGetFieldCount(hDBF);
	//printf ("%ld Columns,  %ld Records in file\n",i,DBFGetRecordCount(hDBF));
	//DBFClose( hDBF );
	/**************************DBF********************************/


	return true;
}

bool CSaveLayerToShape::SaveArc( const char* fileName, VectorLayer *lay)
{
	const char * shpfilename = fileName ;

	int		nShapeType, nVertices, *panParts, nVMax =  0;
	double	*padfX, *padfY, *padfZ = NULL, *padfM = NULL;
	SHPObject	*psObject;

	// create shp and shx
	SHPHandle	hSHP;
	nShapeType = SHPT_ARC;
	hSHP = SHPCreate(shpfilename, nShapeType );
	SHPClose( hSHP );

	//creat dbf
	DBFHandle	hDBF;
	hDBF = DBFCreate(shpfilename);
	if( hDBF == NULL )
	{
		qDebug( "DBFCreate failed.");
		return false;
	}

	//if( DBFAddField( hDBF, "NAME", FTString, 20, 0 ) == -1 )
	//{
	//	qDebug( "DBFAddField  failed.\n");
	//}
	//if( DBFAddField( hDBF, "TYPE",FTInteger , 9, 0 ) == -1 )
	//{
	//	qDebug( "DBFAddField  failed.\n");
	//}
	//if( DBFAddField( hDBF, "WIDTH", FTInteger, 9, 3 ) == -1 )
	//{
	//	qDebug( "DBFAddField  failed.\n");
	//}

	if( DBFAddField( hDBF, "VALUE ", FTDouble, 9, 2 ) == -1 )
	{
		qDebug( "DBFAddField  failed.\n");
	}

	int 	iRecord = DBFGetRecordCount( hDBF );
	hSHP = SHPOpen( shpfilename, "r+" );

	QHash<QString,CSymbolContourLine*> *HashLineList = lay->Get2DHashLineList();	
	QHash<QString,CSymbolContourLine*>::iterator i ;

// 	SymbolLayer * symbollayer = dynamic_cast<SymbolLayer * >(lay);
// 	if (symbollayer==NULL)
// 	{
// 		return false;
// 	}
// 	SymbolData* temp = symbollayer->getSymbolData();
// 	QVector<CSymbolContourLine*>  *contour = temp->getSymbolContourLine();
	int arcNum = 0 ;
 
	for (i = HashLineList->begin(); i != HashLineList->end(); ++i)
	{
		CSymbolContourLine *poly = i.value(); 
		QVector3DArray* vecterArray = poly->OriginalArray(); 
		nVMax = vecterArray->count();

		padfX = (double *) malloc(sizeof(double) * nVMax);
		padfY = (double *) malloc(sizeof(double) * nVMax);
		padfZ = (double *) malloc(sizeof(double) * nVMax);
		panParts = (int *) malloc(sizeof(int) * nVMax );
		panParts[0] = 0;
		for (int vertex = 0 ;vertex<nVMax;vertex++)
		{
			QVector3D tempV = vecterArray->at(vertex);
			double x = tempV.x();
			double y = tempV.y();
			padfX[vertex] = x;
			padfY[vertex] = y;
		}
		nVertices =  vecterArray->count();;
		psObject = SHPCreateObject( nShapeType, -1, 1, panParts, NULL,nVertices, padfX, padfY, NULL, NULL );
		SHPWriteObject( hSHP, -1, psObject );


		CSymbolAttribute * attr = poly->GetAttribute();

		std::string atrr_name = (attr->GetLineType()).toStdString();
		const char * str = atrr_name.data();
		//DBFWriteStringAttribute(hDBF, arcNum, 0,str);
		//DBFWriteIntegerAttribute(hDBF, arcNum, 1, attr->GetSymbolType());
		//DBFWriteIntegerAttribute(hDBF, arcNum, 2, attr->GetLineWidth());
		DBFWriteDoubleAttribute(hDBF, arcNum, 0, attr->GetValue());

		SHPDestroyObject( psObject );
		arcNum ++;

		free( panParts );
		free( padfX );
		free( padfY );
		free( padfZ );
		free( padfM );

	}
	SHPClose( hSHP );
	DBFClose( hDBF );
	return true;

}

bool CSaveLayerToShape::SavePolygon(const char* fileName, VectorLayer *lay )
{
	const char * shpfilename = fileName;
	/**************************SHP&SHX********************************/
	int		nShapeType, nVertices = 0, *panParts, nVMax =  0;
	double	*padfX, *padfY, *padfZ = NULL, *padfM = NULL;
	SHPObject	*psObject;
	const char  *tuple = "";

	SHPHandle	hSHP;
	nShapeType = SHPT_POLYGON;
	hSHP = SHPCreate(shpfilename, nShapeType );
	SHPClose( hSHP );

	//creat dbf
	DBFHandle	hDBF;
	hDBF = DBFCreate(shpfilename);
	if( hDBF == NULL )
	{
		qDebug( "DBFCreate failed.");
		return false;
	}

	if( DBFAddField( hDBF, "VALUE ", FTDouble, 9, 2 ) == -1 )
	{
		qDebug( "DBFAddField  failed.\n");
	}

	int 	iRecord = DBFGetRecordCount( hDBF );

	hSHP = SHPOpen( shpfilename, "r+" );
	QHash<QString,GISPolygon*> *HashPolygon = lay->Get2DFDnoTessel();
	QHash<QString,gpc_polygon*> hashPolyCombine;
	QStringList keylist = HashPolygon->uniqueKeys();

	//多边形合并
	for (int i = 0 ;i< keylist.count();i++)
	{
		QList<GISPolygon *> valuelist = HashPolygon->values(keylist.at(i));
		gpc_polygon * tempPoly = combinePolygon(valuelist);
		hashPolyCombine.insert(keylist.at(i),tempPoly);
	}


	//QHash<QString,gpc_polygon*>::iterator iter;

	//int arcNum = 0 ;
	//for (iter = hashPolyCombine.begin() ; iter != hashPolyCombine.end(); ++iter  )
	//{
	//	gpc_polygon *poly  = iter.value();
	//	panParts = (int *) malloc(sizeof(int) * (poly->num_contours)) ;
	//	int c, v;
	//	nVertices = 0;
	//	for (c= 0; c < poly->num_contours; c++)
	//	{
	//		panParts[c] = nVertices;
	//		nVertices += poly->contour[c].num_vertices;
	//	}

	//	padfX = (double *) malloc(sizeof(double) * nVertices);
	//	padfY = (double *) malloc(sizeof(double) * nVertices);

	//	int vertex = 0 ; 
	//	for (v= 0; v < poly->num_contours; v++)
	//	{
	//		for (int index= 0; index < poly->contour[v].num_vertices; index++)
	//		{
	//			padfX[vertex] = poly->contour[v].vertex[index].x;
	//			padfY[vertex] = poly->contour[v].vertex[index].y;
	//			vertex ++;
	//		}
	//	}
	//	psObject = SHPCreateObject( nShapeType, -1,poly->num_contours, panParts, NULL,nVertices, padfX, padfY, NULL, NULL );
	//	SHPWriteObject( hSHP, -1, psObject );

	//	DBFWriteDoubleAttribute(hDBF, arcNum, 0, (iter.key()).toFloat());

	//	SHPDestroyObject( psObject );
	//	arcNum++;
	//	free( panParts );
	//	free( padfX );
	//	free( padfY );
	//	gpc_free_polygon(poly);
	//}


	//多边形求差
	QHash<QString,gpc_polygon*> *hp = differencePolygon(&hashPolyCombine);

	QHash<QString,gpc_polygon*>::iterator iter;

	int arcNum = 0 ;
	for (iter= hp->begin() ; iter != hp->end(); ++iter  )
	{
		gpc_polygon *poly  = iter.value();
		panParts = (int *) malloc(sizeof(int) * (poly->num_contours)) ;
		int c, v;
		nVertices = 0;
		for (c= 0; c < poly->num_contours; c++)
		{
			panParts[c] = nVertices;
			nVertices += poly->contour[c].num_vertices;
		}

		padfX = (double *) malloc(sizeof(double) * nVertices);
		padfY = (double *) malloc(sizeof(double) * nVertices);

		int vertex = 0 ; 
		for (v= 0; v < poly->num_contours; v++)
		{
			for (int index= 0; index < poly->contour[v].num_vertices; index++)
			{
				padfX[vertex] = poly->contour[v].vertex[index].x;
				padfY[vertex] = poly->contour[v].vertex[index].y;
				vertex ++;
			}
		}
		psObject = SHPCreateObject( nShapeType, -1,poly->num_contours, panParts, NULL,nVertices, padfX, padfY, NULL, NULL );
		SHPWriteObject( hSHP, -1, psObject );

		DBFWriteDoubleAttribute(hDBF, arcNum, 0, (iter.key()).toFloat());

		SHPDestroyObject( psObject );
		arcNum++;
		free( panParts );
		free( padfX );
		free( padfY );
		gpc_free_polygon(poly);
	}
	SHPClose( hSHP );
	DBFClose( hDBF );
	return true;
}

gpc_polygon *CSaveLayerToShape::convertToGPC_Poly( GISPolygon *poly )
{
	gpc_polygon *p =new gpc_polygon; 
	
	p->num_contours = 1;
	MALLOC(p->hole, p->num_contours * sizeof(int),"hole flag array creation", int);
	MALLOC(p->contour, p->num_contours * sizeof(gpc_vertex_list), "contour creation", gpc_vertex_list);
	for (int c= 0; c < p->num_contours; c++)
	{
		QVector3DArray* ary = poly->getVectexArray();
		p->contour[c].num_vertices = ary->count();
		p->hole[c]= FALSE; /* Assume all contours to be external */

		MALLOC(p->contour[c].vertex, p->contour[c].num_vertices* sizeof(gpc_vertex), "vertex creation", gpc_vertex);

		for (int v= 0; v < p->contour[c].num_vertices; v++)
		{
			QVector3D tempV = ary->at(v);
			double x = tempV.x();
			double y = tempV.y();
			p->contour[c].vertex[v].x = x;
			p->contour[c].vertex[v].y = y;
		}
	}
	return p;
}

gpc_polygon * CSaveLayerToShape::combinePolygon( QList<GISPolygon *> p )
{
	gpc_polygon *subject = convertToGPC_Poly(p.at(0));
	

	for (int i = 1 ;i < p.count();i++)
	{
		gpc_polygon *result = new gpc_polygon;
		gpc_polygon *clip = convertToGPC_Poly(p.at(i));
		gpc_polygon_clip(GPC_UNION,subject,clip,result);
		subject = result;
	}

	return subject;

}

QHash<QString,gpc_polygon*> * CSaveLayerToShape::differencePolygon( QHash<QString,gpc_polygon*> *hp )
{
	int polycount = hp->count();
	double * keysort = (double*)malloc(sizeof(double) * polycount);
	QHash <QString ,double> tempHash;

	QHash<QString,gpc_polygon*>::iterator it;
	int index = 0 ;
	for (it = hp->begin();it != hp->end();++it)
	{
		tempHash.insert(it.key(),it.key().toDouble());
		keysort[index] = it.key().toDouble();
		index ++;
	}
	double b ;

	//冒泡排序(升序)
	for (int j = 0; j<polycount-1;j ++)
	{
		for (int i = 0; i<polycount-1 -j;i++)
		{
			if (keysort[i] > keysort[i+1])
			{
				b = keysort[i];
				keysort[i] = keysort[i+1];
				keysort[i+1] = b;
			}
		}
	}

	QHash<QString,gpc_polygon*> *final_hp = new QHash<QString,gpc_polygon*>;

	int begin = 0;
	for (int i = begin ;i<polycount ;i++)
	{

		gpc_polygon *subject = hp->value(tempHash.key(keysort[i]));

		for (int j = begin+1 ;j < /*((begin+2 >polycount)? */polycount/* : begin+2)*/;j++)
		{
			gpc_polygon *result = new gpc_polygon ;
			gpc_polygon *clip = hp->value(tempHash.key(keysort[j]));
			gpc_polygon_clip(GPC_DIFF,subject,clip,result);
			subject = result;
		}

		final_hp->insert(tempHash.key(keysort[i]),subject);
		begin++;

	}

	free(keysort) ;

	QHash<QString, gpc_polygon*>::Iterator itt = hp->begin();
	while( itt != hp->end())
	{
		gpc_polygon * po = itt.value();
		gpc_free_polygon(po);
		itt = hp->erase(itt);

	}

	return final_hp;
}
