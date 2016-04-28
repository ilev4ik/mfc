#include "mathground.h"
#include <sstream>
#include <iomanip>

#define DIM 3

CString operator+ (double l, CString r)
{
	if (l == 0.) return "";
	CString str;
	str.Format(TEXT("%f"), l);
	return str+r;
}

CString operator+ (CString l, double r)
{
	CString str;
	str.Format(TEXT("%f"), r);
	return str+l;
}

MathTool::MathTool(std::map <CString, DOUBLE> EQ)
{
	a(1, 1) = EQ["x^2"];
	a(2, 2) = EQ["y^2"];
	a(3, 3) = EQ["c"];
	a(1, 2) = a(2, 1) = EQ["xy"]/2;
	a(1, 3) = a(3, 1) = EQ["x"]/2;
	a(2, 3) = a(3, 2) = EQ["y"]/2;

	this->setInvariants();
	this->setQuadraticForm();
	this->setLambdas();
	this->setClassification();
}

void MathTool::setInvariants()
{

	// Инварианты относительно поворота и сдвига системы координат
	this->Delta =
		a(1, 1)*a(2, 2)*a(3, 3) + a(3, 1)*a(1, 2)*a(2, 3) +
		a(2, 1)*a(1, 3)*a(3, 2) - a(1, 1)*a(2, 3)*a(3, 2) -
		a(3, 1)*a(2, 2)*a(1, 3) - a(2, 1)*a(1, 2)*a(3, 3);

	this->D = a(1, 1)*a(2, 2) - a(1, 2)*a(1, 2);
	this->I = a(1, 1) + a(2, 2);

	// Инвариант относительно поворота системы координат
	this->B =
		a(1, 1)*a(3, 3) - a(1, 3)*a(1, 3) +
		a(2, 2)*a(3, 3) - a(2, 3)*a(2, 3);
}

void MathTool::setQuadraticForm()
{
	// Характеристическая квадратичная форма
	std::stringstream ss;
	ss << "F(x,y)=" << std::fixed;
	if (a(1, 1))
	{
		ss << (a(1, 1) > 0 ? "+" : "") <<
			std::setprecision(1) <<
			a(1, 1) << "x^2";
	}

	if (a(1, 2))
	{

		ss << (a(1, 2) > 0 ? "+" : "") <<
			std::setprecision(1) <<
			2 * a(1, 2) << "xy";
	}

	if (a(2, 2))
	{

		ss << (a(2, 2) > 0 ? "+" : "") <<
			std::setprecision(1) <<
			a(2, 2) << "y^2";
	}

	std::string str = ss.str();
	this->F_xy = str.c_str();
}

void MathTool::setLambdas()
{
	// Корни характеристического уравнения
	double d = // дискриминант :)
		std::pow(a(1, 1) + a(2, 2), 2.) -
		4 * (a(1, 1)*a(2, 2) - a(1, 2)*a(1, 2));
	// L1 >= L2 ==>
	this->L1 = 0.5*(a(1, 1) + a(2, 2) + std::sqrt(d));
	this->L2 = 0.5*(a(1, 1) + a(2, 2) - std::sqrt(d));
}

void MathTool::setClassification()
{
	std::stringstream ss;
	ss << "Кривая второго порядка типа: ";
	if (Delta == 0)
	{
		ss << "вырожденн";
		if (D > 0)
		{
			ss << "ый эллипс (точка)";
			CURVE_STATE = DOT;
		}
		else if (D < 0)
		{
			ss << "ая гипербола (пара пересекающихся прямых)";
			CURVE_STATE = INTERSECTING;
		}
		else
		{
			ss << "ая парабола ";
			if (B < 0)
			{
				ss << "(пара параллельных прямых).";
				CURVE_STATE = PARALLEL;
			}
			else if (B == 0)
			{
				ss << "(две слившиеся параллельные прямые).";
				CURVE_STATE = COINCIDING;
			}
			else ss << "(ни одной вещественной точки, пара"
						"мнимых параллельных прямых)";
		}			
	}
	else
	{
		ss << "невырожденн";
		if (D == 0)
		{
			ss << "ая нецентральная парабола.";
			CURVE_STATE = PARABOLA;
		}
		else
		{	
			if (D < 0)
			{
				ss << "ая центральная гипербола";
				CURVE_STATE = HIPERBOLA;
			}
			else if (D > 0 && Delta*I < 0)
			{
				ss << "ый эллипс";
				if (a(1, 1) == a(2, 2) && a(1, 2) == 0)
					ss << " (окружность).";
				CURVE_STATE = ELLIPS;
			}
			else ss << "ый мнимый эллипс (ни одной вещественной точки).";
		}
	}

	std::string str = ss.str();
	this->clif = str.c_str();
}

SquareMatrix::SquareMatrix()
{
	matrix = new double*[DIM];

	for (int i = 0; i < DIM; ++i)
		matrix[i] = new double[DIM];
}

double& SquareMatrix::operator() (int i, int j)
{ 
	return matrix[i-1][j-1];
}