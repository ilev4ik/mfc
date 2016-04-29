#include "mathground.h"
#include <sstream>
#include <iomanip>

#define DIM 3
#define EPS 1e-4

// �� �� ������������:)
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
	this->setCanonical();
}

void MathTool::setInvariants()
{

	// ���������� ������������ �������� � ������ ������� ���������
	this->Delta =
		a(1, 1)*a(2, 2)*a(3, 3) + a(3, 1)*a(1, 2)*a(2, 3) +
		a(2, 1)*a(1, 3)*a(3, 2) - a(1, 1)*a(2, 3)*a(3, 2) -
		a(3, 1)*a(2, 2)*a(1, 3) - a(2, 1)*a(1, 2)*a(3, 3);

	this->D = a(1, 1)*a(2, 2) - a(1, 2)*a(1, 2);
	this->I = a(1, 1) + a(2, 2);

	// ��������� ������������ �������� ������� ���������
	this->B =
		a(1, 1)*a(3, 3) - a(1, 3)*a(1, 3) +
		a(2, 2)*a(3, 3) - a(2, 3)*a(2, 3);
}

void MathTool::setQuadraticForm()
{
	// ������������������ ������������ �����
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
	// ����� ������������������� ���������
	double d = // ������������ :)
		std::pow(a(1, 1) + a(2, 2), 2.) -
		4 * (a(1, 1)*a(2, 2) - a(1, 2)*a(1, 2));
	// L1 >= L2 ==>
	this->L1 = 0.5*(a(1, 1) + a(2, 2) + std::sqrt(d));
	this->L2 = 0.5*(a(1, 1) + a(2, 2) - std::sqrt(d));
}

void MathTool::setClassification()
{
	std::stringstream ss;
	ss << "������ ������� ������� ����: ";
	if (Delta == 0)
	{
		ss << "���������";
		if (D > 0)
		{
			ss << "�� ������ (�����)";
			CURVE_STATE = DOT;
		}
		else if (D < 0)
		{
			ss << "�� ��������� (���� �������������� ������)";
			CURVE_STATE = INTERSECTING;
		}
		else
		{
			ss << "�� �������� ";
			if (B < 0)
			{
				ss << "(���� ������������ ������).";
				CURVE_STATE = PARALLEL;
			}
			else if (B == 0)
			{
				ss << "(��� ��������� ������������ ������).";
				CURVE_STATE = COINCIDING;
			}
			else ss << "(�� ����� ������������ �����, ����"
						"������ ������������ ������)";
		}			
	}
	else
	{
		ss << "�����������";
		if (D == 0)
		{
			ss << "�� ������������� ��������.";
			CURVE_STATE = PARABOLA;
		}
		else
		{	
			if (D < 0)
			{
				ss << "�� ����������� ���������";
				CURVE_STATE = HIPERBOLA;
			}
			else if (D > 0 && Delta*I < 0)
			{
				ss << "�� ������";
				if (a(1, 1) == a(2, 2) && a(1, 2) == 0)
					ss << " (����������).";
				CURVE_STATE = ELLIPS;
			}
			else ss << "�� ������ ������ (�� ����� ������������ �����).";
		}
	}

	std::string str = ss.str();
	this->clif = str.c_str();
}

void MathTool::setCanonical()
{
	this->canonical = CanonicalView[CURVE_STATE].c_str();
}

void MathTool::setSubscribtion()
{
	if (CURVE_STATE == ELLIPS || CURVE_STATE == DOT)
	{
		ca = -1 / L2 * Delta / D;
		cb = -1 / L1 * Delta / D;
		cp = cb*cb / ca;
	}
	else if (CURVE_STATE == HIPERBOLA || CURVE_STATE == INTERSECTING)
	{
		ca = -1 / L1 * Delta / D;
		cb = 1 / L2 * Delta / D;
		cp = cb*cb / ca;
	}
	else if (CURVE_STATE == PARABOLA)	
	{
		cp = 1 / I*std::sqrt(-Delta / I);
	}
	else if (CURVE_STATE == PARALLEL)
	{
		ca = std::sqrt(a(3, 3)/a(1,1));
	}
	else if (CURVE_STATE == COINCIDING)
	{
		ca = std::sqrt(1/a(1,1));
	}
	else ca = cb = cp = 0;			// ������ ��� ��� ������ ������, � �� �� 
									// ��������� � ERROR, �.�. ��� ������� 
									// �� ���. ���
}

void MathTool::setFDE()
{
	switch (CURVE_STATE)
	{
	case HIPERBOLA:
		focus = new CPoint[2];
		focus[0].y = focus[1].y = 0;
		focus[0].x = ca / 2;
		focus[1].x = cb / 2;
		e = std::sqrt(ca*ca + cb*cb) / ca;
		dir = new double[2];
		dir[0] = ca / e;
		dir[1] = -ca / e;
		cp = cb*cb / ca;
	case PARABOLA:
		focus = new CPoint[1];
		focus[0] = cp / 2;
		e = std::sqrt(1 + (cb*cb) / (ca*ca));
		dir = new double[1];
		dir[0] = -cp / 2;
	case ELLIPS:
		e = std::sqrt(1 - (cb*cb) / (ca*ca));
		dir = new double[2];
		dir[0] = dir[1] = cp / (e*(1+e));
		dir[1] *= -1;
		focus = new CPoint[2];
		focus[0].x = focus[1].x = cp / (1 + e);
		focus[0].x *= -1;
		focus[0].y = focus[1].y = 0;
		cp = cb*cb / ca;
	}
}

void MathTool::setEqExcent()
{
	std::stringstream ss;
	ss << "y^2=" << std::fixed;
	ss << std::setprecision(1) << 2 * cp << "x-(1-" <<
		e*e << ")x^2";

	std::string str = ss.str();
	this->Y_exc = str.c_str();
}

void MathTool::setPolar()
{
	std::stringstream ss;
	ss << "rho=" << std::fixed;
	ss << cp << "/(1+" << std::setprecision(1) <<
		e << "*cos(phi))";

	std::string str = ss.str();
	this->polar = str.c_str();
}

MathTool::~MathTool()
{
	delete[] focus;
	delete[] dir;
}

SquareMatrix::SquareMatrix()
{
	matrix = new double*[DIM];

	for (int i = 0; i < DIM; ++i)
		matrix[i] = new double[DIM];
}

SquareMatrix::~SquareMatrix()
{
	for (int i = 0; i < DIM; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}

double& SquareMatrix::operator() (int i, int j)
{ 
	return matrix[i-1][j-1];
}