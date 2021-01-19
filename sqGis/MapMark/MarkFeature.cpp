#include "MarkFeature.h"
#include "PointMarkFeature.h"
#include "LineMarkFeature.h"
#include "PolygonMarkFeature.h"

unsigned long long MarkFeature::_FEATURE_COUNT_ID = 0;

QStringList MarkFeature::MarkTypeCaption =
			QStringList() << QStringLiteral("点") << QStringLiteral("折线") << QStringLiteral("多边形");

MarkFeature::MarkFeature(const QString& name, QgsWkbTypes::GeometryType mark_type)
	: QgsFeature(MarkFeature::getFeaturdId())
{
	_name = name;
	_markType = mark_type;
}


MarkFeature::~MarkFeature()
{
}

void MarkFeature::setAttrs()
{
	setAttribute("name", name());
	setAttribute("category", category());
	setAttribute("state", state());
}

void MarkFeature::fromPoints(QVector<QgsPoint> points)
{
	setGeometryFromPoints(points);
	setAttrs();
}

MarkFeature* MarkFeature::createMarkFeature(QgsWkbTypes::GeometryType type, const QString& name)
{
	MarkFeature* feature;
	switch (type)
	{
	case QgsWkbTypes::PointGeometry:
		feature = new PointMarkFeature(name);
		break;
	case QgsWkbTypes::LineGeometry:
		feature = new LineMarkFeature(name);
		break;
	case QgsWkbTypes::PolygonGeometry:
		feature = new PolygonMarkFeature(name);
		break;
	}
	return feature;
}
