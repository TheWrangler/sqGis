#include "sqGis.h"
//#include <QtWidgets/QApplication>

#include <qgsapplication.h>

int main(int argc, char *argv[])
{
	/*QApplication a(argc, argv);
	sqGis w;
	w.show();
	return a.exec();*/

	QgsApplication a(argc, argv, true);
	QgsApplication::setPrefixPath("C:/OSGeo4W/apps/qgis", true);
	QgsApplication::initQgis();    //初始化QGIS应用
	sqGis w;    //创建一个窗体，类似于Qt
	w.show();

	return a.exec();
}
