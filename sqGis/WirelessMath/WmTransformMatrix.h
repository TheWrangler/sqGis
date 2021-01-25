#pragma once
#include "WmVertex.h"

class WmTransformMatrix
{
public:
	WmTransformMatrix();
	WmTransformMatrix(const WmVertex& row1, const WmVertex& row2, const WmVertex& row3);
	WmTransformMatrix(double row1col1, double row1col2, double row1col3, double row2col1, double row2col2, double row2col3, double row3col1, double row3col2, double row3col3);
	~WmTransformMatrix();

protected:
	WmVertex _wmVertex[3];

public:
	void setRow(unsigned char row, double c1, double c2, double c3) { _wmVertex[row] = WmVertex(c1,c2,c3); }
	void setRow(unsigned char row, const WmVertex& vertex) { _wmVertex[row] = vertex; }
	const WmVertex& row(unsigned char row) const { return _wmVertex[row]; }

	WmTransformMatrix transpose() const;//矩阵转置
	WmVertex operator*(const WmVertex& vertex) const;//矩阵与向量的乘积
	WmTransformMatrix operator*(const WmTransformMatrix& wmTransformMatrix) const;//矩阵与矩阵的乘积

	virtual WmVertex transform(const WmVertex& vertex) const;

	/*
	* 地球经纬坐标系至地球直角坐标系的转换
	*
	* x:指向0度经线（本初子午面）
	* z:指向北极
	* y:与xoz构成右手坐标系
	*
	*/
	static WmTransformMatrix createSpherial2Earth(double lon, double lat, double hei, bool rad = false);
	static WmTransformMatrix createSpherial2Earth(const WmVertex& wmVertex, bool rad = false);

	/*
	* 地球直角坐标系至地理坐标系的转换
	*
	* x:指向当地的地理东向
	* y:指向当地的地理北向
	* z:指向当地的地理天向
	*
	*/
	static WmTransformMatrix createEarth2Geo(double lon, double lat, bool rad = false);
	static WmTransformMatrix createEarth2Geo(const WmVertex& wmVertex, bool rad = false);
};

