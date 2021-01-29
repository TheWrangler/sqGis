#include "MeasureAnnotationItem.h"

MeasureAnnotationItem::MeasureAnnotationItem(QgsTextAnnotation* annotation, QgsMapCanvas* mapCanvas)
	: QgsMapCanvasAnnotationItem(annotation, mapCanvas)
{
	_format = "";

	for (unsigned char i = 0; i < MEASURE_VARIABLE_ROLE; i++)
		_measureValue[i] = 0;
}


MeasureAnnotationItem::~MeasureAnnotationItem()
{
}

void MeasureAnnotationItem::moveTo(QPoint point)
{
	QgsPointXY pt = toMapCoordinates(point);
	annotation()->setMapPosition(pt);
}

void MeasureAnnotationItem::updateAnnotation()
{
	QgsTextAnnotation* textAnnotation = (QgsTextAnnotation*)annotation();
	QString content = format();

	QTextDocument* textDocument = new QTextDocument();
	textDocument->setPlainText(content);
	textAnnotation->setDocument(textDocument);
}

QgsTextAnnotation* MeasureAnnotationItem::createTextAnnotation(QgsMapLayer* layer)
{
	QgsTextAnnotation* textAnnotation = new QgsTextAnnotation();
	if(layer != NULL)
		textAnnotation->setMapLayer(layer);//����ǵĿɼ�����Ҫ��ĳ��ͼ��ͬ��������Ҫ������ͼ��
	textAnnotation->setFrameSize(QSizeF(140, 70));
	
	QTextDocument* textDocument = new QTextDocument();
	textAnnotation->setDocument(textDocument);

	return textAnnotation;
}