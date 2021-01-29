#pragma once
#include "MeasureAnnotationItem.h"

class AreaMeasureAnnotationItem : public MeasureAnnotationItem
{
public:
	AreaMeasureAnnotationItem(QgsTextAnnotation* annotation, QgsMapCanvas* mapCanvas);
	~AreaMeasureAnnotationItem();

	typedef enum tagAreaMeasureRole
	{
		AreaMeasureRole_SurfaceArea = 0,
		AreaMeasureRole_SpaceArea
	}AreaMeasureRole;

protected:
	QString format() const;

public:
	void updateSurfaceArea(double area);
	void updateSpaceArea(double area);
};

