#pragma once
#include "MarkFeature.h"
#include <QColor>

class LineMarkFeature : public MarkFeature
{
public:
	LineMarkFeature(const QString& name);
	~LineMarkFeature();

protected:
	double _width;
	QString _color;
	QString _linestyle;

protected:
	void setGeometryFromPoints(const QVector<QgsPoint> points);
	void setAttrs();

public:
	double width() const { return _width; }
	QString color() const { return _color; }
	QString lineStyle() const { return _linestyle; }
	void setWidth(double width) { _width = width; }
	void setColor(QString color) { _color = color; }
	void setLineStyle(QString linestyle) { _linestyle = linestyle; }
};

