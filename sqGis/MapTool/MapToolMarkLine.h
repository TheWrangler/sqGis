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
	virtual void notify();
	virtual void vertexPoints();

	void transformCoordinates(QPoint& canvasPt, QgsPointXY& layerPt, QgsPointXY& mapPt);
	void startCapturing();
	void stopCapturing();

	virtual void initRubberBand();
	virtual void addVertex(QPoint canvasPt);
	virtual void removeLastVertex();

public:
	void canvasReleaseEvent(QgsMapMouseEvent *e);
	void canvasMoveEvent(QgsMapMouseEvent *e);
	void keyPressEvent(QKeyEvent *e);
};

