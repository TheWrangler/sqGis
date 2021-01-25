#pragma once
#include "MapToolMarkLine.h"
#include "./MapCanvasItem/DistanceMeasureAnnotationItem.h"
#include "./WirelessMath/WmGeoCalculator.h"
#include <qgsmapcanvasannotationitem.h>

class MapToolDistanceMeasure : public MapToolMarkLine
{
	Q_OBJECT

public:
	MapToolDistanceMeasure(QgsMapCanvas* canvas, QgsMapLayer* layer);
	~MapToolDistanceMeasure();

protected:
	DistanceMeasureAnnotationItem* _annotation;
	WmGeoCalculator _wmGeoCalculator;

protected:
	void initGeoCalculator(const QgsPointXY& srcPoint);
	void updateDistance(const QgsPointXY& destPoint);

	void notify();

	void initAnnotation();
	void startCapturing();
	void stopCapturing();

public:
	void canvasReleaseEvent(QgsMapMouseEvent *e);
	void canvasMoveEvent(QgsMapMouseEvent *e);

signals:
	void measureAdded(QgsMapLayer* layer, QgsWkbTypes::GeometryType type, const QVector<QgsPointXY>& points, QgsMapCanvasItem* measureItem);
};

