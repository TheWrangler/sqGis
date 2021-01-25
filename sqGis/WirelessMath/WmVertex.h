#pragma once

class WmVertex
{
public:
	WmVertex();
	WmVertex(double x, double y, double z, double m = 0);
	~WmVertex();

protected:
	double _x;
	double _y;
	double _z;
	double _m;

public:
	void setX(double x) { _x = x; }
	void setY(double y) { _y = y; }
	void setZ(double z) { _z = z; }
	void setM(double m) { _m = m; }

	void setXYZ(double x, double y, double z) { _x = x; _y = y; _z = z; }

	double x() const { return _x; }
	double y() const { return _y; }
	double z() const { return _z; }
	double m() const { return _m; }

	WmVertex operator+(const WmVertex& vertex) const;
	WmVertex operator-(const WmVertex& vertex) const;
	double operator*(const WmVertex& vertex) const;//向量点乘积
	double norm() const;//向量的模
	double dist(const WmVertex& vertex) const;//两个向量的欧式距离
};

