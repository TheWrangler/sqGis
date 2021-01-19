#pragma once
#include "MarkFeature.h"

class PointMarkFeature : public MarkFeature
{
public:
	PointMarkFeature(const QString& name);
	~PointMarkFeature();

protected:
	QString _svg;
	double _size;
	double _rotation;

protected:
	void setGeometryFromPoints(const QVector<QgsPoint> points);
	void setAttrs();

public:
	QString svg() const { return _svg; }
	double size() const { return _size; }
	double rotation() const { return _rotation; }
	void setSvg(const QString& svg) { _svg = svg; }
	void setSize(double size) { _size = size; }
	void setRotation(double rotation) { _rotation = rotation; }
};

