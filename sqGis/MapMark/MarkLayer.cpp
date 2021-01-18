#include "MarkLayer.h"
#include "PointMarkLayer.h"
#include "LineMarkLayer.h"
#include "PolygonMarkLayer.h"
#include "options.h"
#include "./MapStyle/MapLabelStyleFactory.h"

#include <qgsvectorlayerlabeling.h>

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
		layer_name = QStringLiteral("点标绘图层");
		layer = new PointMarkLayer(layer_name);
		break;
	case QgsWkbTypes::LineGeometry:
		layer_name = QStringLiteral("线标绘图层");
		layer = new LineMarkLayer(layer_name);
		break;
	case QgsWkbTypes::PolygonGeometry:
		layer_name = QStringLiteral("多边形标绘图层");
		layer = new PolygonMarkLayer(layer_name);
		break;
	}

	if (!layer->isValid())
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("初始化标绘图层") << layer_name << QStringLiteral("失败!");
#endif
		return NULL;
	}

	layer->activeLabeling(mark_type,QString("name"));
	return layer;
}

bool MarkLayer::searchFeature(QgsFeatureId id, QgsFeature& feature)
{
	QgsVectorDataProvider* provider = dataProvider();
	QgsFeatureIterator it = provider->getFeatures(QgsFeatureRequest());
	if (!it.isValid())
		return false;

	while (it.nextFeature(feature))
	{
		if (feature.id() != id)
			continue;

		return true;
	}

	return false;
}

void MarkLayer::activeLabeling(QgsWkbTypes::GeometryType mark_type, QString& field)
{
	QgsPalLayerSettings layersettings;
	switch (mark_type)
	{
	case QgsWkbTypes::PointGeometry:
		MapLabelStyleFactory::createLabelStyle(layersettings, QStringLiteral("黑体"), 13, QColor("darkCyan"), QColor("cyan"));
		layersettings.fieldName = "name";
		layersettings.placement = QgsPalLayerSettings::AroundPoint;
		break;
	case QgsWkbTypes::LineGeometry:
		MapLabelStyleFactory::createLabelStyle(layersettings, QStringLiteral("黑体"), 13, QColor("darkCyan"), QColor("cyan"));
		layersettings.fieldName = "name";
		layersettings.placement = QgsPalLayerSettings::Curved;
		break;
	case QgsWkbTypes::PolygonGeometry:
		MapLabelStyleFactory::createLabelStyle(layersettings, QStringLiteral("黑体"), 13, QColor("darkCyan"));
		layersettings.fieldName = "name";
		layersettings.placement = QgsPalLayerSettings::AroundPoint;
		break;
	}
	layersettings.fieldName = field;

	QgsVectorLayerSimpleLabeling* labeling = new QgsVectorLayerSimpleLabeling(layersettings);
	setLabeling(labeling);
	setLabelsEnabled(true);
}

void MarkLayer::updateMarkGeometry(QgsFeatureId id, QgsPointSequence& points)
{
	startEditing();
}

void MarkLayer::updateMarkAttribute(QgsFeatureId id, QString attribute, QVariant& value)
{
	startEditing();
}