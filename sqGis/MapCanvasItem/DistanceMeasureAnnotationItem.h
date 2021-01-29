#pragma once
#include "MeasureAnnotationItem.h"

class DistanceMeasureAnnotationItem : public MeasureAnnotationItem
{
public:
	DistanceMeasureAnnotationItem(QgsTextAnnotation* annotation, QgsMapCanvas* mapCanvas);
	~DistanceMeasureAnnotationItem();

	typedef enum tagDistanceMeasureRole
	{
		DistanceMeasureRole_SurfaceDistance = 0,
		DistanceMeasureRole_SpaceDistance,
		DistanceMeasureRole_Azi,
		DistanceMeasureRole_Ele
	}DistanceMeasureRole;

protected:
	QString format() const;

public:
	void updateSurfaceDistance(double distance);
	void updateSpaceDistance(double distance);
	void updateAzi(double azi);
	void updateEle(double ele);
};

