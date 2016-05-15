#ifndef MAIN_WIN
#define MAIN_WIN
// директивы препроцессора дл€ решени€ вс€ческих проблем с линковкой

#include "DotsDialog.h"
#include "frame.h"
#include "automato.h"
#include "mathground.h"

class CMainWin: public CFrameWnd
{
public:
	CMainWin();
// slots
	afx_msg void OnCommand_SHOW_DIALOG();
	afx_msg void OnEnter();

	void setInfro_LT();
	void setInfo_LB();
	void setInfo_RB();
private:
	FiniteAutomato* CurveEq;	// конечный автомат дл€ разбора строки +
	MathTool* tool;				// знает всю математику +
	CMenu m_wndMenu;			// менюшка сверху
	DotsDialog* DDialog;		// окно дл€ 5 точек

	// фреймы главного окна
	LeftBottomFrame*	frame_LB;
	LeftTopFrame*		frame_LT;
	RightTopFrame*		frame_RT;
	RightBottomFrame*	frame_RB;

	// объ€вл€ем карту сообщений
	DECLARE_MESSAGE_MAP()
};


class CApp: public CWinApp
{
public:
	BOOL InitInstance();	// создаЄт экземл€р приложени€
	void PreLoadState();
};

#endif