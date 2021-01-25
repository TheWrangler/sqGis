#include "LineMarkFeature.h"
#include <QColor>


LineMarkFeature::LineMarkFeature(const QString& name)
	: MarkFeature(name,QgsWkbTypes::LineGeometry)
{
	_width = 0.5;
	_color = "blue";
	_linestyle = "dash";
}

LineMarkFeature::~LineMarkFeature()
{
}

void LineMarkFeature::setGeometryFromPoints(const QVector<QgsPoint> points)
{
	QgsGeometry geo;
	geo.addPart(points, QgsWkbTypes::LineGeometry);
	setGeometry(geo);
}

void LineMarkFeature::setAttrs()
{
	MarkFeature::setAttrs();

	setAttribute("width", width());
	setAttribute("color", color());
	setAttribute("linestyle", lineStyle());
}
