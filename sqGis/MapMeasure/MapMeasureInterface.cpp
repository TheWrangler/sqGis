#include "MapMeasureInterface.h"

MapMeasureInterface::MapMeasureInterface()
{
	_annotation = NULL;
	_visualItem = NULL;
}

MapMeasureInterface::~MapMeasureInterface()
{
}

void MapMeasureInterface::moveAnnotationItem(const QPoint& point)
{
	if (_annotation == NULL)
		return;

	_annotation->moveTo(point);
}

void MapMeasureInterface::removeAnnotation(QgsMapCanvas* canvas)
{
	if (_annotation == NULL)
		return;

	canvas->scene()->removeItem(_annotation);
	_annotation = NULL;
}

void MapMeasureInterface::removeVisualItem(QgsMapCanvas* canvas)
{
	if (_visualItem == NULL)
		return;

	canvas->scene()->removeItem(_visualItem);
	_visualItem = NULL;
}