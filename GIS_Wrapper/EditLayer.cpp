#include "EditLayer.h"
#include "Layer.h"

EditLayer::EditLayer(DataProvider* data, CoordinateTransform* ct) : Layer(data, ct)
{
	//layerRoot = new QFork;
	renderVol = new QRenderVolume;
	QObject::connect(renderVol, SIGNAL(signal_Finded(GLenum, QRenderable*)), this, SLOT(FindedUnit(GLenum, QRenderable*)));
}


EditLayer::~EditLayer(void)
{
	/*if(layerRoot)
	{
		delete layerRoot;
	}*/

	/*if(renderVol)
	{
		delete renderVol;
	}*/
}

int EditLayer::CreateLayer( const QString& filename )
{
	//renderVol->AttribSet(*m_nAttributeSet);
	//openFileNameList.append(filename);

	//if(!dataProvider) return false;

	//dataProvider->InitDataProvider();
	//dataProvider->OpenFile(filename);
	//QString oSourceProj = dataProvider->GetProInfo();
	//if(oSourceProj.size() != 0)
	//{
	//	coordTrans->setSourceCRS(oSourceProj);
	//	dataProvider->addProj(coordTrans);
	//}
	//
	//VECTOR_DATA_TYPE dataType = dataProvider->GetDataType();

	//switch(dataType)
	//{
	//case VECTOR_POINT:
	//	dataProvider->ParentLayer(this);
	//	dataProvider->DumpPoint(renderVol);	
	//	break;
	//case VECTOR_LINE:
	//	dataProvider->ParentLayer(this);
	//	dataProvider->DumpLine(renderVol);

	//case VECTOR_POLYGON:
	//	dataProvider->ParentLayer(this);
	//	dataProvider->DumpPolygon(renderVol);
	//	break;
	//default:
	//	dataProvider->ParentLayer(this);
	//	dataProvider->DumpAll(renderVol);
	//	break;
	//}

	//QLeaf* leaf = new QLeaf;
	//leaf->InsertRenderable(renderVol);
	//int iR = layerRoot->InsertChild(leaf);
	//return iR;
	return 0;
}

int EditLayer::CreateLayer(QNode* node)
{
	//layerRoot->InsertChild((QNode*)node);
	return 0;
}

int EditLayer::CreateLayer( const QStringList& namelist )
{
	openFileNameList = namelist;
	ReloadLayer();
	return true;
}

int EditLayer::InitLayer()
{
	return 0;
}

void EditLayer::MountLayer( QNode* root )
{

}

int EditLayer::ReloadLayer()
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

QString EditLayer::GetProInfo( void )
{
	return dataProvider->GetProInfo();
}

void EditLayer::Clear()
{
	//layerRoot->Clear();
}

void EditLayer::TransformLayer()
{
	//renderVol->FindDataUseSignal(GL_VERTEX_ARRAY);
	renderVol->FindUnitUseSignal();
}

void EditLayer::FindedData( QDataArray* data )
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

QRenderable* EditLayer::GetLayerData()
{
	return renderVol;
}

void EditLayer::FindedUnit( GLenum type, QRenderable* data )
{
	
	QRenderUnit* unit = data->AsRenderUnit();
	if(!unit) return;
	if(unit->CallListIsDirty())
		unit->CallListDirty(false);

	QDataArray* unitData = unit->VertexArray();

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
			QVector3D temp(pfData[i * 3 + 0], pfData[i * 3 + 1], pfData[i * 3 + 2]), res;
			res = coordTrans->Transform(temp);
			pfData[i * 3 + 0] = res.x();
			pfData[i * 3 + 1] = res.y();
			pfData[i * 3 + 2] = res.z();
		}
		break;
	}
}

void EditLayer::operator=( EditLayer &l )
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

QRenderVolume *EditLayer::GetVolume()
{
	return renderVol;

}