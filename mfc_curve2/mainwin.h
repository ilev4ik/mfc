#ifndef MAIN_WIN
#define MAIN_WIN

#include "canvas.h"
#include "DotsDialog.h"

class CMainWin: public CFrameWnd
{
public:
	CMainWin();
// slots
	afx_msg void OnCommand_SHOW_DIALOG();
private:
	CMenu m_wndMenu;
	DotsDialog* DDialog;
	MainCanvas* MCanvas;	

	DECLARE_MESSAGE_MAP()
};

class CApp: public CWinApp
{
public:
	BOOL InitInstance();
};

#endif