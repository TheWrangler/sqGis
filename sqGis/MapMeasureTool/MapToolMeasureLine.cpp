#include "MapToolMeasureLine.h"

MapToolMeasureLine::MapToolMeasureLine(QgsMapCanvas* canvas)
	: QgsMapTool(canvas)
{
	_measureInterface = NULL;
	_capturing = false;

	setCursor(Qt::CrossCursor);
}

MapToolMeasureLine::~MapToolMeasureLine()
{
}

void MapToolMeasureLine::setMeasureInterface(MapMeasureLine* measureInterface)
{
	_measureInterface = measureInterface;
}

MapMeasureLine* MapToolMeasureLine::measureInterface()
{
	return _measureInterface;
}

void MapToolMeasureLine::removeMeasureInterface()
{
	_measureInterface->removeAnnotation(mCanvas);
	_measureInterface->removeVisualItem(mCanvas);

	_measureInterface = NULL;
}

void MapToolMeasureLine::startCapturing(const QPoint& canvasPt)
{
	QgsPointXY mpoint = toMapCoordinates(canvasPt);
	_measureInterface->setStartPoint(mpoint);
	
	_measureInterface->initAnnotation(mCanvas,NULL);
	_measureInterface->initCalculator(mCanvas);
	
	_capturing = true;
}

void MapToolMeasureLine::stopCapturing(const QPoint& canvasPt)
{
	QgsPointXY mpoint = toMapCoordinates(canvasPt);
	_measureInterface->setEndPoint(mpoint);
	_measureInterface->updateMeasure();

	_capturing = false;
}

void MapToolMeasureLine::clearCapturing()
{
	_measureInterface->removeAnnotation(mCanvas);
	_measureInterface->removeVisualItem(mCanvas);

	_capturing = false;
}

void MapToolMeasureLine::canvasReleaseEvent(QgsMapMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		if (!_capturing)
			startCapturing(e->pos());
		else if (_capturing)
		{
			stopCapturing(e->pos());
			notify();
		}
	}
	else if (e->button() == Qt::RightButton)
	{
		if (_capturing)
			clearCapturing();
	}
}

void MapToolMeasureLine::canvasMoveEvent(QgsMapMouseEvent *e)
{
	if (!_capturing)
		return;

	QgsPointXY mpoint = toMapCoordinates(e->pos());
	_measureInterface->tryingMeasure(mpoint);
	_measureInterface->annotation()->moveTo(e->pos());
}

void MapToolMeasureLine::notify()
{
	emit(measureAdded(_measureInterface->startPoint(),
						_measureInterface->endPoint(),
						_measureInterface->visualItem(),
						_measureInterface->annotation()));

	_measureInterface->resetAnnotation();
	_measureInterface->resetAnnotation();
}
