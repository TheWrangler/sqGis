#pragma once
#include "GcVertex.h"

namespace GeoCalculator
{
	class __declspec(dllexport) GcGeoCalculator
	{
	public:
		GcGeoCalculator(bool rad = false);
		GcGeoCalculator(const GcVertex& src, const GcVertex& dst, bool rad = false);
		GcGeoCalculator(double srcLon, double srcLat, double srcHei, double dstLon, double dstLat, double dstHei, bool rad = false);
		~GcGeoCalculator();

	protected:
		GcVertex _srcVertex;
		GcVertex _dstVertex;
		bool _rad;

		GcVertex _dstCoorInSrc;

	protected:
		void update();

	public:
		bool rad() const { return _rad; }
		const GcVertex& srcVertex() const { return _srcVertex; }
		const GcVertex& dstVertex() const { return _dstVertex; }
		void setSrcVertex(const GcVertex& vertex, bool rad = false);
		void setDstVertex(const GcVertex& vertex, bool rad = false);

		GcVertex srcCoorInEarth() const;
		GcVertex dstCoorInEarth() const;
		GcVertex dstCoorInSrc() const;

		double azi() const;//方位角度
		double ele() const;//俯仰角度
		double distance() const;//空间距离
	};
}


