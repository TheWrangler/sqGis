#include "MapToolMath.h"

MapToolMath::MapToolMath()
{
}


MapToolMath::~MapToolMath()
{
}

QgsPointXY MapToolMath::convertCoor(const QgsPointXY& src, bool toWSG84)
{
	QgsCoordinateReferenceSystem dstCrs("EPSG:4326");
	QgsCoordinateReferenceSystem srcCrs("EPSG:3857");
	QgsCoordinateTransform crsTrans;
	if (toWSG84)
	{
		crsTrans.setSourceCrs(srcCrs);
		crsTrans.setDestinationCrs(dstCrs);
	}
	else
	{
		crsTrans.setSourceCrs(dstCrs);
		crsTrans.setDestinationCrs(srcCrs);
	}

	QgsPointXY dst;
	dst = crsTrans.transform(src);
	return dst;
}