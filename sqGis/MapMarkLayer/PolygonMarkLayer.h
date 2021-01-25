#pragma once
#include "MarkLayer.h"

class PolygonMarkLayer : public MarkLayer
{
public:
	PolygonMarkLayer(QString layername);
	~PolygonMarkLayer();

public:
	MarkFeature* appendMark(const QVector<QgsPoint>& points, bool refresh = false);
	void updateMarkGeometry(QgsFeatureId id, QgsPointSequence& points);
	void updateMarkAttribute(QgsFeatureId id, QString attribute, QVariant& value);
};

