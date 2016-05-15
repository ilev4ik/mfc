#ifndef AUTOMATO
#define AUTOMATO

#include <afxwin.h>
#include <map>

class FiniteAutomato
{
public:
	FiniteAutomato(CString);
	void START();			// начало разбора дл€ кон. автомата
	CString getResult();	// дл€ вывода результата в CEdit
	void inRightOrder();	// правильный пор€док + отбрасываем нулевые значени€

	typedef std::map <CString, DOUBLE> comp_map;	// переопредел€ем название типа
	comp_map eq_map;								// объ€влем переменную определенного ранее типа
private:
	void Analise();			// вызываетс€ непосредственно после START()
	void toNormalForm();	// ?

	// определение базовых лексем строки
	enum STATE {
		DIG,
		VAR,
		PM
	} state; // состо€ние

	// текущие: 
	CString var, coef;	// переменна€ и коэффициент
	CString EQ;			// сама строка, подлежаща€ разбору
	char ch, pm;		// текущий char (символ) и знак (+,-,=)
};

#endif