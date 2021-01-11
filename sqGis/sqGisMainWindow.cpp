#include "sqGisMainWindow.h"
#include "ConvertCoorDlg.h"
#include "GeometryEditDlg.h"
#include "options.h"
#include "AboutDlg.h"

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
	
	_layerPropertyTableView = new QTableView(this);
	_logTextEdit = new LogTextEdit(this);

	initDockWidgets();
	initStatusBar();

	_mapLayerManager = new MapLayerManager();
	_layerManagerFrame->attachMapLayerManager(_mapLayerManager);

	initMapCanvas();
	initMapTools();
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

	QDockWidget* dockWidget2 = new QDockWidget(QStringLiteral("图层属性"), this);
	dockWidget2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dockWidget2->setWidget(_layerPropertyTableView);
	addDockWidget(Qt::LeftDockWidgetArea, dockWidget2);

	QDockWidget* dockWidget3 = new QDockWidget(QStringLiteral("过程信息"), this);
	dockWidget3->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dockWidget3->setWidget(_logTextEdit);
	addDockWidget(Qt::LeftDockWidgetArea, dockWidget3);

	tabifyDockWidget(dockWidget2, dockWidget3);
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

	_pointsMarkLayerNum = 0;
	_linesMarkLayerNum = 0;
	_polygonsMarkLayerNum = 0;
}

void sqGisMainWindow::initMapTools()
{
	//canvas tools
	_mapToolPan = new QgsMapToolPan(_mapCanvas);
	_mapToolPan->setAction(ui.panAction);

	_mapToolZoomIn = new QgsMapToolZoom(_mapCanvas,false);
	_mapToolZoomIn->setAction(ui.zoomInAction);

	_mapToolZoomOut = new QgsMapToolZoom(_mapCanvas,true);
	_mapToolZoomOut->setAction(ui.zoomOutAction);

	_mapToolPoint = new QgsMapToolEmitPoint(_mapCanvas);
	_mapToolPoint->setAction(ui.markPointAction);

	//连接鼠标在画布上移动的信号
	connect(_mapCanvas, SIGNAL(xyCoordinates(QgsPointXY)), this, SLOT(showCursorCoor(QgsPointXY)));

	//连接标绘地理点的信号
	connect(_mapToolPoint, SIGNAL(canvasClicked(const QgsPointXY &, Qt::MouseButton)),this, SLOT(addPointMark(const QgsPointXY &, Qt::MouseButton)));
}

void sqGisMainWindow::createPointsMarkLayer(QString layerName)
{
	QgsVectorLayer* layer = new QgsVectorLayer("Point?crs=epsg:3857&index=yes", layerName, "memory");
	QgsVectorDataProvider* provider = layer->dataProvider();
	if (!layer->isValid())
	{
		QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("初始化点标绘图层失败!\n"));
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("初始化标绘图层") << layerName << QStringLiteral("失败!");
#endif
		return;
	}

	_pointsMarkLayerNum++;

	QList<QgsField> fields;
	QgsField("name", QVariant::String);
	QgsField("type", QVariant::String);
	QgsField("height", QVariant::Double);
	provider->addAttributes(fields);

	_layerManagerFrame->addMapLayerToView(layer);
}

void sqGisMainWindow::createLinesMarkLayer(QString layerName)
{
	QgsVectorLayer* layer = new QgsVectorLayer("Line?crs=epsg:3857&index=yes", layerName, "memory");
	QgsVectorDataProvider* provider = layer->dataProvider();
	if (!layer->isValid())
	{
		QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("初始化线标绘图层失败!\n"));
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("初始化标绘图层") << layerName << QStringLiteral("失败!");
#endif
		return;
	}

	_linesMarkLayerNum++;

	QList<QgsField> fields;
	QgsField("name", QVariant::String);
	QgsField("type", QVariant::String);
	QgsField("height", QVariant::Double);
	provider->addAttributes(fields);

	_layerManagerFrame->addMapLayerToView(layer);
}

void sqGisMainWindow::createPolygonMarkLayer(QString layerName)
{

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
#if PROMPT_DEBUG_MSG
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

void sqGisMainWindow::addPointMark(const QgsPointXY & pt, Qt::MouseButton button)
{
	GeometryEditDlg dlg;
	dlg.attachGeometry(NULL);
	if (dlg.exec() != QDialog::Accepted)
		return;

	//QgsGeometry


}

void sqGisMainWindow::on_newPointsMarkLayerAction_triggered()
{
	bool isOK;
	QString preName = QStringLiteral("点标绘图层") + QString::number(_pointsMarkLayerNum + 1, 10);
	QString layerName;

	do
	{
		layerName = QInputDialog::getText(
			this,
			QStringLiteral("新建点标绘图层"),
			QStringLiteral("请输入图层名"),
			QLineEdit::Normal,
			preName,
			&isOK
		);
	} while (_mapLayerManager->isLayerExist(layerName));
	

	if (!isOK)
		return;

	createPointsMarkLayer(layerName);
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
}

void sqGisMainWindow::on_openPostGisLayerAction_triggered()
{
	//QgsDataSourceUri uri;
	//uri.setConnection("localhost","5432",)
}

void sqGisMainWindow::on_removeLayerAction_triggered()
{
	_layerManagerFrame->on_removeLayerBtn_clicked();
}

void sqGisMainWindow::on_selectAction_triggered()
{
	_mapCanvas->unsetMapTool(_mapCanvas->mapTool());
	_mapCanvas->unsetCursor();
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
	_mapCanvas->setMapTool(_mapToolPoint);
	_mapCanvas->setCursor(QPixmap(":/img/position_x24"));
}

void sqGisMainWindow::on_markLineAction_triggered()
{

}

void sqGisMainWindow::on_markPolygonAction_triggered()
{

}

void sqGisMainWindow::on_convertCoorAction_triggered()
{
	ConvertCoorDlg dlg;
	dlg.exec();
}

void sqGisMainWindow::on_aboutAction_triggered()
{
	AboutDlg dlg;
	dlg.exec();
}