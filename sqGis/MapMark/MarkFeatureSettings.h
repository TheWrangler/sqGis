#pragma once
#include <QVector>
#include <QString>
#include <qgspoint.h>

class MarkFeatureSettings
{
public:
	static QStringList MarkTypeCaption;

	MarkFeatureSettings(QString layer_name, QgsWkbTypes::GeometryType mark_type);
	~MarkFeatureSettings();

protected:
	QgsWkbTypes::GeometryType _markType;

	QString _layerName;

	QVector<QgsPoint> _points;

	QString _name;
	QString _category;
	QString _state;

	double _rotation;
	double _mark_size;

public:
	QgsWkbTypes::GeometryType markType() { return _markType; }

	void appendMarkPoint(QgsPoint& point);
	void clearMarkPoint();

	void setName(QString name) { _name = name; }
	void setCategory(QString category) { _category = category; }
	void setState(QString state) { _state = state; }
	void setRotation(double rotation) { _rotation = rotation; }
	void setMarkSize(double mark_size) { _mark_size = mark_size; }

	QString layerName() { return _layerName; }
	QVector<QgsPoint>* markPoints() { return &_points; }

	QString name() { return _name; }
	QString category() { return _category; }
	QString state() { return _state; }
	double rotation() { return _rotation; }
	double markSize() { return _mark_size; }
};

