#pragma once
#include <qgsmapcanvas.h>
#include <qgsmaplayer.h>
#include <qgsmapmouseevent.h>
#include <qgsmaptoolidentify.h>

class MapToolMovePoint : public QgsMapToolIdentify
{
public:
	MapToolMovePoint(QgsMapCanvas *canvas, QgsMapLayer* layer);
	~MapToolMovePoint();

protected:
	QgsMapLayer* _layer;
	bool _dragging;
	bool _feature;
	QgsFeatureId _featureId;

public:
	void canvasPressEvent(QgsMapMouseEvent *e);
	void canvasMoveEvent(QgsMapMouseEvent *e);
	void canvasReleaseEvent(QgsMapMouseEvent *e);
};

