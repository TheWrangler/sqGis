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
	QgsApplication::initQgis();    //��ʼ��QGISӦ��
	sqGis w;    //����һ�����壬������Qt
	w.show();

	return a.exec();
}
