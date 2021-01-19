#include "MarkLayer.h"
#include "PointMarkLayer.h"
#include "LineMarkLayer.h"
#include "PolygonMarkLayer.h"
#include "options.h"
#include "./MapStyle/MapLabelStyleFactory.h"
#include "./MapStyle/MarkerSymbolFactory.h"

#include <qgsvectorlayerlabeling.h>
#include <qgsrenderer.h>
#include <qgssinglesymbolrenderer.h>
//#include <qgssymbol.h>

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
		layer_name = QStringLiteral("����ͼ��");
		layer = new PointMarkLayer(layer_name);
		break;
	case QgsWkbTypes::LineGeometry:
		layer_name = QStringLiteral("�߱��ͼ��");
		layer = new LineMarkLayer(layer_name);
		break;
	case QgsWkbTypes::PolygonGeometry:
		layer_name = QStringLiteral("����α��ͼ��");
		layer = new PolygonMarkLayer(layer_name);
		break;
	}

	if (!layer->isValid())
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("��ʼ�����ͼ��") << layer_name << QStringLiteral("ʧ��!");
#endif
		return NULL;
	}

	layer->activeLabeling(mark_type,QString("name"));
	layer->setLayerRenderer(mark_type);
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
		MapLabelStyleFactory::createLabelStyle(layersettings, QStringLiteral("����"), 12, QColor("darkCyan"), QColor("cyan"),0.4);
		layersettings.placement = QgsPalLayerSettings::AroundPoint;
		break;
	case QgsWkbTypes::LineGeometry:
		MapLabelStyleFactory::createLabelStyle(layersettings, QStringLiteral("����"), 12, QColor("darkCyan"), QColor("cyan"),0.4);
		layersettings.placement = QgsPalLayerSettings::Curved;
		break;
	case QgsWkbTypes::PolygonGeometry:
		MapLabelStyleFactory::createLabelStyle(layersettings, QStringLiteral("����"), 12, QColor("darkCyan"));
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

		QString svgfile = QCoreApplication::applicationDirPath() + "/markers/ground_station.svg";
		QgsMarkerSymbol* symbol = MarkerSymbolFactory::createSvgMarkerSymbol(svgfile);
		render->setSymbol(symbol);
	}
}

void MarkLayer::updateMarkGeometry(QgsFeatureId id, QgsPointSequence& points)
{
	startEditing();
}

void MarkLayer::updateMarkAttribute(QgsFeatureId id, QString attribute, QVariant& value)
{
	startEditing();
}