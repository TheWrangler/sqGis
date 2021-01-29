#include "DistanceMeasureAnnotationItem.h"
#include <QTextDocument>

DistanceMeasureAnnotationItem::DistanceMeasureAnnotationItem(QgsTextAnnotation* annotation, QgsMapCanvas* mapCanvas)
	: MeasureAnnotationItem(annotation,mapCanvas)
{
	_format = QStringLiteral("�ر����:%1km\n�ռ����:%2km\n��λ�Ƕ�:%3��\n�����Ƕ�:%4��\n");
}


DistanceMeasureAnnotationItem::~DistanceMeasureAnnotationItem()
{
}

QString DistanceMeasureAnnotationItem::format() const
{
	QString content = _format.arg(_measureValue[0]).arg(_measureValue[1]).arg(_measureValue[2]).arg(_measureValue[3]);
	return content;
}

void DistanceMeasureAnnotationItem::updateSurfaceDistance(double distance)
{
	setMeasureVariable(DistanceMeasureRole_SurfaceDistance,distance);
}

void DistanceMeasureAnnotationItem::updateSpaceDistance(double distance)
{
	setMeasureVariable(DistanceMeasureRole_SpaceDistance, distance);
}

void DistanceMeasureAnnotationItem::updateAzi(double azi)
{
	setMeasureVariable(DistanceMeasureRole_Azi, azi);
}

void DistanceMeasureAnnotationItem::updateEle(double ele)
{
	setMeasureVariable(DistanceMeasureRole_Ele, ele);
}