#include "GraticuleCoordinates.h"
#include "GL_Include.h"
#include "QBitmapText.h"
#include "ColorAttribute.h"
#include "EarthSphere.h"
#include "ImageTexture.h"
#include "Const.h"
#include "TextureAttribute.h"
#include "RadProjTransform.h"
#include "GeoTo3DTransform.h"
#define DEL_CODE 1
#define M_PI (3.14159265358979323846)

QGraticuleCoordinates::QGraticuleCoordinates(DataProvider* data, CoordinateTransform* ct) : Layer(data, ct)
	, m_constantInterval(false)
	, pLongLineData(0)
	, pLatLineData(0)
	//, pAllAnntation(0)
{
	root = new QObject;
	renderVol = new QRenderVolume;
	QObject::connect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(FindedUnit(GLenum, QRenderable*)));
	m_LongLatRect.left_top.first = (-70.0);
	m_LongLatRect.left_top.second = (89.9);
	m_LongLatRect.right_top.first = (-70.0004);
	m_LongLatRect.right_top.second = (89.9);
	m_LongLatRect.left_bottom.first = (-70.0);
	m_LongLatRect.left_bottom.second = (-89.9);
	m_LongLatRect.right_bottom.first = (-70.0004);
	m_LongLatRect.right_bottom.second = (-89.9);
	m_LongLatLineData.latitude_line_interval = 10.0;
	m_LongLatLineData.longitude_line_interval = 10.0;

	m_colorAttribute = new ColorAttribute();
}

QGraticuleCoordinates::~QGraticuleCoordinates(void)
{
	/*if(renderVol)
	{
		delete renderVol;
		renderVol = NULL;
	}*/
	if (root)
	{
		delete root;
		root = NULL;
	}
	if (m_colorAttribute)
	{
		delete m_colorAttribute;
		m_colorAttribute = NULL;
	}
}

int QGraticuleCoordinates::CreateLayer( const QString& filename )
{
	return false;
}
/*************************************************
函数名：CreatLayer
函数功能：创建层并绘制经纬线和坐标
完成日期：2010年1月
作者： 陈志学
**************************************************/
int QGraticuleCoordinates::CreateLayer(QNode* node)
{
	renderVol->AttribSet(*m_nAttributeSet);
	int i = 0;
	if(!coordTrans || !node)
		return false;
	//OldMakeLongLatLine();
	//MakeLongLatLineData();
	//MakeLongLatLineStartAndEnd();
	//MakeLongLatLine();
	
	/*EarthSphere* pEarth2 = new EarthSphere;
	pEarth2->SetColorBlend(true);
	pEarth2->InitShpere(6450, 30, QColor(255, 255, 255, 150));*/
#if 0
	//地壳
	EarthSphere* pEarth = new EarthSphere;
	//pEarth->EnableTextrue(true);
	pEarth->SetColorBlend(false);
	pEarth->InitShpere(6378, 30, QColor(255, 255, 255, 255));
#endif
#if 1
	//内核
	EarthSphere* pEarthCore = new EarthSphere;
	pEarthCore->SetColorBlend(false);
	pEarthCore->InitShpere(1228, 30, QColor(255, 245, 0, 255));
	//外核
	EarthSphere* pEarthCore1 = new EarthSphere;
	pEarthCore->SetColorBlend(false);
	pEarthCore->InitShpere(3488, 30, QColor(249, 165, 9, 255));
	//D层
	EarthSphere* pEarthDLayer = new EarthSphere;
	pEarthCore->SetColorBlend(false);
	pEarthCore->InitShpere(3678, 30, QColor(151, 216, 41, 255));
	//下地幔
	EarthSphere* pEarthMesosphere = new EarthSphere;
	pEarthCore->SetColorBlend(false);
	pEarthCore->InitShpere(5728, 30, QColor(78, 121, 180, 255));
	//过渡区
	EarthSphere* pEarthTransitionRagion = new EarthSphere;
	pEarthCore->SetColorBlend(false);
	pEarthCore->InitShpere(5978, 30, QColor(108, 205, 152, 255));
	//上地幔
	EarthSphere* pEarthUpperMantle = new EarthSphere;
	pEarthCore->SetColorBlend(false);
	pEarthCore->InitShpere(6338, 30, QColor(77, 77, 77, 255));
	//pEarth->SetBakData();
	
	renderVol->InsertRenderable(pEarthCore);
	renderVol->InsertRenderable(pEarthCore1);
	renderVol->InsertRenderable(pEarthDLayer);
	renderVol->InsertRenderable(pEarthMesosphere);
	renderVol->InsertRenderable(pEarthTransitionRagion);
	renderVol->InsertRenderable(pEarthUpperMantle);
#endif
#if 0
	renderVol->InsertRenderable(pEarth);
	//setColor(QVector4D(1.0, 1.0, 1.0, 0.0));

	ImageTexture* pImageTextrue = new ImageTexture;
	pImageTextrue->SetGLGLWidget(m_pOpenglView);
	QImage* pLocalImage = new QImage(CIPASApplication::ConfigPath() + "/land_shallow_topo_2048.jpg");
	pImageTextrue->TargetTextrue(GL_TEXTURE_2D);
	//pImageTextrue->LoadTextrueFromQimage((const QImage*)pImageTextrue);
	pImageTextrue->SetImage(pLocalImage);
	
	TextureAttribute* pTextrueAttribute = new TextureAttribute;
	pTextrueAttribute->SetTexture(pImageTextrue);
	
	m_nAttributeSet->insert(QE_ATTRI_TEXTURE0, pTextrueAttribute);
	
	renderVol->AttribSet(*m_nAttributeSet);
#endif
	layerRoot->InsertRenderable(renderVol);
	return true;
}

