#pragma once

#include <QtWidgets/QMainWindow>
//#include "ui_sqGis.h"
#include <qmenu.h>
#include <qaction.h>
#include <qgsmapcanvas.h>

class sqGis : public QMainWindow
{
	Q_OBJECT

public:
	sqGis(QWidget *parent = Q_NULLPTR);

private:
	//Ui::sqGisClass ui;
	QMenu *fileMenu;
	QAction *openFileAction;

	//map canvas
	QgsMapCanvas *mapCanvas;
	QList<QgsMapLayer *> layers;

	public slots:
	void on_openFileAction_triggered();
	//

public:
	void addVectorLayer();
};
