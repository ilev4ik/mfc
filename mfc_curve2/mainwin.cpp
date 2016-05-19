#include <string.h>
#include "mainwin.h"
#include "res.h"
#include <sstream>
#include <iomanip>

CApp App;

void CApp::PreLoadState() {}

BOOL CApp::InitInstance()
{
	m_pMainWnd = new CMainWin;
	m_pMainWnd->ShowWindow(SW_RESTORE);
	m_pMainWnd->UpdateWindow();
	
	return TRUE;
}

CMainWin::CMainWin()
{
	this->Create(0, TEXT("������������ ������ ������� �������"),
					WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME
					| WS_BORDER);
	this->m_wndMenu.LoadMenu(IDR_MENU1);	// ��������� ���� �� ����� �������
	SetMenu(&m_wndMenu);					// ���������� ����

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

// ���������� ����� ���������
BEGIN_MESSAGE_MAP(CMainWin, CFrameWnd)
	ON_COMMAND(ID_PLOT, CMainWin::OnCommand_SHOW_DIALOG)
	ON_COMMAND(ID_ENTER, CMainWin::OnEnter)
END_MESSAGE_MAP()

afx_msg void CMainWin::OnEnter()
{
	if (this->frame_LT->EQEditOld->GetWindowTextLengthW() == 0)
		return;

	std::map <CString, DOUBLE> eq_map;

	CString EQ;
	this->frame_LT->EQEditOld->GetWindowTextW(EQ);

	// �������� ������� ��� ������� ������
	this->CurveEq = new FiniteAutomato(EQ);
	CurveEq->START();

	this->tool = new MathTool(CurveEq->eq_map);
	this->setInfro_LT();
	this->setInfo_LB();
	this->setInfo_RB();

	this->frame_RT->pf = this->tool->plot;
	this->frame_RT->p_isdefined = TRUE;

	this->frame_RT->InvalidateRect(NULL, FALSE);
}

CString operator+ (CString l, double r)
{
	CString str;
	str.Format(TEXT("%f"), r);
	return str + l;
}

std::stringstream& operator<< (std::stringstream& ss, const Point& p)
{
	ss << "(" << p.x << "," << p.y << ")";
	return ss;
}

void CMainWin::setInfo_LB()
{
	std::stringstream ss;

	CString str("����� �������� � ������:\n");
	str += TEXT("������������������ �����: ") + tool->F_xy + TEXT('\n');

	ss << std::fixed << std::setprecision(2);
	ss << "����� ������������������� ���������: L1=" << tool->L1 
		<< ", L2=" << tool->L2 << '\n';

	ss << "����������: Delta = " << tool->Delta << ", "
		<< "D=" << tool->D << ", I=" << tool->I << ", B=" << tool->B << '\n';

	std::string s = ss.str();
	CString c = s.c_str();
	str += c;

	str += TEXT("�������������: ") + tool->clif + '\n';
	str += TEXT("������������ ���: ") + tool->canonical + '\n';

	ss.str(std::string());
	ss.clear();
	ss << "������ � ������������� ����: a^2=" << tool->plot.ca*tool->plot.ca << 
		", b^2=" << tool->plot.cb*tool->plot.cb << ", p=" << tool->plot.cp << '\n';
	ss << "�����(�): ";
	for (int i = 0; i < tool->plot.focus.size(); ++i)
	{
		ss << "F_" << i + 1 << "=";
		ss << tool->plot.focus[i] << " ";
	}
	ss << "\n";
	ss << "��������������: " << tool->e << '\n';
	ss << "����������(�): ";

	for (int i = 0; i < tool->plot.dir.size(); ++i)
	{
		ss << "x=" << tool->plot.dir[i] << ", ";
	}
	ss << "\n";

	if (tool->plot.center != nullptr)
	{
		ss << "����� ������: ";
		ss << *tool->plot.center << "\n";
	}
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