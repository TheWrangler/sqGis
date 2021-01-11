#include "MapLayerManager.h"
#include "options.h"
#include <QStandardItemModel>

MapLayerManager::MapLayerManager()
{
	
}

MapLayerManager::~MapLayerManager()
{
}

MapLayerManager::PTR_MapLayerItem MapLayerManager::getMapLayerItem(QString layerName)
{
	QList<PTR_MapLayerItem>::iterator it = _mapLayerItems.begin();
	while (it != _mapLayerItems.end())
	{
		if ((*it)->_layer->name() == layerName)
			return (*it);

		it++;
	}

	return NULL;
}

void MapLayerManager::refreshMapLayers()
{
	_mapLayers.clear();

	QList<PTR_MapLayerItem>::iterator it = _mapLayerItems.begin();
	while (it != _mapLayerItems.end())
	{
		if ((*it)->_visible)
			_mapLayers.append((*it)->_layer);

		it++;
	}
}

bool MapLayerManager::isLayerExist(QString layerName)
{
	if (getMapLayerItem(layerName) == NULL)
		return false;
	else return true;
}

bool MapLayerManager::isLayerVisible(QString layerName)
{
	PTR_MapLayerItem layerItem = getMapLayerItem(layerName);
	return layerItem->_visible;
}

void MapLayerManager::addMapLayer(QgsMapLayer* layer, bool visible)
{
	if (isLayerExist(layer->name()))
		return;

	PTR_MapLayerItem layerItem = new MapLayerItem;
	layerItem->_layer = layer;
	layerItem->_visible = visible;

	if (_mapLayerItems.isEmpty())
		_mapLayerItems.append(layerItem);
	else
	{
		QList<PTR_MapLayerItem>::iterator it = _mapLayerItems.begin();
		_mapLayerItems.insert(it, layerItem);
	}

	refreshMapLayers();
}

QgsMapLayer* MapLayerManager::getMapLayer(QString layerName) 
{
	PTR_MapLayerItem layerItem = getMapLayerItem(layerName);
	if (layerItem == NULL)
		return NULL;
	else return layerItem->_layer;
}

void MapLayerManager::hideMapLayer(QString layerName)
{
	PTR_MapLayerItem layer = getMapLayerItem(layerName);
	if (layer == NULL)
		return;

	layer->_visible = false;
	refreshMapLayers();
}

void MapLayerManager::showMapLayer(QString layerName)
{
	PTR_MapLayerItem layer = getMapLayerItem(layerName);
	if (layer == NULL)
		return;

	layer->_visible = true;
	refreshMapLayers();
}

void MapLayerManager::forwardMapLayer(QString layerName)
{
	QList<PTR_MapLayerItem>::iterator itr = _mapLayerItems.begin();
	int layerIndex = 0;
	while (itr != _mapLayerItems.end())
	{
		if ((*itr)->_layer->name() == layerName)
			break;

		layerIndex++;
		itr++;
	}

	if (layerIndex == 0)
		return;

	_mapLayerItems.swap(layerIndex - 1, layerIndex);
	refreshMapLayers();
}

void MapLayerManager::backwardMapLayer(QString layerName)
{
	QList<PTR_MapLayerItem>::iterator itr = _mapLayerItems.begin();
	int layerIndex = 0;
	while (itr != _mapLayerItems.end())
	{
		if ((*itr)->_layer->name() == layerName)
			break;

		layerIndex++;
		itr++;
	}

	if (layerIndex == _mapLayerItems.count() - 1)
		return;

	_mapLayerItems.swap(layerIndex, layerIndex + 1);
	refreshMapLayers();
}

void MapLayerManager::deleteMapLayer(QString layerName)
{
	QList<PTR_MapLayerItem>::iterator it = _mapLayerItems.begin();
	while (it != _mapLayerItems.end())
	{
		if ((*it)->_layer->name() != layerName)
		{ 
			it++;
			continue;
		}

		_mapLayerItems.erase(it);
		refreshMapLayers();
#if PROMPT_INFO_MSG
		qInfo() << QStringLiteral("É¾³ýÍ¼²ã") << layerName;
#endif
		return;
	}
}