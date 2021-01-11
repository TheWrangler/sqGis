#include "LayerManagerFrame.h"
#include "options.h"
#include <QStandardItemModel>

LayerManagerFrame::LayerManagerFrame(QWidget *parent)
	: QFrame(parent)
{
	ui.setupUi(this);

	_mapLayerManager = NULL;
}

LayerManagerFrame::~LayerManagerFrame()
{
}

void LayerManagerFrame::attachMapLayerManager(MapLayerManager* mapLayerManager)
{
	_mapLayerManager = mapLayerManager;
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

	_mapLayerManager->forwardMapLayer(layerName);
	emit(layersChanged());
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

	_mapLayerManager->backwardMapLayer(layerName);
	emit(layersChanged());
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

	_mapLayerManager->deleteMapLayer(layerName);
	emit(layersChanged());
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
}