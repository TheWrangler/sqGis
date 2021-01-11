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
	LayerManagerFrame* _layerManagerFrame;
	QTableView* _layerPropertyTableView;
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
	QgsMapToolPan *_mapToolPan;//�϶�
	QgsMapToolZoom *_mapToolZoomIn;//�Ŵ�
	QgsMapToolZoom *_mapToolZoomOut;//��С

	//��湤��
	QgsMapToolEmitPoint *_mapToolPoint;//����

	//���ͼ�����
	unsigned int _pointsMarkLayerNum;//�������Ŀ
	unsigned int _linesMarkLayerNum;//���ֱ�����Ŀ
	unsigned int _polygonsMarkLayerNum;//����α�����Ŀ

protected:
	void initDockWidgets();
	void initStatusBar();
	void initMapCanvas();
	void initMapTools();

	//�������㣬ʹ��Memory provider
	void createPointsMarkLayer(QString layerName);
	void createLinesMarkLayer(QString layerName);
	void createPolygonMarkLayer(QString layerName);

protected slots:
	void refreshMapCanvas();
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
};
