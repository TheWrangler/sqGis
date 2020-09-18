#include "sqGis.h"
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qgsvectorlayer.h>

sqGis::sqGis(QWidget *parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);

	this->resize(600, 400);

	// create the menus and then add the actions to them.
	fileMenu = this->menuBar()->addMenu("File");
	openFileAction = new QAction("Open", this);
	this->connect(openFileAction, SIGNAL(triggered(bool)), this, SLOT(on_openFileAction_triggered()));
	fileMenu->addAction(openFileAction);

	// initialize the map canvas
	mapCanvas = new QgsMapCanvas();
	this->setCentralWidget(mapCanvas);

	mapCanvas->setCanvasColor(QColor(255, 255, 255));
	mapCanvas->setVisible(true);
	mapCanvas->enableAntiAliasing(true);
}

void sqGis::on_openFileAction_triggered() {
	addVectorLayer();
}

void sqGis::addVectorLayer()
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
	mapCanvas->setExtent(vecLayer->extent());
	layers.append(vecLayer);
	mapCanvas->setLayers(layers);
	mapCanvas->refresh();
}