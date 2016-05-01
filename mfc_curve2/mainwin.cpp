#include <string.h>
#include "mainwin.h"
#include "res.h"
#include <sstream>
#include <iomanip>

CApp App;

BOOL CApp::InitInstance()
{
	m_pMainWnd = new CMainWin;
	m_pMainWnd->ShowWindow(SW_RESTORE);
	m_pMainWnd->UpdateWindow();
	
	return TRUE;
}

CMainWin::CMainWin()
{
	this->Create(0, TEXT("Визуализация кривых второго порядка"),
					WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME
					| WS_BORDER);
	this->m_wndMenu.LoadMenu(IDR_MENU1);	// Загрузить меню из файла ресурса
	SetMenu(&m_wndMenu);					// Установить меню

	CRect mainRect;
	this->GetClientRect(mainRect);

	CRect rect;
	GetClientRect(rect);
	this->LoadAccelTable(MAKEINTRESOURCE(IDR_MAINFRAME));

	this->frame_LT = new LeftTopFrame(this,
		CRect(0, 0, rect.Width() / 2, rect.Height() / 2)
		);

	this->frame_RT = new RightTopFrame(this, CRect(
		rect.Width() / 2, 0, rect.Width(), rect.Height() / 2));

	this->frame_LB = new LeftBottomFrame(this,
		CRect(0, rect.Height() / 2, rect.Width() / 2, rect.Height()));

	this->frame_RB = new RightBottomFrame(this,
		CRect(rect.Width() / 2, rect.Height() / 2, rect.Width(), rect.Height()));
}

afx_msg void CMainWin::OnCommand_SHOW_DIALOG()
{
	this->DDialog = new DotsDialog();
	DDialog->DoModal();
}

// Реализация карты сообщений
BEGIN_MESSAGE_MAP(CMainWin, CFrameWnd)
	ON_COMMAND(ID_PLOT, CMainWin::OnCommand_SHOW_DIALOG)
	ON_COMMAND(ID_ENTER, CMainWin::OnEnter)
END_MESSAGE_MAP()

afx_msg void CMainWin::OnEnter()
{
	std::map <CString, DOUBLE> eq_map;

	CString EQ;
	this->frame_LT->EQEditOld->GetWindowTextW(EQ);

	// Конечный автомат для разбора строки
	this->CurveEq = new FiniteAutomato(EQ);
	CurveEq->START();
	this->tool = new MathTool(CurveEq->eq_map);

	this->setInfro_LT();
	this->setInfo_LB();
	this->setInfo_RB();
}

CString operator+ (CString l, double r)
{
	CString str;
	str.Format(TEXT("%f"), r);
	return str + l;
}

std::stringstream& operator<< (std::stringstream& ss, const CPoint& p)
{
	ss << "(" << p.x << "," << p.y << ")";
	return ss;
}

void CMainWin::setInfo_LB()
{
	std::stringstream ss;

	CString str("Общие сведения о кривой:\n");
	str += TEXT("Характеристическая форма: ") + tool->F_xy + TEXT('\n');

	ss << std::fixed << std::setprecision(2);
	ss << "Корни характеристического уравнения: L1=" << tool->L1 
		<< ", L2=" << tool->L2 << '\n';

	ss << "Инварианты: Delta = " << tool->Delta << ", "
		<< "D=" << tool->D << ", I=" << tool->I << ", B=" << tool->B << '\n';

	std::string s = ss.str();
	CString c = s.c_str();
	str += c;

	str += TEXT("Классификация: ") + tool->clif + '\n';
	str += TEXT("Канонический вид: ") + tool->canonical + '\n';

	ss.str(std::string());
	ss.clear();
	ss << "Замена к каноническому виду: a^2=" << tool->ca2 << ", b^2=" << tool->cb2 << ", p=" <<tool->cp << '\n';
	ss << "Фокус(ы): ";
	for (int i = 0; i < tool->focus.size(); ++i)
	{
		ss << "F_" << i + 1 << "=";
		ss << tool->focus[i] << " ";
	}
	ss << "\n";
	ss << "Эксцентриситет: " << tool->e << '\n';
	ss << "Директриса(ы):";

	for (int i = 0; i < tool->dir.size(); ++i)
	{
		ss << "x=" << tool->dir[i] << ", ";
	}
	ss << "\n";
	s = ss.str();
	c = s.c_str();
	str += c;
	this->frame_LB->InfoLabel->SetWindowTextW(str);
}

void CMainWin::setInfro_LT()
{
	this->frame_LT->EQEditOld->SetWindowTextW(CurveEq->getResult());
	this->frame_LT->EQEditOld->SetFocus();
	this->frame_LT->EQEditOld->SetSel(0, -1);	// select all text and move cursor at the end
	this->frame_LT->EQEditOld->SetSel(-1);		// caret is on the last position	
}

void CMainWin::setInfo_RB()
{
	this->frame_RB->polar->SetWindowTextW(tool->polar);
	this->frame_RB->excent->SetWindowTextW(tool->Y_exc);
}