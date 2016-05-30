#include "mathground.h"
#include <sstream> // for stringstream
#include <iomanip>
#include "curvetypes.h"

#define EPS 1e-4

using namespace std;

MathTool::MathTool(std::map <CString, DOUBLE> EQ)
{
	plot.a(1, 1) = EQ["x^2"];
	plot.a(2, 2) = EQ["y^2"];
	plot.a(3, 3) = EQ["c"];
	plot.a(1, 2) = plot.a(2, 1) = EQ["xy"]/2;
	plot.a(1, 3) = plot.a(3, 1) = EQ["x"]/2;
	plot.a(2, 3) = plot.a(3, 2) = EQ["y"]/2;

	this->setInvariants();
	this->setQuadraticForm();
	this->setLambdas();
	this->setClassification();
	this->setCanonical();
	this->setSubscribtion();
	this->setFDE();
	this->setEqExcent();
	this->setPolar();
	this->setCenter();
}

void MathTool::setInvariants()
{

	// Инварианты относительно поворота и сдвига системы координат
	this->Delta =
		plot.a(1, 1)*plot.a(2, 2)*plot.a(3, 3) + plot.a(3, 1)*plot.a(1, 2)*plot.a(2, 3) +
		plot.a(2, 1)*plot.a(1, 3)*plot.a(3, 2) - plot.a(1, 1)*plot.a(2, 3)*plot.a(3, 2) -
		plot.a(3, 1)*plot.a(2, 2)*plot.a(1, 3) - plot.a(2, 1)*plot.a(1, 2)*plot.a(3, 3);

	this->D = plot.a(1, 1)*plot.a(2, 2) - plot.a(1, 2)*plot.a(1, 2);
	this->I = plot.a(1, 1) + plot.a(2, 2);

	// Инвариант относительно поворота системы координат
	this->B =
		plot.a(1, 1)*plot.a(3, 3) - plot.a(1, 3)*plot.a(1, 3) +
		plot.a(2, 2)*plot.a(3, 3) - plot.a(2, 3)*plot.a(2, 3);
}

void MathTool::setQuadraticForm()
{
	// Характеристическая квадратичная форма
	std::stringstream ss;
	ss << "F(x,y)=" << std::fixed;
	if (plot.a(1, 1))
	{
		ss << (plot.a(1, 1) > 0 ? "+" : "") <<
			std::setprecision(1) <<
			plot.a(1, 1) << "x^2";
	}

	if (plot.a(1, 2))
	{

		ss << (plot.a(1, 2) > 0 ? "+" : "") <<
			std::setprecision(1) <<
			2 * plot.a(1, 2) << "xy";
	}

	if (plot.a(2, 2))
	{

		ss << (plot.a(2, 2) > 0 ? "+" : "") <<
			std::setprecision(1) <<
			plot.a(2, 2) << "y^2";
	}

	std::string str = ss.str();
	this->F_xy = str.c_str();
}

void MathTool::setLambdas()
{
	// Корни характеристического уравнения
	double d = // дискриминант :)
		std::pow(plot.a(1, 1) + plot.a(2, 2), 2.) -
		4 * (plot.a(1, 1)*plot.a(2, 2) - plot.a(1, 2)*plot.a(1, 2));
	// L1 >= L2 ==>
	this->L1 = 0.5*(plot.a(1, 1) + plot.a(2, 2) + std::sqrt(d));
	this->L2 = 0.5*(plot.a(1, 1) + plot.a(2, 2) - std::sqrt(d));
}

void MathTool::setClassification()
{
	CurveType CURVE_STATE = _ERROR;
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
				if (plot.a(1, 1) == plot.a(2, 2) && plot.a(1, 2) == 0)
					ss << " (окружность).";
				CURVE_STATE = ELLIPS;
			}
			else ss << "ый мнимый эллипс (ни одной вещественной точки).";
		}
	}
	plot.CURVE_STATE = CURVE_STATE;
	std::string str = ss.str();
	this->clif = str.c_str();
}

void MathTool::setCanonical()
{
	this->canonical = CanonicalView[plot.CURVE_STATE].c_str();
}

