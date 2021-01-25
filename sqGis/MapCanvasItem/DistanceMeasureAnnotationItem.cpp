#include "DistanceMeasureAnnotationItem.h"
#include <QTextDocument>

QString DistanceMeasureAnnotationItem::_format = QStringLiteral("地表距离:%1km\n空间距离:%2km\n方位角度:%3°\n俯仰角度:%4°\n");

DistanceMeasureAnnotationItem::DistanceMeasureAnnotationItem(QgsTextAnnotation* annotation, QgsMapCanvas* mapCanvas)
	: QgsMapCanvasAnnotationItem(annotation,mapCanvas)
{
	_surfaceDistance = 0.0;
	_spaceDistance = 0.0;
	_azi = 0.0;
	_ele = 0.0;
}


DistanceMeasureAnnotationItem::~DistanceMeasureAnnotationItem()
{
}

void DistanceMeasureAnnotationItem::moveTo(QPoint point)
{
	QgsPointXY pt = toMapCoordinates(point);
	annotation()->setMapPosition(pt);
}

void DistanceMeasureAnnotationItem::updateSurfaceDistance(double distance)
{
	updateAll(distance, _spaceDistance,_azi,_ele);
}

void DistanceMeasureAnnotationItem::updateSpaceDistance(double distance)
{
	updateAll(_surfaceDistance, distance, _azi, _ele);
}

void DistanceMeasureAnnotationItem::updateAzi(double azi)
{
	updateAll(_surfaceDistance, _spaceDistance, azi, _ele);
}

void DistanceMeasureAnnotationItem::updateEle(double ele)
{
	updateAll(_surfaceDistance, _spaceDistance, _azi, ele);
}

void DistanceMeasureAnnotationItem::updateAll(double surfaceDistance, double spaceDistance, double azi, double ele)
{
	_surfaceDistance = surfaceDistance;
	_spaceDistance = spaceDistance;
	_azi = azi;
	_ele = ele;

	QgsTextAnnotation* textAnnotation = (QgsTextAnnotation*)annotation();
	QString content = _format.arg(_surfaceDistance).arg(_spaceDistance).arg(_azi).arg(_ele);

	QTextDocument* textDocument = new QTextDocument();
	textDocument->setPlainText(content);
	textAnnotation->setDocument(textDocument);
}

DistanceMeasureAnnotationItem* DistanceMeasureAnnotationItem::create(QgsMapCanvas* mapCanvas, QgsMapLayer* layer)
{
	QgsTextAnnotation* textAnnotation = new QgsTextAnnotation();
	if(layer != NULL)
		textAnnotation->setMapLayer(layer);//若标记的可见性需要与某个图层同步，则需要关联该图层
	textAnnotation->setFrameSize(QSizeF(150, 70));

	QTextDocument* textDocument = new QTextDocument();
	textDocument->setPlainText(QStringLiteral("地表距离:\n空间距离:\n方位角度:\n俯仰角度:\n"));
	textAnnotation->setDocument(textDocument);

	DistanceMeasureAnnotationItem* annotation = new DistanceMeasureAnnotationItem(textAnnotation, mapCanvas);
	return annotation;
}