#include <afxwin.h>
#include <map>

class FiniteAutomato
{
public:
	FiniteAutomato(CString);
	void START();
	CString getResult();
	void inRightOrder();
private:
	void Analise();
	void toNormalForm();

	enum STATE {
		DIG,
		VAR,
		PM
	} state;
	CString var, coef;
	CString EQ;
	char ch, pm;
	std::map <CString, DOUBLE> eq_map;
};