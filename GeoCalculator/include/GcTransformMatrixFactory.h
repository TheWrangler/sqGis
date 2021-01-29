#pragma once
#include "GcTransformMatrix.h"

namespace GeoCalculator
{
	class __declspec(dllexport) GcTransformMatrixFactory
	{
	public:
		GcTransformMatrixFactory();
		~GcTransformMatrixFactory();

	public:
		/*
		* 生成地球经纬坐标系至地球直角坐标系的转换矩阵
		*
		* x:指向0度经线（本初子午面）
		* z:指向北极
		* y:与xoz构成右手坐标系
		*
		*/
		static GcTransformMatrix createSpherial2Earth(double lon, double lat, double hei, bool rad = false);
		static GcTransformMatrix createSpherial2Earth(const GcVertex& gcVertex, bool rad = false);

		/*
		* 生成地球直角坐标系至地理坐标系的转换矩阵
		*
		* x:指向当地的地理东向
		* y:指向当地的地理北向
		* z:指向当地的地理天向
		*
		*/
		static GcTransformMatrix createEarth2Geo(double lon, double lat, bool rad = false);
		static GcTransformMatrix createEarth2Geo(const GcVertex& gcVertex, bool rad = false);

		/*
		* 生成地理坐标系至载体坐标系的转换矩阵
		*/
		static GcTransformMatrix createGeo2Platform(double lon, double lat, double yaw, double pitch, double roll, bool rad = false);
		static GcTransformMatrix createGeo2Platform(const GcVertex& gcPosVertex, const GcVertex& gcAttiVertex, bool rad = false);
	};
}

