#include "MapToolDistanceMeasure.h"
#include "MapToolMath.h"
#include <qgstextannotation.h>
#include <QTextDocument>

MapToolDistanceMeasure::MapToolDistanceMeasure(QgsMapCanvas* canvas, QgsMapLayer* layer)
	: MapToolMarkLine(canvas, layer)
{
	_annotation = NULL;
}


MapToolDistanceMeasure::~MapToolDistanceMeasure()
{
}

void MapToolDistanceMeasure::initGeoCalculator(const QgsPointXY& srcPoint)
{
	QgsPointXY point = MapToolMath::convertCoor(srcPoint);
	_wmGeoCalculator.setSrcVertex(WmVertex(point.x(), point.y(), 0.0));
}

void MapToolDistanceMeasure::updateDistance(const QgsPointXY& destPoint)
{
	QgsPointXY point = MapToolMath::convertCoor(destPoint);
	_wmGeoCalculator.setDstVertex(WmVertex(point.x(), point.y(), 0.0));
}

void MapToolDistanceMeasure::initAnnotation()
{
	_annotation = DistanceMeasureAnnotationItem::create(mCanvas, NULL);
}

void MapToolDistanceMeasure::startCapturing()
{
	initAnnotation();
	MapToolMarkLine::startCapturing();
}

void MapToolDistanceMeasure::stopCapturing()
{
	MapToolMarkLine::stopCapturing();

	if (_annotation != NULL && _vertexPoints.count() != 2)
	{
		mCanvas->scene()->removeItem(_annotation);
		_annotation = NULL;
	}
}

void MapToolDistanceMeasure::canvasReleaseEvent(QgsMapMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		if (!_capturing)
		{ 
			startCapturing();
			initGeoCalculator(toLayerCoordinates(_layer,e->pos()));
		}

		addVertex(e->pos());
		if (_capturedPoints.count() == 2)
		{
			stopCapturing();
			notify();
		}
	}
	else if (e->button() == Qt::RightButton)
	{
		stopCapturing();
		notify();
	}
}

void MapToolDistanceMeasure::canvasMoveEvent(QgsMapMouseEvent *e)
{
	static unsigned int index = 0;
	if (_annotation != NULL && _capturing)
	{ 
		updateDistance(toLayerCoordinates(_layer, e->pos()));
		_annotation->updateAll(0.0, _wmGeoCalculator.distance(),_wmGeoCalculator.azi(),_wmGeoCalculator.ele());
		_annotation->moveTo(e->pos());
	}

	MapToolMarkLine::canvasMoveEvent(e);
}

void MapToolDistanceMeasure::notify()
{
	if (_vertexPoints.count() != 2)
		return;

	emit(measureAdded(_layer, QgsWkbTypes::LineGeometry, _vertexPoints, _annotation));
	_annotation = NULL;
}
