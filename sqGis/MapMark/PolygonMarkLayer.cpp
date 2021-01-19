#include "PolygonMarkLayer.h"
#include "../options.h"


PolygonMarkLayer::PolygonMarkLayer(QString layername)
	:MarkLayer("polygon?crs=epsg:3857&index=yes&field=name:string(255)&field=category:string(255)&field=state:string(255)&field=strokecolor:string(255)&field=strokewidth:double(3,1)&field=strokestyle:string(255)&field=filledcolor:string(255)&field=filledstyle:string(255)", layername)
{
}


PolygonMarkLayer::~PolygonMarkLayer()
{
}

MarkFeature* PolygonMarkLayer::appendMark(const QVector<QgsPoint>& points)
{
	MarkFeature* feature = MarkFeature::createMarkFeature(QgsWkbTypes::PolygonGeometry);
	addFieldsToFeature(feature);
	feature->fromPoints(points);
	addFeature(*feature);
	return feature;
}

void PolygonMarkLayer::updateMarkGeometry(QgsFeatureId id, QgsPointSequence& points)
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
	geo.addPart(points, QgsWkbTypes::PolygonGeometry);
	changeGeometry(feature.id(), geo);
}

void PolygonMarkLayer::updateMarkAttribute(QgsFeatureId id, QString attribute, QVariant& value)
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