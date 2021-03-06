#include <QFile>

#include <qgsapplication.h>
#include <qgsproviderregistry.h>

#include "sqGisMainWindow.h"

int main(int argc, char *argv[])
{
	QgsApplication a(argc, argv, true);
	a.setWindowIcon(QPixmap(":/img/logo_x48"));

	QFile qss("F:/sqGis/Win32/Release/qss/Adaptic.qss");
	qss.open(QFile::ReadOnly);
	a.setStyleSheet(qss.readAll());
	qss.close();


	QgsApplication::setPrefixPath("D:/OSGeo4W/apps/qgis-ltr", true);
	QgsProviderRegistry::instance("D:/OSGeo4W/apps/qgis-ltr/plugins");
	QgsApplication::initQgis();

	sqGisMainWindow w;
	w.showMaximized();

	return a.exec();
}
