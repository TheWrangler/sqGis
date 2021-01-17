#pragma once
#include "MarkLayer.h"

class PolygonMarkLayer : public MarkLayer
{
public:
	PolygonMarkLayer(QString layername);
	~PolygonMarkLayer();

public:
	QgsFeatureId appendMark(MarkFeatureSettings& markFeatureSettings);
	QgsFeatureId updateMarkGeometry(QString markname, QgsPointSequence& points);
	QgsFeatureId updateMarkAttribute(QString markname, QString attribute, QVariant& value);
};

