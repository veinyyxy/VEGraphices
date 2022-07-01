#include "Proj4Transform.h"


Proj4Transform::Proj4Transform(void) : m_pContext(0), mSourceProjection(0), mDestinationProjection(0)
{
	initialise();
	initProMap();
	initGeoMap();
}


Proj4Transform::~Proj4Transform(void)
{
	// free the proj objects
	if ( mSourceProjection != 0 )
	{
        //pj_free( mSourceProjection );
	}
	if ( mDestinationProjection != 0 )
	{
        //pj_free( mDestinationProjection );
	}
}

void Proj4Transform::setSourceCRS (QString ProjString)
{
	strSourceProjection = ProjString;

	setSourceProjName(QString("WGS84"));	

    mSourceProjection = proj_create(m_pContext, ProjString.toUtf8());
	if ( mSourceProjection == NULL )
	{
		qDebug("Set mSourceProjection fail");
	}
}

void Proj4Transform::setDestCRS (QString ProjString)
{
	strDestinationProjection = ProjString;

//	strDestinationProName = QString("LONGLAT");
	
	if(Pro_map.contains(ProjString))
	{
		setDesProjName(Pro_map.find(ProjString).value());
	}		

    //mDestinationProjection = pj_init_plus(ProjString.toUtf8());
	if ( mDestinationProjection == NULL )
	{
		qDebug("Set mDestinationProjection fail");
	}
}

void Proj4Transform::initialise()
{
	mSourceProjection = NULL;
	mDestinationProjection = NULL;
}

QVector3D Proj4Transform::Transform(const QVector3D vertex, TRANSFORM_DIRECTION direction)
{

	if ( mSourceProjection == mDestinationProjection) return vertex;

	x = vertex.x();
	y = vertex.y();
	z = vertex.z();

	transformCoords( 1, &x, &y, &z, direction );

	return QVector3D(x, y, z);

}

QVector3D Proj4Transform::Transform(GLdouble gLx, GLdouble gLy, GLdouble gLz, TRANSFORM_DIRECTION direction)
{
	return Transform( QVector3D( (qreal)gLx, (qreal)gLy, (qreal)gLz ), direction );
}

QRectF Proj4Transform::TransformBoundingBox(const QRectF rectangle, TRANSFORM_DIRECTION direction)
{
	if ( mSourceProjection == mDestinationProjection ) return rectangle;

	xMAX = rectangle.right();
	yMAX = rectangle.top();
	xMIN = rectangle.left();
	yMIN = rectangle.bottom();

	z = 0.0;
	transformCoords( 1, &xMAX, &yMAX, &z, direction );
	transformCoords( 1, &xMIN, &yMIN, &z, direction );

	return QRectF( xMIN, yMAX, xMAX - xMIN, yMAX - yMIN );

}

int Proj4Transform::transformCoords( const int& numPoints, double *x, double *y, double *z, TRANSFORM_DIRECTION direction )
{
#if 0
	if ( mDestinationProjection == NULL )
	{
		qDebug("DestinationProjection NULL");
		return 0;
	}
	if ( mSourceProjection == NULL )
	{
		qDebug("SourceProjection NULL");
		return 0;
	}

	// use proj4 to do the transform
	// if the source/destination projection is lat/long, convert the points to radians prior to transforming
	if (( pj_is_latlong( mDestinationProjection ) && ( direction == POSITIVE_DIRECTION ) )
		|| ( pj_is_latlong( mSourceProjection ) && ( direction == FORWARD_DIRECTION ) ) )
	{
		for (i = 0; i < numPoints; ++i )
		{
			x[i] *= DEG_TO_RAD;
			y[i] *= DEG_TO_RAD;
			z[i] *= DEG_TO_RAD;
		}
	}

	if ( direction == POSITIVE_DIRECTION )
	{
		projResult = pj_transform( mDestinationProjection, mSourceProjection, numPoints, 0, x, y, z );
	}
	else
	{
		projResult = pj_transform( mSourceProjection, mDestinationProjection, numPoints, 0, x, y, z );
	}

	if ( projResult != 0 )
	{
		//something bad happened....

		for ( int i = 0; i < numPoints; ++i )
		{
			if ( direction == FORWARD_DIRECTION )
			{
				points += QString( "(%1, %2)\n" ).arg( x[i] ).arg( y[i] );
			}
			else
			{
				points += QString( "(%1, %2)\n" ).arg( x[i] * RAD_TO_DEG ).arg( y[i] * RAD_TO_DEG );
			}

			msg = QString( "%1 of %2 failed with error: %3\n" ).arg( x[i] ).arg( y[i] ).arg( QString::fromUtf8( pj_strerrno( projResult ) ) );
			qDebug("pj_transform fail");
			qDebug()<<msg;

		}

		
		return 0;

	}

	// if the result is lat/long, convert the results from radians back to degrees
	if (( pj_is_latlong( mDestinationProjection ) && ( direction == FORWARD_DIRECTION ) )
		|| ( pj_is_latlong( mSourceProjection ) && ( direction == POSITIVE_DIRECTION ) ) )
	{
		for ( int i = 0; i < numPoints; ++i )
		{
			x[i] *= RAD_TO_DEG;
			y[i] *= RAD_TO_DEG;
			z[i] *= RAD_TO_DEG;
		}
	}
	return 1;
#else
    return 0;
#endif
}

