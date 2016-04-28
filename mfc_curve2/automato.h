#ifndef AUTOMATO
#define AUTOMATO

#include <afxwin.h>
#include <map>

class FiniteAutomato
{
public:
	FiniteAutomato(CString);
	void START();
	CString getResult();
	void inRightOrder();

	typedef std::map <CString, DOUBLE> comp_map;
	comp_map eq_map;
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
};

#endif