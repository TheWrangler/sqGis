#include "MapToolMarkAbstract.h"



MapToolMarkAbstract::MapToolMarkAbstract(QgsMapCanvas* canvas, QgsMapLayer* layer)
	: QgsMapTool(canvas)
{
	_layer = layer;
	_vertexPoints.clear();
	setCursor(Qt::CrossCursor);
}


MapToolMarkAbstract::~MapToolMarkAbstract()
{
}
