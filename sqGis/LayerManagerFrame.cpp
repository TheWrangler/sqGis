#include "LayerManagerFrame.h"
#include "options.h"
#include "./MapMarkLayer/MarkLayer.h"

#include <QTreeWidget>

LayerManagerFrame::LayerManagerFrame(QWidget *parent)
	: QFrame(parent)
{
	ui.setupUi(this);
	initLayerTreeView();
	_mapLayerManager = NULL;
}

LayerManagerFrame::~LayerManagerFrame()
{
}

void LayerManagerFrame::initLayerTreeView()
{
	ui.mapLayerTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.mapLayerTreeView->setSelectionMode(QAbstractItemView::SingleSelection);

	QStandardItemModel* model = new QStandardItemModel(ui.mapLayerTreeView);
	model->setHorizontalHeaderLabels(QStringList() << QStringLiteral("Í¼²ãÃû³Æ"));
	ui.mapLayerTreeView->setModel(model);

	QStandardItem * item;
	for (unsigned int i = 0; i < MapLayerManager::MapLayerRole_Sum; i++)
	{
		item = new QStandardItem(MapLayerManager::LayerRoleCaption[i]);
		model->appendRow(item);
	}

	connect(ui.mapLayerTreeView, SIGNAL(clicked(const QModelIndex&)),
		this, SLOT(on_mapLayerTreeItem_Clicked(const QModelIndex&)));
}

QStandardItem* LayerManagerFrame::getLayerRoleItem(MapLayerManager::MapLayerRoleType role)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	QList<QStandardItem*> res = model->findItems(MapLayerManager::LayerRoleCaption[role]);
	if (res.count() == 0)
	{
#if PROMPT_CRITICAL_MSG
		qInfo() << QStringLiteral("ÎÞ·¨ÕÒµ½Í¼²ã·ÖÀà") << MapLayerManager::LayerRoleCaption[role];
#endif
		return NULL;
	}

	return res.at(0);
}

bool LayerManagerFrame::isLayerItem(QModelIndex index)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	QModelIndex parent = model->parent(index);

	QStandardItem* item = model->itemFromIndex(parent);
	if (item == NULL)
		return false;

	if (!MapLayerManager::LayerRoleCaption.contains(item->text()))
		return false;

	return true;
}

QgsMapLayer* LayerManagerFrame::isFeatureItem(QModelIndex index)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	QModelIndex parent = model->parent(index);

	QStandardItem* item = model->itemFromIndex(parent);
	if (item == NULL)
		return NULL;

	return  _mapLayerManager->getMapLayer(item->text());
}

