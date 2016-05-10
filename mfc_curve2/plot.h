#ifndef PLOT
#define PLOT

#include "curvetypes.h"
#include <vector>
#include <afxwin.h>

// ��, ��� ����� ��� ���������� !!! �������� ��� ������ � �����!!!
struct PlotFeatures {
	// ����
	PlotFeatures();
	std::vector <double> dir;
	double ca, cb, cp;		// coef a,b,p
	std::vector <CPoint> focus;	// �����(�)

	// ��� ������ (��)
	CurveType CURVE_STATE;
};

#endif