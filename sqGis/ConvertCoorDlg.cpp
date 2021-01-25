#include "ConvertCoorDlg.h"
#include "FeatureEditDlg.h"
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
	QgsPointXY originPt(ui.m_xEdit->text().toDouble(),ui.m_yEdit->text().toDouble());
	QgsPointXY pt = FeatureEditDlg::convertCoor(originPt,true);
	ui.m_lonEdit->setText(QString::number(pt.x(), 10, 12));
	ui.m_latEdit->setText(QString::number(pt.y(), 10, 12));
}

void ConvertCoorDlg::on_convertEPSG3857Btn_clicked()
{
	QgsPointXY originPt(ui.m_lonEdit->text().toDouble(), ui.m_latEdit->text().toDouble());
	QgsPointXY pt = FeatureEditDlg::convertCoor(originPt, false);
	ui.m_xEdit->setText(QString::number(pt.x(), 10, 12));
	ui.m_yEdit->setText(QString::number(pt.y(), 10, 12));
}
