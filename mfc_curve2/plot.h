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
	// ����
	PlotFeatures();
	std::vector <double> dir;
	double ca, cb, cp;			// coef a,b,p
	std::vector <Point> focus;	// �����(�)
	Point* center;				// ����� ������
	// ��� ������ (��)
	CurveType CURVE_STATE;
};

#endif