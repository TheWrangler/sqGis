#pragma once
#include <qgspallabeling.h>

class MapLabelStyleFactory
{
public:
	MapLabelStyleFactory();
	~MapLabelStyleFactory();

public:
	static void createLabelStyle(QgsPalLayerSettings& palLayerSettings, QString& fontFamily, double fontSize, QColor& textColor);
	static void createLabelStyle(QgsPalLayerSettings& palLayerSettings, QString& fontFamily, double fontSize, QColor& textColor, QColor& backgroundColor, double opacity=1);
};

