#ifndef FRAMES
#define FRAMES

#include <afxwin.h>

class LeftTopFrame: public CFrameWnd
{
public:
	LeftTopFrame(CWnd*, CRect);
	~LeftTopFrame();
private:
	CEdit* EQEditOld;
	CStatic* EQLabel;
public:
	afx_msg void OnShift2();
	afx_msg void OnEnter();
	DECLARE_MESSAGE_MAP()
};

class RightTopFrame: public CFrameWnd
{
public:
	RightTopFrame(CWnd*, CRect);
};

class LeftBottomFrame: public CFrameWnd
{
public:
	LeftBottomFrame(CWnd*, CRect);
};

class RightBottomFrame: public CFrameWnd
{
public:
	RightBottomFrame(CWnd*, CRect);
};

#endif