#include "plot.h"

PlotFeatures::PlotFeatures()
{
	CURVE_STATE = _ERROR;
	center = nullptr;
}

Point::Point(double x, double y)
{
	this->x = x;
	this->y = y;
}

// p in real coordinates not client
void PlotFeatures::calcTangentInPoint(Point p)
{
	double as = a(2, 3);
	this->tk =
		-(a(1, 1)*p.x + a(1, 2)*p.y + a(1, 3)) / (a(1, 2)*p.x + a(2, 2)*p.y + a(2, 3));

	this->tc =
		-(a(1, 3)*p.x + a(2, 3)*p.y + a(3, 3)) / (a(1, 2)*p.x + a(2, 2)*p.y + a(2, 3));
}

void PlotFeatures::calcNormalInPoint(Point p)
{
	this->nk =
		(a(1, 2)*p.x + a(2, 2)*p.y + a(2, 3)) / (a(1, 1)*p.x + a(1, 2)*p.y + a(1, 3));

	this->nc =
		-p.x*(a(1, 2)*p.x + a(2, 2)*p.y + a(2, 3)) / (a(1, 1)*p.x + a(1, 2)*p.y + a(1, 3))+p.y;
}