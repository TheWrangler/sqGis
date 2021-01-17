#include "MarkLayer.h"
#include "PointMarkLayer.h"
#include "LineMarkLayer.h"
#include "PolygonMarkLayer.h"
#include "options.h"

MarkLayer::MarkLayer(QString path, QString layername)
	: QgsVectorLayer(path, layername, "memory")
{
	setExtent(QgsRectangle(-20037508.3427892, -20037508.3427892, 20037508.3427892, 20037508.3427892));
}

MarkLayer::~MarkLayer()
{
}

MarkLayer* MarkLayer::createLayer(QgsWkbTypes::GeometryType mark_type)
{
	QString layer_name;
	MarkLayer* layer;
	switch (mark_type)
	{
	case QgsWkbTypes::PointGeometry:
		layer_name = QStringLiteral("µã±ê»æÍ¼²ã");
		layer = new PointMarkLayer(layer_name);
		break;
	case QgsWkbTypes::LineGeometry:
		layer_name = QStringLiteral("Ïß±ê»æÍ¼²ã");
		layer = new LineMarkLayer(layer_name);
		break;
	case QgsWkbTypes::PolygonGeometry:
		layer_name = QStringLiteral("¶à±ßÐÎ±ê»æÍ¼²ã");
		layer = new PolygonMarkLayer(layer_name);
		break;
	}

	if (!layer->isValid())
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("³õÊ¼»¯±ê»æÍ¼²ã") << layer_name << QStringLiteral("Ê§°Ü!");
#endif
		return NULL;
	}

	return layer;
}

bool MarkLayer::searchFeature(QString mark_name, QgsFeature& feature)
{
	QgsVectorDataProvider* provider = dataProvider();
	QgsFeatureIterator it = provider->getFeatures(QgsFeatureRequest());
	if (!it.isValid())
		return false;

	while (it.nextFeature(feature))
	{
		if (feature.attribute("name").toString() != mark_name)
			continue;

		return true;
	}

	return false;
}

QgsFeatureId MarkLayer::updateMarkGeometry(QString markname, QgsPointSequence& points)
{
	startEditing();

	return 0;
}

QgsFeatureId MarkLayer::updateMarkAttribute(QString markname, QString attribute, QVariant& value)
{
	startEditing();

	return 0;
}