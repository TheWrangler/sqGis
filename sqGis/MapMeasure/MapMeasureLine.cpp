#include "MapMeasureLine.h"
#include <qgsrubberband.h>

MapMeasureLine::MapMeasureLine()
{

}

MapMeasureLine::~MapMeasureLine()
{
}

void MapMeasureLine::setStartPoint(const QgsPointXY& point)
{
	_startPoint = point;

	if (_visualItem == NULL)
		return;

	QgsRubberBand* rubberBand = (QgsRubberBand*)_visualItem;
	int bandSize = rubberBand->numberOfVertices();
	if(bandSize == 0)
		rubberBand->addPoint(_startPoint);
	else if (bandSize == 1)
	{
		rubberBand->removeLastPoint(0, false);
		rubberBand->addPoint(_startPoint);
	}
}

void MapMeasureLine::setEndPoint(const QgsPointXY& point)
{
	_endPoint = point;

	if (_visualItem == NULL)
		return;
	QgsRubberBand* rubberBand = (QgsRubberBand*)_visualItem;
	rubberBand->addPoint(_endPoint);

	moveVisualItem(_endPoint);
}

void MapMeasureLine::moveVisualItem(const QgsPointXY& point)
{
	QgsRubberBand* rubberBand = (QgsRubberBand*)_visualItem;
	rubberBand->movePoint(point);
}

void MapMeasureLine::initAnnotation(QgsMapCanvas* canvas, QgsMapLayer* layer)
{
	QColor color = QColor("red");
	color.setAlphaF(0.78);

	QgsRubberBand* rubberBand = new QgsRubberBand(canvas, QgsWkbTypes::LineGeometry);
	rubberBand->setWidth(2);
	rubberBand->setColor(color);
	rubberBand->setLineStyle(Qt::DotLine);

	if (rubberBand->numberOfVertices() == 0)
		rubberBand->addPoint(_startPoint);

	_visualItem = rubberBand;
	_visualItem->show();
}
