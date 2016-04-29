#ifndef MAIN_WIN
#define MAIN_WIN

#include "DotsDialog.h"
#include "frame.h"

class CMainWin: public CFrameWnd
{
public:
	CMainWin();
// slots
	afx_msg void OnCommand_SHOW_DIALOG();
	afx_msg void OnEnter();
private:
	CMenu m_wndMenu;
	DotsDialog* DDialog;	
	LeftBottomFrame*	frame_LB;
	LeftTopFrame*		frame_LT;
	RightTopFrame*		frame_RT;
	RightBottomFrame*	frame_RB;

	DECLARE_MESSAGE_MAP()
};

class CApp: public CWinApp
{
public:
	BOOL InitInstance();
};

#endif