void MathTool::setSubscribtion()
{
	int CURVE_STATE = this->plot.CURVE_STATE;
	if (CURVE_STATE == ELLIPS || CURVE_STATE == DOT)
	{
		plot.ca = sqrt(-(1 / L2) * (Delta / D));
		plot.cb = sqrt(-(1 / L1) * (Delta / D));
		plot.cp = plot.cb*plot.cb / plot.ca;
	}
	else if (CURVE_STATE == HIPERBOLA)
	{
		plot.ca = sqrt(-(1 / L1) * (Delta / D));
		plot.cb = sqrt((1 / L2) * (Delta / D));
		plot.cp = plot.cb*plot.cb / plot.ca;
	}
	else if (CURVE_STATE == PARABOLA)	
	{
		plot.cp = 1 / I*sqrt(-Delta / I);
	}
	else if (CURVE_STATE == PARALLEL)
	{
		plot.ca = sqrt(-plot.a(3, 3) / plot.a(1, 1));
	}
	else if (CURVE_STATE == COINCIDING)
	{
		plot.ca = sqrt(1/plot.a(1,1));
	}
	else if (CURVE_STATE == INTERSECTING)
	{
		plot.k = sqrt(-plot.a(1, 1) / plot.a(2, 2));
	}
	else plot.ca = plot.cb = plot.cp = 0;
}

void MathTool::setFDE()
{
	switch (plot.CURVE_STATE)
	{
	case HIPERBOLA:
		plot.focus.push_back(Point(sqrt(plot.ca*plot.ca + plot.cb*plot.cb), 0));
		plot.focus.push_back(Point(-sqrt(plot.ca*plot.ca + plot.cb*plot.cb), 0));
		e = std::sqrt(plot.ca*plot.ca + plot.cb*plot.cb) / plot.ca;
		plot.dir.push_back(plot.ca / e);
		plot.dir.push_back(-plot.ca / e);
		plot.cp = plot.cb*plot.cb / e;
		break;
	case PARABOLA:
		plot.focus.push_back(Point(plot.cp / 2, 0));
		e = std::sqrt(1 + (plot.cb*plot.cb) / (plot.ca*plot.ca));
		plot.dir.push_back(-plot.cp / 2);
		break;
	case ELLIPS:
		plot.cp = (plot.cb*plot.cb) / plot.ca;
		e = sqrt(1 - (plot.cb*plot.cb) / (plot.ca*plot.ca));
		if (e != 0)
		{
			plot.dir.push_back(-plot.ca / e);
			plot.dir.push_back(plot.ca / e);
			plot.focus.push_back(Point(sqrt(plot.ca*plot.ca-plot.cb*plot.cb), 0));
			plot.focus.push_back(Point(-sqrt(plot.ca*plot.ca - plot.cb*plot.cb), 0));
		}
		else
		{
			plot.dir.push_back(0);
			plot.focus.push_back(Point(0, 0));
		}

		break;
	}
}

void MathTool::setEqExcent()
{
	std::stringstream ss;
	ss << "y^2=" << std::fixed;
	ss << std::setprecision(1) << 2 * plot.cp << "x-(1-" <<
		e*e << ")x^2";

	std::string str = ss.str();
	this->Y_exc = str.c_str();
}

void MathTool::setPolar()
{
	std::stringstream ss;
	ss << "R=" << std::fixed;
	ss << std::setprecision(1) << plot.cp << "/(1+" <<
		e << "*cos(phi))";

	std::string str = ss.str();
	this->polar = str.c_str();
}

void MathTool::setCenter()
{
	if (D != 0)
		this->plot.center = new Point(
				(plot.a(1, 2)*plot.a(2, 3) - plot.a(1, 3)*plot.a(2, 2)) / D, 
				(plot.a(1, 3)*plot.a(1, 2) - plot.a(1, 1)*plot.a(2, 3)) / D
				);
}

double d = 0;
int MathTool::det(int n, double mat[5][5])
{
	int c, subi, i, j, subj;
	double submat[5][5];
	if (n == 2)
	{
		return((mat[0][0] * mat[1][1]) - (mat[1][0] * mat[0][1]));
	}
	else
	{
		for (c = 0; c < n; c++)
		{
			subi = 0;
			for (i = 1; i < n; i++)
			{
				subj = 0;
				for (j = 0; j < n; j++)
				{
					if (j == c)
					{
						continue;
					}
					submat[subi][subj] = mat[i][j];
					subj++;
				}
				subi++;
			}
			d = d + (pow(-1, c) * mat[0][c] * det(n - 1, submat));
		}
	}
	return d;
}