int QGraticuleCoordinates::CreateLayer( const QStringList& namelist )
{
	return false;
}

int QGraticuleCoordinates::CreateLayer( double X,double Y,double h,double R,double Res )
{
	int c_resolution = 1000/Res;
	int foot = 50*c_resolution*1000;
	int numofcircle = (int)((R*1000/foot)/Res);
	//画圆
	for (int i = 1 ;i <numofcircle+1; i++)
	{
		Line *ln = new Line;
		QVector3DArray* pVectex = ln->getVectexArray();
		QVector4DArray* pColor = ln->getColorArray();
		QVector3DArray* ptemp = DrawCircle(X,Y,h,foot*i);
		for (int m = 0 ;m<ptemp->count();m++)
		{
			pColor->push_back(QVector4D(0.96, 0.0, 0.08, 255));
			pVectex->push_back(ptemp->at(m));
		}
		ln->Mode(GL_LINES);
		//ln->SetLineStyle(1,1,0x0104);
		//ln->SetBakData();
		//ln->SetPrepareTransform(true);
		ln->SetColorArray();
		renderVol->InsertRenderableNotDel(ln);

	}
	if (int(R*1000/Res) % foot)
	{
		Line *ln = new Line;
		QVector3DArray* pVectex = ln->getVectexArray();
		QVector4DArray* pColor = ln->getColorArray();
		QVector3DArray* ptemp = DrawCircle(X,Y,h,R*1000/Res);
		for (int m = 0 ;m<ptemp->count();m++)
		{
			pColor->push_back(QVector4D(0.96, 0.35, 0.08, 255));
			pVectex->push_back(ptemp->at(m));
		}
		ln->Mode(GL_LINES);
		//ln->SetBakData();
	//	ln->SetPrepareTransform(true);
		ln->SetColorArray();
		renderVol->InsertRenderableNotDel(ln);
	}
	//画线
	for (int j = 0 ;j < 6;j++)
	{
		Line *line = new Line;
		QVector3DArray* pPoint = line->getVectexArray();
		QVector4DArray* pColor = line->getColorArray();
		// 	pPoint->push_back(QVector3D((R*cos(30*j*PI/180)),( R*sin(30*j*PI/180)),h));
		// 	pPoint->push_back(QVector3D((- R*cos(30*j*PI/180)),(- R*sin(30*j*PI/180)),h));
		// 	pColor->push_back(QVector4D(1.0, 0, 0, 255));
		// 	pColor->push_back(QVector4D(1.0, 0, 0, 255));
		double x1 = (R*1000.0/Res) *cos(30*j*PI/180)/60;
		double y1 = (R*1000.0/Res)*sin(30*j*PI/180)/60;
		double x2 = -(R*1000.0/Res)*cos(30*j*PI/180)/60;
		double y2 = -(R*1000.0/Res)*sin(30*j*PI/180)/60;
		double tempx,tempy;
		tempx = x1*60;tempy = y1*60;
		for(int i = 1;i<60;i++)
		{
			pPoint->push_back(QVector3D((tempx),(tempy),h));
			pColor->push_back(QVector4D(0.96, 0.35, 0.08, 255));
			tempx = tempx-x1;
			tempy = tempy-y1;
		}
		tempx = x2;tempy = y2;
		for (int i = 1;i<60;i++)
		{
			pPoint->push_back(QVector3D((tempx),(tempy),h));
			pColor->push_back(QVector4D(0.96, 0.35, 0.08, 255));
			tempx = tempx+x2;
			tempy = tempy+y2;
		}
		//line->SetBakData();
		line->Mode(GL_LINES);
		//line->SetPrepareTransform(true);
		line->SetColorArray();
		renderVol->InsertRenderableNotDel(line);
	}
	return layerRoot->InsertRenderable(renderVol);
}

