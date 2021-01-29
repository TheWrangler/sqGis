#include "MapToolMarkPolygon.h"

MapToolMarkPolygon::MapToolMarkPolygon(QgsMapCanvas* canvas, QgsMapLayer* layer)
	: MapToolMarkLine(canvas,layer)
{
}

MapToolMarkPolygon::~MapToolMarkPolygon()
{
}

void MapToolMarkPolygon::initRubberBand()
{
	QColor color = QColor("red");
	color.setAlphaF(0.50);

	_rubberBand = new QgsRubberBand(mCanvas, QgsWkbTypes::PolygonGeometry);
	_rubberBand->setWidth(2);
	_rubberBand->setColor(color);

	_tempRubberBand = new QgsRubberBand(mCanvas, QgsWkbTypes::PolygonGeometry);
	_tempRubberBand->setWidth(2);
	_tempRubberBand->setColor(color);
	_tempRubberBand->setLineStyle(Qt::DotLine);
}

void MapToolMarkPolygon::vertexPoints()
{
	_vertexPoints.clear();

	if (_capturedPoints.count() < 3)
		return;

	for (unsigned int i = 0; i < _capturedPoints.count(); i++)
		_vertexPoints.push_back(_capturedPoints.at(i));

	_vertexPoints.push_back(_capturedPoints.at(0));
}

void MapToolMarkPolygon::addVertex(const QPoint& canvasPt)
{
	QgsPointXY layerPt;
	QgsPointXY mapPt;

	transformCoordinates(canvasPt, layerPt, mapPt);
	_capturedPoints.append(layerPt);

	_rubberBand->addPoint(mapPt);
	
	_tempRubberBand->reset(QgsWkbTypes::PolygonGeometry);
	const QgsPointXY* firstPoint = _rubberBand->getPoint(0, 0);
	_tempRubberBand->addPoint(*firstPoint);
	_tempRubberBand->movePoint(mapPt);
	_tempRubberBand->addPoint(mapPt);
}

void MapToolMarkPolygon::removeLastVertex()
{
	if (!_capturing)
		return;

	int bandSize = _rubberBand->numberOfVertices();
	int tempBandSize = _tempRubberBand->numberOfVertices();

	int numPoints = _capturedPoints.count();

	if (bandSize < 1 || numPoints < 1)
		return;

	_rubberBand->removePoint(-1);

	if (bandSize > 1)
	{
		if (tempBandSize > 1)
		{
			const QgsPointXY* point = _rubberBand->getPoint(0, bandSize - 2);
			_tempRubberBand->movePoint(tempBandSize - 2, *point);
		}
	}
	else _tempRubberBand->reset(QgsWkbTypes::PolygonGeometry);

	_capturedPoints.removeLast();
}

void MapToolMarkPolygon::notify()
{
	if (_vertexPoints.count() >= 4)                 
		emit(markAdded(_layer, QgsWkbTypes::PolygonGeometry, _vertexPoints));
}
