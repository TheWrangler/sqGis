#include "PointMarkFeature.h"
#include <QCoreApplication>


PointMarkFeature::PointMarkFeature(const QString& name)
	: MarkFeature(name,QgsWkbTypes::PointGeometry)
{
	_svg= QCoreApplication::applicationDirPath() + "/markers/position.svg";
	_size = 13.0;
	_rotation = 0.0;
}


PointMarkFeature::~PointMarkFeature()
{
}

void PointMarkFeature::setGeometryFromPoints(const QVector<QgsPoint> points)
{
	QgsGeometry geo;
	geo.addPart(points, QgsWkbTypes::PointGeometry);
	setGeometry(geo);
}

void PointMarkFeature::setAttrs()
{
	MarkFeature::setAttrs();

	setAttribute("rotation", rotation());
	setAttribute("svg", svg());
	setAttribute("size", size());
}