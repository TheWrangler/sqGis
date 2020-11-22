#include "sqGisMainWindow.h"
#include "ConvertCoorDlg.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QList>
#include <QTreeView>
#include <QStandardItemModel>
#include <QIcon>
#include <QLabel>

#include <qgis.h>
#include <qgsvectorlayer.h>
#include <qgsrasterlayer.h>
#include <qgsgeometry.h>
#include <qgsfeatureid.h>

sqGisMainWindow::sqGisMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	initStatusBar();

	initQgsMapLayerTreeView();

	initMapCanvas();
	initMapTools();
}

sqGisMainWindow::~sqGisMainWindow()
{
}

void sqGisMainWindow::initStatusBar()
{
	m_uiCursorCoorLabel = new QLabel("Lon=?, Lat=?");
	m_uiCursorCoorLabel->setAlignment(Qt::AlignCenter);
	m_uiCursorCoorLabel->setMinimumSize(m_uiCursorCoorLabel->sizeHint());

	statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
	statusBar()->setSizeGripEnabled(false);
	statusBar()->addWidget(m_uiCursorCoorLabel);
}

void sqGisMainWindow::initQgsMapLayerTreeView()
{
	QStandardItemModel* model = new QStandardItemModel(ui.qgsMapLayerTreeView);
	model->setHorizontalHeaderLabels(QStringList() << QStringLiteral("图层名称") << QStringLiteral("可见性"));
	ui.qgsMapLayerTreeView->setModel(model);
}

void sqGisMainWindow::initQgsMapLayerPropertyTableView()
{

}

void sqGisMainWindow::initMapCanvas()
{
	m_mapCanvas = new QgsMapCanvas();
	this->setCentralWidget(m_mapCanvas);

	m_mapCanvas->setCanvasColor(QColor(255, 255, 255));
	m_mapCanvas->setVisible(true);
	m_mapCanvas->enableAntiAliasing(true);

	m_mapCanvas->setLayers(m_mapLayers);

	QgsPointXY center(12956100, 4845690);
	m_mapCanvas->setCenter(center);
}

void sqGisMainWindow::initMapTools()
{
	//canvas tools
	m_mapToolPan = new QgsMapToolPan(m_mapCanvas);
	m_mapToolPan->setAction(ui.panAction);

	m_mapToolZoomIn = new QgsMapToolZoom(m_mapCanvas,false);
	m_mapToolZoomIn->setAction(ui.zoomInAction);

	m_mapToolZoomOut = new QgsMapToolZoom(m_mapCanvas,true);
	m_mapToolZoomOut->setAction(ui.zoomOutAction);

	//connect canvas mouse coordinate signal
	connect(m_mapCanvas, SIGNAL(xyCoordinates(QgsPointXY)), this, SLOT(showCursorCoor(QgsPointXY)));
}

void sqGisMainWindow::updateMapLayerView()
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.qgsMapLayerTreeView->model());
	if (model->hasChildren())
		model->removeRows(0, model->rowCount());

	QList<QgsMapLayer*>::iterator it = m_mapLayers.begin();
	while (it != m_mapLayers.end())
	{
		insertLayerToView(*it);
		it++;
	}
}

void sqGisMainWindow::addMapLayerToView(QgsMapLayer* layer)
{
	m_mapLayers.append(layer);
	m_mapCanvas->setExtent(layer->extent());
	m_mapCanvas->setLayers(m_mapLayers);
	m_mapCanvas->refresh();

	insertLayerToView(layer);
}

void sqGisMainWindow::insertLayerToView(QgsMapLayer* layer)
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.qgsMapLayerTreeView->model());

	QStandardItem * item = new QStandardItem(QString(layer->name()));
	model->appendRow(item);
	//int idx = model->rowCount();
	//model->setItem(idx - 1, 1, new QStandardItem(layer->name()));
	//model->setItem(idx - 1, 2, new QStandardItem((layer->crs()).description()));

	QgsMapLayerType type = layer->type();
	switch (type)
	{
		case QgsMapLayerType::VectorLayer :
			item->setIcon(QIcon(":/img/vector_x16"));
			//updateQgsVectorLayerFeatureView(dynamic_cast<QgsVectorLayer*>(layer),item);
			break;
		case QgsMapLayerType::RasterLayer :
			item->setIcon(QIcon(":/img/raster_x16"));
			break;
		case QgsMapLayerType::MeshLayer:
			item->setIcon(QIcon(":/img/vector_layer.png"));
			break;
		case QgsMapLayerType::PluginLayer:
			item->setIcon(QIcon(":/img/vector_layer.png"));
			break;
	}
}

