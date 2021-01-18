#pragma once
#include "MarkLayer.h"
class LineMarkLayer : public MarkLayer
{
public:
	LineMarkLayer(QString layername);
	~LineMarkLayer();

public:
	QgsFeatureId appendMark(MarkFeatureSettings& markFeatureSettings);
	void updateMarkGeometry(QgsFeatureId id, QgsPointSequence& points);
	void updateMarkAttribute(QgsFeatureId id, QString attribute, QVariant& value);
};

