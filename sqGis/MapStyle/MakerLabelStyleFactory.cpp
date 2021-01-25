#include "MakerLabelStyleFactory.h"



MakerLabelStyleFactory::MakerLabelStyleFactory()
{
}


MakerLabelStyleFactory::~MakerLabelStyleFactory()
{
}

void MakerLabelStyleFactory::createLabelStyle(QgsPalLayerSettings& palLayerSettings, QString& fontFamily, double fontSize,QColor& textColor)
{
	QgsTextFormat textFormat;
	textFormat.setColor(textColor);
	textFormat.setFont(QFont(fontFamily));
	textFormat.setSize(fontSize);

	palLayerSettings.setFormat(textFormat);
}

void MakerLabelStyleFactory::createLabelStyle(QgsPalLayerSettings& palLayerSettings, QString& fontFamily, double fontSize, QColor& textColor, QColor& backgroundColor, double opacity)
{
	QgsTextBackgroundSettings textBackgroundSettings;
	textBackgroundSettings.setType(QgsTextBackgroundSettings::ShapeRectangle);
	textBackgroundSettings.setOpacity(opacity);
	textBackgroundSettings.setFillColor(backgroundColor);
	textBackgroundSettings.setEnabled(true);

	QgsTextFormat textFormat;
	textFormat.setColor(textColor);
	textFormat.setFont(QFont(fontFamily));
	textFormat.setSize(fontSize);
	textFormat.setBackground(textBackgroundSettings);

	palLayerSettings.setFormat(textFormat);
}
