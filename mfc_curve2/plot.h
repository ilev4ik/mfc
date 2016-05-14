#ifndef PLOT
#define PLOT

#include "curvetypes.h"
#include "matrix.h"
#include <vector>
#include <afxwin.h>

class Point
{
public:
	Point(double, double);
	double x;
	double y;
};

struct PlotFeatures {
	// инфа
public:
	PlotFeatures();

	std::vector <double> dir;
	double ca, cb, cp;			// coef a,b,p
	std::vector <Point> focus;	// фокус(ы)
	Point* center;				// центр кривой
	double k;					// коэф линейности
	double tk, tc;				// для касательность k, c
	double nk, nc;				// для нормали k, c

	SquareMatrix a;

	void calcTangentInPoint(Point);
	void calcNormalInPoint(Point);
	// тип кривой (ых)
	CurveType CURVE_STATE;
};

#endif