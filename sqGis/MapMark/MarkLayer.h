#pragma once
#include <QString>
#include <qgsfeature.h>
#include <qgsvectorlayer.h>
#include "MarkFeatureSettings.h"

class MarkLayer : public QgsVectorLayer
{
public:
	MarkLayer(QString path,QString layername);
	~MarkLayer();

public:
	static MarkLayer* createLayer(QgsWkbTypes::GeometryType mark_type);
	bool searchFeature(QgsFeatureId id, QgsFeature& feature);

	virtual void activeLabeling(QgsWkbTypes::GeometryType mark_type,QString& field);
	virtual QgsFeatureId appendMark(MarkFeatureSettings& markFeatureSettings) = 0;
	virtual void updateMarkGeometry(QgsFeatureId id, QgsPointSequence& points);
	virtual void updateMarkAttribute(QgsFeatureId id, QString attribute, QVariant& value);
};

