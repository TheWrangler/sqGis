#include "DistanceMeasureAnnotationItem.h"
#include <QTextDocument>

QString DistanceMeasureAnnotationItem::_format = QStringLiteral("�ر����:%1km\n�ռ����:%2km\n��λ�Ƕ�:%3��\n�����Ƕ�:%4��\n");

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
		textAnnotation->setMapLayer(layer);//����ǵĿɼ�����Ҫ��ĳ��ͼ��ͬ��������Ҫ������ͼ��
	textAnnotation->setFrameSize(QSizeF(150, 70));

	QTextDocument* textDocument = new QTextDocument();
	textDocument->setPlainText(QStringLiteral("�ر����:\n�ռ����:\n��λ�Ƕ�:\n�����Ƕ�:\n"));
	textAnnotation->setDocument(textDocument);

	DistanceMeasureAnnotationItem* annotation = new DistanceMeasureAnnotationItem(textAnnotation, mapCanvas);
	return annotation;
}