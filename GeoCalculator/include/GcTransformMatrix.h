#pragma once
#include "GcVertex.h"

namespace GeoCalculator
{
	class __declspec(dllexport) GcTransformMatrix
	{
	public:
		GcTransformMatrix();
		GcTransformMatrix(const GcVertex& row1, const GcVertex& row2, const GcVertex& row3);
		GcTransformMatrix(double row1col1, double row1col2, double row1col3, double row2col1, double row2col2, double row2col3, double row3col1, double row3col2, double row3col3);
		~GcTransformMatrix();

	protected:
		GcVertex _gcVertex[3];//变换矩阵的行向量

	public:
		void setRow(unsigned char row, double c1, double c2, double c3) { _gcVertex[row] = GcVertex(c1, c2, c3); }
		void setRow(unsigned char row, const GcVertex& vertex) { _gcVertex[row] = vertex; }
		const GcVertex& row(unsigned char row) const { return _gcVertex[row]; }

		GcTransformMatrix transpose() const;//矩阵转置
		GcVertex operator*(const GcVertex& vertex) const;//矩阵与向量的乘积
		GcTransformMatrix operator*(const GcTransformMatrix& gcTransformMatrix) const;//矩阵与矩阵的乘积

		virtual GcVertex transform(const GcVertex& vertex) const;
	};
}


