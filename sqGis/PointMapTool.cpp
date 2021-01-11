#include "PointMapTool.h"
#include "qgsgeometry.h"
#include "qgsmapmouseevent.h"
#include "options.h"

#include <QDebug>

PointMapTool::PointMapTool(QgsMapCanvas* canvas)
			:QgsMapToolEmitPoint(canvas)
{
	
}


PointMapTool::~PointMapTool()
{
}

void PointMapTool::canvasPressEvent(QgsMapMouseEvent *e)
{
	QgsPointXY pt = toMapCoordinates(e->pixelPoint());

#if PROMPT_DEBUG_MSG
	qDebug() << "mouse clicked geometry position:" << pt.x() << pt.y();
#endif
}
