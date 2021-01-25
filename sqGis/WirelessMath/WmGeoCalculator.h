#pragma once
#include "WmVertex.h"

class WmGeoCalculator
{
public:
	WmGeoCalculator();
	WmGeoCalculator(const WmVertex& src, const WmVertex& dst, bool rad = false);
	WmGeoCalculator(double srcLon, double srcLat, double srcHei, double dstLon, double dstLat, double dstHei, bool rad = false);
	~WmGeoCalculator();

protected:
	WmVertex _srcVertex;
	WmVertex _dstVertex;
	bool _rad;

	WmVertex _dstCoorInSrc;

protected:
	void update();

public:
	const WmVertex& srcVertex() { return _srcVertex; }
	const WmVertex& dstVertex() { return _dstVertex; }
	void setSrcVertex(const WmVertex& vertex, bool rad = false) { _srcVertex = vertex; _rad = rad; update(); }
	void setDstVertex(const WmVertex& vertex, bool rad = false) { _dstVertex = vertex; _rad = rad; update(); }

	WmVertex srcCoorInEarth();
	WmVertex dstCoorInEarth();
	WmVertex dstCoorInSrc();

	double azi();//��λ�Ƕ�
	double ele();//�����Ƕ�
	double distance();//�ռ����
};

