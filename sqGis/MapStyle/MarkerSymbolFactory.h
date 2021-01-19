#pragma once
#include <qgssymbol.h>

class MarkerSymbolFactory
{
public:
	MarkerSymbolFactory();
	~MarkerSymbolFactory();

public:
	static QgsMarkerSymbol* createSvgMarkerSymbol(const QString& svgpath);
	static QgsLineSymbol* createSimpleLineSymbol();
	static QgsFillSymbol* createSimpleFillSymbol();
};

