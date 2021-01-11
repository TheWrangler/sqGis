#pragma once

#include <QFrame>
#include <QTreeView>
#include <QStandardItemModel>

#include "ui_LayerManagerFrame.h"
#include "MapLayerManager.h"
#include "qgsmaplayer.h"

class LayerManagerFrame : public QFrame
{
	Q_OBJECT

public:
	LayerManagerFrame(QWidget *parent = Q_NULLPTR);
	~LayerManagerFrame();

private:
	Ui::LayerManagerFrame ui;
	MapLayerManager* _mapLayerManager;

	void setLayerIcon(QStandardItem* item, QgsMapLayerType type, bool visible = true);

public:
	QTreeView* getLayerTreeView() { return ui.mapLayerTreeView; }
	void attachMapLayerManager(MapLayerManager* mapLayerManager);

	void addMapLayerToView(QgsMapLayer* layer, bool visible = true);
	void hideMapLayerFromView(QString layerName);
	void showMapLayerFromView(QString layerName);
	void forwardMapLayerFromView(QString layerName);
	void backwardMapLayerFromView(QString layerName);
	void deleteMapLayerFromView(QString layerName);

signals:
	void layersChanged(QgsMapLayer* layer);

protected slots :
	void on_upLayerBtn_clicked();
	void on_downLayerBtn_clicked();
	void on_visibleLayerBtn_clicked();

public slots:
	void on_removeLayerBtn_clicked();
};