void sqGisMainWindow::insertQgsVectorLayerFeatureItem(QgsFeature& feature, QStandardItem* parent)
{
	QStandardItem * item = new QStandardItem(feature.id());
	parent->appendRow(item);
	int idx = parent->rowCount();
	

	QgsGeometry geom = feature.geometry();
	switch (geom.type())
	{
		case QgsWkbTypes::PointGeometry:
			break;
		case QgsWkbTypes::LineGeometry:
			break;
		case QgsWkbTypes::PolygonGeometry:
			break;
		default:
			break;
	}
}

void sqGisMainWindow::showCursorCoor(QgsPointXY qgsPoint)
{
	qDebug() << "cursor: " << qgsPoint.x() << "," << qgsPoint.y();

	QgsCoordinateReferenceSystem dstCrs("EPSG:4326");
	QgsCoordinateReferenceSystem srcCrs("EPSG:3857");
	QgsCoordinateTransform crsTrans;
	crsTrans.setSourceCrs(srcCrs);
	crsTrans.setDestinationCrs(dstCrs);
	QgsPointXY pt = crsTrans.transform(qgsPoint);
	m_uiCursorCoorLabel->setText(pt.toString());
}

void sqGisMainWindow::on_openVectorLayerAction_triggered()
{
	QFileDialog fileDialog;
	fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog.setViewMode(QFileDialog::Detail);
	fileDialog.setFileMode(QFileDialog::ExistingFile);
	fileDialog.setWindowTitle(QStringLiteral("请选择矢量文件"));
	//fileDialog.setDefaultSuffix("xml");
	fileDialog.setNameFilter(QStringLiteral("矢量文件(*.shp)"));
	if (fileDialog.exec() != QDialog::Accepted)
		return;

	QString fileName = fileDialog.selectedFiles()[0];
	QgsVectorLayer* layer = new QgsVectorLayer(fileName, fileName, "ogr");

	//矢量地图数据一般为WGS84系，EPSG:4326

	if (!layer->isValid())
	{
		QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("初始化图层失败!\n") + fileName);
		return;
	}

	addMapLayerToView(layer);
}

void sqGisMainWindow::on_openRasterLayerAction_triggered()
{
	QFileDialog fileDialog;
	fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog.setViewMode(QFileDialog::Detail);
	fileDialog.setFileMode(QFileDialog::ExistingFile);
	fileDialog.setWindowTitle(QStringLiteral("请选择栅格文件"));
	//fileDialog.setDefaultSuffix("xml");
	fileDialog.setNameFilter(QStringLiteral("栅格文件(*.tif)"));
	if (fileDialog.exec() != QDialog::Accepted)
		return;

	QString fileName = fileDialog.selectedFiles()[0];
	QgsRasterLayer* layer = new QgsRasterLayer(fileName, fileName, "gdal");


	if (!layer->isValid())
	{
		QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("初始化图层失败!\n") + fileName);
		return;
	}

	addMapLayerToView(layer);
}

void sqGisMainWindow::on_openLocalTilesLayerAction_triggered()
{
	QFileDialog fileDialog;
	fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog.setViewMode(QFileDialog::Detail);
	fileDialog.setFileMode(QFileDialog::ExistingFile);
	fileDialog.setWindowTitle(QStringLiteral("请选择TMS服务配置文件"));
	//fileDialog.setDefaultSuffix("xml");
	fileDialog.setNameFilter(QStringLiteral("XML文件(*.xml)"));
	if (fileDialog.exec() != QDialog::Accepted)
		return;

	QString filename = fileDialog.selectedFiles()[0];
	qDebug() << "Local Tiles TMS XML:" << filename;
	//QFileInfo fi(filename);
	//QString basename = fi.baseName();
	QgsRasterLayer* layer = new QgsRasterLayer(filename, filename);

	if (!layer->isValid())
	{
		QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("初始化图层失败!\n") + filename);
		return;
	}

	addMapLayerToView(layer);
}

void sqGisMainWindow::on_openOpenStreetMapLayerAction_triggered()
{
	QString filename = QCoreApplication::applicationDirPath() + "/tms/openstreetmap_tms.xml";
	qDebug() << "OpenStreetMap TMS XML:" << filename;
	QgsRasterLayer* layer = new QgsRasterLayer(filename, filename);

	if (!layer->isValid())
	{
		QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("初始化图层失败!\n") + filename);
		return;
	}

	addMapLayerToView(layer);
}

void sqGisMainWindow::on_openPostGisLayerAction_triggered()
{
	//QgsDataSourceUri uri;
	//uri.setConnection("localhost","5432",)
}

