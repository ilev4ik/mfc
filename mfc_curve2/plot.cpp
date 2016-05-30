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

// скал€рное произведение
double dot(Point a, Point b)
{
	return a.x*b.x + a.y*b.y;
}

// модуль
double mod(Point a)
{
	return std::sqrt(a.x*a.x+a.y*a.y);
}

void PlotFeatures::calcDiameterInPoint(Point p)
{
	const Point Ox(1,0);
	double _cos = dot(p, Ox)/(mod(p)*mod(Ox));	// cos угла составл€ющего с Ox
	double _sin = std::sqrt(1 - _cos*_cos);

	if (p.y < 0) _sin *= -1;

	this->dk =
		-(a(1, 1)*_cos + a(1, 2)*_sin) / (a(1, 2)*_cos + a(2, 2)*_cos);

	this->dc =
		-(a(1, 3)*_cos + a(2, 3)*_sin) / (a(1, 2)*_cos + a(2, 2)*_cos);
}