#include "PointMarkLayer.h"
#include <qgsfeature.h>
#include <qgsgeometry.h>
#include "options.h"

PointMarkLayer::PointMarkLayer(QString layername)
	:MarkLayer("Point?crs=epsg:3857&index=yes&field=name:string(255)&field=category:string(255)&field=state:string(255)&field=rotation:double(5,2)&field=mark_size:double(3,1)", layername)
{

}

PointMarkLayer::~PointMarkLayer()
{
}

QgsFeatureId PointMarkLayer::appendMark(MarkFeatureSettings& markFeatureSettings)
{
	static QgsFeatureId id = 0;
	QgsFeature feature(++id);
	QgsVectorDataProvider* provider = dataProvider();

	QgsGeometry geo;
	geo.addPart(*(markFeatureSettings.markPoints()), QgsWkbTypes::PointGeometry);

	QgsFields fields = provider->fields();
	feature.setFields(fields,true);

	feature.setGeometry(geo);
	feature.setAttribute("name", markFeatureSettings.name());
	feature.setAttribute("category", markFeatureSettings.category());
	feature.setAttribute("state", markFeatureSettings.state());
	feature.setAttribute("rotation", markFeatureSettings.rotation());
	feature.setAttribute("mark_size", markFeatureSettings.markSize());

	if(!provider->addFeature(feature))
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("Ìí¼ÓÍ¼²ãÔªËØ") << markFeatureSettings.name() << QStringLiteral("Ê§°Ü!");
#endif
		return 0;
	}

	return id;
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