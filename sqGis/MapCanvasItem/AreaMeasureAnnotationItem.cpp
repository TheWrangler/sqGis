#include "AreaMeasureAnnotationItem.h"



AreaMeasureAnnotationItem::AreaMeasureAnnotationItem(QgsTextAnnotation* annotation, QgsMapCanvas* mapCanvas)
	: MeasureAnnotationItem(annotation, mapCanvas)
{
	_format = QStringLiteral("�ر����:%1km^2\n�ռ����:%2km^2");
}


AreaMeasureAnnotationItem::~AreaMeasureAnnotationItem()
{
}

QString AreaMeasureAnnotationItem::format() const
{
	QString content = _format.arg(_measureValue[0]).arg(_measureValue[1]);
	return content;
}

void AreaMeasureAnnotationItem::updateSurfaceArea(double area)
{
	setMeasureVariable(AreaMeasureRole_SurfaceArea, area);
}

void AreaMeasureAnnotationItem::updateSpaceArea(double area)
{
	setMeasureVariable(AreaMeasureRole_SpaceArea, area);
}