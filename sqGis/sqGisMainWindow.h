#pragma once

#include <QMainWindow>
#include <QStandardItem>

#include "ui_sqGisMainWindow.h"

#include <qgsmapcanvas.h>
#include <qgsmaplayer.h>
#include <qgsfeature.h>
#include <qgsmaptoolpan.h>
#include <qgsmaptoolzoom.h>

class QLabel;

class sqGisMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	sqGisMainWindow(QWidget *parent = Q_NULLPTR);
	~sqGisMainWindow();

private:
	Ui::sqGisMainWindow ui;
	QLabel *m_uiCursorCoorLabel;


protected:
	//map canvas
	QgsMapCanvas *m_mapCanvas;
	//map layers
	QList<QgsMapLayer *> m_mapLayers;
	//map tools
	QgsMapToolPan *m_mapToolPan;
	QgsMapToolZoom *m_mapToolZoomIn;
	QgsMapToolZoom *m_mapToolZoomOut;

protected:
	void initStatusBar();
	void initMapCanvas();
	void initMapTools();

	void initQgsMapLayerTreeView();
	void initQgsMapLayerPropertyTableView();

	void addVectorLayer();
	void addRasterLayer();

	void updateQgsMapLayerView();
	void updateQgsVectorLayerFeatureView(QgsVectorLayer* layer,QStandardItem* parent);
	void insertQgsMapLayerItem(QStandardItemModel* model, QgsMapLayer* layer);
	void insertQgsVectorLayerFeatureItem(QgsFeature& feature, QStandardItem* parent);

protected slots:
	void showCursorCoor(QgsPointXY qgsPoint);
	void on_openVectorLayerAction_triggered();
	void on_openRasterLayerAction_triggered();

	void on_selectAction_triggered();
	void on_zoomInAction_triggered();
	void on_zoomOutAction_triggered();
	void on_panAction_triggered();
};
