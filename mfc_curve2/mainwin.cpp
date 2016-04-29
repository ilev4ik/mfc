#include <string.h>
#include "mainwin.h"
#include "res.h"
#include "automato.h"

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
	std::map <CString, DOUBLE> eq_map;

	CString EQ;
	this->frame_LT->EQEditOld->GetWindowTextW(EQ);

	// �������� ������� ��� ������� ������
	FiniteAutomato CurveEq(EQ);
	CurveEq.START();

	this->frame_LT->EQEditOld->SetWindowTextW(CurveEq.getResult());
	this->frame_LT->EQEditOld->SetFocus();
	this->frame_LT->EQEditOld->SetSel(0, -1);	// select all text and move cursor at the end
	this->frame_LT->EQEditOld->SetSel(-1);		// caret is on the last position	
}