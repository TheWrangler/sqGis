#include "LineMarkLayer.h"
#include "options.h"


LineMarkLayer::LineMarkLayer(QString layername)
	:MarkLayer("linestring?crs=epsg:3857&index=yes&field=name:string(255)&field=category:string(255)&field=state:string(255)", layername)
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

	if (!provider->addFeature(feature))
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("Ìí¼ÓÍ¼²ãÔªËØ") << markFeatureSettings.name() << QStringLiteral("Ê§°Ü!");
#endif
		return 0;
	}

	return id;
}

QgsFeatureId LineMarkLayer::updateMarkGeometry(QString markname, QgsPointSequence& points)
{
	QgsFeature feature;
	if (!searchFeature(markname, feature))
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("²éÕÒÍ¼²ãÔªËØ") << markname << QStringLiteral("Ê§°Ü!");
#endif
		return 0;
	}

	MarkLayer::updateMarkGeometry(markname, points);

	QgsGeometry geo;
	geo.addPart(points, QgsWkbTypes::LineGeometry);
	changeGeometry(feature.id(), geo);
	return feature.id();
}

QgsFeatureId LineMarkLayer::updateMarkAttribute(QString markname, QString attribute, QVariant& value)
{
	QgsFeature feature;
	if (!searchFeature(markname, feature))
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("²éÕÒÍ¼²ãÔªËØ") << markname << QStringLiteral("Ê§°Ü!");
#endif
		return 0;
	}

	MarkLayer::updateMarkAttribute(markname, attribute, value);

	return feature.id();
}