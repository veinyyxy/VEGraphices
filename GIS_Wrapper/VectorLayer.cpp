#include "Polygon.h"
#include "VectorLayer.h"
#include "Layer.h"
#include <QtCore/QTime>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
//#include <QtCore/QTextCodec>
//#include <CDataElement>
//#include "cdataprovidermanager.h"
#include "GeoTo3DTransform.h"
//#ifdef _DEBUG
//#include "vld.h"
//#endif

VectorLayer::VectorLayer(DataProvider* data, CoordinateTransform* ct) : Layer(data, ct)
{
	//layerRoot = new QFork;
	renderVol = new QRenderVolume;
	QObject::connect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(FindedUnit(GLenum, QRenderable*)),Qt::DirectConnection);
	m_pProfileLine = new Line;
	m_bStart = false;
	m_Proj4Trans = new Proj4Transform;
	isTrans = true;
}

VectorLayer::~VectorLayer(void)
{
	if(NULL != m_pProfileLine)
	{
		delete m_pProfileLine;
		m_pProfileLine = NULL;
	}

	if(NULL != m_Proj4Trans)
	{
		delete m_Proj4Trans;
		m_Proj4Trans = NULL;
	}
}

int VectorLayer::CreateLayer( const QString& filename )
{
	int iR = 0;
	renderVol->AttribSet(*m_nAttributeSet);
	openFileNameList.append(filename);

	if(!dataProvider) return false;

	dataProvider->InitDataProvider();
	dataProvider->OpenFile(filename);
	QString oSourceProj = dataProvider->GetProInfo();
	if(oSourceProj.size() != 0)
	{
		coordTrans->setSourceCRS(oSourceProj);
		dataProvider->addProj(coordTrans);
	}
//	QString des = coordTrans->getDesCRS();

	VECTOR_DATA_TYPE dataType = dataProvider->GetDataType();

	switch(dataType)
	{
	case VECTOR_POINT:
		dataProvider->ParentLayer(this);
		dataProvider->DumpPoint(renderVol);
		break;
	case VECTOR_LINE:
		dataProvider->ParentLayer(this);
		dataProvider->DumpLine(renderVol);
		break;
	case VECTOR_POLYGON:
		{
			QTime startTime = QTime::currentTime();
			dataProvider->ParentLayer(this);
			dataProvider->DumpPolygon(renderVol);
			startTime = QTime::currentTime();
		}
		break;
	default:
		dataProvider->ParentLayer(this);
		dataProvider->DumpAll(renderVol);
		break;
	}
	iR = layerRoot->InsertRenderable(renderVol);
	/*QLeaf* leaf = new QLeaf;
	leaf->InsertRenderable(renderVol);
	iR = layerRoot->InsertChild(leaf);*/
	return iR;
}

int VectorLayer::CreateLayer(QNode* node)
{
	layerRoot->InsertChild((QNode*)node);
	return 0;
}

int VectorLayer::CreateLayer( const QStringList& namelist )
{
	openFileNameList = namelist;
	ReloadLayer();
	return true;
}

int VectorLayer::InitLayer()
{
	return 0;
}

void VectorLayer::MountLayer( QNode* root )
{
}

int VectorLayer::ReloadLayer()
{
	if(!layerRoot)
		layerRoot = new QFork;

	QStringListIterator iter(openFileNameList);
	while(iter.hasNext())
	{
		CreateLayer(iter.next());
	}
	return true;
}

QString VectorLayer::GetProInfo( void )
{
	return dataProvider->GetProInfo();
}

void VectorLayer::Clear()
{
	//layerRoot->Clear();
}

void VectorLayer::TransformLayer()
{
	//renderVol->FindDataUseSignal(GL_VERTEX_ARRAY);
	renderVol->FindUnitUseSignal();

//	dataProvider->addProj(coordTrans);
}

void VectorLayer::FindedData( QDataArray* data )
{
	if(!data) return;
	GLint dimen = data->ArrayDimension();
	if( dimen != 3) return;
	GLint arrayLen = data->getDataCount();
	GLdouble* pdData;
	GLfloat* pfData;

	switch(data->DataType())
	{
	case GL_DOUBLE:
		pdData = (GLdouble*)(data->getDataPointer());
		for(int i = 0; i < arrayLen; i++)
		{
			QVector3D temp(pdData[i * 3 + 0], pdData[i * 3 + 1], pdData[i * 3 + 2]), res;
			res = coordTrans->Transform(temp);
			pdData[i * 3 + 0] = res.x();
			pdData[i * 3 + 1] = res.y();
			pdData[i * 3 + 2] = res.z();
		}
		break;
	case GL_FLOAT:
		pfData = (GLfloat*)(data->getDataPointer());
		for(int i = 0; i < arrayLen; i++)
		{
			QVector3D temp(pfData[i * 3 + 0], pfData[i * 3 + 1], pfData[i * 3 + 2]), res;
			res = coordTrans->Transform(temp);
			pfData[i * 3 + 0] = res.x();
			pfData[i * 3 + 1] = res.y();
			pfData[i * 3 + 2] = res.z();
		}
		break;
	}
}

