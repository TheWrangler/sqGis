#pragma once
class WmConstVariable
{
public:
	WmConstVariable();
	~WmConstVariable();

public:
	static const double pi;
	static const double a;//������򳤰��ᣬkm
	static const double e2;//��һƫ����ƽ��

public:
	static double angle2rad(double angle);
	static double rad2angle(double rad);
	static double n(double lat, bool rad = false);//î��Ȧ���ʰ뾶
};

