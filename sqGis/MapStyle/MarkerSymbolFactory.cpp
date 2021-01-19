#include "MarkerSymbolFactory.h"
#include <qgsmarkersymbollayer.h>
#include <qgslinesymbollayer.h>
#include <qgsfillsymbollayer.h>

MarkerSymbolFactory::MarkerSymbolFactory()
{
}

MarkerSymbolFactory::~MarkerSymbolFactory()
{
}

QgsMarkerSymbol* MarkerSymbolFactory::createSvgMarkerSymbol(const QString& svgpath)
{
	QgsSvgMarkerSymbolLayer* svgMarkSymbolLayer = new QgsSvgMarkerSymbolLayer(svgpath);
	QgsProperty dataPropertyAngle = QgsProperty::fromField(QString("rotation"));
	svgMarkSymbolLayer->setDataDefinedProperty(QgsSymbolLayer::PropertyAngle, dataPropertyAngle);
	QgsProperty dataPropertySize = QgsProperty::fromField(QString("size"));
	svgMarkSymbolLayer->setDataDefinedProperty(QgsSymbolLayer::PropertySize, dataPropertySize);
	QgsProperty dataPropertyFile = QgsProperty::fromField(QString("svg"));
	svgMarkSymbolLayer->setDataDefinedProperty(QgsSymbolLayer::PropertyFile, dataPropertyFile);

	QgsMarkerSymbol* symbol = new QgsMarkerSymbol();
	symbol->deleteSymbolLayer(0);
	symbol->appendSymbolLayer(svgMarkSymbolLayer);
	return symbol;
}

QgsLineSymbol* MarkerSymbolFactory::createSimpleLineSymbol()
{
	QgsSimpleLineSymbolLayer* simpleLineSymbolLayer = new QgsSimpleLineSymbolLayer();
	QgsProperty dataPropertyColor= QgsProperty::fromField(QString("color"));
	simpleLineSymbolLayer->setDataDefinedProperty(QgsSymbolLayer::PropertyStrokeColor, dataPropertyColor);
	QgsProperty dataPropertyWidth= QgsProperty::fromField(QString("width"));
	simpleLineSymbolLayer->setDataDefinedProperty(QgsSymbolLayer::PropertyStrokeWidth, dataPropertyWidth);
	QgsProperty dataPropertyStyle = QgsProperty::fromField(QString("linestyle"));
	simpleLineSymbolLayer->setDataDefinedProperty(QgsSymbolLayer::PropertyStrokeStyle, dataPropertyStyle);

	QgsLineSymbol* symbol = new QgsLineSymbol();
	symbol->deleteSymbolLayer(0);
	symbol->appendSymbolLayer(simpleLineSymbolLayer);
	return symbol;
}

QgsFillSymbol* MarkerSymbolFactory::createSimpleFillSymbol()
{
	QgsSimpleFillSymbolLayer* simpleFillSymbolLayer = new QgsSimpleFillSymbolLayer();
	QgsProperty dataPropertyColor = QgsProperty::fromField(QString("strokecolor"));
	simpleFillSymbolLayer->setDataDefinedProperty(QgsSymbolLayer::PropertyStrokeColor, dataPropertyColor);
	QgsProperty dataPropertyWidth = QgsProperty::fromField(QString("strokewidth"));
	simpleFillSymbolLayer->setDataDefinedProperty(QgsSymbolLayer::PropertyStrokeWidth, dataPropertyWidth);
	QgsProperty dataPropertyStyle = QgsProperty::fromField(QString("strokestyle"));
	simpleFillSymbolLayer->setDataDefinedProperty(QgsSymbolLayer::PropertyStrokeStyle, dataPropertyStyle);
	QgsProperty dataPropertyFillColor = QgsProperty::fromField(QString("filledcolor"));
	simpleFillSymbolLayer->setDataDefinedProperty(QgsSymbolLayer::PropertyFillColor, dataPropertyFillColor);
	QgsProperty dataPropertyFillStyle = QgsProperty::fromField(QString("filledstyle"));
	simpleFillSymbolLayer->setDataDefinedProperty(QgsSymbolLayer::PropertyFillStyle, dataPropertyFillStyle);

	QgsFillSymbol* symbol = new QgsFillSymbol();
	symbol->deleteSymbolLayer(0);
	symbol->appendSymbolLayer(simpleFillSymbolLayer);
	return symbol;
}