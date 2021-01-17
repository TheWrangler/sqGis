#include "MapToolMarkPoint.h"

MapToolMarkPoint::MapToolMarkPoint(QgsMapCanvas *canvas, QgsMapLayer* layer)
	: MapToolMarkAbstract(canvas, layer)
{

}

MapToolMarkPoint::~MapToolMarkPoint()
{
}

void MapToolMarkPoint::notify()
{
	if (!_vertexPoints.isEmpty())
		emit(markAdded(_layer, QgsWkbTypes::PointGeometry, _vertexPoints));
}

void MapToolMarkPoint::vertexPoints()
{
	_vertexPoints.clear();
	_vertexPoints.push_back(_pt);
}

void MapToolMarkPoint::canvasReleaseEvent(QgsMapMouseEvent *e)
{
	_pt = toLayerCoordinates(_layer,e->pos());
	vertexPoints();
	notify();
}
