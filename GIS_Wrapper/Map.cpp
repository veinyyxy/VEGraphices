#include "Map.h"

Map::Map(void) : mapRoot(0), mapScale(1.0), dataProvider(0), coordTrans(0), m_pMapUndoStack(0)
{
	mapRoot = NULL;
	mapRoot = new QFork(this);
    currentLayer = NULL;
}

Map::~Map(void)
{
	if(mapRoot)
	{
		delete mapRoot;
		mapRoot = NULL; 
	}
}

Layer* Map::GetLayer( const QString& name )
{
	if (layerMap.contains(name))
	{
		return layerMap[name];
	}		
	else 
		return 0;
}

int Map::AddLayer( const QString& name, Layer* mapLayer )
{
	layerMap[name] = mapLayer;
	mapRoot->InsertChild(mapLayer->DrawLayer());
	return true;
}

bool Map::LinkMap( const Map* pMap )
{
	if(mapRoot)
	{
		mapRoot->InsertChild(((Map*)pMap)->DrawMap());
	}
	return false;
}

double Map::GetMapScale()
{
	return mapScale;
}

void Map::SetMapScale( double scale )
{
	mapScale = scale;
}

void Map::TramsformMap()
{
	QMapIterator<QString, Layer*> layerIter(layerMap);
	while(layerIter.hasNext())
	{
		Layer* l = layerIter.next().value();
		if(l)
			l->TransformLayer();
	}
}

int Map::RemoveLayer( const QString& name, bool bIsDel )
{
	int iR = -1;
	QMap<QString, Layer*>::Iterator itera = layerMap.find(name);
	if(itera != layerMap.end())
	{
		int i = mapRoot->RemoveChild(itera.value()->DrawLayer(), bIsDel);
		//qDebug("%d", i);
		delete itera.value();
		itera.value() = NULL;
		currentLayer = NULL;
		iR = layerMap.remove(name);
	}
	return iR;
}

QNode* Map::DrawMap()
{
	return mapRoot;
	/*QMapIterator<QString, Layer*> layerIter(layerMap);
	while(layerIter.hasNext())
	{
		Layer* l = layerIter.next().value();
		l->GetLayerData()->Render();
	}
	return 0;*/
}

Layer* Map::CreateVectorLayer( const QString& name )
{
	if(dataProvider == 0 || coordTrans == 0) return 0;
	Layer* l = new VectorLayer(dataProvider, coordTrans);
	l->CreateLayer(name);
	return l;
}

Layer* Map::CreateVectorLayer( const QStringList& namelist )
{
	if(dataProvider == 0 || coordTrans == 0) return 0;
	Layer* l = new VectorLayer(dataProvider, coordTrans);
	l->CreateLayer(namelist);
	return l;
}

void Map::SetDataProvider( DataProvider* dp )
{
	dataProvider = dp;
}

DataProvider* Map::GetDataProvider()
{
	return dataProvider;
}

void Map::SetCoordinateTransform( CoordinateTransform* ct )
{
	coordTrans = ct;
}

CoordinateTransform* Map::GetCoordinateTransform()
{
	return coordTrans;
}

void Map::MoveMap(const QVector3D& pos)
{
	QMatrix4x4 mov;
	mov.translate(pos);
	mapRoot->Transform(mov);
}

void Map::RotateMap(qreal a, const QVector3D& aix)
{
	QMatrix4x4 rot;
	rot.rotate(a, aix);
	mapRoot->Transform(rot);
}

void Map::ScaleMap( qreal s )
{
	QMatrix4x4 sca;
	sca.scale(s);
	mapRoot->Transform(sca);
}

bool Map::isCurrentLayerHide()
{
	bool isHide = true;
	if (CurrentLayer()!=NULL)
	{
		isHide = CurrentLayer()->isLayerHide();
	}
	return isHide;
}

bool Map::HideLayer( const QString& name, bool bl )
{
	Layer* mapLayer;
	if(bl)
	{
		if (layerMap.isEmpty())
		{
			return false;
		}
		QMap<QString, Layer*>::Iterator itera = layerMap.find(name);
		if(itera != layerMap.end())
		{
			mapRoot->RemoveChild(itera.value()->DrawLayer(), false);
			itera.value()->isLayerHide(bl);
		}
	}
	else
	{
		mapLayer = layerMap.value(name);
		if (mapLayer != NULL)
		{
			mapRoot->InsertChild(mapLayer->DrawLayer());
			mapLayer->isLayerHide(bl);
		}
	}
	return true;
}

Layer* Map::CurrentLayer()
{
	return currentLayer;
}

void Map::CurrentLayer( Layer* l )
{
	currentLayer = l;
}

void Map::Draw()
{
	QMapIterator<QString, Layer*> layerIter(layerMap);
	while(layerIter.hasNext())
	{
		QString k = layerIter.next().key();
		//Layer* l = layerIter.next().value();
		Layer* l = layerMap[k];
		l->Draw();
	}
}

void Map::UndoSatck(QUndoStack* udos)
{
	m_pMapUndoStack = udos;
}

QUndoStack* Map::UndoSatck()
{
	return m_pMapUndoStack;
}

void Map::CopyTo( Map* pOther )
{

}

Map& Map::operator+=( const Map& other )
{
	QMap<QString, Layer*>::iterator i = other.layerMap.begin(), res = this->layerMap.end(), notFound = this->layerMap.end();
	for(; i != other.layerMap.end(); i++)
	{
		res = this->layerMap.find(i.key());
		if(res == notFound)
			AddLayer(i.key(), i.value());
		res = notFound;
	}
	return *this;
}

Map& Map::operator-=( const Map& other )
{
	QMap<QString, Layer*>::iterator i = other.layerMap.begin(), res = this->layerMap.end(), notFound = this->layerMap.end();
	for(; i != other.layerMap.end(); i++)
	{
		res = this->layerMap.find(i.key());
		if(res != notFound)
			RemoveLayer(i.key(), false);
		res = notFound;
	}
	return *this;
}

Map& Map::operator=( const Map& other )
{
	QMap<QString, Layer*>::iterator i = other.layerMap.begin();
	for(; i != other.layerMap.end(); i++)
	{
		AddLayer(i.key(), i.value());
	}
	this->currentLayer = other.currentLayer;
	this->dataProvider = other.dataProvider;
	this->coordTrans = other.coordTrans;
	this->mapScale = other.mapScale;
	this->mapTransf = other.mapTransf;
	this->rouAix = other.rouAix;
	this->rouAngle = other.rouAngle;
	this->m_pMapUndoStack = other.m_pMapUndoStack;
	this->mapRoot = other.mapRoot;
	return *this;
}

QStringList Map::GetLayerNameList()
{
	QStringList nameList;
	QMap<QString, Layer*>::iterator itstart = layerMap.begin();
	for(; itstart != layerMap.end(); itstart++)
	{
		nameList.push_back(itstart.key());
	}
	return nameList;
}

bool Map::SeparateMap( const Map* pMap )
{
	if(mapRoot)
	{
		mapRoot->RemoveChild(((Map*)pMap)->DrawMap(), false);
	}
	return true;
}