int QGraticuleCoordinates::ReloadLayer()
{
	return false;
}

QString QGraticuleCoordinates::GetProInfo( void )
{
	return "";
}

void QGraticuleCoordinates::Clear( void )
{

}

void QGraticuleCoordinates::TransformLayer()
{
	renderVol->FindUnitUseSignal();
}


QRenderable* QGraticuleCoordinates::GetLayerData()
{
	return renderVol;
}

QRenderable* QGraticuleCoordinates::GetMap()
{
	return renderVol;
}

void QGraticuleCoordinates::FindedUnit( GLenum type, QRenderable* data )
{
	QRenderUnit* unit = data->AsRenderUnit();

	if(!unit) return;
	if(unit->CallListIsDirty())
		unit->CallListDirty(false);
	unit->NeedCalculateA();					//重新计算角度

	//GISPolygon* polygon = dynamic_cast<GISPolygon*>(unit);

	QDataArray* unitData = unit->VertexArray();
	QVector3DArray * OriginalArray = dynamic_cast<QVector3DArray *>(unit->VertexArray()) /*OriginalArray()*/;   //取得原始数据
	GLint dimen = OriginalArray->ArrayDimension();
	if( dimen != 3) return;
	GLint arrayLen = OriginalArray->getDataCount();
	GLdouble* pdData;
	GLfloat* pfData;
	switch(unitData->DataType())
	{
	case GL_DOUBLE:
		pdData = (GLdouble*)(unitData->getDataPointer());
		for(int i = 0; i < arrayLen; i++)
		{
			pdData[i * 3 + 2] = OriginalArray->at(i).z()+LayerZ();
		}
		break;
	case GL_FLOAT:
		pfData = (GLfloat*)(unitData->getDataPointer());

		for(int i = 0; i < arrayLen; i++)
		{
			float zzz = LayerZ();
			pfData[i * 3 + 2] = /*OriginalArray->at(i).z()+*/LayerZ();
		}
		break;
	}
	//QRenderUnit* unit = data->AsRenderUnit();

	//EarthSphere* pES = dynamic_cast<EarthSphere*>(unit);
	//if(pES)
	//	return;

	//if(!unit) return;
	//if(unit->CallListIsDirty())
	//	unit->CallListDirty(false);

	//QDataArray* unitData = unit->VertexArray();
	//QVector3DArray * OriginalArray = unit->OriginalArray();   //取得原始数据

	//if(!unitData || !coordTrans) return;

	//GLint dimen = unitData->ArrayDimension();
	//if( dimen != 3) return;
	//GLint arrayLen = unitData->getDataCount();
	//GLdouble* pdData;
	//GLfloat* pfData;

	//switch(unitData->DataType())
	//{
	//case GL_DOUBLE:
	//	pdData = (GLdouble*)(unitData->getDataPointer());
	//	for(int i = 0; i < arrayLen; i++)
	//	{
	//		QVector3D temp(pdData[i * 3 + 0], pdData[i * 3 + 1], pdData[i * 3 + 2]), res;
	//		res = coordTrans->Transform(temp);
	//		pdData[i * 3 + 0] = res.x();
	//		pdData[i * 3 + 1] = res.y();
	//		pdData[i * 3 + 2] = LayerZ();
	//	}
	//	break;
	//case GL_FLOAT:
	//	pfData = (GLfloat*)(unitData->getDataPointer());

	//	for(int i = 0; i < arrayLen; i++)
	//	{
	//		QVector3D temp(OriginalArray->at(i).x(), OriginalArray->at(i).y(), /*OriginalArray->at(i).z()+*/LayerZ()), res;
	//		GeoTo3DTransform* p3d = dynamic_cast<GeoTo3DTransform*>(coordTrans);
	//		int iGraphType = GraphicsType();
	//		if(p3d && iGraphType >0 )
	//		{
	//			qreal rz = temp.z();
	//			rz = (rz + 2) * 1000;//abs(rz)
	//			temp.setZ(rz);
	//		}
	//		if(!(coordTrans->Transform(temp, res)))
	//		{
	//			pfData[i * 3 + 0] = res.x();
	//			pfData[i * 3 + 1] = res.y();
	//			pfData[i * 3 + 2] = res.z();
	//		}
	//		else
	//		{
	//			qDebug()<<"faile transform";
	//		}

	//	}
	//	break;
	//}
}

