#pragma once
#include "MarkFeature.h"
class PolygonMarkFeature : public MarkFeature
{
public:
	PolygonMarkFeature(const QString& name);
	~PolygonMarkFeature();

protected:
	double _strokeWidth;
	QString _strokeColor;
	QString _strokeStyle;

	QString _filledColor;
	QString _filledStyle;

protected:
	void setGeometryFromPoints(const QVector<QgsPoint> points);
	void setAttrs();

public:
	double strokeWidth() const { return _strokeWidth; }
	QString strokeColor() const { return _strokeColor; }
	QString strokeStyle() const { return _strokeStyle; }
	QString filledColor() const { return _filledColor; }
	QString filledStyle() const { return _filledStyle; }

	void setStrokeWidth(unsigned int width) { _strokeWidth = width; }
	void setStrokeColor(QString color) { _strokeColor = color; }
	void setStrokeStyle(QString style) { _strokeStyle = style; }
	void setFilledColor(QString color) { _filledColor = color; }
	void setFilledStyle(QString style) { _filledStyle = style; }
};

