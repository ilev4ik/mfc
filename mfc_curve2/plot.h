#ifndef PLOT
#define PLOT

#include "curvetypes.h"
#include <vector>
#include <afxwin.h>

// всё, что нужно для построения !!! добавить для прямых и точки!!!
struct PlotFeatures {
	// инфа
	PlotFeatures();
	std::vector <double> dir;
	double ca, cb, cp;		// coef a,b,p
	std::vector <CPoint> focus;	// фокус(ы)

	// тип кривой (ых)
	CurveType CURVE_STATE;
};

#endif