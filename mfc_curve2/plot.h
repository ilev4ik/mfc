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

// структура для right top frame
struct PlotFeatures {
	// инфа
public:
	PlotFeatures();

	std::vector <double> dir;
	double ca, cb, cp;			// coef a,b,p
	std::vector <Point> focus;	// фокус(ы)
	Point* center;				// центр кривой
	double k;					// коэф линейности для пересекающихся прямых

	double tk, tc;				// для касательность k, c (tangent)
	double nk, nc;				// для нормали k, c (normal)
	double dk, dc;				// для диаметра k, c (диаметр)

	SquareMatrix a;

	void calcTangentInPoint(Point);
	void calcNormalInPoint(Point);
	void calcDiameterInPoint(Point);
	// тип кривой (ых)
	CurveType CURVE_STATE;
};

#endif