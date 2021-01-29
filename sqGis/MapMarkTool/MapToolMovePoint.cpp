#include "MapToolMovePoint.h"
#include <qgsvectorlayer.h>
#include "../options.h"

MapToolMovePoint::MapToolMovePoint(QgsMapCanvas *canvas, QgsMapLayer* layer)
	: QgsMapToolIdentify(canvas)
{
	_layer = layer;
	_dragging = false;
	_feature = false;

	setCursor(Qt::CrossCursor);
}

MapToolMovePoint::~MapToolMovePoint()
{
}

void MapToolMovePoint::canvasPressEvent(QgsMapMouseEvent *e)
{
	QList<IdentifyResult> res = identify(e->x(), e->y(), QList<QgsMapLayer*>() << _layer, TopDownAll);
	if (res.count() > 0)
	{
		_dragging = true;
		_feature = true;
		_featureId = res[0].mFeature.id();
	}
	else
	{
		_dragging = false;
		_feature = false;
	}
}

void MapToolMovePoint::canvasMoveEvent(QgsMapMouseEvent *e)
{
	if (!_dragging)
		return;

	QgsPointXY pt = toLayerCoordinates(_layer, e->pos());

	QgsGeometry geo;
	QgsPointSequence points;
	points.append(QgsPoint(pt.x(), pt.y(), 0));
	geo.addPart(points, QgsWkbTypes::PointGeometry);

	QgsVectorLayer* layer = dynamic_cast<QgsVectorLayer*>(_layer);
	if (layer == NULL)
	{
#if PROMPT_CRITICAL_MSG
		qCritical() << QStringLiteral("×ª»»ÎªÊ¸Á¿Í¼²ã") << _layer->name() << QStringLiteral("Ê§°Ü!");
#endif
	}
	layer->changeGeometry(_featureId,geo);
	mCanvas->refresh();
}

void MapToolMovePoint::canvasReleaseEvent(QgsMapMouseEvent *e)
{
	_dragging = false;
	_feature = false;
}
