#pragma once

#include <QMainWindow>
#include <QStandardItem>
#include <QLabel>
#include <QTreeView>
#include <QTableView>

#include "ui_sqGisMainWindow.h"
#include "MapLayerManager.h"
#include "LayerManagerFrame.h"
#include "./LogQt5/LogHandlerWrapper.h"
#include "./LogQt5/LogTextEdit.h"
#include "./MapMarkTool/MapToolMarkSelect.h"
#include "./MapMarkTool/MapToolMarkPoint.h"
#include "./MapMarkTool/MapToolMarkLine.h"
#include "./MapMarkTool/MapToolMarkPolygon.h"
#include "./MapMeasureTool/MapToolMeasureLine.h"

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
	LayerManagerFrame* _layerManagerFrame;
	LogTextEdit* _logTextEdit;

	//״̬����ʾ��ǩ
	QLabel *_uiTooltipLabel;
	QLabel *_uiCursorCoorLabel;

protected:
	//��ͼ����
	QgsMapCanvas *_mapCanvas;

	//ͼ�������
	MapLayerManager* _mapLayerManager;

	//��ͼ�鿴����
	MapToolMarkSelect *_mapToolMarkSelect;//ѡ��
	QgsMapToolPan *_mapToolPan;//�϶�
	QgsMapToolZoom *_mapToolZoomIn;//�Ŵ�
	QgsMapToolZoom *_mapToolZoomOut;//��С

	//��湤��
	MapToolMarkPoint *_mapToolMarkPoint;//����
	MapToolMarkLine *_mapToolMarkLine; //�������
	MapToolMarkPolygon *_mapToolMarkPolygon;//�������

	//��������
	MapToolMeasureLine *_mapToolMeasureDistance;//�߲�������

protected:
	void initDockWidgets();
	void initStatusBar();
	void initMapCanvas();
	void initMapTools();

protected slots:
	void refreshMapCanvas(QgsMapLayer* layer = NULL);
	void showCursorCoor(QgsPointXY qgsPoint);
	void addLayerMark(QgsMapLayer* layer, QgsWkbTypes::GeometryType type, const QVector<QgsPointXY>& points);

	void tick_triggered();

	void on_openVectorLayerAction_triggered();
	void on_openRasterLayerAction_triggered();
	void on_openLocalTilesLayerAction_triggered();
	void on_openOpenStreetMapLayerAction_triggered();
	void on_removeLayerAction_triggered();

	void on_selectAction_triggered();
	void on_zoomInAction_triggered();
	void on_zoomOutAction_triggered();
	void on_panAction_triggered();

	void on_markPointAction_triggered();
	void on_markLineAction_triggered();
	void on_markPolygonAction_triggered();

	void on_distanceAngleMeasureAction_triggered();

	void on_convertCoorAction_triggered();
	void on_navMsgReceiveAction_triggered();

	void on_aboutAction_triggered();
};
