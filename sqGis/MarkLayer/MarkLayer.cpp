#include "MarkLayer.h"
#include "PointMarkLayer.h"
#include "options.h"

MarkLayer::MarkLayer(QString path, QString layername)
	: QgsVectorLayer(path, layername, "memory")
{
}


MarkLayer::~MarkLayer()
{
}

MarkLayer* MarkLayer::createLayer(MarkFeatureSettings::MarkType mark_type)
{
	QString layer_name;
	MarkLayer* layer;
	switch (mark_type)
	{
	case MarkFeatureSettings::MarkType_Point:
		layer_name = QStringLiteral("µã±ê»æÍ¼²ã");
		layer = new PointMarkLayer(layer_name);
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