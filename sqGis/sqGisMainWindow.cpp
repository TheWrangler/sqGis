#include "sqGisMainWindow.h"
#include "ConvertCoorDlg.h"
#include "NavMsgReceiverDlg.h"
#include "FeatureEditDlg.h"
#include "options.h"
#include "AboutDlg.h"
#include "./MapMark/MarkLayer.h"
#include "./MapMark/PointMarkLayer.h"
#include "./MapMark/MarkFeature.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QList>
#include <QTreeView>
#include <QStandardItemModel>
#include <QIcon>
#include <QLabel>
#include <QInputDialog>
#include <QDockWidget>

#include <qgis.h>
#include <qgsvectorlayer.h>
#include <qgsrasterlayer.h>
#include <qgsgeometry.h>
#include <qgsfeatureid.h>

sqGisMainWindow::sqGisMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	_layerManagerFrame = new LayerManagerFrame(this);
	connect(_layerManagerFrame, SIGNAL(layersChanged(QgsMapLayer*)), this, SLOT(refreshMapCanvas(QgsMapLayer*)));
	
	_logTextEdit = new LogTextEdit(this);

	initDockWidgets();
	initStatusBar();

	_mapLayerManager = new MapLayerManager();
	_layerManagerFrame->attachMapLayerManager(_mapLayerManager);

	initMapCanvas();
	initMapTools();


	//QTimer *timer = new QTimer(this);
	//connect(timer, SIGNAL(timeout()), this, SLOT(tick_triggered()));
	//timer->start(300);
}

sqGisMainWindow::~sqGisMainWindow()
{
}

void sqGisMainWindow::initDockWidgets()
{
	setDockNestingEnabled(true);

	QDockWidget* dockWidget1 = new QDockWidget(QStringLiteral("图层管理"),this);
	dockWidget1->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dockWidget1->setWidget(_layerManagerFrame);
	addDockWidget(Qt::LeftDockWidgetArea, dockWidget1);

	QDockWidget* dockWidget2 = new QDockWidget(QStringLiteral("过程信息"), this);
	dockWidget2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dockWidget2->setWidget(_logTextEdit);
	addDockWidget(Qt::LeftDockWidgetArea, dockWidget2);

	tabifyDockWidget(dockWidget2, dockWidget1);
}

void sqGisMainWindow::initStatusBar()
{
	statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
	statusBar()->setSizeGripEnabled(false);

	_uiTooltipLabel = new QLabel();
	_uiTooltipLabel->setMinimumSize(_uiTooltipLabel->sizeHint());
	_uiTooltipLabel->setAlignment(Qt::AlignHCenter);
	statusBar()->addWidget(_uiTooltipLabel);

	_uiCursorCoorLabel = new QLabel("Lon=?, Lat=?");
	_uiCursorCoorLabel->setAlignment(Qt::AlignCenter);
	_uiCursorCoorLabel->setMinimumSize(_uiCursorCoorLabel->sizeHint());
	statusBar()->addWidget(_uiCursorCoorLabel);
}

void sqGisMainWindow::initMapCanvas()
{
	_mapCanvas = new QgsMapCanvas();
	this->setCentralWidget(_mapCanvas);

	_mapCanvas->setCanvasColor(QColor(255, 255, 255));
	_mapCanvas->setVisible(true);
	_mapCanvas->enableAntiAliasing(true);

	_mapCanvas->setLayers(_mapLayerManager->getMapLayers());

	QgsPointXY center(12956100, 4845690);
	_mapCanvas->setCenter(center);
}

void sqGisMainWindow::initMapTools()
{
	//canvas tools
	_mapToolMarkSelect = new MapToolMarkSelect(_mapCanvas);
	_mapToolMarkSelect->setAction(ui.selectAction);

	_mapToolPan = new QgsMapToolPan(_mapCanvas);
	_mapToolPan->setAction(ui.panAction);

	_mapToolZoomIn = new QgsMapToolZoom(_mapCanvas,false);
	_mapToolZoomIn->setAction(ui.zoomInAction);

	_mapToolZoomOut = new QgsMapToolZoom(_mapCanvas,true);
	_mapToolZoomOut->setAction(ui.zoomOutAction);

	_mapToolMarkPoint = new MapToolMarkPoint(_mapCanvas,NULL);
	_mapToolMarkPoint->setAction(ui.markPointAction);

	_mapToolMarkLine = new MapToolMarkLine(_mapCanvas,NULL);
	_mapToolMarkLine->setAction(ui.markLineAction);

	_mapToolMarkPolygon = new MapToolMarkPolygon(_mapCanvas, NULL);
	_mapToolMarkPolygon->setAction(ui.markPolygonAction);

	//连接鼠标在画布上移动的信号
	connect(_mapCanvas, SIGNAL(xyCoordinates(QgsPointXY)), this, SLOT(showCursorCoor(QgsPointXY)));

	//连接标绘信号
	connect(_mapToolMarkPoint, SIGNAL(markAdded(QgsMapLayer*, QgsWkbTypes::GeometryType, const QVector<QgsPointXY>&)),
		this, SLOT(addLayerMark(QgsMapLayer*, QgsWkbTypes::GeometryType, const QVector<QgsPointXY>&)));
	connect(_mapToolMarkLine, SIGNAL(markAdded(QgsMapLayer*, QgsWkbTypes::GeometryType, const QVector<QgsPointXY>&)), 
		this, SLOT(addLayerMark(QgsMapLayer*, QgsWkbTypes::GeometryType, const QVector<QgsPointXY>&)));
	connect(_mapToolMarkPolygon, SIGNAL(markAdded(QgsMapLayer*, QgsWkbTypes::GeometryType, const QVector<QgsPointXY>&)), 
		this, SLOT(addLayerMark(QgsMapLayer*, QgsWkbTypes::GeometryType, const QVector<QgsPointXY>&)));
}

