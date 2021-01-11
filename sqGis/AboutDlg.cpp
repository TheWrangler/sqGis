#include "AboutDlg.h"

AboutDlg::AboutDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	initAppInfo();
	initOSGInfo();
	initQGisInfo();
	initQtInfo();
}

AboutDlg::~AboutDlg()
{
}

void AboutDlg::initAppInfo()
{
	QString filename = QCoreApplication::applicationDirPath() + "/resource/logo.png";
	QPixmap pixmap(filename);
	ui.logoLabel->setPixmap(pixmap);
	ui.logoLabel->show();
}

void AboutDlg::initOSGInfo()
{
	QString filename = QCoreApplication::applicationDirPath() + "/resource/osg.png";
	QPixmap pixmap(filename);
	ui.osgLabel->setPixmap(pixmap);
	ui.osgLabel->show();
}

void AboutDlg::initQGisInfo()
{
	QString filename = QCoreApplication::applicationDirPath() + "/resource/qgis.png";
	QPixmap pixmap(filename);
	ui.qgisLabel->setPixmap(pixmap);
	ui.qgisLabel->show();
}

void AboutDlg::initQtInfo()
{
	QString filename = QCoreApplication::applicationDirPath() + "/resource/qt.png";
	QPixmap pixmap(filename);
	ui.qtLabel->setPixmap(pixmap);
	ui.qtLabel->show();
}
