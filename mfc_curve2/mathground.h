#ifndef MATH_GROUND
#define MATH_GROUND

#include <map>
#include <afxwin.h>

class SquareMatrix
{
public:
	SquareMatrix();
	double& operator() (int i, int j);
private:
	double** matrix;
};

// порядок важен! должен совпадать с enum CurveType в след. классе
const std::string STR_CurveType[] = {
	"No real points determined",
	"x^2/a^2+y^2/b^2=1",
	"x^2/a^2-y^2/b^2=1",
	"y^2=2px",
	"x^2/a^2+y^2/b^2=0",
	"x^2/a^2-y^2/b^2=0",
	"x^2/a^2=1",
	"x^2=0"
};

class MathTool
{
public:
	MathTool(const std::map <CString, DOUBLE>);
	SquareMatrix a;
private:
	void setInvariants();
	void setQuadraticForm();
	void setLambdas();
	void setClassification();

	enum CurveType {
		_ERROR,
		ELLIPS,
		HIPERBOLA,
		PARABOLA,
		DOT,
		INTERSECTING,
		PARALLEL,
		COINCIDING
	} CURVE_STATE = _ERROR;

	double Delta, D, I, B;	// Инварианты
	double L1, L2;			// Корни хар. ур-я
	CString F_xy;
	CString clif;			// CLassIFication
};

#endif