QRenderable* VectorLayer::GetLayerData()
{
	return renderVol;
}

void VectorLayer::FindedUnit( GLenum type, QRenderable* data )
{
#if 1
	//QVector<int> delIndex;
	QRenderUnit* unit = data->AsRenderUnit();
	if (!isTrans)
	{
		return;
	}
	if(!unit) return;
	if(unit->CallListIsDirty())
		unit->CallListDirty(false);
	unit->NeedCalculateA();					//重新计算角度

	GISPolygon* polygon = dynamic_cast<GISPolygon*>(unit);

	QDataArray* unitData = unit->VertexArray();
	QVector3DArray * OriginalArray = unit->OriginalArray();   //取得原始数据

	unit->SetRenderNodeMask(true);

	if(!unitData || !coordTrans) return;

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
			QVector3D temp(pdData[i * 3 + 0], pdData[i * 3 + 1], pdData[i * 3 + 2]), res;
			res = coordTrans->Transform(temp);
			pdData[i * 3 + 0] = res.x();
			pdData[i * 3 + 1] = res.y();
			pdData[i * 3 + 2] = res.z();
		}
		break;
	case GL_FLOAT:
		pfData = (GLfloat*)(unitData->getDataPointer());

		for(int i = 0; i < arrayLen; i++)
		{

			QVector3D temp(OriginalArray->at(i).x(), OriginalArray->at(i).y(), /*OriginalArray->at(i).z()+*/LayerZ()), res;
			//QVector3D temp1(pfData[i * 3 + 0], pfData[i * 3 + 1], /*pfData[i * 3 + 2]*/LayerZ());
			GeoTo3DTransform* p3d = dynamic_cast<GeoTo3DTransform*>(coordTrans);

			if(p3d)
			{
				temp.setZ(OriginalArray->at(i).z());
			}
			if(!(coordTrans->Transform(temp, res)))
			{
				pfData[i * 3 + 0] = res.x();
				pfData[i * 3 + 1] = res.y();
				pfData[i * 3 + 2] = res.z();
			}
			else
			{
				qDebug()<<"faile transform";
			}

		}
		break;
	}
#else
	//QVector<int> delIndex;
	QRenderUnit* unit = data->AsRenderUnit();
	if (!isTrans)
	{
		return;
	}
	if(!unit) return;
	if(unit->CallListIsDirty())
		unit->CallListDirty(false);
	unit->NeedCalculateA();					//重新计算角度

	//GISPolygon* polygon = dynamic_cast<GISPolygon*>(unit);

	QDataArray* unitData = unit->VertexArray();
	QVector3DArray * OriginalArray = unit->OriginalArray();   //取得原始数据

	QString des = coordTrans->getDesProjName(); //获取目标投影名称
	unit->SetRenderNodeMask(true);

	if(!unitData || !coordTrans) return;

	GLint dimen = unitData->ArrayDimension();
	if( dimen != 3) return;
	GLint arrayLen = unitData->getDataCount();
	GLdouble* pdData;
	GLfloat* pfData;

	switch(unitData->DataType())
	{
	case GL_DOUBLE:
		pdData = (GLdouble*)(unitData->getDataPointer());
		for(int i = 0; i < arrayLen; i++)
		{
			QVector3D temp(pdData[i * 3 + 0], pdData[i * 3 + 1], pdData[i * 3 + 2]), res;
			res = coordTrans->Transform(temp);
			pdData[i * 3 + 0] = res.x();
			pdData[i * 3 + 1] = res.y();
			pdData[i * 3 + 2] = res.z();
		}
		break;
	case GL_FLOAT:
		pfData = (GLfloat*)(unitData->getDataPointer());

		for(int i = 0; i < arrayLen; i++)
		{
			//QVector3D temp(OriginalArray->at(i).x(), OriginalArray->at(i).y(), /*OriginalArray->at(i).z()+*/LayerZ()), res;
			QVector3D temp1(OriginalArray->at(i).x(), OriginalArray->at(i).y(), /*OriginalArray->at(i).z()+*/LayerZ());
			QVector3D temp(pfData[i * 3 + 0], pfData[i * 3 + 1], /*pfData[i * 3 + 2]*/LayerZ()), res;
			
			if(!(coordTrans->Transform(temp, res)))
			{
				pfData[i * 3 + 0] = res.x();
				pfData[i * 3 + 1] = res.y();
				pfData[i * 3 + 2] = res.z();
			}
			else
			{
				qDebug()<<"faile transform";
			}
			temp.setX(pfData[i * 3 + 0]);temp.setY(pfData[i * 3 + 1]), temp.setZ(pfData[i * 3 + 2]);
			temp.setX(pfData[i * 3 + 0]);temp.setY(pfData[i * 3 + 1]), temp.setZ(pfData[i * 3 + 2]);
		}
		break;
	}