void QGraticuleCoordinates::MakeLongLatLine()
{
#if !DEL_CODE
	bool bFirstLoop = true;
	int ii = 0;
	qreal longValue = 0, latValue = 0, Zvalue = 0;
	qreal qrInterval = 1.0;//m_LongLatLineData.longitude_line_interval;
	int LineNum = m_LongLatLineData.longitude_line.count();
	Line* pAllLongLine = new Line[LineNum];
	pLongLineData = pAllLongLine;
	//计算经度线
	for(LONG_LAT_LINE_MAP::iterator i = m_LongLatLineData.longitude_line.begin(); i != m_LongLatLineData.longitude_line.end(); i++)
	{
		Zvalue = LayerZ();
		LONG_LAT_LINE line = i.key();
		m_LongLatLineData.longitude_line[line] = pAllLongLine + ii;
		LONG_LAT_POINT startPoint = line.first;
		LONG_LAT_POINT endPoint = line.second;
		//////////////////debug//////////////////
		//qDebug()<<startPoint.first<<", "<<startPoint.second<<"       "<<endPoint.first<<", "<<endPoint.second;
		////////////////////////////////////////////
		longValue = startPoint.first;
		qreal length = endPoint.second - startPoint.second;
		qreal incLineNum = abs(length / qrInterval);
		QVector3DArray* pVectex = (pAllLongLine + ii)->getVectexArray();
		pVectex->push_back(QVector3D(startPoint.first, startPoint.second, Zvalue));
		qreal startValue = 0;
		if(m_constantInterval)
		{
			for(int ip = 0; 0.000001 < abs(length / incLineNum) - ip; ip++)
			{
				latValue = startPoint.second + incLineNum;
				pVectex->push_back(QVector3D(longValue, latValue, Zvalue));
			}
		}
		else
		{
			latValue = startPoint.second;
			int count = abs((endPoint.second - startPoint.second)/qrInterval) - 1;
			while(0 < count--)
			{
				latValue -= qrInterval;
				pVectex->push_back(QVector3D(longValue, latValue, Zvalue));
			}
		}
		pVectex->push_back(QVector3D(endPoint.first, endPoint.second, Zvalue));
		//renderVol->InsertRenderableNotDel(pAllLongLine + ii);
		(pAllLongLine + ii)->SetBakData();
		ii++;
	}
	//////////////////debug//////////////////
	//qDebug()<<"---------------------------------------------------";
	////////////////////////////////////////////
	ii = 0;
	LineNum = m_LongLatLineData.latitude_line.count();
	qrInterval = 1.0;//m_LongLatLineData.latitude_line_interval;
	Line* pAllLatLine = new Line[LineNum];
	pLatLineData = pAllLatLine;
	latValue = 0.0;
	longValue = 0.0;
	//计算纬度线
	for(LONG_LAT_LINE_MAP::iterator i = m_LongLatLineData.latitude_line.begin(); i != m_LongLatLineData.latitude_line.end(); i++)
	{
		LONG_LAT_LINE line = i.key();
		m_LongLatLineData.latitude_line[line] = pAllLatLine + ii;
		LONG_LAT_POINT startPoint = line.first;
		LONG_LAT_POINT endPoint = line.second;
		//////////////////debug//////////////////
		//qDebug()<<startPoint.first<<", "<<startPoint.second<<"       "<<endPoint.first<<", "<<endPoint.second;
		////////////////////////////////////////////
		latValue = startPoint.second;
		qreal length = endPoint.first - startPoint.first;
		qreal incLineNum = length / qrInterval;
		QVector3DArray* pVectex = (pAllLatLine + ii)->getVectexArray();
		pVectex->push_back(QVector3D(startPoint.first, startPoint.second, Zvalue));
		if(m_constantInterval)
		{
			for(int ip = 0; 0.000001 < ip - abs(length / incLineNum); ip++)
			{
				longValue = startPoint.first + incLineNum;
				pVectex->push_back(QVector3D(longValue, latValue, Zvalue));
			}
		}
		else
		{
			longValue = startPoint.first;
			int count = abs((endPoint.first - startPoint.first)/qrInterval) - 1;
			while(longValue >= startPoint.first/*-70*/ && longValue < 180)
			{
				longValue += qrInterval;
				//qDebug()<<longValue;
				pVectex->push_back(QVector3D(longValue, latValue, Zvalue));
				if(longValue == 180) longValue = -longValue;
			}
			while(longValue >= -180 && longValue <= endPoint.first)
			{
				longValue += qrInterval;
				pVectex->push_back(QVector3D(longValue, latValue, Zvalue));
				//qDebug()<<longValue;
			}
		}
		pVectex->push_back(QVector3D(endPoint.first, endPoint.second, Zvalue));
		//renderVol->InsertRenderableNotDel(pAllLatLine + ii);
		(pAllLatLine + ii)->SetBakData();
		ii++;
	}

	layerRoot->InsertRenderable(renderVol);
#endif
}

