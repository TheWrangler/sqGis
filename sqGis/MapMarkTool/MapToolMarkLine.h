#pragma once
#include "MapToolMarkAbstract.h"
#include <qgsrubberband.h>

class MapToolMarkLine : public MapToolMarkAbstract
{
	Q_OBJECT

public:
	MapToolMarkLine(QgsMapCanvas* canvas,QgsMapLayer* layer);
	~MapToolMarkLine();

protected:
	QVector<QgsPointXY> _capturedPoints;

	bool _capturing;
	QgsRubberBand* _rubberBand;
	QgsRubberBand* _tempRubberBand;

protected:
	void transformCoordinates(const QPoint& canvasPt, QgsPointXY& layerPt, QgsPointXY& mapPt);

	virtual void notify();
	virtual void vertexPoints();

	virtual void startCapturing(const QPoint& canvasPt);
	virtual void stopCapturing();

	virtual void initRubberBand();
	virtual void addVertex(const QPoint& canvasPt);
	virtual void removeLastVertex();

public:
	void canvasReleaseEvent(QgsMapMouseEvent *e);
	void canvasMoveEvent(QgsMapMouseEvent *e);
	void keyPressEvent(QKeyEvent *e);
};

