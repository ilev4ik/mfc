#include "automato.h"
#include <sstream>

FiniteAutomato::FiniteAutomato(CString str) : EQ(str), var("c") 
{
	ch = str[0]; 
	pm = '+';
	eq_map["x^2"], eq_map["xy"],
	eq_map["y^2"], eq_map["c"],
	eq_map["x"], eq_map["y"];
}

void FiniteAutomato::toNormalForm()
{
	int N = this->EQ.GetLength();
	if (EQ[N - 2] == '=' && EQ[N - 1] == '0')
		return;

	int eq_idx = EQ.Find(TEXT("="), 0), step = 1;
	
	bool ispos = true;
	if (EQ[eq_idx + 1] == '-') // after eq + or - ?
	{
		ispos = false;
		step = 0;
	}

	EQ.Replace(TEXT("="), ispos? TEXT("-"): TEXT(""));
	for (int i = eq_idx+step; i < N-1; ++i)
	{
		if (EQ[i] == '-') EQ.SetAt(i, wchar_t('+'));
		else if (EQ[i] == '+') EQ.SetAt(i, wchar_t('-'));
	}

	EQ += "=0";
}

void FiniteAutomato::START()
{
	this->toNormalForm();

	if (isdigit(ch))
	{
		state = DIG;
		coef = ch;
	}
	else if (isalpha(ch))
	{
		state = VAR;
		var = ch;
		coef = "1";
	}
	else if (ch == '-' || ch == '+')
	{
		state = PM;
		pm = ch;
	}

	ch = ::tolower(EQ[1]);
	this->Analise();
	int a = 5;
}

void FiniteAutomato::Analise()
{
	int i = 1;
	while (ch != '\0')
	{
		if (isdigit(ch))
		{
			if (state == DIG)
			{
				coef += ch;
			}
			else if (state == PM)
			{
				coef += pm;
				coef += ch;
				pm = 'u';
			}
			state = DIG;
		}
		else if (isalpha(ch))
		{
			if (state == DIG)
			{
				var = ch;
			}
			else if (state == VAR)
			{
				var += ch;
			}
			else if (state == PM)
			{
				coef += pm;
				coef += "1";
				var = ch;
			}
			state = VAR;
		}
		else if (ch == '^')
		{
			char t = EQ[++i];
			var += ch;
			var += t;
		}
		else if (ch == '-' || ch == '+')
		{
			eq_map[var] += _wtof(coef);
			coef = "", var = "c";
			state = PM;
			pm = ch;
		}
		else if (ch == '=')
		{
			eq_map[var] += _wtof(coef);
		}
		else if (ch == '.')
		{
			char d = EQ[++i];
			coef += ch;
			coef += d;
		}

		if (var == "yx")
		{
			var = "xy";
		}
		ch = ::tolower(EQ[++i]);
	}
}

void FiniteAutomato::inRightOrder()
{
	std::stringstream ss;
	if (eq_map.find("x^2")->second)
	{
		ss << (eq_map.find("x^2")->second > 0 ? "+" : "")
			<< eq_map.find("x^2")->second << "x^2";
	}

	if (eq_map.find("y^2")->second)
		ss << (eq_map.find("y^2")->second > 0 ? "+" : "")
		<< eq_map.find("y^2")->second << "y^2";

	if (eq_map.find("xy")->second)
		ss << (eq_map.find("xy")->second > 0 ? "+" : "")
		<< eq_map.find("xy")->second << "xy";

	if (eq_map.find("x")->second)
		ss << (eq_map.find("x")->second > 0 ? "+" : "")
		<< eq_map.find("x")->second << "x";

	if (eq_map.find("y")->second)
		ss << (eq_map.find("y")->second > 0 ? "+" : "")
		<< eq_map.find("y")->second << "y";

	if (eq_map.find("c")->second)
		ss << (eq_map.find("c")->second > 0 ? "+" : "")
		<< eq_map.find("c")->second;

	ss << "=0";
	std::string temp = ss.str();
	this->EQ = temp.c_str();
}

CString FiniteAutomato::getResult()
{
	this->inRightOrder();
	return this->EQ;
}