void QGraticuleCoordinates::MakeLongLineData(const GLdouble* pLineData, int iLongSize)
{
#if !DEL_CODE
	GLdouble* pLongPoint[2];
	*pLongPoint = (GLdouble*)pLineData;
	for(int i = 0; i<iLongSize;i++)	
	{
		Line* lon_line = new Line;
		QVector3DArray* tempArrayPoint = lon_line->getVectexArray();
		for(int j =0; j<iLongSize;j++)
		{
			QVector3D jwd(pLongPoint[i*iLongSize+j][0], pLongPoint[i*iLongSize+j][1], LayerZ());
			tempArrayPoint->push_back(jwd);
		}
		lon_line->SetBakData();
		//renderVol->InsertRenderable(lon_line);
	}
	/*for(int i = 0; iLongSize < i; i++)
	{
		Line* pLine = new Line;
		pLine->getVectexArray()->push_back();
		m_LongLatLineData.longitude_line.push_back(pLine);
	}*/
#endif
}

void QGraticuleCoordinates::MakeLatLineData( const GLdouble* pLineData, int iLatSize )
{
	
}

void QGraticuleCoordinates::MakeLongLatLineStartAndEnd()
{
#if !DEL_CODE
	qreal v1x, v1y, v2x, v2y;
	//计算垂直线的起点和终点
	//左区间线数量
	int top_bottomRegion, leftRegion, rightRegion;
	top_bottomRegion = abs((m_LongLatRect.right_bottom.second - m_LongLatRect.right_top.second) / m_LongLatLineData.latitude_line_interval);
	leftRegion = abs((180 - m_LongLatRect.left_top.first) / m_LongLatLineData.longitude_line_interval);
	rightRegion = abs((-180 - m_LongLatRect.right_top.first) / m_LongLatLineData.longitude_line_interval);

	if( top_bottomRegion < 0 || leftRegion < 0 || rightRegion < 0)
		return;

	m_LongLatLineData.longitude_line.insert(LONG_LAT_LINE(m_LongLatRect.left_top, m_LongLatRect.left_bottom), 0);
	v1x = m_LongLatRect.left_top.first;
	v1y = m_LongLatRect.left_top.second;
	v2x = m_LongLatRect.left_bottom.first;
	v2y = m_LongLatRect.left_bottom.second;
	while(leftRegion--)
	{
		v1x += m_LongLatLineData.longitude_line_interval;
		v2x = v1x;
		LONG_LAT_POINT startPoint(v1x, v1y);
		LONG_LAT_POINT endPoint(v2x, v2y);
		LONG_LAT_LINE qtline(startPoint, endPoint);
		m_LongLatLineData.longitude_line.insert(qtline, 0);
	}
	v1x = m_LongLatRect.left_bottom.first;
	while(rightRegion--)
	{
		v1x -= m_LongLatLineData.longitude_line_interval;
		v2x = v1x;
		LONG_LAT_POINT startPoint(v1x, v1y);
		LONG_LAT_POINT endPoint(v2x, v2y);
		LONG_LAT_LINE qtline(startPoint, endPoint);
		m_LongLatLineData.longitude_line.insert(qtline, 0);
	}
	m_LongLatLineData.longitude_line.insert(LONG_LAT_LINE(m_LongLatRect.right_top, m_LongLatRect.right_bottom), 0);

	//计算水平线的起点和终点
	v1x = m_LongLatRect.left_top.first;
	v1y = m_LongLatRect.left_top.second;
	v2x = m_LongLatRect.right_top.first;
	v2y = m_LongLatRect.right_top.second;
	m_LongLatLineData.latitude_line.insert(LONG_LAT_LINE(m_LongLatRect.left_top, m_LongLatRect.right_top), 0);
	while(top_bottomRegion--)
	{
		v1y -= m_LongLatLineData.latitude_line_interval;
		v2y = v1y;
		LONG_LAT_POINT startPoint(v1x, v1y);
		LONG_LAT_POINT endPoint(v2x, v2y);
		LONG_LAT_LINE qtline(startPoint, endPoint);
		m_LongLatLineData.latitude_line.insert(qtline, 0/*lon_line*/);
	}
	m_LongLatLineData.latitude_line.insert(LONG_LAT_LINE(m_LongLatRect.left_bottom, m_LongLatRect.right_bottom), 0);
#endif
}

