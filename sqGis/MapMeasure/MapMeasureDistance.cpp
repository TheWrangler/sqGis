#include "MapMeasureDistance.h"
#include "../MapCanvasItem/DistanceMeasureAnnotationItem.h"
#include "../MapMarkTool/MapToolMath.h"

MapMeasureDistance::MapMeasureDistance()
	: MapMeasureLine()
{
	_surfaceDistance = 0.0;
}

MapMeasureDistance::~MapMeasureDistance()
{
}

void MapMeasureDistance::initAnnotation(QgsMapCanvas* canvas, QgsMapLayer* layer)
{
	QgsTextAnnotation* textAnnotation = MeasureAnnotationItem::createTextAnnotation(layer);
	_annotation = new DistanceMeasureAnnotationItem(textAnnotation, canvas);

	MapMeasureLine::initAnnotation(canvas,layer);
}

void MapMeasureDistance::initCalculator(QgsMapCanvas* canvas)
{
	QgsPointXY lpoint = MapToolMath::convertCoor(_startPoint);
	_gcGeoCalculator.setSrcVertex(GcVertex(lpoint.x(), lpoint.y(), 0.0));

	QgsMapSettings settings = canvas->mapSettings();
	_distanceAreaCalculator.setSourceCrs(settings.destinationCrs(), settings.transformContext());
	_distanceAreaCalculator.setEllipsoid(settings.destinationCrs().ellipsoidAcronym());

	_surfaceDistance = 0.0;
}

void MapMeasureDistance::measure(const QgsPointXY& point)
{
	QgsPointXY lpoint = MapToolMath::convertCoor(point);
	_gcGeoCalculator.setDstVertex(GcVertex(lpoint.x(), lpoint.y(), 0.0));
	_surfaceDistance = _distanceAreaCalculator.measureLine(_startPoint, point) / 1000;

	_annotation->setMeasureVariable(DistanceMeasureAnnotationItem::DistanceMeasureRole_SurfaceDistance, _surfaceDistance);
	_annotation->setMeasureVariable(DistanceMeasureAnnotationItem::DistanceMeasureRole_SpaceDistance, _gcGeoCalculator.distance());
	_annotation->setMeasureVariable(DistanceMeasureAnnotationItem::DistanceMeasureRole_Azi, _gcGeoCalculator.azi());
	_annotation->setMeasureVariable(DistanceMeasureAnnotationItem::DistanceMeasureRole_Ele, _gcGeoCalculator.ele());
}

void MapMeasureDistance::updateMeasure()
{
	measure(_endPoint);
	_annotation->updateAnnotation();
}

void MapMeasureDistance::tryingMeasure(const QgsPointXY& point)
{
	measure(point);
	_annotation->updateAnnotation();
	moveVisualItem(point);
}