void sqGisMainWindow::refreshMapCanvas(QgsMapLayer* layer)
{
	if (layer != NULL)
		_mapCanvas->setExtent(layer->extent());

	_mapCanvas->setLayers(_mapLayerManager->getMapLayers());
	_mapCanvas->refresh();

#if PROMPT_DEBUG_MSG
	qDebug() << QStringLiteral("刷新地图画布");
#endif
}

void sqGisMainWindow::showCursorCoor(QgsPointXY qgsPoint)
{
#if PROMPT_DEBUG_MSG & PROMPT_MOUSE_TRACK
	qDebug() << QStringLiteral("光标坐标: ") << qgsPoint.x() << "," << qgsPoint.y();
#endif

	QgsCoordinateReferenceSystem dstCrs("EPSG:4326");
	QgsCoordinateReferenceSystem srcCrs("EPSG:3857");
	QgsCoordinateTransform crsTrans;
	crsTrans.setSourceCrs(srcCrs);
	crsTrans.setDestinationCrs(dstCrs);
	QgsPointXY pt = crsTrans.transform(qgsPoint);
	_uiCursorCoorLabel->setText(pt.toString());
}

void sqGisMainWindow::addLayerMark(QgsMapLayer* layer, QgsWkbTypes::GeometryType type, const QVector<QgsPointXY>& points)
{
	QVector<QgsPoint> geoPoints;
	QVector<const QgsPointXY>::iterator it = points.begin();
	while (it != points.end())
	{
		QgsPoint point((*it).x(), (*it).y(), 0);
		geoPoints.push_back(point);

		it++;
	}

	MarkLayer* markLayer = dynamic_cast<MarkLayer*>(layer);
	if (markLayer == NULL)
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("转换为标绘图层") << layer->name() << QStringLiteral("失败!");
#endif
	}

	MarkFeature* feature = markLayer->appendMark(geoPoints);
	markLayer->triggerRepaint();

	_layerManagerFrame->updateLayerFeatureView(markLayer);

	FeatureEditDlg dlg;
	dlg.attachFeatureSettings(feature);
	if (dlg.exec() != QDialog::Accepted)
	{
		delete feature;
		return;
	}

	delete feature;
}

void sqGisMainWindow::tick_triggered()
{
	static double lon = 104.0;

	PointMarkLayer* layer = (PointMarkLayer*)(_mapLayerManager->getMapLayer(QStringLiteral("点标绘图层")));
	if (layer == NULL)
		return;

	QgsPointXY src(lon, 38.0);
	QgsPointXY dst = FeatureEditDlg::convertCoor(src,false);
	QVector<QgsPoint> points;
	points.append(QgsPoint(dst.x(), dst.y(), 0));
	layer->startEditing();
	//layer->updateMarkGeometry(QStringLiteral("点"), points);
	layer->commitChanges();

	lon += 0.001;
}

void sqGisMainWindow::on_openVectorLayerAction_triggered()
{
	QFileDialog fileDialog;
	fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog.setViewMode(QFileDialog::Detail);
	fileDialog.setFileMode(QFileDialog::ExistingFile);
	fileDialog.setWindowTitle(QStringLiteral("请选择矢量文件"));
	fileDialog.setNameFilter(QStringLiteral("矢量文件(*.shp)"));
	if (fileDialog.exec() != QDialog::Accepted)
		return;

	QString fileName = fileDialog.selectedFiles()[0];
	QgsVectorLayer* layer = new QgsVectorLayer(fileName, fileName, "ogr");

	//矢量地图数据一般为WGS84系，EPSG:4326

	if (!layer->isValid())
	{
		QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("初始化图层失败!\n") + fileName);
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("初始化图层") << layer->name() << QStringLiteral("失败!");
#endif
		return;
	}

	_layerManagerFrame->addMapLayerToView(layer);
	refreshMapCanvas(layer);
}