int Proj4Transform::Transform(const QVector3DArray& src, QVector3DArray& dst, TRANSFORM_DIRECTION direction)
{
	return 0;
}

int Proj4Transform::Transform( const QVector3D& src, QVector3D& dst )
{
#if 0
	int projResult = 0;
	double dX, dY, dZ;

	if ( mDestinationProjection == NULL || mSourceProjection == NULL)
	{
		return projResult;
	}
	dX = src.x() * DEG_TO_RAD;
	dY = src.y() * DEG_TO_RAD;
	dZ = src.z() * DEG_TO_RAD;
	projResult = pj_transform(mSourceProjection, mDestinationProjection, 1, 0, &dX, &dY, &dZ);
	dst.setX(dX);
	dst.setY(dY);
	dst.setZ(src.z());
	//dst.setZ(dZ);
	return projResult;
#else
    return 0;
#endif
}


/********************************************************
 函数名称： ImportFromESRI
 参数    ： 文件路径
 返回值  ： Proj4 类型的字符串
 作者    ： 陈志学
 日期    ： 2010.12.27
 ********************************************************/
QString Proj4Transform::ImportFromESRI(const char *  pszFname )
{
#if 0
	OGRSpatialReference projection;

	char **papszPrj = CSLLoad(pszFname); 

	OGRErr eErr = projection.importFromESRI(papszPrj); 

	CSLDestroy( papszPrj );
	char *destproj =NULL;
	projection.exportToProj4(&destproj);
	QString temp ;	
	temp = destproj;
	return temp.trimmed();
#else
    return "";
#endif
}

QString Proj4Transform::getDesCRS()
{
	return strDestinationProjection;
}

QString Proj4Transform::getSourceCRS()
{
	return strSourceProjection;
}

 QString Proj4Transform::getDesProjName()
 {
 	return strDestinationProName;
 }
 
 QString Proj4Transform::getSourceProjName()
 {
 	return strSourceProName;
 }

 void Proj4Transform::setSourceProjName(QString ProjString)
 {
	 strSourceProName = ProjString;
 }

 void Proj4Transform::setDesProjName(QString ProjString)
 {
	 strDestinationProName = ProjString;
 } 

 
 void Proj4Transform::initProMap()
 {
	
	 if (!Pro_map.isEmpty()) return;

	 Pro_map.insert(QString("+proj=gstmerc +lat_0=34 +lon_0=108 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs"),QString("GAUSS"));
	 Pro_map.insert(QString("+proj=lcc +lat_1=30 +lat_2=62 +lat_0=0 +lon_0=110 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs"),QString("LAMBERT"));
	 Pro_map.insert(QString("+proj=merc +lat_0=34 +lon_0=110 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs"),QString("MERCATOR"));
	 Pro_map.insert(QString("+proj=eqc +lat_ts=0 +lat_0=-3 +lon_0=110 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs"),QString("LONGLAT"));
	 Pro_map.insert(QString("+proj=stere +lat_0=90 +lon_0=110 +x_0=0 +y_0=11000000 +a=6378140 +b=6356755.288157528 +units=m +no_defs"),QString("STEREOGRAM"));	
	 Pro_map.insert(QString("+proj=stere +lat_0=-90 +lon_0=110 +x_0=0 +y_0=-20000000 +a=6378140 +b=6356755.288157528 +units=m +no_defs"),QString("STEREOGRAM1"));
	 Pro_map.insert(QString("+proj=aea +lat_1=27n +lat_2=45n +lon_0=110e +x_0=0 +y_0=0 +ellps=clrk66"),QString("ALBERS"));

	 Pro_map.insert(QString("+proj=robin +lon_0=110 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs"),QString("ROBINSON"));
	 Pro_map.insert(QString("+proj=moll +lon_0=110 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs"),QString("MOLLWEIDE"));
     Pro_map.insert(QString("+proj=ortho +lat_0=34 +lon_0=110 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs"),QString("ORTHOGRAPHIC"));
	 
 }

  void Proj4Transform::initGeoMap()
  {
	  
	  if(!Geo_map.isEmpty()) return;

	  Geo_map.insert(QString("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs"),QString("WGS84"));
	  Geo_map.insert(QString("+proj=longlat +a=6378140 +b=6356755.288157528 +no_defs"),QString("XIAN80"));
	  Geo_map.insert(QString("+proj=cea +lon_0=0 +lat_ts=30 +x_0=0 +y_0=0 +a=6371228 +b=6371228 +units=m +no_defs"),QString("LONGLAT"));
	  
  }

  
