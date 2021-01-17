#include "MapToolMarkSelect.h"
#include <qgsvectorlayer.h>
#include "../options.h"

MapToolMarkSelect::MapToolMarkSelect(QgsMapCanvas *canvas)
	: QgsMapToolIdentify(canvas)
{
	_layer = NULL;
	setCursor(Qt::ArrowCursor);
}

MapToolMarkSelect::~MapToolMarkSelect()
{
}

void MapToolMarkSelect::canvasReleaseEvent(QgsMapMouseEvent *e)
{
	QList<IdentifyResult> res = identify(e->x(), e->y(), TopDownStopAtFirst,VectorLayer);
	if (res.count() > 0)
	{
		QgsMapLayer* layer = res[0].mLayer;
		QgsFeature feature = res[0].mFeature;

		QgsVectorLayer* vectorLayer = dynamic_cast<QgsVectorLayer*>(layer);

		if (vectorLayer == NULL)
		{
#if PROMPT_CRITICAL_MSG
			qCritical() << QStringLiteral("×ª»»ÎªÊ¸Á¿Í¼²ã") << _layer->name() << QStringLiteral("Ê§°Ü!");
#endif
		}

		if (e->modifiers() & Qt::ShiftModifier)
			vectorLayer->select(feature.id());
		else
		{
			vectorLayer->removeSelection();
			vectorLayer->select(feature.id());
		}

		_layer = layer;
	}
	else
	{
		if(_layer != NULL)
			((QgsVectorLayer*)_layer)->removeSelection();
	}
}
