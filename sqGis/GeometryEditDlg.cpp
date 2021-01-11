#include "GeometryEditDlg.h"
#include <QMessageBox>

GeometryEditDlg::GeometryEditDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.m_formatCheck, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged(int)));

	ui.m_okBtn->setIcon(QIcon(":/img/ok_x24"));
	ui.m_cancelBtn->setIcon(QIcon(":/img/cancel_x24"));

	m_pGeometry = NULL;
}

GeometryEditDlg::~GeometryEditDlg()
{

}

void GeometryEditDlg::on_m_okBtn_clicked()
{
	accept();
}

void GeometryEditDlg::on_m_cancelBtn_clicked()
{
	reject();
}

void GeometryEditDlg::onStateChanged(int state)
{
	
}

void GeometryEditDlg::attachGeometry(QgsGeometry* geometry)
{
	createBelongToLayerItem();
	createGeoTypeItem();
	createGeoNameItem();
	createGeoPositionsItem();
}

void GeometryEditDlg::createBelongToLayerItem()
{
	QStringList list;
	list.append(QStringLiteral("所属图层"));
	list.append(QStringLiteral("点标绘图层"));

	QTreeWidgetItem* item = new QTreeWidgetItem(list);
	ui.m_geoTree->addTopLevelItem(item);
}

void GeometryEditDlg::createGeoTypeItem()
{
	QStringList list;
	list.append(QStringLiteral("类型"));
	list.append(QStringLiteral("点"));
	
	QTreeWidgetItem* item = new QTreeWidgetItem(list);
	ui.m_geoTree->addTopLevelItem(item);
}

void GeometryEditDlg::createGeoNameItem()
{
	QStringList list;
	list.append(QStringLiteral("名称"));
	list.append(QStringLiteral("点1"));

	QTreeWidgetItem* item = new QTreeWidgetItem(list);
	ui.m_geoTree->addTopLevelItem(item);
}

void GeometryEditDlg::createGeoPositionsItem()
{
	QStringList list;
	list.append(QStringLiteral("坐标"));
	list.append(QStringLiteral("(经度,纬度)"));

	QTreeWidgetItem* item = new QTreeWidgetItem(list);
	ui.m_geoTree->addTopLevelItem(item);

	list.clear();
	list.append("#1");
	list.append("(29.121212,28.212121)");
	QTreeWidgetItem* childItem = new QTreeWidgetItem(list);
	item->addChild(childItem);
}