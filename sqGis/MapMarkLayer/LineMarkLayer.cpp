#include "LineMarkLayer.h"
#include "../options.h"


LineMarkLayer::LineMarkLayer(QString layername)
	:MarkLayer("linestring?crs=epsg:3857&index=yes&field=name:string(255)&field=category:string(255)&field=state:string(255)&field=width:double(3,1)&field=color:string(255)&field=linestyle:string(255)", layername)
{
	
}

LineMarkLayer::~LineMarkLayer()
{
}

MarkFeature* LineMarkLayer::appendMark(const QVector<QgsPoint>& points, bool refresh)
{
	MarkFeature* feature = MarkFeature::createMarkFeature(QgsWkbTypes::LineGeometry);
	addFieldsToFeature(feature);
	feature->fromPoints(points);
	addFeature(*feature);

	MarkLayer::appendMark(points, refresh);

	return feature;
}

void LineMarkLayer::updateMarkGeometry(QgsFeatureId id, QgsPointSequence& points)
{
	QgsFeature feature;
	if (!searchFeature(id, feature))
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("²éÕÒÍ¼²ãÔªËØ") << id << QStringLiteral("Ê§°Ü!");
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
		qCritical() << QStringLiteral("²éÕÒÍ¼²ãÔªËØ") << id << QStringLiteral("Ê§°Ü!");
#endif
		return;
	}

	MarkLayer::updateMarkAttribute(id, attribute, value);
}