#include <string.h>
#include <stdio.h>
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
	this->Create(0, TEXT("Простейшее приложение на MFC"));
	this->m_wndMenu.LoadMenu(IDR_MENU1);	// Загрузить меню из файла ресурса
	SetMenu(&m_wndMenu);					// Установить меню

}

afx_msg void CMainWin::OnCommand_PLOT()
{
	this->DDialog = new DotsDialog();
	this->DDialog->DoModal();
}

// Реализация карты сообщений
BEGIN_MESSAGE_MAP(CMainWin, CFrameWnd)
	ON_COMMAND(ID_PLOT, OnCommand_PLOT)
END_MESSAGE_MAP()
