// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GEOCALCULATOR_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GEOCALCULATOR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//#ifdef GEOCALCULATOR_EXPORTS
//#define GEOCALCULATOR_API __declspec(dllexport)
//#else
//#define GEOCALCULATOR_API __declspec(dllimport)
//#endif

#include "GcVertex.h"
#include "GcConstVariable.h"
#include "GcTransformMatrix.h"
#include "GcTransformMatrixFactory.h"
#include "GcGeoCalculator.h"

//// This class is exported from the GeoCalculator.dll
//class GEOCALCULATOR_API CGeoCalculator {
//public:
//	CGeoCalculator(void);
//	// TODO: add your methods here.
//};
//
//extern GEOCALCULATOR_API int nGeoCalculator;
//
//GEOCALCULATOR_API int fnGeoCalculator(void);
