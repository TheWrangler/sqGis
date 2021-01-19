#include "PolygonMarkFeature.h"
#include <QColor>


PolygonMarkFeature::PolygonMarkFeature(const QString& name)
	: MarkFeature(name, QgsWkbTypes::PolygonGeometry)
{
	_strokeWidth = 1;
	_strokeColor = "green";
	_strokeStyle = "dot";
	_filledColor = "red";
	_filledStyle = "dense5";
}


PolygonMarkFeature::~PolygonMarkFeature()
{
}

void PolygonMarkFeature::setGeometryFromPoints(const QVector<QgsPoint> points)
{
	QgsGeometry geo;
	geo.addPart(points, QgsWkbTypes::PolygonGeometry);
	setGeometry(geo);
}

void PolygonMarkFeature::setAttrs()
{
	MarkFeature::setAttrs();

	setAttribute("strokewidth", strokeWidth());
	setAttribute("strokecolor", strokeColor());
	setAttribute("strokestyle", strokeStyle());
	setAttribute("filledcolor", filledColor());
	setAttribute("filledstyle", filledStyle());
}
