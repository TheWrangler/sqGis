#include "LineMarkLayer.h"
#include "options.h"


LineMarkLayer::LineMarkLayer(QString layername)
	:MarkLayer("linestring?crs=epsg:3857&index=yes&field=name:string(255)&field=category:string(255)&field=state:string(255)&field=rotation:double(5,2)", layername)
{
	
}

LineMarkLayer::~LineMarkLayer()
{
}

QgsFeatureId LineMarkLayer::appendMark(MarkFeatureSettings& markFeatureSettings)
{
	static QgsFeatureId id = 0;
	QgsFeature feature(++id);
	QgsVectorDataProvider* provider = dataProvider();

	QgsGeometry geo;
	geo.addPart(*(markFeatureSettings.markPoints()), QgsWkbTypes::LineGeometry);

	QgsFields fields = provider->fields();
	feature.setFields(fields, true);

	feature.setGeometry(geo);
	feature.setAttribute("name", markFeatureSettings.name());
	feature.setAttribute("category", markFeatureSettings.category());
	feature.setAttribute("state", markFeatureSettings.state());
	feature.setAttribute("rotation", markFeatureSettings.rotation());

	if (!provider->addFeature(feature))
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("����ͼ��Ԫ��") << markFeatureSettings.name() << QStringLiteral("ʧ��!");
#endif
		return 0;
	}

	return id;
}

void LineMarkLayer::updateMarkGeometry(QgsFeatureId id, QgsPointSequence& points)
{
	QgsFeature feature;
	if (!searchFeature(id, feature))
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("����ͼ��Ԫ��") << id << QStringLiteral("ʧ��!");
#endif
		return;
	}

	MarkLayer::updateMarkGeometry(id, points);

	QgsGeometry geo;
	geo.addPart(points, QgsWkbTypes::LineGeometry);
	changeGeometry(feature.id(), geo);
}

void LineMarkLayer::updateMarkAttribute(QgsFeatureId id, QString attribute, QVariant& value)
{
	QgsFeature feature;
	if (!searchFeature(id, feature))
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("����ͼ��Ԫ��") << id << QStringLiteral("ʧ��!");
#endif
		return;
	}

	MarkLayer::updateMarkAttribute(id, attribute, value);
}