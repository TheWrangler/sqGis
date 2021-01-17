#pragma once
#include <qgsvectorlayer.h>
#include <qgsvectordataprovider.h>

#include "MarkLayer.h"
#include "MarkFeatureSettings.h"

class PointMarkLayer : public MarkLayer
{
public:
	PointMarkLayer(QString layername);
	~PointMarkLayer();

public:
	QgsFeatureId appendMark(MarkFeatureSettings& markFeatureSettings);
	QgsFeatureId updateMarkGeometry(QString markname, QgsPointSequence& points);
	QgsFeatureId updateMarkAttribute(QString markname, QString attribute, QVariant& value);
};

