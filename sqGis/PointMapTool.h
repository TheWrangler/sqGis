#pragma once
#include "qgsmaptoolemitpoint.h"

class PointMapTool : public QgsMapToolEmitPoint
{
public:
	PointMapTool(QgsMapCanvas* canvas);
	~PointMapTool();

public:
	void canvasPressEvent(QgsMapMouseEvent *e);
};