#endif
}

void VectorLayer::operator=( VectorLayer &l )
{
	this->layerRoot = l.layerRoot;
	this->layerName = l.layerName;
	this->layerZ = l.layerZ;
	this->minScale = l.minScale;
	this->maxScale = l.maxScale;
	this->coordTrans = l.coordTrans;
	this->dataProvider = l.dataProvider;
	this->renderVol = l.renderVol;
	this->openFileNameList = l.openFileNameList;
}

void VectorLayer::PickUnit( GLenum type, QRenderable* data )
{
	//QRenderUnit* unit = data->AsRenderUnit();
	/*if(!unit)
		return;

	if(unit->PickUp(&pickPoint))
		m_pickUpObject = data;*/

	if(data->PickUp(&pickPoint))
		m_pickUpObject = data;
}

QRenderable* VectorLayer::PickUp( const QVector3D& point )
{
	m_pickUpObject = 0;
	pickPoint = point;
	bool res1(false), res2(false);
	res1 = QObject::disconnect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(FindedUnit(GLenum, QRenderable*)));
	res2 = QObject::connect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(PickUnit(GLenum, QRenderable*)),Qt::DirectConnection);
	renderVol->FindUnitUseSignal();
	res1 = QObject::disconnect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(PickUnit(GLenum, QRenderable*)));
	QObject::connect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(FindedUnit(GLenum, QRenderable*)),Qt::DirectConnection);

	return m_pickUpObject;
}

/************************************************************************/
/* 函数功能： 做剖面所需要的动作										*/
/* 参数说明:  剖面线第一个点											*/
/* 返 回 值:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void VectorLayer::StartProfile(QVector3D & point)
{
	QRenderable* pickObj = 0;
	int iIndex = -1;
	QVector3D glPoint(point.x(),point.y(),0),scPoint ;
	ogTOs(glPoint,scPoint);
	if (this->LayerType() == 4)
	{
		pickObj = this->PickUp(scPoint);
		if(pickObj)
		{
			QBitmapText* pSp = dynamic_cast<QBitmapText*>(pickObj);
			if(NULL != pSp)
			{
				iIndex = pSp->GetPickIndex();
				StationValues pValues = pSp->StationValueArray()->at(iIndex);
				emit IdentiferEnd(&pValues) ;
			}
		}
	}

// 	QVector3DArray * pArray = m_pProfileLine->getVectexArray();
// 	QVector4DArray * pColor = m_pProfileLine->getColorArray();
// 	pArray->Clear();
// 	pColor->Clear();
// 	pArray->push_back(point);
// 	pColor->push_back(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
// 	m_IniPoint = point;
// 	if(!m_bStart)
// 	{
// 		m_pProfileLine->SetColorArray();
// 		renderVol->InsertRenderable(m_pProfileLine);
// 		m_bStart = true;
// 	}
// 	emit  VectorMouse_Down(TransformData(point));

// 	QVector3DArray * pArray = m_pProfileLine->getVectexArray();
// 	QVector4DArray * pColor = m_pProfileLine->getColorArray();
// 	pArray->Clear();
// 	pColor->Clear();
// 	pArray->push_back(point);
// 	pColor->push_back(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
// 	m_IniPoint = point;
// 	if(!m_bStart)
// 	{
// 		m_pProfileLine->SetColorArray();
// 		renderVol->InsertRenderable(m_pProfileLine);
// 		m_bStart = true;
// 	}
// 	emit  VectorMouse_Down(TransformData(point));
}

/************************************************************************/
/* 函数功能： 做剖面所需要的动作										*/
/* 参数说明:  剖面线的最后一个点										*/
/* 返 回 值:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void VectorLayer::ProcessProfile(QVector3D & point)
{
// 	if(m_bStart)
// 	{
// 		QVector3DArray * pArray = m_pProfileLine->getVectexArray();
// 		QVector4DArray * pColor = m_pProfileLine->getColorArray();
//
// 		pArray->Clear();
//
// 		pColor->push_back(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
// 		pArray->push_back(m_IniPoint);
// 		pColor->push_back(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
// 		pArray->push_back(point);
// 	}
//

// 	if(m_bStart)
// 	{
// 		QVector3DArray * pArray = m_pProfileLine->getVectexArray();
// 		QVector4DArray * pColor = m_pProfileLine->getColorArray();
//
// 		pArray->Clear();
//
// 		pColor->push_back(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
// 		pArray->push_back(m_IniPoint);
// 		pColor->push_back(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
// 		pArray->push_back(point);
// 	}
}

/************************************************************************/
/* 函数功能： 做剖面所需要的动作										*/
/* 参数说明:  结事剖面线最后一个点										*/
/* 返 回 值:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
void VectorLayer::EndProfile(QVector3D & point)
{
// 	renderVol->RemoveRenderable(m_pProfileLine);
//
// 	m_bStart = false;
// 	SetProfile(false);						//剖面处理结事将该层设为正常

/*	emit  VectorMouse_Up(TransformData(point));*/
}

