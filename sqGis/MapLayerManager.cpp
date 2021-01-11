#include "MapLayerManager.h"

#include <QStandardItemModel>

MapLayerManager::MapLayerManager()
{
	_mapLayerView = NULL;
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

void MapLayerManager::addMapLayerToView(QgsMapLayer* layer, bool visible)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(_mapLayerView->model());
	QStandardItem * item = new QStandardItem(QString(layer->name()));
	//model->appendRow(item);
	model->insertRow(0, item);
	//int idx = model->rowCount();
	//model->setItem(idx - 1, 1, new QStandardItem(layer->name()));
	//model->setItem(idx - 1, 2, new QStandardItem((layer->crs()).description()));

	QgsMapLayerType type = layer->type();
	switch (type)
	{
		case QgsMapLayerType::VectorLayer:
			item->setIcon(QIcon(":/img/vector_x16"));
			break;
		case QgsMapLayerType::RasterLayer:
			item->setIcon(QIcon(":/img/raster_x16"));
			break;
		//case QgsMapLayerType::MeshLayer:
		//	item->setIcon(QIcon(":/img/vector_layer.png"));
		//	break;
		//case QgsMapLayerType::PluginLayer:
		//	item->setIcon(QIcon(":/img/vector_layer.png"));
		//	break;
	}
}

void MapLayerManager::hideMapLayerFromView(QString layerName)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(_mapLayerView->model());
	for (int i = 0; i < model->rowCount(); i++)
	{
		QStandardItem *item = model->item(i);
		if (item->text() != layerName)
			continue;

		item->setIcon(QIcon(":/img/unvisible_x16"));
		return;
	}
}

void MapLayerManager::showMapLayerFromView(QString layerName)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(_mapLayerView->model());
	for (int i = 0; i < model->rowCount(); i++)
	{
		QStandardItem *item = model->item(i);
		if (item->text() != layerName)
			continue;

		QgsMapLayer *layer = getMapLayer(layerName);
		QgsMapLayerType type = layer->type();
		switch (type)
		{
			case QgsMapLayerType::VectorLayer:
				item->setIcon(QIcon(":/img/vector_x16"));
				break;
			case QgsMapLayerType::RasterLayer:
				item->setIcon(QIcon(":/img/raster_x16"));
				break;
		}
		return;
	}
}

void MapLayerManager::forwardMapLayerFromView(QString layerName)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(_mapLayerView->model());
	for (int i = 0; i < model->rowCount(); i++)
	{
		QStandardItem *item = model->item(i);
		if (item->text() != layerName)
			continue;

		if (i == 0)
			return;

		QStandardItem *swapItem = model->item(i - 1);
		model->setItem(i - 1, item);
		model->setItem(i, swapItem);
		QModelIndex index = model->index(i - 1, 0);
		_mapLayerView->setCurrentIndex(index);
		return;
	}
}

void MapLayerManager::backwardMapLayerFromView(QString layerName)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(_mapLayerView->model());
	for (int i = 0; i < model->rowCount(); i++)
	{
		QStandardItem *item = model->item(i);
		if (item->text() != layerName)
			continue;

		if (i == model->rowCount() - 1)
			return;

		QStandardItem *swapItem = model->item(i + 1);
		model->setItem(i + 1, item);
		model->setItem(i, swapItem);
		QModelIndex index = model->index(i+1,0);
		_mapLayerView->setCurrentIndex(index);
		return;
	}
}

void MapLayerManager::deleteMapLayerFromView(QString layerName)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(_mapLayerView->model());
	for (int i = 0; i < model->rowCount(); i++)
	{
		QStandardItem *item = model->item(i);
		if (item->text() != layerName)
			continue;

		model->removeRow(i);
		return;
	}
}

void MapLayerManager::attachMapLayerView(QTreeView* tree)
{
	if (_mapLayerView != NULL)
	{
		QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(_mapLayerView->model());
		if (model->hasChildren())
			model->removeRows(0, model->rowCount());
	}

	_mapLayerView = tree;
	QStandardItemModel* model = new QStandardItemModel(_mapLayerView);
	model->setHorizontalHeaderLabels(QStringList() << QStringLiteral("Í¼²ãÃû³Æ"));
	_mapLayerView->setModel(model);
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

	addMapLayerToView(layer, visible);
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
	hideMapLayerFromView(layerName);
	refreshMapLayers();
}

void MapLayerManager::showMapLayer(QString layerName)
{
	PTR_MapLayerItem layer = getMapLayerItem(layerName);
	if (layer == NULL)
		return;

	layer->_visible = true;
	showMapLayerFromView(layerName);
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

#if PROMPT_DEBUG_MSG
	qDebug() << "Selected layer index in Layer TreeView:" << layerIndex;
#endif

	_mapLayerItems.swap(layerIndex - 1, layerIndex);
	forwardMapLayerFromView(layerName);
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

#if PROMPT_DEBUG_MSG
	qDebug() << "Selected layer index in Layer TreeView:" << layerIndex;
#endif

	_mapLayerItems.swap(layerIndex, layerIndex + 1);
	backwardMapLayerFromView(layerName);
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
		deleteMapLayerFromView(layerName);
		refreshMapLayers();
		return;
	}
}

void MapLayerManager::refreshMapCanvas(QgsMapCanvas* mapCanvas, QgsMapLayer* layer)
{
	if(layer != NULL)
		mapCanvas->setExtent(layer->extent());

	mapCanvas->setLayers(_mapLayers);
	mapCanvas->refresh();
}