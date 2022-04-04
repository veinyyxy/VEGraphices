/********************************************************************
   ** 文件名:ContourShadedLayer
   ** Copyright (c) css

   ** 创建人: 吴恩平

   ** 日 期:  2010-04-07
   ** 修改人:
   ** 日 期:
   ** 描 述:  色版图图层类
   **
   ** 版 本:  1.0
   **----------------------------------------------------------------*/

#include "ContourShadedLayer.h"

ContourShadedLayer::ContourShadedLayer(DataProvider* data, CoordinateTransform* ct) : Layer(data, ct)
{
	renderVol = new QRenderVolume;
	QObject::connect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(FindedUnit(GLenum, QRenderable*)),Qt::DirectConnection);
}

ContourShadedLayer::~ContourShadedLayer(void)
{
}
int ContourShadedLayer::InitLayer()
{
	return 0;
}
int ContourShadedLayer::CreateLayer( const QString& filename )
{
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
	case VECTOR_POLYGON:
		dataProvider->ParentLayer(this);
		dataProvider->DumpPolygon(renderVol);
		break;
	default:
		dataProvider->ParentLayer(this);
		dataProvider->DumpAll(renderVol);
		break;
	}

	/*QLeaf* leaf = new QLeaf;
	leaf->InsertRenderable(renderVol);*/
	int iR = layerRoot->InsertRenderable(renderVol);
	return iR;
}

int ContourShadedLayer::CreateLayer(QNode* node)
{
	layerRoot->InsertChild((QNode*)node);
	return 0;
}
int ContourShadedLayer::CreateLayer( const QStringList& namelist )
{
	openFileNameList = namelist;
	ReloadLayer();
	return true;
}
int ContourShadedLayer::ReloadLayer()
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
void ContourShadedLayer::TransformLayer()
{
	//renderVol->FindDataUseSignal(GL_VERTEX_ARRAY);
	renderVol->FindUnitUseSignal();
}
QString ContourShadedLayer::GetProInfo( void )
{
	return dataProvider->GetProInfo();
}
void ContourShadedLayer::Clear()
{
	//layerRoot->Clear();
}
void ContourShadedLayer::MountLayer( QNode* root )
{
}
QRenderable* ContourShadedLayer::GetLayerData()
{
	return renderVol;
}
void ContourShadedLayer::operator=( ContourShadedLayer &l )
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
void ContourShadedLayer::FindedData( QDataArray* data )
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
void ContourShadedLayer::FindedUnit( GLenum type, QRenderable* data )
{
	QRenderUnit* unit = data->AsRenderUnit();
	if(!unit) return;
	if(unit->CallListIsDirty())
		unit->CallListDirty(false);

	QDataArray* unitData = unit->VertexArray();
	QVector3DArray * OriginalArray = unit->OriginalArray();   //取得原始数据

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
			//QVector3D temp, res;
			//temp = OriginalArray->at(i);
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
			float x = 0 ;
			x = OriginalArray->at(i).x();
			QVector3D temp(OriginalArray->at(i).x(), OriginalArray->at(i).y(), OriginalArray->at(i).z()), res;
			//QVector3D temp(pfData[i * 3 + 0], pfData[i * 3 + 1], pfData[i * 3 + 2]), res;
// 			QVector3D temp, res;
			// 			temp = OriginalArray->at(i);
			res = coordTrans->Transform(temp);
			pfData[i * 3 + 0] = res.x();
			pfData[i * 3 + 1] = res.y();
			pfData[i * 3 + 2] = res.z();
			//qDebug() <<","<< pfData[i * 3 + 0] <<","<< pfData[i * 3 + 1] <<","<< pfData[i * 3 + 2];
		}
		break;
	}
}