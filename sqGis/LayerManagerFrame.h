#pragma once

#include <QFrame>
#include <QTreeView>
#include "ui_LayerManagerFrame.h"
#include "MapLayerManager.h"

class LayerManagerFrame : public QFrame
{
	Q_OBJECT

public:
	LayerManagerFrame(QWidget *parent = Q_NULLPTR);
	~LayerManagerFrame();

private:
	Ui::LayerManagerFrame ui;
	MapLayerManager* _mapLayerManager;

public:
	QTreeView* getLayerTreeView() { return ui.mapLayerTreeView; }
	void attachMapLayerManager(MapLayerManager* mapLayerManager);

signals:
	void layersChanged();

protected slots :
	void on_upLayerBtn_clicked();
	void on_downLayerBtn_clicked();
	void on_visibleLayerBtn_clicked();

public slots:
	void on_removeLayerBtn_clicked();
};