/************************************************************************/
/* 绘制经纬网格线                                                       */
/* lugsh																*/
/************************************************************************/
void QGraticuleCoordinates::DrawGrid()
{
#if !DEL_CODE
	//计算绘制纬线的数量
	int latCount = 180 / m_LongLatLineData.latitude_line_interval;		//计算纬线数量
	int LonCount = 360 /m_LongLatLineData.longitude_line_interval;		//计算经线数量
	LonCount++;latCount++;
	Line* pAllLongLine = new Line[LonCount];
	pLongLineData = pAllLongLine;
	//绘制经线
	int StartValue = 0;
	for(int i = 0 ; i < LonCount ; i++)
	{

		Line* line = pLongLineData +i;
		QVector3DArray* pVectex = line->getVectexArray();

		//加上第一个点
		pVectex->push_back(QVector3D(StartValue,-89.9, 0));


		for( int j = -89 ; j <90 ; j++)
		{
			pVectex->push_back(QVector3D(StartValue,j, 0));
		}

		//增加最后一个点
		pVectex->push_back(QVector3D(StartValue,89.9, 0));

		
		//renderVol->InsertRenderableNotDel(line);
		line->SetBakData();

		//插入起始点
		LONG_LAT_POINT startPoint(StartValue, -89.9);
		LONG_LAT_POINT endPoint(StartValue, 89.9);
		LONG_LAT_LINE qtline(startPoint, endPoint);
		m_LongLatLineData.longitude_line.insert(qtline, line);

		StartValue = StartValue + m_LongLatLineData.longitude_line_interval;
		if (StartValue==360)
			StartValue=357;
	}


	//绘制纬度
	Line* pAllLatLine = new Line[latCount];
	pLatLineData = pAllLatLine;

	int StartLatValue = 0;

	//北纬
	for(int i = 0 ; i < latCount/2+1 ; i++)
	{
		if(StartLatValue >= 90)
		{
			StartLatValue = 89.9;
		}
		Line* line = pLatLineData +i;
		QVector3DArray* pVectex = line->getVectexArray();

		for( int j = 0 ; j <360 ; j++)
		{
			pVectex->push_back(QVector3D(j,StartLatValue, 0));
		}

		
		//插入起始点
		LONG_LAT_POINT startPoint(0, StartLatValue);
		LONG_LAT_POINT endPoint(360, StartLatValue);
		LONG_LAT_LINE qtline(startPoint, endPoint);
		m_LongLatLineData.latitude_line.insert(qtline, line);
		//renderVol->InsertRenderableNotDel(line);
		line->SetBakData();

		StartLatValue = StartLatValue + m_LongLatLineData.latitude_line_interval;
		if(StartLatValue > 90)
		{
			break;
		}

	}

	//南纬
 	StartLatValue = 0;
 	for(int i = latCount /2+1; i< latCount; i++)
 	{
 		StartLatValue = StartLatValue - m_LongLatLineData.latitude_line_interval;
 		if(StartLatValue <= -90)
 		{
 			StartLatValue = -89.9;
 		}
 		Line* line = pLatLineData +i;
 		QVector3DArray* pVectex = line->getVectexArray();
 
 		for( int j = 0 ; j <360 ; j++)
 		{
 			pVectex->push_back(QVector3D(j,StartLatValue, 0));
		}
		LONG_LAT_POINT startPoint(0, StartLatValue);
		LONG_LAT_POINT endPoint(360, StartLatValue);
		LONG_LAT_LINE qtline(startPoint, endPoint);
		m_LongLatLineData.latitude_line.insert(qtline, line);

 		//renderVol->InsertRenderableNotDel(line);
 		line->SetBakData();
 		if(StartLatValue < -90)
 		{
 			break;
 		}
 
 	}



	layerRoot->InsertRenderable(renderVol);
#endif
}
void QGraticuleCoordinates::SetGridLineColor( const LONG_LAT_LINE& line, QColor color )
{
#if !DEL_CODE
	LONG_LAT_LINE_MAP::iterator findedLong = m_LongLatLineData.longitude_line.find(line);
	LONG_LAT_LINE_MAP::iterator findedLat = m_LongLatLineData.latitude_line.find(line);
	QAttributeSet aset;
	Line* pline = 0;
	if(m_LongLatLineData.longitude_line.end() != findedLong)
	{
		pline = findedLong.value();
	}
	else if(m_LongLatLineData.latitude_line.end() != findedLat)
	{
		pline = findedLat.value();
	}
	else
		pline = 0;

	if(pline)
	{
		ColorAttribute* colorAttribute = new ColorAttribute;
		colorAttribute->Color(QVector4D(color.redF(), color.greenF(), color.blueF(), color.alphaF()));
		aset.insert(QE_ATTRI_COLOR, colorAttribute);
		pline->AttribSet(aset);
	}
#endif
}

