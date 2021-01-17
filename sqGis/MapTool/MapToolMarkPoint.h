#pragma once
#include "MapToolMarkAbstract.h"

class MapToolMarkPoint : public MapToolMarkAbstract
{
	Q_OBJECT

public:
	MapToolMarkPoint(QgsMapCanvas *canvas,QgsMapLayer* layer);
	~MapToolMarkPoint();

protected:
	QgsPointXY _pt;

protected:
	void notify();
	void vertexPoints();

public:
	void canvasReleaseEvent(QgsMapMouseEvent *e);
};

