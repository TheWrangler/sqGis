#include <QFile>

#include <qgsapplication.h>
#include "sqGisMainWindow.h"

int main(int argc, char *argv[])
{
	QgsApplication a(argc, argv, true);

	QFile qss("F:/sqGis/Win32/Release/qss/Integrid.qss");
	qss.open(QFile::ReadOnly);
	a.setStyleSheet(qss.readAll());
	qss.close();


	QgsApplication::setPrefixPath("D:/OSGeo4W/apps/qgis-ltr", true);
	QgsApplication::initQgis();

	sqGisMainWindow w;
	w.showMaximized();

	return a.exec();
}
