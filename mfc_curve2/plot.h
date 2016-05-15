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

// ��������� ��� right top frame
struct PlotFeatures {
	// ����
public:
	PlotFeatures();

	std::vector <double> dir;
	double ca, cb, cp;			// coef a,b,p
	std::vector <Point> focus;	// �����(�)
	Point* center;				// ����� ������
	double k;					// ���� ���������� ��� �������������� ������

	double tk, tc;				// ��� ������������� k, c (tangent)
	double nk, nc;				// ��� ������� k, c (normal)
	double dk, dc;				// ��� �������� k, c (�������)

	SquareMatrix a;

	void calcTangentInPoint(Point);
	void calcNormalInPoint(Point);
	void calcDiameterInPoint(Point);
	// ��� ������ (��)
	CurveType CURVE_STATE;
};

#endif