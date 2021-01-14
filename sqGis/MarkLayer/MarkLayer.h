#pragma once
#include <QString>
#include "qgsfeature.h"
#include "qgsvectorlayer.h"
#include "MarkFeatureSettings.h"

class MarkLayer : public QgsVectorLayer
{
public:
	MarkLayer(QString path,QString layername);
	~MarkLayer();

public:
	static MarkLayer* createLayer(MarkFeatureSettings::MarkType mark_type);
	bool searchFeature(QString mark_name, QgsFeature& feature);

	virtual QgsFeatureId appendMark(MarkFeatureSettings* markFeatureSettings) = 0;
};