void QGraticuleCoordinates::SetGridColor( QColor color )
{
#if !DEL_CODE
	QVector4D fdColor;
	fdColor.setX(color.redF());
	fdColor.setY(color.greenF());
	fdColor.setZ(color.blueF());
	fdColor.setW(color.alphaF());
	setColor(fdColor);
	/*QAttributeSet* pAttribSet = renderVol->AttribSet();
	if(pAttribSet)
	{
		m_colorAttribute->Color(fdColor);
		renderVol->AttribSet(m_gridAttribute);
		pAttribSet->insert(QE_ATTRI_COLOR, m_colorAttribute);
	}
	else
	{
		m_colorAttribute->Color(fdColor);
		renderVol->AttribSet(m_gridAttribute);
		m_gridAttribute.insert(QE_ATTRI_COLOR, m_colorAttribute);
		renderVol->AttribSet(m_gridAttribute);
	}*/
#endif
}

void QGraticuleCoordinates::clearLine( const LONG_LAT_LINE& line )
{
#if !DEL_CODE
	LONG_LAT_LINE_MAP::iterator findedLong = m_LongLatLineData.longitude_line.find(line);
	LONG_LAT_LINE_MAP::iterator findedLat = m_LongLatLineData.latitude_line.find(line);
	Line* pline = 0;
	if(m_LongLatLineData.longitude_line.end() != findedLong)
	{
		pline = findedLong.value();
	}
	else if(m_LongLatLineData.latitude_line.end() != findedLat)
	{
		pline = findedLat.value();
	}
	else
		pline = 0;

	if(pline)
	{
		renderVol->RemoveRenderable(pline);
		m_deleteLine.push_back(pline);
	}
#endif
}

