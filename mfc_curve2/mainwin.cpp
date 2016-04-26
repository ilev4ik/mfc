#include <string.h>
#include "mainwin.h"
#include "res.h"

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
					WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX);
	this->m_wndMenu.LoadMenu(IDR_MENU1);	// ��������� ���� �� ����� �������
	SetMenu(&m_wndMenu);					// ���������� ����

	CRect mainRect;
	this->GetClientRect(mainRect);
	this->MCanvas = new MainCanvas(this, &mainRect);
}

afx_msg void CMainWin::OnCommand_SHOW_DIALOG()
{
	this->DDialog = new DotsDialog();
	DDialog->DoModal();
}

// ���������� ����� ���������
BEGIN_MESSAGE_MAP(CMainWin, CFrameWnd)
	ON_COMMAND(ID_PLOT, OnCommand_SHOW_DIALOG)
END_MESSAGE_MAP()
