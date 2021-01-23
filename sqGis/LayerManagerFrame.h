#pragma once

#include <QFrame>
#include <QTreeView>
#include <QStandardItemModel>

#include "ui_LayerManagerFrame.h"
#include "MapLayerManager.h"
#include <qgsmaplayer.h>
#include <qgsvectorlayer.h>

class LayerManagerFrame : public QFrame
{
	Q_OBJECT

public:
	LayerManagerFrame(QWidget *parent = Q_NULLPTR);
	~LayerManagerFrame();

private:
	bool isLayerItem(QModelIndex index);
	QgsMapLayer* isFeatureItem(QModelIndex index);

protected:
	Ui::LayerManagerFrame ui;
	MapLayerManager* _mapLayerManager;

	void initLayerTreeView();
	QStandardItem* getLayerRoleItem(MapLayerManager::MapLayerRoleType role);
	void setLayerIcon(QStandardItem* item, QgsMapLayerType type, MapLayerManager::MapLayerRoleType role, bool visible);
	QStandardItem* getLayerViewItem(QString layerName);
	void updateFeaturePropertyView(QgsFeature& feature);

	void showMapLayerItem(QStandardItem* item, QgsMapLayer* layer, bool visible);
	void forwardMapLayerItem(QStandardItem* item);
	void backwardMapLayerItem(QStandardItem* item);
	void deleteMapLayerItem(QStandardItem* item);
	void deleteLayerFeatureItem(QStandardItem* item, QgsMapLayer* layer);
	
public:
	QTreeView* getLayerTreeView() { return ui.mapLayerTreeView; }
	void attachMapLayerManager(MapLayerManager* mapLayerManager);
	bool addMapLayerToView(QgsMapLayer* layer, MapLayerManager::MapLayerRoleType role = MapLayerManager::MapLayerRole_BaseMap, bool visible = true);
	void updateLayerFeatureView(QgsMapLayer* layer);

signals:
	void layersChanged(QgsMapLayer* layer);

protected slots :
	void on_upLayerBtn_clicked();
	void on_downLayerBtn_clicked();
	void on_visibleLayerBtn_clicked();

	void on_mapLayerTreeItem_Clicked(const QModelIndex &index);

public slots:
	void on_removeLayerBtn_clicked();
};
