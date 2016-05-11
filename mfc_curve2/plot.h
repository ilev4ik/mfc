#ifndef PLOT
#define PLOT

#include "curvetypes.h"
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
	PlotFeatures();
	std::vector <double> dir;
	double ca, cb, cp;			// coef a,b,p
	std::vector <Point> focus;	// фокус(ы)
	Point* center;				// центр кривой
	// тип кривой (ых)
	CurveType CURVE_STATE;
};

#endif