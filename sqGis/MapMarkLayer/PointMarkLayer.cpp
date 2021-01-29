#include "PointMarkLayer.h"
#include <qgsfeature.h>
#include <qgsgeometry.h>
#include "../options.h"

PointMarkLayer::PointMarkLayer(QString layername)
	:MarkLayer("Point?crs=epsg:3857&index=yes&field=name:string(255)&field=category:string(255)&field=state:string(255)&field=svg:string(255)&field=rotation:double(5,2)&field=size:double(3,1)", layername)
{

}

PointMarkLayer::~PointMarkLayer()
{
}

MarkFeature* PointMarkLayer::appendMark(const QVector<QgsPoint>& points, bool refresh)
{
	MarkFeature* feature = MarkFeature::createMarkFeature(QgsWkbTypes::PointGeometry);
	addFieldsToFeature(feature);
	feature->fromPoints(points);
	addFeature(*feature);

	MarkLayer::appendMark(points, refresh);

	return feature;
}

void PointMarkLayer::updateMarkGeometry(QgsFeatureId id, QgsPointSequence& points)
{
	QgsFeature feature;
	if (!searchFeature(id, feature))
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("²éÕÒÍ¼²ãÔªËØ") << id << QStringLiteral("Ê§°Ü!");
#endif
		return;
	}

	MarkLayer::updateMarkGeometry(id,points);

	QgsGeometry geo;
	geo.addPart(points, QgsWkbTypes::PointGeometry);
	changeGeometry(feature.id(), geo);
}

void PointMarkLayer::updateMarkAttribute(QgsFeatureId id, QString attribute, QVariant& value)
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