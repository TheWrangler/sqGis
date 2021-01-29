#pragma once
#include "GcGeoCalculator.h"

namespace GeoCalculator
{
	class GcPlatformCalculator : public GcGeoCalculator
	{
	public:
		GcPlatformCalculator(bool rad = false);
		GcPlatformCalculator(const GcVertex& src, const GcVertex& dst, const GcVertex& atti, bool rad = false);
		~GcPlatformCalculator();

	protected:
		GcVertex _attiVertex;//载体姿态

	protected:
		void update();

	public:
		const GcVertex& attiVertex() const { return _attiVertex; }
		void setAttiVertex(const GcVertex& vertex, bool rad = false);

		GcVertex dstCoorInSrc() const;

		double azi() const;//方位角度
		double ele() const;//俯仰角度
	};
}

