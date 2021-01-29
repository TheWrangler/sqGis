#pragma once
#include "../MapCanvasItem/MeasureAnnotationItem.h"

class MapMeasureInterface
{
public:
	MapMeasureInterface();
	~MapMeasureInterface();

protected:
	MeasureAnnotationItem* _annotation;//测量结果标记
	QgsMapCanvasItem* _visualItem;//测量可视对象

public:
	void moveAnnotationItem(const QPoint& point);

public:
	MeasureAnnotationItem* annotation() const { return _annotation; }
	void removeAnnotation(QgsMapCanvas* canvas);
	void resetAnnotation() { _annotation = NULL; }

	QgsMapCanvasItem* visualItem() const { return _visualItem; }
	void removeVisualItem(QgsMapCanvas* canvas);
	void resetVisualItem() { _visualItem = NULL; }

	virtual void initAnnotation(QgsMapCanvas* canvas, QgsMapLayer* layer) = 0;
	virtual void initCalculator(QgsMapCanvas* canvas) = 0;
	virtual void updateMeasure() = 0;
	virtual void tryingMeasure(const QgsPointXY& point) = 0;
};

