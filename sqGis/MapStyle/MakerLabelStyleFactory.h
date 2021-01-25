#pragma once
#include <qgspallabeling.h>

class MakerLabelStyleFactory
{
public:
	MakerLabelStyleFactory();
	~MakerLabelStyleFactory();

public:
	static void createLabelStyle(QgsPalLayerSettings& palLayerSettings, QString& fontFamily, double fontSize, QColor& textColor);
	static void createLabelStyle(QgsPalLayerSettings& palLayerSettings, QString& fontFamily, double fontSize, QColor& textColor, QColor& backgroundColor, double opacity=1);
};