void sqGisMainWindow::on_removeLayerAction_triggered()
{
	on_removeLayerBtn_clicked();
}

void sqGisMainWindow::on_selectAction_triggered()
{
	m_mapCanvas->unsetMapTool(m_mapCanvas->mapTool());
	m_mapCanvas->unsetCursor();
}

void sqGisMainWindow::on_zoomInAction_triggered()
{
	m_mapCanvas->setMapTool(m_mapToolZoomIn);
	m_mapCanvas->setCursor(QPixmap(":/img/zoom-in_x24"));
}

void sqGisMainWindow::on_zoomOutAction_triggered()
{
	m_mapCanvas->setMapTool(m_mapToolZoomOut);
	m_mapCanvas->setCursor(QPixmap(":/img/zoom-out_x24"));
}

void sqGisMainWindow::on_panAction_triggered()
{
	m_mapCanvas->setMapTool(m_mapToolPan);
}

void sqGisMainWindow::on_convertCoorAction_triggered()
{
	ConvertCoorDlg dlg;
	//dlg.setWindowModality(Qt::WindowModal);
	dlg.exec();
}

void sqGisMainWindow::on_upLayerBtn_clicked()
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.qgsMapLayerTreeView->model());
	int row = ui.qgsMapLayerTreeView->currentIndex().row();
	QModelIndex index = model->index(row, 0);
	QString layerName = model->data(index).toString();

	qDebug() << "Selected layer in Layer TreeView:" << layerName;

	QList<QgsMapLayer*>::iterator itr = m_mapLayers.begin();
	int layerIndex = 0;
	while (itr != m_mapLayers.end())
	{
		if ((*itr)->name() == layerName)
			break;

		layerIndex++;
		itr++;
	}

	if (layerIndex == 0)
		return;

	qDebug() << "Selected layer index in Layer TreeView:" << layerIndex;
	m_mapLayers.swap(layerIndex - 1, layerIndex);
	updateMapLayerView();

	m_mapCanvas->setLayers(m_mapLayers);
	m_mapCanvas->refresh();
}

void sqGisMainWindow::on_downLayerBtn_clicked()
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.qgsMapLayerTreeView->model());
	int row = ui.qgsMapLayerTreeView->currentIndex().row();
	QModelIndex index = model->index(row, 0);
	QString layerName = model->data(index).toString();

	qDebug() << "Selected layer in Layer TreeView:" << layerName;

	QList<QgsMapLayer*>::iterator itr = m_mapLayers.begin();
	int layerIndex = 0;
	while (itr != m_mapLayers.end())
	{
		if ((*itr)->name() == layerName)
			break;

		layerIndex++;
		itr++;
	}

	if (layerIndex == m_mapLayers.count()-1)
		return;

	qDebug() << "Selected layer index in Layer TreeView:" << layerIndex;
	m_mapLayers.swap(layerIndex, layerIndex+1);
	updateMapLayerView();

	m_mapCanvas->setLayers(m_mapLayers);
	m_mapCanvas->refresh();
}

void sqGisMainWindow::on_removeLayerBtn_clicked()
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.qgsMapLayerTreeView->model());
	int row = ui.qgsMapLayerTreeView->currentIndex().row();
	QModelIndex index = model->index(row, 0);
	QString layerName = model->data(index).toString();

	qDebug() << "Selected layer in Layer TreeView:" << layerName;

	QList<QgsMapLayer*>::iterator itr = m_mapLayers.begin();
	int layerIndex = 0;
	while (itr != m_mapLayers.end())
	{
		if ((*itr)->name() == layerName)
			break;

		layerIndex++;
		itr++;
	}
	
	if (itr == m_mapLayers.end())
		return;

	m_mapLayers.removeAt(layerIndex);
	updateMapLayerView();

	m_mapCanvas->setLayers(m_mapLayers);
	m_mapCanvas->refresh();
}

void sqGisMainWindow::on_visibleLayerBtn_clicked()
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.qgsMapLayerTreeView->model());
	int row = ui.qgsMapLayerTreeView->currentIndex().row();
	QModelIndex index = model->index(row, 0);
	QString layerName = model->data(index).toString();

	qDebug() << "Selected layer in Layer TreeView:" << layerName;

	QList<QgsMapLayer*>::iterator itr = m_mapLayers.begin();
	int layerIndex = 0;
	while (itr != m_mapLayers.end())
	{
		if ((*itr)->name() == layerName)
			break;

		layerIndex++;
		itr++;
	}

	if (itr == m_mapLayers.end())
		return;
}
