#pragma once

#include <QMainWindow>
#include <QStandardItem>
#include <QLabel>

#include "ui_sqGisMainWindow.h"
#include "MapLayerManager.h"
#include "PointMapTool.h"

#include <qgsmapcanvas.h>
#include <qgsmaplayer.h>
#include <qgsfeature.h>
#include <qgsmaptoolpan.h>
#include <qgsmaptoolzoom.h>

class sqGisMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	sqGisMainWindow(QWidget *parent = Q_NULLPTR);
	~sqGisMainWindow();

private:
	Ui::sqGisMainWindow ui;

	//状态栏显示标签
	QLabel *_uiTooltipLabel;
	QLabel *_uiCursorCoorLabel;

protected:
	//地图画布
	QgsMapCanvas *_mapCanvas;

	//图层管理器
	MapLayerManager* _mapLayerManager;

	//地图查看工具
	QgsMapToolPan *_mapToolPan;//拖动
	QgsMapToolZoom *_mapToolZoomIn;//放大
	QgsMapToolZoom *_mapToolZoomOut;//缩小

	//标绘工具
	QgsMapToolEmitPoint *_mapToolPoint;//标绘点

	//标绘图层计数
	unsigned int _pointsMarkLayerNum;//点标绘层数目
	unsigned int _linesMarkLayerNum;//折现标绘层数目
	unsigned int _polygonsMarkLayerNum;//多边形标绘层数目

protected:
	void initStatusBar();
	void initMapCanvas();
	void initMapTools();

	//创建标绘层，使用Memory provider
	void createPointsMarkLayer(QString layerName);
	void createLinesMarkLayer(QString layerName);
	void createPolygonMarkLayer(QString layerName);

protected slots:
	void showCursorCoor(QgsPointXY qgsPoint);
	void addPointMark(const QgsPointXY & pt, Qt::MouseButton button);

	void on_newPointsMarkLayerAction_triggered();

	void on_openVectorLayerAction_triggered();
	void on_openRasterLayerAction_triggered();
	void on_openLocalTilesLayerAction_triggered();
	void on_openOpenStreetMapLayerAction_triggered();
	void on_openPostGisLayerAction_triggered();
	void on_removeLayerAction_triggered();

	void on_selectAction_triggered();
	void on_zoomInAction_triggered();
	void on_zoomOutAction_triggered();
	void on_panAction_triggered();

	void on_markPointAction_triggered();
	void on_markLineAction_triggered();
	void on_markPolygonAction_triggered();

	void on_convertCoorAction_triggered();

	void on_upLayerBtn_clicked();
	void on_downLayerBtn_clicked();
	void on_removeLayerBtn_clicked();
	void on_visibleLayerBtn_clicked();
};
