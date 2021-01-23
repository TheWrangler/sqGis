#include "MapLayerManager.h"
#include "options.h"
#include <QStandardItemModel>

QStringList MapLayerManager::LayerRoleCaption = QStringList() << QStringLiteral("基本底图") << QStringLiteral("我的标绘") << QStringLiteral("我的测量");

MapLayerManager::MapLayerManager()
{
	
}

MapLayerManager::~MapLayerManager()
{
}

QList<MapLayerManager::PTR_MapLayerItem>* MapLayerManager::getMapLyerList(MapLayerRoleType role)
{
	switch (role)
	{
	case MapLayerRole_BaseMap:
		return &_baseMapLayerItems;
	case MapLayerRole_Mark:
		return &_markMapLayerItems;
	case MapLayerRole_Measure:
		return &_measureMapLayerItems;
	default:
		return NULL;
	}
}

MapLayerManager::PTR_MapLayerItem MapLayerManager::getMapLayerItem(QString layerName)
{
	QList<PTR_MapLayerItem>::iterator it = _baseMapLayerItems.begin();
	while (it != _baseMapLayerItems.end())
	{
		if ((*it)->_layer->name() == layerName)
			return (*it);

		it++;
	}

	it = _markMapLayerItems.begin();
	while (it != _markMapLayerItems.end())
	{
		if ((*it)->_layer->name() == layerName)
			return (*it);

		it++;
	}

	it = _measureMapLayerItems.begin();
	while (it != _measureMapLayerItems.end())
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

	QList<PTR_MapLayerItem>::iterator it = _measureMapLayerItems.begin();
	while (it != _measureMapLayerItems.end())
	{
		if ((*it)->_visible)
			_mapLayers.append((*it)->_layer);

		it++;
	}

	it = _markMapLayerItems.begin();
	while (it != _markMapLayerItems.end())
	{
		if ((*it)->_visible)
			_mapLayers.append((*it)->_layer);

		it++;
	}

	it = _baseMapLayerItems.begin();
	while (it != _baseMapLayerItems.end())
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

void MapLayerManager::addMapLayer(QgsMapLayer* layer, MapLayerRoleType role, bool visible)
{
	if (isLayerExist(layer->name()))
		return;

	PTR_MapLayerItem layerItem = new MapLayerItem;
	layerItem->_layer = layer;
	layerItem->_role = role;
	layerItem->_visible = visible;

	QList<PTR_MapLayerItem>* layerList = getMapLyerList(role);

	if (layerList->isEmpty())
		layerList->append(layerItem);
	else
	{
		QList<PTR_MapLayerItem>::iterator it = layerList->begin();
		layerList->insert(it, layerItem);
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

MapLayerManager::MapLayerRoleType MapLayerManager::getMapLayerRole(QString layerName)
{
	PTR_MapLayerItem layerItem = getMapLayerItem(layerName);
	if (layerItem == NULL)
	{
#if PROMPT_CRITICAL_MSG
		qInfo() << QStringLiteral("无法找到图层的分类") << layerName;
#endif
		return MapLayerRole_Invalid;
	}
	else return layerItem->_role;
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
	QList<PTR_MapLayerItem>* layers = getMapLyerList(getMapLayerRole(layerName));

	QList<PTR_MapLayerItem>::iterator itr = layers->begin();
	int layerIndex = 0;
	while (itr != layers->end())
	{
		if ((*itr)->_layer->name() == layerName)
			break;

		layerIndex++;
		itr++;
	}

	if (layerIndex == 0)
		return;

	layers->swap(layerIndex - 1, layerIndex);
	refreshMapLayers();
}

void MapLayerManager::backwardMapLayer(QString layerName)
{
	QList<PTR_MapLayerItem>* layers = getMapLyerList(getMapLayerRole(layerName));

	QList<PTR_MapLayerItem>::iterator itr = layers->begin();
	int layerIndex = 0;
	while (itr != layers->end())
	{
		if ((*itr)->_layer->name() == layerName)
			break;

		layerIndex++;
		itr++;
	}

	if (layerIndex == layers->count() - 1)
		return;

	layers->swap(layerIndex, layerIndex + 1);
	refreshMapLayers();
}

void MapLayerManager::deleteMapLayer(QString layerName)
{
	QList<PTR_MapLayerItem>* layers = getMapLyerList(getMapLayerRole(layerName));

	QList<PTR_MapLayerItem>::iterator it = layers->begin();
	while (it != layers->end())
	{
		if ((*it)->_layer->name() != layerName)
		{ 
			it++;
			continue;
		}

		layers->erase(it);
		refreshMapLayers();
#if PROMPT_INFO_MSG
		qInfo() << QStringLiteral("删除图层") << layerName;
#endif
		return;
	}
}