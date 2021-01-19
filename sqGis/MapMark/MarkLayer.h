#pragma once
#include <QString>
#include <qgsfeature.h>
#include <qgsvectorlayer.h>
#include "MarkFeature.h"

class MarkLayer : public QgsVectorLayer
{
public:
	MarkLayer(QString path,QString layername);
	~MarkLayer();

protected:
	void activeLabeling(QgsWkbTypes::GeometryType mark_type,QString& field);
	void setLayerRenderer(QgsWkbTypes::GeometryType mark_type);
	void addFieldsToFeature(QgsFeature* feature);
	void addFeature(QgsFeature& feature);

public:
	static MarkLayer* createLayer(QgsWkbTypes::GeometryType mark_type);
	bool searchFeature(QgsFeatureId id, QgsFeature& feature);
 
	virtual MarkFeature* appendMark(const QVector<QgsPoint>& points)=0;
	virtual void updateMarkGeometry(QgsFeatureId id, QgsPointSequence& points);
	virtual void updateMarkAttribute(QgsFeatureId id, QString attribute, QVariant& value);
};

