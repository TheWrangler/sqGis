#include "WmGeoCalculator.h"
#include "WmTransformMatrix.h"
#include "WmConstVariable.h"
#include "math.h"

WmGeoCalculator::WmGeoCalculator()
{
	_srcVertex = WmVertex(0, 0, 0);
	_dstVertex = WmVertex(0, 0, 0);
	_rad = false;

	update();
}

WmGeoCalculator::WmGeoCalculator(const WmVertex& src, const WmVertex& dst, bool rad)
{
	_srcVertex = src;
	_dstVertex = dst;
	_rad = rad;

	update();
}

WmGeoCalculator::WmGeoCalculator(double srcLon, double srcLat, double srcHei, double dstLon, double dstLat, double dstHei, bool rad)
{
	_srcVertex = WmVertex(srcLon, srcLat, srcHei);
	_dstVertex = WmVertex(dstLon, dstLat, dstHei);
	_rad = rad;

	update();
}

WmGeoCalculator::~WmGeoCalculator()
{
}

void WmGeoCalculator::update()
{
	_dstCoorInSrc = dstCoorInSrc();
}

WmVertex WmGeoCalculator::srcCoorInEarth()
{
	WmTransformMatrix wmTransformMatrix = WmTransformMatrix::createSpherial2Earth(_srcVertex, _rad);
	double n = WmConstVariable::n(_srcVertex.y(), _rad);
	WmVertex vertex(n, _srcVertex.z(), n*(1 - WmConstVariable::e2));
	return wmTransformMatrix.transform(vertex);
}

WmVertex WmGeoCalculator::dstCoorInEarth()
{
	WmTransformMatrix wmTransformMatrix = WmTransformMatrix::createSpherial2Earth(_dstVertex, _rad);
	double n = WmConstVariable::n(_dstVertex.y(), _rad);
	WmVertex vertex(n, _dstVertex.z(), n*(1 - WmConstVariable::e2));
	return wmTransformMatrix.transform(vertex);
}

WmVertex WmGeoCalculator::dstCoorInSrc()
{
	WmVertex wmSrcCoor = srcCoorInEarth();
	WmVertex wmDstCoor = dstCoorInEarth();
	WmTransformMatrix wmTransformMatrix = WmTransformMatrix::createEarth2Geo(_srcVertex, _rad);
	return wmTransformMatrix.transform(wmDstCoor - wmSrcCoor);
}

double WmGeoCalculator::azi()
{
	double azi = WmConstVariable::rad2angle(atan2(_dstCoorInSrc.x(),_dstCoorInSrc.y()));
	if (azi < 0)
		azi += 360;

	return azi;
}

double WmGeoCalculator::ele()
{
	double r = sqrt(_dstCoorInSrc.x() * _dstCoorInSrc.x() + _dstCoorInSrc.y() * _dstCoorInSrc.y());
	double ele = WmConstVariable::rad2angle(atan2(_dstCoorInSrc.z(), r));
	return ele;
}

double WmGeoCalculator::distance()
{
	return _dstCoorInSrc.norm();
}