void LayerManagerFrame::setLayerIcon(QStandardItem * item, QgsMapLayerType type, MapLayerManager::MapLayerRoleType role, bool visible)
{
	if (!visible)
		item->setIcon(QIcon(":/img/unvisible_x16"));
	else
	{
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
}

QStandardItem* LayerManagerFrame::getLayerViewItem(QString layerName)
{
	MapLayerManager::MapLayerRoleType role = _mapLayerManager->getMapLayerRole(layerName);
	QStandardItem* parent = getLayerRoleItem(role);
	unsigned int count = parent->rowCount();

	for (int i = 0; i < count; i++)
	{
		QStandardItem *item = parent->child(i);
		if (item->text() != layerName)
			continue;

		return item;
	}

	return NULL;
}

void LayerManagerFrame::updateFeaturePropertyView(QgsFeature& feature)
{
	ui.mapPropertyTreeWidget->clear();

	QStringList list;
	QTreeWidgetItem* item;

	list.append("ID");
	list.append(QString::number(feature.id()));
	item = new QTreeWidgetItem(list);
	ui.mapPropertyTreeWidget->addTopLevelItem(item);

	QStringList attr = feature.fields().names();
	QList<QString>::iterator it = attr.begin();
	
	while (it != attr.end())
	{
		QVariant v = feature.attribute(*it);

		list.clear();
		list.append(*it);
		list.append(v.toString());

		item = new QTreeWidgetItem(list);
		ui.mapPropertyTreeWidget->addTopLevelItem(item);

		it++;
	}
}

void LayerManagerFrame::attachMapLayerManager(MapLayerManager* mapLayerManager)
{
	_mapLayerManager = mapLayerManager;
}

bool LayerManagerFrame::addMapLayerToView(QgsMapLayer* layer, MapLayerManager::MapLayerRoleType role, bool visible)
{
	if (_mapLayerManager->isLayerExist(layer->name()))
	{
#if PROMPT_INFO_MSG
		qInfo() << QStringLiteral("ÎÞ·¨Ìí¼ÓÖØÃûÍ¼²ã") << layer->name();
#endif
		return false;
	}
		
	QStandardItem* parent = getLayerRoleItem(role);

	QStandardItem * item = new QStandardItem(layer->name());
	parent->insertRow(0, item);

	QgsMapLayerType type = layer->type();
	setLayerIcon(item, type, role, visible);

#if PROMPT_INFO_MSG
	qInfo() << QStringLiteral("Ìí¼ÓÍ¼²ã") << layer->name();
#endif

	_mapLayerManager->addMapLayer(layer, role, visible);

	ui.mapLayerTreeView->expandAll();

	return true;
}

void LayerManagerFrame::showMapLayerItem(QStandardItem* item, QgsMapLayer* layer,bool visible)
{
	MapLayerManager::MapLayerRoleType role = _mapLayerManager->getMapLayerRole(layer->name());
	QgsMapLayerType type = layer->type();

	if (item != NULL)
		setLayerIcon(item, type, role, visible);

	if(!visible)
		_mapLayerManager->hideMapLayer(layer->name());
	else _mapLayerManager->showMapLayer(layer->name());
}

void LayerManagerFrame::forwardMapLayerItem(QStandardItem* item)
{
	unsigned int row = item->row();
	QStandardItem* parent = item->parent();
	if (row == 0)
		return;

	QStandardItem* new_item = new QStandardItem(item->text());
	parent->removeRow(item->row());
	parent->insertRow(row - 1, new_item);

	QgsMapLayer* layer = _mapLayerManager->getMapLayer(new_item->text());
	QgsMapLayerType type = layer->type();
	MapLayerManager::MapLayerRoleType role = _mapLayerManager->getMapLayerRole(new_item->text());
	setLayerIcon(new_item, type, role, true);

	_mapLayerManager->backwardMapLayer(new_item->text());

	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	QModelIndex index = model->index(row - 1, 0, parent->index());
	ui.mapLayerTreeView->setCurrentIndex(index);

	if (role == MapLayerManager::MapLayerRole_Mark
		|| role == MapLayerManager::MapLayerRole_Measure)
		updateLayerFeatureView(layer);
}

void LayerManagerFrame::backwardMapLayerItem(QStandardItem* item)
{
	unsigned int row = item->row();
	QStandardItem* parent = item->parent();
	if (row >= parent->rowCount()-1)
		return;

	QStandardItem* new_item = new QStandardItem(item->text());
	parent->removeRow(item->row());
	parent->insertRow(row + 1, new_item);

	QgsMapLayer* layer = _mapLayerManager->getMapLayer(new_item->text());
	QgsMapLayerType type = layer->type();
	MapLayerManager::MapLayerRoleType role = _mapLayerManager->getMapLayerRole(new_item->text());
	setLayerIcon(new_item, type, role, true);

	_mapLayerManager->backwardMapLayer(new_item->text());

	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	QModelIndex index = model->index(row + 1, 0, parent->index());
	ui.mapLayerTreeView->setCurrentIndex(index);

	if(role == MapLayerManager::MapLayerRole_Mark 
		|| role == MapLayerManager::MapLayerRole_Measure)
		updateLayerFeatureView(layer);
}

void LayerManagerFrame::deleteMapLayerItem(QStandardItem* item)
{
	_mapLayerManager->deleteMapLayer(item->text());

	QStandardItem* parent = item->parent();
	parent->removeRow(item->row());
}

void LayerManagerFrame::deleteLayerFeatureItem(QStandardItem* item, QgsMapLayer* layer)
{
	MarkLayer* markLayer = dynamic_cast<MarkLayer*>(layer);
	if (markLayer == NULL)
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("×ª»»Îª±ê»æÍ¼²ã:") << layer->name() << QStringLiteral("Ê§°Ü");
#endif
		return;
	}
	QgsFeatureId id = item->data().toULongLong();
	markLayer->removeMark(id,true);

	QStandardItem* parent = item->parent();
	parent->removeRow(item->row());
}

