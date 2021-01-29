#pragma once
#include <qgsmapcanvas.h>
#include <qgsmapcanvasannotationitem.h>
#include <qgstextannotation.h>

class MeasureAnnotationItem : public QgsMapCanvasAnnotationItem
{
public:
	MeasureAnnotationItem(QgsTextAnnotation* annotation, QgsMapCanvas* mapCanvas);
	~MeasureAnnotationItem();

	static const unsigned char MEASURE_VARIABLE_ROLE = 10;

protected:
	QString _format;
	double _measureValue[MEASURE_VARIABLE_ROLE];

protected:
	virtual QString format() const = 0;

public:
	void setFormat(const QString& format) { _format = format; }
	void setMeasureVariable(unsigned char role, double value) { _measureValue[role] = value; }
	
	void moveTo(QPoint point);
	void updateAnnotation();

	static QgsTextAnnotation* createTextAnnotation(QgsMapLayer* layer = NULL);
};