/************************************************************************/
/* 函数功能： 将当前投影坐标转换为经纬坐标								*/
/* 参数说明:  结事剖面线最后一个点										*/
/* 返 回 值:  NULL														*/
/* 作    者:  lugsh														*/
/* 日    期:  2011 年 04 月 07 日										*/
/************************************************************************/
QVector3D VectorLayer::TransformData(QVector3D & glPostion)
{
	QSettings setting("ProjectionType.ini", QSettings::IniFormat);
    //setting.setIniCodec(QTextCodec::codecForName("UTF-8"));

	m_Proj4Trans->setSourceCRS(coordTrans->getDesCRS());
	m_Proj4Trans->setDestCRS(setting.value("/GeographicCoordinate_CONFIG/WGS84").toString());

	return ( m_Proj4Trans->Transform(glPostion));
}

void VectorLayer::sTOog( const QVector3D& src, QVector3D& des )
{
	GLint    viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ = 0.0f;
	GLdouble posX, posY, posZ;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	winX = src.x();
	winY = viewport[3] - src.y();
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(src.x(), src.y(), src.z(), modelview, projection, viewport, &posX, &posY, &posZ);
	des.setX(posX);
	des.setY(posY);
	des.setZ(0.0);
}

void VectorLayer::ogTOs( const QVector3D& src, QVector3D& des )
{
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
	gluProject(src.x(), src.y(), src.z(), modelview, projection, viewport, &x, &y, &z);
	des.setX(x);
	des.setY(y);
	des.setZ(z);
}

void VectorLayer::SaveUnit( GLenum type, QRenderable* data )
{

}

void VectorLayer::SaveToShape()
{
	bool res1(false), res2(false);
	res1 = QObject::disconnect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(FindedUnit(GLenum, QRenderable*)));
	res2 = QObject::connect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(SaveUnit(GLenum, QRenderable*)),Qt::DirectConnection);
	renderVol->FindUnitUseSignal();
	res1 = QObject::disconnect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(SaveUnit(GLenum, QRenderable*)));
	QObject::connect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(FindedUnit(GLenum, QRenderable*)),Qt::DirectConnection);

}

void VectorLayer::DumpGraphicsAttribute(GEOMETRY_SET& resultSet)
{
	bool res1(false), res2(false);
	res1 = QObject::disconnect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(FindedUnit(GLenum, QRenderable*)));
	res2 = QObject::connect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(DumpUnitAttribute(GLenum, QRenderable*)),Qt::DirectConnection);
	renderVol->FindUnitUseSignal();
	resultSet = m_tempGeometrySet;
	m_tempGeometrySet.clear();
	res1 = QObject::disconnect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(DumpUnitAttribute(GLenum, QRenderable*)));
	QObject::connect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(FindedUnit(GLenum, QRenderable*)),Qt::DirectConnection);
}

void VectorLayer::DumpUnitAttribute( GLenum type, QRenderable* data )
{
	QRenderUnit* unit = data->AsRenderUnit();
	if(!unit) return;

	//Geometry* pgeo = dynamic_cast<Geometry*>(unit);

	if(unit)
	{
		int iID = unit->GetID();
		GEOMETRY_SET::iterator findResult = m_tempGeometrySet.find(iID);
		if(findResult == m_tempGeometrySet.end())
		{
			QVector<QRenderUnit*> GeoSet;
			GeoSet.push_back(unit);
			m_tempGeometrySet.insert(iID, GeoSet);
		}
		else
		{
			findResult.value().push_back(unit);
		}

	}
}
