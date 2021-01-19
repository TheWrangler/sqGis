#pragma once
#include <qgsvectorlayer.h>
#include <qgsvectordataprovider.h>

#include "MarkLayer.h"

class PointMarkLayer : public MarkLayer
{
public:
	PointMarkLayer(QString layername);
	~PointMarkLayer();

public:
	MarkFeature* appendMark(const QVector<QgsPoint>& points);
	void updateMarkGeometry(QgsFeatureId id, QgsPointSequence& points);
	void updateMarkAttribute(QgsFeatureId id, QString attribute, QVariant& value);
};

