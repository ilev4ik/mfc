#ifndef MATH_GROUND
#define MATH_GROUND

#include <map>
#include <afxwin.h>

class SquareMatrix
{
public:
	SquareMatrix();
	~SquareMatrix();
	double& operator() (int i, int j);
private:
	double** matrix;
};

// ������� �����! ������ ��������� � enum CurveType � ����. ������
const std::string CanonicalView[] = {
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
	~MathTool();
	SquareMatrix a;

	// left bottom frame
	CString F_xy;			// ������������������ �����
	double L1, L2;			// ����� ���. ��-�
	double Delta, D, I, B;	// ����������
	CString clif;			// CLassIFication
	CString canonical;		// ������������ ���
	double ca, cb, cp;		// coef a,b,p
	CPoint* focus;			// �����(�)
	double e, *dir;			// ������������� � ����������

	// right bottom frame
	CString Y_exc;			// ��������� ����� �������������
	CString polar;			// � �������� �����������
private:
	void setInvariants();
	void setQuadraticForm();
	void setLambdas();
	void setClassification();
	void setCanonical();
	void setSubscribtion();
	void setFDE();
	void setEqExcent();
	void setPolar();

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
};

#endif
