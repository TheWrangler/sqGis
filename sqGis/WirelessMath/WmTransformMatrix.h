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

	WmTransformMatrix transpose() const;//����ת��
	WmVertex operator*(const WmVertex& vertex) const;//�����������ĳ˻�
	WmTransformMatrix operator*(const WmTransformMatrix& wmTransformMatrix) const;//���������ĳ˻�

	virtual WmVertex transform(const WmVertex& vertex) const;

	/*
	* ����γ����ϵ������ֱ������ϵ��ת��
	*
	* x:ָ��0�Ⱦ��ߣ����������棩
	* z:ָ�򱱼�
	* y:��xoz������������ϵ
	*
	*/
	static WmTransformMatrix createSpherial2Earth(double lon, double lat, double hei, bool rad = false);
	static WmTransformMatrix createSpherial2Earth(const WmVertex& wmVertex, bool rad = false);

	/*
	* ����ֱ������ϵ����������ϵ��ת��
	*
	* x:ָ�򵱵صĵ�����
	* y:ָ�򵱵صĵ�����
	* z:ָ�򵱵صĵ�������
	*
	*/
	static WmTransformMatrix createEarth2Geo(double lon, double lat, bool rad = false);
	static WmTransformMatrix createEarth2Geo(const WmVertex& wmVertex, bool rad = false);
};

