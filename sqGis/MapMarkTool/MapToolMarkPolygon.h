#pragma once
#include "MapToolMarkLine.h"

class MapToolMarkPolygon : public MapToolMarkLine
{
	Q_OBJECT

public:
	MapToolMarkPolygon(QgsMapCanvas* canvas, QgsMapLayer* layer);
	~MapToolMarkPolygon();

protected:
	void vertexPoints();
	void notify();

	void initRubberBand();
	void addVertex(const QPoint& canvasPt);
	void removeLastVertex();
};

