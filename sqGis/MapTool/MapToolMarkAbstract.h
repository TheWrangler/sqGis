#pragma once
#include <qgsmaptool.h>
#include <qgsmapcanvas.h>
#include <qgsmaplayer.h>
#include <qgsmapmouseevent.h>

class MapToolMarkAbstract : public QgsMapTool
{
	Q_OBJECT

public:
	MapToolMarkAbstract(QgsMapCanvas *canvas, QgsMapLayer* layer);
	~MapToolMarkAbstract();

protected:
	QgsMapLayer* _layer;
	QVector<QgsPointXY> _vertexPoints;

protected:
	virtual void notify()=0;
	virtual void vertexPoints()=0;

public:
	void setDestMarkLayer(QgsMapLayer* layer) { _layer = layer; }
	
signals:
	void markAdded(QgsMapLayer* layer, QgsWkbTypes::GeometryType type, const QVector<QgsPointXY>& points);
};

