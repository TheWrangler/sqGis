#pragma once
#include "MarkLayer.h"
class LineMarkLayer : public MarkLayer
{
public:
	LineMarkLayer(QString layername);
	~LineMarkLayer();

public:
	QgsFeatureId appendMark(MarkFeatureSettings& markFeatureSettings);
	QgsFeatureId updateMarkGeometry(QString markname, QgsPointSequence& points);
	QgsFeatureId updateMarkAttribute(QString markname, QString attribute, QVariant& value);
};

