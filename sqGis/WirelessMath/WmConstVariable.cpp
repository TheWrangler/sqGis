#include "WmConstVariable.h"
#include "math.h"

const double WmConstVariable::pi = 3.1415926;
const double WmConstVariable::a = 6378.137;
const double WmConstVariable::e2 = 0.00669437999013;

WmConstVariable::WmConstVariable()
{
}


WmConstVariable::~WmConstVariable()
{
}

double WmConstVariable::angle2rad(double angle)
{
	return angle / 180.0 * pi;
}

double WmConstVariable::rad2angle(double rad)
{
	return rad * 180.0 / pi;
}

double WmConstVariable::n(double lat, bool rad)
{
	if (!rad)
		lat = angle2rad(lat);

	return a / sqrt(1 - e2*sin(lat)*sin(lat));
}
