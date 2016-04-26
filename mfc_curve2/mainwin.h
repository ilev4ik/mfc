#include <afxwin.h>
#include "DotsDialog.h"

class CMainWin: public CFrameWnd
{
public:
	CMainWin();
	afx_msg void OnCommand_PLOT();
private:
	CMenu m_wndMenu;
	DotsDialog* DDialog;
	DECLARE_MESSAGE_MAP()
};

class CApp: public CWinApp
{
public:
	BOOL InitInstance();
};