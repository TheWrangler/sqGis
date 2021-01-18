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
	void updateMarkGeometry(QgsFeatureId id, QgsPointSequence& points);
	void updateMarkAttribute(QgsFeatureId id, QString attribute, QVariant& value);
};

