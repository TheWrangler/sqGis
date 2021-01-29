#include "MapToolMarkLine.h"

MapToolMarkLine::MapToolMarkLine(QgsMapCanvas* canvas, QgsMapLayer* layer)
	: MapToolMarkAbstract(canvas,layer)
{
	_capturing = false;
	_rubberBand = NULL;
	_tempRubberBand = NULL;
}


MapToolMarkLine::~MapToolMarkLine()
{
}

void MapToolMarkLine::initRubberBand()
{
	QColor color = QColor("red");
	color.setAlphaF(0.78);

	_rubberBand = new QgsRubberBand(mCanvas, QgsWkbTypes::LineGeometry);
	_rubberBand->setWidth(2);
	_rubberBand->setColor(color);

	_tempRubberBand = new QgsRubberBand(mCanvas, QgsWkbTypes::LineGeometry);
	_tempRubberBand->setWidth(2);
	_tempRubberBand->setColor(color);
	_tempRubberBand->setLineStyle(Qt::DotLine);
}

void MapToolMarkLine::vertexPoints()
{
	_vertexPoints.clear();

	if (_capturedPoints.count() < 2)
		return;

	for (unsigned int i = 0; i < _capturedPoints.count(); i++)
		_vertexPoints.push_back(_capturedPoints.at(i));
}

void MapToolMarkLine::transformCoordinates(const QPoint& canvasPt, QgsPointXY& layerPt, QgsPointXY& mapPt)
{
	layerPt = toLayerCoordinates(_layer, canvasPt);
	mapPt = toMapCoordinates(canvasPt);
}

void MapToolMarkLine::startCapturing(const QPoint& canvasPt)
{
	initRubberBand();

	_rubberBand->show();
	_tempRubberBand->show();
	_capturing = true;
}

void MapToolMarkLine::stopCapturing()
{
	if (_rubberBand != NULL)
	{
		mCanvas->scene()->removeItem(_rubberBand);
		_rubberBand = NULL;
	}

	if(_tempRubberBand != NULL)
	{
		mCanvas->scene()->removeItem(_tempRubberBand);
		_tempRubberBand = NULL;
	}

	_capturing = false;
	vertexPoints();
	_capturedPoints.clear();
}

void MapToolMarkLine::addVertex(const QPoint& canvasPt)
{
	QgsPointXY layerPt;
	QgsPointXY mapPt;

	transformCoordinates(canvasPt,layerPt,mapPt);
	_capturedPoints.append(layerPt);
	
	_rubberBand->addPoint(mapPt);
	_tempRubberBand->reset(QgsWkbTypes::LineGeometry);
	_tempRubberBand->addPoint(mapPt);
}

void MapToolMarkLine::removeLastVertex()
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
	else _tempRubberBand->reset(QgsWkbTypes::LineGeometry);

	_capturedPoints.removeLast();
}

void MapToolMarkLine::notify()
{
	if (_vertexPoints.count() >= 2)
		emit(markAdded(_layer, QgsWkbTypes::LineGeometry, _vertexPoints));
}

void MapToolMarkLine::canvasReleaseEvent(QgsMapMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		if (!_capturing)
			startCapturing(e->pos());
		addVertex(e->pos());
	}
	else if (e->button() == Qt::RightButton)
	{
		stopCapturing();
		notify();
	}
}

void MapToolMarkLine::canvasMoveEvent(QgsMapMouseEvent *e)
{
	if (_tempRubberBand == NULL || !_capturing)
		return;

	QgsPointXY layerPt;
	QgsPointXY mapPt;

	transformCoordinates(e->pos(), layerPt, mapPt);

	_tempRubberBand->movePoint(mapPt);
}

void MapToolMarkLine::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Backspace || e->key() == Qt::Key_Delete)
	{
		removeLastVertex();
		e->ignore();
	}
	else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
	{
		stopCapturing();
		notify();
	}
}
