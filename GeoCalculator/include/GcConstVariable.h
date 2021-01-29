#pragma once

namespace GeoCalculator
{
	class __declspec(dllexport) GcConstVariable
	{
	public:
		GcConstVariable();
		~GcConstVariable();

	public:
		static const double pi;
		static const double a;//大地椭球长半轴，km
		static const double e2;//第一偏心率平方

	public:
		static double angle2rad(double angle);
		static double rad2angle(double rad);
		static double n(double lat, bool rad = false);//卯酉圈曲率半径
	};
}


