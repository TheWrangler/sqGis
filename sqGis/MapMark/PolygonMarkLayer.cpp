#include "PolygonMarkLayer.h"
#include "../options.h"


PolygonMarkLayer::PolygonMarkLayer(QString layername)
	:MarkLayer("polygon?crs=epsg:3857&index=yes&field=name:string(255)&field=category:string(255)&field=state:string(255)", layername)
{
}


PolygonMarkLayer::~PolygonMarkLayer()
{
}

QgsFeatureId PolygonMarkLayer::appendMark(MarkFeatureSettings& markFeatureSettings)
{
	static QgsFeatureId id = 0;
	QgsFeature feature(++id);
	QgsVectorDataProvider* provider = dataProvider();

	QgsGeometry geo;
	geo.addPart(*(markFeatureSettings.markPoints()), QgsWkbTypes::PolygonGeometry);

	QgsFields fields = provider->fields();
	feature.setFields(fields, true);

	feature.setGeometry(geo);
	feature.setAttribute("name", markFeatureSettings.name());
	feature.setAttribute("category", markFeatureSettings.category());
	feature.setAttribute("state", markFeatureSettings.state());

	if (!provider->addFeature(feature))
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("Ìí¼ÓÍ¼²ãÔªËØ") << markFeatureSettings.name() << QStringLiteral("Ê§°Ü!");
#endif
		return 0;
	}

	return id;
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