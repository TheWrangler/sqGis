#pragma once
#include <qgsmapcanvas.h>
#include <qgsmaplayer.h>
#include <qgsmapmouseevent.h>
#include <qgsmaptoolidentify.h>

class MapToolMarkSelect : public QgsMapToolIdentify
{
public:
	MapToolMarkSelect(QgsMapCanvas *canvas);
	~MapToolMarkSelect();

protected:
	QgsMapLayer* _layer;

public:
	void canvasReleaseEvent(QgsMapMouseEvent *e);
};

