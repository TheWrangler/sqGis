#include "FeatureEditDlg.h"
#include <QMessageBox>

FeatureEditDlg::FeatureEditDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.m_formatCheck, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged(int)));

	ui.m_okBtn->setIcon(QIcon(":/img/ok_x24"));
	ui.m_cancelBtn->setIcon(QIcon(":/img/cancel_x24"));

	_featureSettings = NULL;
}

FeatureEditDlg::~FeatureEditDlg()
{

}

void FeatureEditDlg::on_m_okBtn_clicked()
{
	accept();
}

void FeatureEditDlg::on_m_cancelBtn_clicked()
{
	reject();
}

QgsPointXY FeatureEditDlg::convertCoor(QgsPointXY& src, bool toWSG84)
{
	QgsCoordinateReferenceSystem dstCrs("EPSG:4326");
	QgsCoordinateReferenceSystem srcCrs("EPSG:3857");
	QgsCoordinateTransform crsTrans;
	if (toWSG84)
	{
		crsTrans.setSourceCrs(srcCrs);
		crsTrans.setDestinationCrs(dstCrs);
	}
	else
	{
		crsTrans.setSourceCrs(dstCrs);
		crsTrans.setDestinationCrs(srcCrs);
	}
	
	QgsPointXY dst;
	dst = crsTrans.transform(src);
	return dst;
}

void FeatureEditDlg::onStateChanged(int state)
{
	
}

void FeatureEditDlg::attachFeatureSettings(MarkFeatureSettings* featureSettings)
{
	_featureSettings = featureSettings;

	createBelongToLayerItem();
	createTypeItem();
	createNameItem();
	createPositionsItem();

	ui.m_geoTree->expandAll();
}

void FeatureEditDlg::createBelongToLayerItem()
{
	QStringList list;
	list.append(QStringLiteral("所属图层"));
	list.append(QStringLiteral("点标绘图层"));

	QTreeWidgetItem* item = new QTreeWidgetItem(list);
	ui.m_geoTree->addTopLevelItem(item);
}

void FeatureEditDlg::createTypeItem()
{
	QStringList list;
	list.append(QStringLiteral("类型"));
	list.append(MarkFeatureSettings::MarkTypeCaption[_featureSettings->markType()]);
	
	QTreeWidgetItem* item = new QTreeWidgetItem(list);
	ui.m_geoTree->addTopLevelItem(item);
}

void FeatureEditDlg::createNameItem()
{
	QStringList list;
	list.append(QStringLiteral("名称"));
	list.append(_featureSettings->name());

	QTreeWidgetItem* item = new QTreeWidgetItem(list);
	ui.m_geoTree->addTopLevelItem(item);
}

void FeatureEditDlg::createPositionsItem()
{
	QStringList list;
	list.append(QStringLiteral("坐标"));
	list.append(QStringLiteral("(经度,纬度,高度)"));

	QTreeWidgetItem* item = new QTreeWidgetItem(list);
	ui.m_geoTree->addTopLevelItem(item);

	QgsPointXY src;
	QgsPointXY dst;

	QVector<QgsPoint>* points = _featureSettings->markPoints();
	QVector<QgsPoint>::iterator it = points->begin();
	unsigned int index = 0;
	while (it != points->end())
	{	
		list.clear();
		
		index++;
		list.append(QString::number(index));

		src.setX((*it).x());
		src.setY((*it).y());
		dst = convertCoor(src);

		QString posString = QString::number(dst.x(), 10, 12)
			+ ","
			+ QString::number(dst.y(), 10, 12)
			+ ","
			+ QString::number((*it).z(), 10, 2);
	
		list.append(posString);

		QTreeWidgetItem* childItem = new QTreeWidgetItem(list);
		item->addChild(childItem);

		it++;
	}	
}