#include "PointMarkLayer.h"
#include "qgsfeature.h"
#include "qgsgeometry.h"
#include "qgsrectangle.h"
#include "options.h"

PointMarkLayer::PointMarkLayer(QString layername)
	:MarkLayer("Point?crs=epsg:3857&index=yes&field=name:string(255)&field=category:string(255)&field=state:string(255)", layername)
{
	setExtent(QgsRectangle(-20037508.3427892, -20037508.3427892, 20037508.3427892, 20037508.3427892));
}

PointMarkLayer::~PointMarkLayer()
{
}

QgsFeatureId PointMarkLayer::appendMark(MarkFeatureSettings* markFeatureSettings)
{
	static QgsFeatureId id = 0;
	QgsFeature feature(++id);
	QgsVectorDataProvider* provider = dataProvider();

	QgsGeometry geo;
	geo.addPart(*(markFeatureSettings->markPoints()), QgsWkbTypes::PointGeometry);

	QgsFields fields = provider->fields();
	feature.setFields(fields,true);

	feature.setGeometry(geo);
	feature.setAttribute("name", markFeatureSettings->name());
	feature.setAttribute("category", markFeatureSettings->category());
	feature.setAttribute("state", markFeatureSettings->state());
	
	if(!provider->addFeature(feature))
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("Ìí¼ÓÍ¼²ãÔªËØ") << markFeatureSettings->name() << QStringLiteral("Ê§°Ü!");
#endif
		return 0;
	}

	return id;
}

QgsFeatureId  PointMarkLayer::updateMark(QString mark_name,QgsPoint point)
{
	QgsFeature feature;
	if (!searchFeature(mark_name, feature))
	{ 
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("²éÕÒÍ¼²ãÔªËØ") << mark_name << QStringLiteral("Ê§°Ü!");
#endif
		return 0;
	}

	QgsGeometry geo;
	QVector<QgsPoint> points;
	points.append(point);
	geo.addPart(points, QgsWkbTypes::PointGeometry);
	changeGeometry(feature.id(), geo);
	return feature.id();
}