#include "MarkerSymbolFactory.h"
#include <qgsmarkersymbollayer.h>


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
	QgsProperty dataPropertySize = QgsProperty::fromField(QString("mark_size"));
	svgMarkSymbolLayer->setDataDefinedProperty(QgsSymbolLayer::PropertySize, dataPropertySize);

	QgsMarkerSymbol* symbol = new QgsMarkerSymbol();
	symbol->deleteSymbolLayer(0);
	symbol->appendSymbolLayer(svgMarkSymbolLayer);
	return symbol;
}