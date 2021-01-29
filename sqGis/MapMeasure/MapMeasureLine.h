#pragma once
#include "MapMeasureInterface.h"
#include <qgsrubberband.h>

class MapMeasureLine : public MapMeasureInterface
{
public:
	MapMeasureLine();
	~MapMeasureLine();

protected:
	QgsPointXY _startPoint;
	QgsPointXY _endPoint;

public:
	void moveVisualItem(const QgsPointXY& point);

	void setStartPoint(const QgsPointXY& point);
	void setEndPoint(const QgsPointXY& point);

	const QgsPointXY& startPoint() const { return _startPoint; }
	const QgsPointXY& endPoint() const { return _endPoint; }

	virtual void initAnnotation(QgsMapCanvas* canvas, QgsMapLayer* layer);
};

