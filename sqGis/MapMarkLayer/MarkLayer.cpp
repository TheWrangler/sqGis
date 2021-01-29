#include "MarkLayer.h"
#include "PointMarkLayer.h"
#include "LineMarkLayer.h"
#include "PolygonMarkLayer.h"
#include "../options.h"
#include "../MapStyle/MakerLabelStyleFactory.h"
#include "../MapStyle/MarkerSymbolFactory.h"

#include <qgsvectorlayerlabeling.h>
#include <qgsrenderer.h>
#include <qgssinglesymbolrenderer.h>

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

	layer->activeLabeling(mark_type,QString("name"));
	layer->setLayerRenderer(mark_type);
	return layer;
}

void MarkLayer::selectFeature(QgsFeatureId id, bool append)
{
	QgsFeatureIds ids;
	if (append)
		ids = selectedFeatureIds();
	else removeSelection();

	ids << id;
	select(ids);
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
		MakerLabelStyleFactory::createLabelStyle(layersettings, QStringLiteral("ºÚÌå"), 13, QColor("white"), QColor("black"),0.5);
		layersettings.placement = QgsPalLayerSettings::AroundPoint;
		break;
	case QgsWkbTypes::LineGeometry:
		MakerLabelStyleFactory::createLabelStyle(layersettings, QStringLiteral("ºÚÌå"), 13, QColor("white"), QColor("black"),0.5);
		layersettings.placement = QgsPalLayerSettings::Curved;
		break;
	case QgsWkbTypes::PolygonGeometry:
		MakerLabelStyleFactory::createLabelStyle(layersettings, QStringLiteral("ºÚÌå"), 13, QColor("white"), QColor("black"), 0.5);
		layersettings.placement = QgsPalLayerSettings::AroundPoint;
		break;
	}
	layersettings.fieldName = field;

	QgsVectorLayerSimpleLabeling* labeling = new QgsVectorLayerSimpleLabeling(layersettings);
	setLabeling(labeling);
	setLabelsEnabled(true);
}

void MarkLayer::setLayerRenderer(QgsWkbTypes::GeometryType mark_type)
{
	if(mark_type == QgsWkbTypes::PointGeometry)
	{
		QgsSingleSymbolRenderer* render = dynamic_cast<QgsSingleSymbolRenderer*>(renderer());

		QString svgfile = QCoreApplication::applicationDirPath() + "/markers/position.svg";
		QgsMarkerSymbol* symbol = MarkerSymbolFactory::createSvgMarkerSymbol(svgfile);
		render->setSymbol(symbol);
	}
	else if (mark_type == QgsWkbTypes::LineGeometry)
	{
		QgsSingleSymbolRenderer* render = dynamic_cast<QgsSingleSymbolRenderer*>(renderer());
		QgsLineSymbol* symbol = MarkerSymbolFactory::createSimpleLineSymbol();
		render->setSymbol(symbol);
	}
	else if (mark_type == QgsWkbTypes::PolygonGeometry)
	{
		QgsSingleSymbolRenderer* render = dynamic_cast<QgsSingleSymbolRenderer*>(renderer());
		QgsFillSymbol* symbol = MarkerSymbolFactory::createSimpleFillSymbol();
		render->setSymbol(symbol);
	}
}

void MarkLayer::addFieldsToFeature(QgsFeature* feature)
{
	QgsVectorDataProvider* provider = dataProvider();
	QgsFields fields = provider->fields();
	feature->setFields(fields, true);
}

void MarkLayer::addFeature(QgsFeature& feature)
{
	QgsVectorDataProvider* provider = dataProvider();

	if (!provider->addFeature(feature))
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("Ìí¼ÓÍ¼²ãÔªËØ") << feature.id() << QStringLiteral("Ê§°Ü!");
#endif
	}
	else
	{
#if PROMPT_INFO_MSG
		qInfo() << QStringLiteral("Ìí¼ÓÍ¼²ãÔªËØ") << feature.id() << ":" <<feature.attribute("name").toString();
#endif
	}
}

void MarkLayer::deleteFeature(QgsFeatureId id)
{
	QgsVectorDataProvider* provider = dataProvider();
	if (!provider->deleteFeatures(QgsFeatureIds() << id))
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("É¾³ýÍ¼²ãÔªËØ") << id << QStringLiteral("Ê§°Ü!");
#endif
	}
	else
	{
#if PROMPT_INFO_MSG
		qInfo() << QStringLiteral("É¾³ýÍ¼²ãÔªËØ") << id;
#endif
	}
}

MarkFeature* MarkLayer::appendMark(const QVector<QgsPoint>& points, bool refresh)
{
	if(refresh)
		triggerRepaint();

	return NULL;
}

void MarkLayer::removeMark(QgsFeatureId id, bool refresh)
{
	deleteFeature(id);

	if(refresh)
		triggerRepaint();
}

void MarkLayer::updateMarkGeometry(QgsFeatureId id, QgsPointSequence& points)
{
	startEditing();
}

void MarkLayer::updateMarkAttribute(QgsFeatureId id, QString attribute, QVariant& value)
{
	startEditing();
}