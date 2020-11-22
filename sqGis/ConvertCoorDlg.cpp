#include "ConvertCoorDlg.h"
#include <qgsgeometry.h>

ConvertCoorDlg::ConvertCoorDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

ConvertCoorDlg::~ConvertCoorDlg()
{
}

void ConvertCoorDlg::on_convertEPSG4326Btn_clicked()
{
	QgsCoordinateReferenceSystem dstCrs("EPSG:4326");
	QgsCoordinateReferenceSystem srcCrs("EPSG:3857");
	QgsCoordinateTransform crsTrans;
	crsTrans.setSourceCrs(srcCrs);
	crsTrans.setDestinationCrs(dstCrs);
	QgsPointXY originPt(ui.m_xEdit->text().toDouble(),ui.m_yEdit->text().toDouble());
	QgsPointXY pt = crsTrans.transform(originPt);
	ui.m_lonEdit->setText(QString::number(pt.x(), 10, 12));
	ui.m_latEdit->setText(QString::number(pt.y(), 10, 12));
}

void ConvertCoorDlg::on_convertEPSG3857Btn_clicked()
{
	QgsCoordinateReferenceSystem dstCrs("EPSG:3857");
	QgsCoordinateReferenceSystem srcCrs("EPSG:4326");
	QgsCoordinateTransform crsTrans;
	crsTrans.setSourceCrs(srcCrs);
	crsTrans.setDestinationCrs(dstCrs);
	QgsPointXY originPt(ui.m_lonEdit->text().toDouble(), ui.m_latEdit->text().toDouble());
	QgsPointXY pt = crsTrans.transform(originPt);
	ui.m_xEdit->setText(QString::number(pt.x(), 10, 12));
	ui.m_yEdit->setText(QString::number(pt.y(), 10, 12));
}