void sqGisMainWindow::on_openRasterLayerAction_triggered()
{
	QFileDialog fileDialog;
	fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog.setViewMode(QFileDialog::Detail);
	fileDialog.setFileMode(QFileDialog::ExistingFile);
	fileDialog.setWindowTitle(QStringLiteral("请选择栅格文件"));
	fileDialog.setNameFilter(QStringLiteral("栅格文件(*.tif)"));
	if (fileDialog.exec() != QDialog::Accepted)
		return;

	QString fileName = fileDialog.selectedFiles()[0];
	QgsRasterLayer* layer = new QgsRasterLayer(fileName, fileName, "gdal");


	if (!layer->isValid())
	{
		QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("初始化图层失败!\n") + fileName);
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("初始化图层") << layer->name() << QStringLiteral("失败!");
#endif
		return;
	}

	_layerManagerFrame->addMapLayerToView(layer);
	refreshMapCanvas(layer);
}

void sqGisMainWindow::on_openLocalTilesLayerAction_triggered()
{
	QFileDialog fileDialog;
	fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog.setViewMode(QFileDialog::Detail);
	fileDialog.setFileMode(QFileDialog::ExistingFile);
	fileDialog.setWindowTitle(QStringLiteral("请选择TMS服务配置文件"));
	fileDialog.setNameFilter(QStringLiteral("TMS服务配置文件(*.xml)"));
	if (fileDialog.exec() != QDialog::Accepted)
		return;

	QString filename = fileDialog.selectedFiles()[0];
	QgsRasterLayer* layer = new QgsRasterLayer(filename, filename);

	if (!layer->isValid())
	{
		QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("初始化图层失败!\n") + filename);
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("初始化图层") << layer->name() << QStringLiteral("失败!");
#endif
		return;
	}

	_layerManagerFrame->addMapLayerToView(layer);
	refreshMapCanvas(layer);
}

void sqGisMainWindow::on_openOpenStreetMapLayerAction_triggered()
{
	QString filename = QCoreApplication::applicationDirPath() + "/tms/openstreetmap_online_tms.xml";
	QgsRasterLayer* layer = new QgsRasterLayer(filename, filename);

	if (!layer->isValid())
	{
		QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("初始化图层失败!\n") + filename);
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("初始化图层") << layer->name() << QStringLiteral("失败!");
#endif
		return;
	}

	_layerManagerFrame->addMapLayerToView(layer);
	refreshMapCanvas(layer);
}

void sqGisMainWindow::on_removeLayerAction_triggered()
{
	_layerManagerFrame->on_removeLayerBtn_clicked();
}

void sqGisMainWindow::on_selectAction_triggered()
{
	_mapCanvas->setMapTool(_mapToolMarkSelect);
}

void sqGisMainWindow::on_zoomInAction_triggered()
{
	_mapCanvas->setMapTool(_mapToolZoomIn);
	_mapCanvas->setCursor(QPixmap(":/img/zoom-in_x24"));
}

void sqGisMainWindow::on_zoomOutAction_triggered()
{
	_mapCanvas->setMapTool(_mapToolZoomOut);
	_mapCanvas->setCursor(QPixmap(":/img/zoom-out_x24"));
}

void sqGisMainWindow::on_panAction_triggered()
{
	_mapCanvas->setMapTool(_mapToolPan);
}

void sqGisMainWindow::on_markPointAction_triggered()
{
	MarkLayer* layer = (MarkLayer*)(_mapLayerManager->getMapLayer(QStringLiteral("点标绘图层")));
	if (layer == NULL)
	{
		layer = MarkLayer::createLayer(QgsWkbTypes::PointGeometry);
		_layerManagerFrame->addMapLayerToView(layer);
		refreshMapCanvas();
	}

	_mapToolMarkPoint->setDestMarkLayer(layer);
	_mapCanvas->setMapTool(_mapToolMarkPoint);
}

void sqGisMainWindow::on_markLineAction_triggered()
{
	MarkLayer* layer = (MarkLayer*)(_mapLayerManager->getMapLayer(QStringLiteral("线标绘图层")));
	if (layer == NULL)
	{
		layer = MarkLayer::createLayer(QgsWkbTypes::LineGeometry);
		_layerManagerFrame->addMapLayerToView(layer);
		refreshMapCanvas();
	}

	_mapToolMarkLine->setDestMarkLayer(layer);
	_mapCanvas->setMapTool(_mapToolMarkLine);
}

void sqGisMainWindow::on_markPolygonAction_triggered()
{
	MarkLayer* layer = (MarkLayer*)(_mapLayerManager->getMapLayer(QStringLiteral("多边形标绘图层")));
	if (layer == NULL)
	{
		layer = MarkLayer::createLayer(QgsWkbTypes::PolygonGeometry);
		_layerManagerFrame->addMapLayerToView(layer);
		refreshMapCanvas();
	}

	_mapToolMarkPolygon->setDestMarkLayer(layer);
	_mapCanvas->setMapTool(_mapToolMarkPolygon);
}

void sqGisMainWindow::on_convertCoorAction_triggered()
{
	ConvertCoorDlg dlg;
	dlg.exec();
}

void sqGisMainWindow::on_navMsgReceiveAction_triggered()
{
	NavMsgReceiverDlg dlg;
	dlg.exec();
}

void sqGisMainWindow::on_aboutAction_triggered()
{
	AboutDlg dlg;
	dlg.exec();
}