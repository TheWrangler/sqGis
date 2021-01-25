#pragma once
#include <qgspoint.h>

class MapToolMath
{
public:
	MapToolMath();
	~MapToolMath();

public:
	static QgsPointXY convertCoor(const QgsPointXY& src, bool toWSG84 = true);
};

