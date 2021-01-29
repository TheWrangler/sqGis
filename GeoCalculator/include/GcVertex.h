#pragma once

namespace GeoCalculator
{
	class __declspec(dllexport) GcVertex
	{
	public:
		GcVertex();
		GcVertex(double x, double y, double z, double m = 0);
		~GcVertex();

	protected:
		double _x;
		double _y;
		double _z;
		double _m;

	public:
		void setX(double x) { _x = x; }
		void setY(double y) { _y = y; }
		void setZ(double z) { _z = z; }
		virtual void setM(double m) { _m = m; }

		void setXYZ(double x, double y, double z) { _x = x; _y = y; _z = z; }

		double x() const { return _x; }
		double y() const { return _y; }
		double z() const { return _z; }
		virtual double m() const { return _m; }

		GcVertex operator+(const GcVertex& vertex) const;
		GcVertex operator-(const GcVertex& vertex) const;
		double operator*(const GcVertex& vertex) const;//向量点乘积
		double norm() const;//向量的模
		double dist(const GcVertex& vertex) const;//两个向量的欧式距离
	};
}


