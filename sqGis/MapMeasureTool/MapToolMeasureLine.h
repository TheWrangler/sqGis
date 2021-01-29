#pragma once
#include <qgsmaptool.h>
#include <qgsmapcanvas.h>
#include <qgsmaplayer.h>
#include <qgsmapmouseevent.h>
#include "../MapMeasure/MapMeasureLine.h"

class MapToolMeasureLine : public QgsMapTool
{
	Q_OBJECT

public:
	MapToolMeasureLine(QgsMapCanvas *canvas);
	~MapToolMeasureLine();

protected:
	MapMeasureLine* _measureInterface;
	bool _capturing;
	
protected:
	void notify();
	void startCapturing(const QPoint& canvasPt);
	void stopCapturing(const QPoint& canvasPt);
	void clearCapturing();

public:
	void setMeasureInterface(MapMeasureLine* measureInterface);
	MapMeasureLine* measureInterface();
	void removeMeasureInterface();

	void canvasReleaseEvent(QgsMapMouseEvent *e);
	void canvasMoveEvent(QgsMapMouseEvent *e);

signals:
	void measureAdded(const QgsPointXY& startPoint,const QgsPointXY& endPoint, QgsMapCanvasItem* visualItem, MeasureAnnotationItem* annotation);
};

