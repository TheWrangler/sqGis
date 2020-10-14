#include "sqGisMainWindow.h"
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
	model->setHorizontalHeaderLabels(QStringList() << QStringLiteral("ID") << QStringLiteral("名称") << QStringLiteral("可见性"));
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

	//QgsCoordinateReferenceSystem crs("EPSG:4326");
	//m_mapCanvas->setDestinationCrs(crs);
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

void sqGisMainWindow::updateQgsMapLayerView()
{
	QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui.qgsMapLayerTreeView->model());
	if (model->hasChildren())
		model->removeRows(0, model->rowCount());

	QList<QgsMapLayer*>::iterator it = m_mapLayers.begin();
	while (it != m_mapLayers.end())
	{
		insertQgsMapLayerItem(model,*it);
		it++;
	}
}

void sqGisMainWindow::updateQgsVectorLayerFeatureView(QgsVectorLayer* layer, QStandardItem* parent)
{
	QgsFeatureIterator it =  layer->getFeatures();
	QgsFeature feature;
	while (it.nextFeature(feature))
	{
		if (!feature.hasGeometry())
			continue;
		insertQgsVectorLayerFeatureItem(feature, parent);
	}
}

void sqGisMainWindow::insertQgsMapLayerItem(QStandardItemModel* model,QgsMapLayer* layer)
{
	QStandardItem * item = new QStandardItem(QString(layer->id()));
	model->appendRow(item);
	int idx = model->rowCount();
	model->setItem(idx - 1, 1, new QStandardItem(layer->name()));
	model->setItem(idx - 1, 2, new QStandardItem((layer->crs()).description()));

	QgsMapLayerType type = layer->type();
	switch (type)
	{
		case QgsMapLayerType::VectorLayer :
			item->setIcon(QIcon(":/img/vector_x16"));
			updateQgsVectorLayerFeatureView(dynamic_cast<QgsVectorLayer*>(layer),item);
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
	m_uiCursorCoorLabel->setText(qgsPoint.toString());
}

void sqGisMainWindow::on_openVectorLayerAction_triggered()
{
	addVectorLayer();
}

void sqGisMainWindow::on_openRasterLayerAction_triggered()
{
	addRasterLayer();
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

void sqGisMainWindow::addVectorLayer()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open shape file"), "", "*.shp");
	QStringList temp = fileName.split('/');
	QString basename = temp.at(temp.size() - 1);
	QgsVectorLayer* vecLayer = new QgsVectorLayer(fileName, basename, "ogr");

	if (!vecLayer->isValid())
	{
		QMessageBox::critical(this, "error", QString("layer is invalid: \n") + fileName);
		return;
	}

	m_mapCanvas->setExtent(vecLayer->extent());
	m_mapLayers.append(vecLayer);
	m_mapCanvas->setLayers(m_mapLayers);
	m_mapCanvas->refresh();

	updateQgsMapLayerView();
}

void sqGisMainWindow::addRasterLayer()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open raster file"), "", "*.tif");
	QStringList temp = fileName.split('/');
	QString basename = temp.at(temp.size() - 1);
	QgsRasterLayer* rasterLayer = new QgsRasterLayer(fileName, basename, "gdal");

	//QgsCoordinateReferenceSystem crs("EPSG:4326");
	QgsCoordinateReferenceSystem crs("EPSG:3857");
	rasterLayer->setCrs(crs);
	

	if (!rasterLayer->isValid())
	{
		QMessageBox::critical(this, "error", QString("layer is invalid: \n") + fileName);
		return;
	}

	m_mapCanvas->setExtent(rasterLayer->extent());
	m_mapLayers.append(rasterLayer);
	m_mapCanvas->setLayers(m_mapLayers);
	m_mapCanvas->refresh();

	updateQgsMapLayerView();
}
