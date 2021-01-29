#pragma once
#include "MapMeasureLine.h"
#include "GeoCalculator.h"

using namespace GeoCalculator;

class MapMeasureDistance : public MapMeasureLine
{
public:
	MapMeasureDistance();
	~MapMeasureDistance();

protected:
	QgsDistanceArea _distanceAreaCalculator;
	GcGeoCalculator _gcGeoCalculator;

	double _surfaceDistance;

protected:
	void measure(const QgsPointXY& point);

public:
	void initAnnotation(QgsMapCanvas* canvas, QgsMapLayer* layer);
	void initCalculator(QgsMapCanvas* canvas);
	void updateMeasure();
	void tryingMeasure(const QgsPointXY& point);
};

