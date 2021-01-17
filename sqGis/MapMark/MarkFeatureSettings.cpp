#include "MarkFeatureSettings.h"

QStringList MarkFeatureSettings::MarkTypeCaption =
			QStringList() << QStringLiteral("��") << QStringLiteral("����") << QStringLiteral("�����");

MarkFeatureSettings::MarkFeatureSettings(QString layer_name, QgsWkbTypes::GeometryType mark_type)
{
	_layerName = layer_name;
	_markType = mark_type;
}


MarkFeatureSettings::~MarkFeatureSettings()
{
}

void MarkFeatureSettings::appendMarkPoint(QgsPoint& point)
{
	_points.append(point);
}

void MarkFeatureSettings::clearMarkPoint()
{
	_points.clear();
}