void LayerManagerFrame::updateLayerFeatureView(QgsMapLayer* layer)
{
	QgsVectorLayer* vectlayer = dynamic_cast<QgsVectorLayer*>(layer);
	if (vectlayer == NULL)
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("×ª»»ÎªÊ¸Á¿Í¼²ã") << layer->name() << QStringLiteral("Ê§°Ü");
#endif
	}

	QStandardItem* item = getLayerViewItem(vectlayer->name());
	int item_num = item->rowCount();
	item->removeRows(0, item_num);

	QgsVectorDataProvider* provider = vectlayer->dataProvider();
	QgsFeatureIterator it = provider->getFeatures(QgsFeatureRequest());
	if (!it.isValid())
		return;

	QgsFeature feature;
	while (it.nextFeature(feature))
	{
		QStandardItem* childItem = new QStandardItem(feature.attribute("name").toString());
		childItem->setData(feature.id());
		item->appendRow(childItem);
	} 

	ui.mapLayerTreeView->expandAll();
}

void LayerManagerFrame::on_upLayerBtn_clicked()
{
	if (!isLayerItem(ui.mapLayerTreeView->currentIndex()))
		return;

	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	QStandardItem* item = model->itemFromIndex(ui.mapLayerTreeView->currentIndex());
#if PROMPT_DEBUG_MSG
	qDebug() << QStringLiteral("Ñ¡ÔñÍ¼²ã:") << item->text();
#endif

	forwardMapLayerItem(item);
	emit(layersChanged(NULL));
}

void LayerManagerFrame::on_downLayerBtn_clicked()
{
	if (!isLayerItem(ui.mapLayerTreeView->currentIndex()))
		return;

	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	QStandardItem* item = model->itemFromIndex(ui.mapLayerTreeView->currentIndex());
#if PROMPT_DEBUG_MSG
	qDebug() << QStringLiteral("Ñ¡ÔñÍ¼²ã:") << item->text();
#endif

	backwardMapLayerItem(item);
	emit(layersChanged(NULL));
}

void LayerManagerFrame::on_removeLayerBtn_clicked()
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	if (isLayerItem(ui.mapLayerTreeView->currentIndex()))
	{
		QStandardItem* item = model->itemFromIndex(ui.mapLayerTreeView->currentIndex());
#if PROMPT_DEBUG_MSG
		qDebug() << QStringLiteral("Ñ¡ÔñÉ¾³ýÍ¼²ã:") << item->text();
#endif

		deleteMapLayerItem(item);
		emit(layersChanged(NULL));
	}
	else
	{
		QgsMapLayer* layer = isFeatureItem(ui.mapLayerTreeView->currentIndex());
		if (layer == NULL)
			return;

		QStandardItem* item = model->itemFromIndex(ui.mapLayerTreeView->currentIndex());
#if PROMPT_DEBUG_MSG
		qDebug() << QStringLiteral("Ñ¡ÔñÉ¾³ýÍ¼²ãÔªËØ:") << item->text();
#endif
		deleteLayerFeatureItem(item, layer);
	}
}

void LayerManagerFrame::on_visibleLayerBtn_clicked()
{
	if (!isLayerItem(ui.mapLayerTreeView->currentIndex()))
		return;

	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	QStandardItem* item = model->itemFromIndex(ui.mapLayerTreeView->currentIndex());
#if PROMPT_DEBUG_MSG
	qDebug() << QStringLiteral("Ñ¡ÔñÍ¼²ã:") << item->text();
#endif

	QgsMapLayer* layer = _mapLayerManager->getMapLayer(item->text());
	if(_mapLayerManager->isLayerVisible(item->text()))
		showMapLayerItem(item, layer, false);
	else showMapLayerItem(item, layer, true);

	emit(layersChanged(NULL));
}

void LayerManagerFrame::on_mapLayerTreeItem_Clicked(const QModelIndex &index)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());

	QgsMapLayer* layer = isFeatureItem(ui.mapLayerTreeView->currentIndex());
	if (layer == NULL)
		return;

	MarkLayer* markLayer = dynamic_cast<MarkLayer*>(layer);
	if (markLayer == NULL)
	{
#if PROMPT_DEBUG_MSG
		qDebug() << QStringLiteral("×ª»»Îª±ê»æÍ¼²ã:") << layer->name() << QStringLiteral("Ê§°Ü");
#endif
		return;
	}

	QStandardItem* item = model->itemFromIndex(ui.mapLayerTreeView->currentIndex());
	QgsFeatureId id = item->data().toULongLong();
	QgsFeature feature;
	if (!markLayer->searchFeature(id, feature))
		return;

	markLayer->selectFeature(id);

	updateFeaturePropertyView(feature);
}