void QGraticuleCoordinates::clearLine()
{
#if !DEL_CODE
	renderVol->ClearNotDel();
	if(pLongLineData) delete[] pLongLineData;
	if(pLatLineData) delete[] pLatLineData;
	if(pAllAnntation) delete pAllAnntation;
	pLongLineData = 0;
	pLatLineData = 0;
	pAllAnntation = 0;

	m_LongLatLineData.longitude_line.clear();
	m_LongLatLineData.latitude_line.clear();
	m_LongLatLineData.line_make.clear();
#endif
}

void QGraticuleCoordinates::MakeAnnotation(const QString& fontName, int iWidth, int iHeight)
{
#if !DEL_CODE
	qreal v1x(0), v1y(0), v2x(0), v2y(0);
	LONG_LAT_POINT startPoint(-70.0, 0);
	LONG_LAT_POINT endPoint(-70.0004, 0);
	v1y = v2y = endPoint.second;
	qreal qrZ = LayerZ();
	QTextureFont* allann = new QTextureFont;
	allann->SetPatchRendering(true);
	//allann->SetFont(fontName);
	allann->initFont(QObject::tr("宋体"), QString(""), iWidth, iHeight, 0);
	allann->SetColor(QVector4D(0,0,0,1));
	int top_bottomRegion, leftRegion, rightRegion;
	leftRegion = abs((180 - startPoint.first) / m_LongLatLineData.longitude_line_interval);
	rightRegion = abs((-180 - endPoint.first) / m_LongLatLineData.latitude_line_interval);
	if(leftRegion <= 0 || rightRegion <= 0)
		return;
	v1x = startPoint.first;
	allann->AddString(QString("%1").arg(v1x), QVector3D(v1x, v2y, qrZ));
	while(leftRegion--)
	{
		v1x += m_LongLatLineData.longitude_line_interval;
		allann->AddString(QString("%1").arg(v1x, 0), QVector3D(v1x, v2y, qrZ));
	}
	v1x = endPoint.first;
	while(rightRegion--)
	{
		v1x -= m_LongLatLineData.longitude_line_interval;
		QString aa = QString("%1").arg(v1x);
		allann->AddString(aa, QVector3D(v1x, v2y, qrZ));
	}
	allann->SetBakData();
	renderVol->InsertRenderableNotDel(allann);
	pAllAnntation = allann;
#endif
}

void QGraticuleCoordinates::clearAnnotation()
{
#if !DEL_CODE
	if(pAllAnntation)
	{
		renderVol->RemoveRenderable(pAllAnntation);
		delete pAllAnntation;
		pAllAnntation = 0;
	}
#endif
}

QVector3DArray * QGraticuleCoordinates::DrawCircle( double X,double Y,double h,double R )
{
	QVector3DArray *circlearray = new QVector3DArray;

	for (int i=1; i<=360; i++)
	{
		QVector3D circle;
		float angle = i-1;
		circle.setX(X+R * sin(M_PI / 180 * angle) );
		circle.setY(Y+R * cos(M_PI / 180 * angle) );
		circle.setZ(h);
		circlearray->push_back(circle);
	}
	return circlearray;
}
