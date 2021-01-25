#include "WmTransformMatrix.h"
#include "WmConstVariable.h"
#include "math.h"

WmTransformMatrix::WmTransformMatrix()
{
	_wmVertex[0] = WmVertex(0,0,0);
	_wmVertex[1] = WmVertex(0, 0, 0);
	_wmVertex[2] = WmVertex(0, 0, 0);
}

WmTransformMatrix::WmTransformMatrix(const WmVertex& r1, const WmVertex& r2, const WmVertex& r3)
{
	_wmVertex[0] = r1;
	_wmVertex[1] = r2;
	_wmVertex[2] = r3;
}

WmTransformMatrix::WmTransformMatrix(double r1c1, double r1c2, double r1c3, double r2c1, double r2c2, double r2c3, double r3c1, double r3c2, double r3c3)
{
	_wmVertex[0].setXYZ(r1c1, r1c2, r1c3);
	_wmVertex[1].setXYZ(r2c1, r2c2, r2c3);
	_wmVertex[2].setXYZ(r3c1, r3c2, r3c3);
}

WmTransformMatrix::~WmTransformMatrix()
{
}

WmTransformMatrix WmTransformMatrix::transpose() const
{
	WmTransformMatrix transformMatrix;
	transformMatrix.setRow(0, _wmVertex[0].x(), _wmVertex[1].x(), _wmVertex[2].x());
	transformMatrix.setRow(1, _wmVertex[0].y(), _wmVertex[1].y(), _wmVertex[2].y());
	transformMatrix.setRow(2, _wmVertex[0].z(), _wmVertex[1].z(), _wmVertex[2].z());
	return transformMatrix;
}

WmVertex WmTransformMatrix::operator*(const WmVertex& vertex) const
{
	WmVertex wmVertex;
	wmVertex.setX(_wmVertex[0] * vertex);
	wmVertex.setY(_wmVertex[1] * vertex);
	wmVertex.setZ(_wmVertex[2] * vertex);
	return wmVertex;
}

WmTransformMatrix WmTransformMatrix::operator*(const WmTransformMatrix& wmTransformMatrix) const
{
	WmTransformMatrix transposeMatrix = wmTransformMatrix.transpose();
	WmTransformMatrix resMatrix;

	resMatrix.setRow(0, this->row(0)*transposeMatrix.row(0), this->row(0)*transposeMatrix.row(1), this->row(0)*transposeMatrix.row(2));
	resMatrix.setRow(1, this->row(1)*transposeMatrix.row(0), this->row(1)*transposeMatrix.row(1), this->row(1)*transposeMatrix.row(2));
	resMatrix.setRow(2, this->row(2)*transposeMatrix.row(0), this->row(2)*transposeMatrix.row(1), this->row(2)*transposeMatrix.row(2));
	return resMatrix;
}

WmVertex WmTransformMatrix::transform(const WmVertex& vertex) const
{
	return operator*(vertex);
}

WmTransformMatrix WmTransformMatrix::createSpherial2Earth(double lon, double lat, double hei, bool rad)
{
	if(!rad)
	{
		lon = WmConstVariable::angle2rad(lon);
		lat = WmConstVariable::angle2rad(lat);
	}

	WmVertex vertex1(cos(lat)*cos(lon), cos(lat)*cos(lon), 0);
	WmVertex vertex2(cos(lat)*sin(lon), cos(lat)*sin(lon), 0);
	WmVertex vertex3(0, sin(lat), sin(lat));

	return WmTransformMatrix(vertex1, vertex2, vertex3);
}

WmTransformMatrix WmTransformMatrix::createSpherial2Earth(const WmVertex& wmVertex, bool rad)
{
	double lon, lat, hei;

	lon = wmVertex.x();
	lat = wmVertex.y();
	hei = wmVertex.z();

	return createSpherial2Earth(lon, lat, hei, rad);
}

WmTransformMatrix WmTransformMatrix::createEarth2Geo(double lon, double lat, bool rad)
{
	if (!rad)
	{
		lon = WmConstVariable::angle2rad(lon);
		lat = WmConstVariable::angle2rad(lat);
	}

	WmVertex vertex1(-sin(lon), cos(lon), 0);
	WmVertex vertex2(-sin(lat)*cos(lon), -sin(lat)*sin(lon), cos(lat));
	WmVertex vertex3(cos(lat)*cos(lon) , cos(lat)*sin(lon), sin(lat));

	return WmTransformMatrix(vertex1, vertex2, vertex3);
}

WmTransformMatrix WmTransformMatrix::createEarth2Geo(const WmVertex& wmVertex, bool rad)
{
	double lon, lat;

	lon = wmVertex.x();
	lat = wmVertex.y();

	return createEarth2Geo(lon, lat, rad);
}