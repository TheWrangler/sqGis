#pragma once
#include "MarkLayer.h"

class LineMarkLayer : public MarkLayer
{
public:
	LineMarkLayer(QString layername);
	~LineMarkLayer();

public:
	MarkFeature* appendMark(const QVector<QgsPoint>& points, bool refresh = false);
	void updateMarkGeometry(QgsFeatureId id, QgsPointSequence& points);
	void updateMarkAttribute(QgsFeatureId id, QString attribute, QVariant& value);
};

