#include "WmVertex.h"
#include "math.h"


WmVertex::WmVertex()
{
	_x = 0.0;
	_y = 0.0;
	_z = 0.0;
	_m = 0.0;
}

WmVertex::WmVertex(double x, double y, double z, double m)
{
	_x = x;
	_y = y;
	_z = z;
	_m = m;
}


WmVertex::~WmVertex()
{
}

WmVertex WmVertex::operator+(const WmVertex& vertex) const
{
	WmVertex wmVertex;
	wmVertex._x = this->_x + vertex._x;
	wmVertex._y = this->_y + vertex._y;
	wmVertex._z = this->_z + vertex._z;
	return wmVertex;
}

WmVertex WmVertex::operator-(const WmVertex& vertex) const
{
	WmVertex wmVertex;
	wmVertex._x = this->_x - vertex._x;
	wmVertex._y = this->_y - vertex._y;
	wmVertex._z = this->_z - vertex._z; 
	return wmVertex;
}

double WmVertex::operator*(const WmVertex& vertex) const
{
	return this->_x * vertex._x + this->_y * vertex._y + this->_z * vertex._z;
}

double WmVertex::norm() const
{
	return sqrt(this->_x * this->_x + this->_y * this->_y + this->_z * this->_z);
}

double WmVertex::dist(const WmVertex& vertex) const
{
	WmVertex wmVertex = this->operator-(vertex);
	return wmVertex.norm();
}