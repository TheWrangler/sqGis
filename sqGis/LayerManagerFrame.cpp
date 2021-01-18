#include "LayerManagerFrame.h"
#include "options.h"
#include "./MapMark/MarkLayer.h"

#include <QTreeWidget>

LayerManagerFrame::LayerManagerFrame(QWidget *parent)
	: QFrame(parent)
{
	ui.setupUi(this);

	ui.mapLayerTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.mapLayerTreeView->setSelectionMode(QAbstractItemView::SingleSelection);

	QStandardItemModel* model = new QStandardItemModel(ui.mapLayerTreeView);
	model->setHorizontalHeaderLabels(QStringList() << QStringLiteral("Í¼²ãÃû³Æ"));
	ui.mapLayerTreeView->setModel(model);


	connect(ui.mapLayerTreeView, SIGNAL(clicked(const QModelIndex&)),
		this, SLOT(on_mapLayerTreeItem_Clicked(const QModelIndex&)));

	_mapLayerManager = NULL;
}

LayerManagerFrame::~LayerManagerFrame()
{
}

void LayerManagerFrame::setLayerIcon(QStandardItem * item, QgsMapLayerType type, bool visible)
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
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	for (int i = 0; i < model->rowCount(); i++)
	{
		QStandardItem *item = model->item(i);
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

void LayerManagerFrame::addMapLayerToView(QgsMapLayer* layer, bool visible)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	QStandardItem * item = new QStandardItem(layer->name());
	model->insertRow(0, item);

	QgsMapLayerType type = layer->type();
	setLayerIcon(item,type,visible);

#if PROMPT_INFO_MSG
	qInfo() << QStringLiteral("Ìí¼ÓÍ¼²ã") << layer->name();
#endif

	_mapLayerManager->addMapLayer(layer, visible);
}

void LayerManagerFrame::hideMapLayerFromView(QString layerName)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	for (int i = 0; i < model->rowCount(); i++)
	{
		QStandardItem *item = model->item(i);
		if (item->text() != layerName)
			continue;

		QgsMapLayer *layer = _mapLayerManager->getMapLayer(layerName);
		QgsMapLayerType type = layer->type();
		setLayerIcon(item, type, false);

		_mapLayerManager->hideMapLayer(layerName);
		return;
	}
}

void LayerManagerFrame::showMapLayerFromView(QString layerName)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	for (int i = 0; i < model->rowCount(); i++)
	{
		QStandardItem *item = model->item(i);
		if (item->text() != layerName)
			continue;

		QgsMapLayer *layer = _mapLayerManager->getMapLayer(layerName);
		QgsMapLayerType type = layer->type();
		setLayerIcon(item, type, true);

		_mapLayerManager->showMapLayer(layerName);
		return;
	}
}

void LayerManagerFrame::forwardMapLayerFromView(QString layerName)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	for (int i = 0; i < model->rowCount(); i++)
	{
		QStandardItem *item = model->item(i);
		if (item->text() != layerName)
			continue;

		if (i == 0)
			return;

		model->removeRow(i);

		item = new QStandardItem(layerName);
		model->insertRow(i - 1, item);

		QgsMapLayer *layer = _mapLayerManager->getMapLayer(layerName);
		QgsMapLayerType type = layer->type();
		setLayerIcon(item, type, true);

		_mapLayerManager->forwardMapLayer(layerName);

		QModelIndex index = model->index(i - 1, 0);
		ui.mapLayerTreeView->setCurrentIndex(index);
		return;
	}
}

void LayerManagerFrame::backwardMapLayerFromView(QString layerName)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	for (int i = 0; i < model->rowCount(); i++)
	{
		QStandardItem *item = model->item(i);
		if (item->text() != layerName)
			continue;

		if (i == model->rowCount() - 1)
			return;

		model->removeRow(i);

		item = new QStandardItem(layerName);
		model->insertRow(i + 1, item);

		QgsMapLayer *layer = _mapLayerManager->getMapLayer(layerName);
		QgsMapLayerType type = layer->type();
		setLayerIcon(item, type, true);

		_mapLayerManager->backwardMapLayer(layerName);

		QModelIndex index = model->index(i + 1, 0);
		ui.mapLayerTreeView->setCurrentIndex(index);
		return;
	}
}

void LayerManagerFrame::deleteMapLayerFromView(QString layerName)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	for (int i = 0; i < model->rowCount(); i++)
	{
		QStandardItem *item = model->item(i);
		if (item->text() != layerName)
			continue;

		model->removeRow(i);

		_mapLayerManager->deleteMapLayer(layerName);
		return;
	}
}

void LayerManagerFrame::updateLayerFeatureView(QgsVectorLayer* layer)
{
	QStandardItem* item = getLayerViewItem(layer->name());
	int item_num = item->rowCount();
	item->removeRows(0, item_num);

	QgsVectorDataProvider* provider = layer->dataProvider();
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
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	int row = ui.mapLayerTreeView->currentIndex().row();
	QModelIndex index = model->index(row, 0);
	QString layerName = model->data(index).toString();

#if PROMPT_DEBUG_MSG
	qDebug() << QStringLiteral("Ñ¡ÔñÍ¼²ã:") << layerName;
#endif

	forwardMapLayerFromView(layerName);
	emit(layersChanged(NULL));
}

void LayerManagerFrame::on_downLayerBtn_clicked()
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	int row = ui.mapLayerTreeView->currentIndex().row();
	QModelIndex index = model->index(row, 0);
	QString layerName = model->data(index).toString();

#if PROMPT_DEBUG_MSG
	qDebug() << QStringLiteral("Ñ¡ÔñÍ¼²ã:") << layerName;
#endif

	backwardMapLayerFromView(layerName);
	emit(layersChanged(NULL));
}

void LayerManagerFrame::on_removeLayerBtn_clicked()
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	int row = ui.mapLayerTreeView->currentIndex().row();
	QModelIndex index = model->index(row, 0);
	QString layerName = model->data(index).toString();

#if PROMPT_DEBUG_MSG
	qDebug() << QStringLiteral("Ñ¡ÔñÍ¼²ã:") << layerName;
#endif

	deleteMapLayerFromView(layerName);
	emit(layersChanged(NULL));
}

void LayerManagerFrame::on_visibleLayerBtn_clicked()
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	int row = ui.mapLayerTreeView->currentIndex().row();
	QModelIndex index = model->index(row, 0);
	QString layerName = model->data(index).toString();

#if PROMPT_DEBUG_MSG
	qDebug() << QStringLiteral("Ñ¡ÔñÍ¼²ã:") << layerName;
#endif


	//TODO:
	emit(layersChanged(NULL));
}

void LayerManagerFrame::on_mapLayerTreeItem_Clicked(const QModelIndex &index)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.mapLayerTreeView->model());
	QStandardItem* item = model->itemFromIndex(index);

	QStandardItem* parent_item = item->parent();
	if (parent_item != NULL)
	{
		QgsMapLayer* layer = _mapLayerManager->getMapLayer(parent_item->text());
		if (layer == NULL)
			return;

		MarkLayer* markLayer = (MarkLayer*)layer;

		QgsFeature feature;
		if (!markLayer->searchFeature(item->data().toULongLong(), feature))
			return;

		updateFeaturePropertyView(feature);
	}
}