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