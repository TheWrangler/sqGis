#pragma once
#include <qgsmapcanvas.h>
#include <qgsmapcanvasannotationitem.h>
#include <qgstextannotation.h>

class DistanceMeasureAnnotationItem : public QgsMapCanvasAnnotationItem
{
public:
	DistanceMeasureAnnotationItem(QgsTextAnnotation* annotation, QgsMapCanvas* mapCanvas);
	~DistanceMeasureAnnotationItem();

protected:
	static QString _format;

	double _surfaceDistance;
	double _spaceDistance;
	double _azi;
	double _ele;

public:
	void moveTo(QPoint point);

	void updateSurfaceDistance(double distance);
	void updateSpaceDistance(double distance);
	void updateAzi(double azi);
	void updateEle(double ele);
	void updateAll(double surfaceDistance, double spaceDistance, double azi, double ele);

public:
	static DistanceMeasureAnnotationItem* create(QgsMapCanvas* mapCanvas,QgsMapLayer* layer = NULL);
};

