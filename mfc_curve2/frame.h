#ifndef FRAMES
#define FRAMES

#include <afxwin.h>

class LeftTopFrame: public CFrameWnd
{
public:
	LeftTopFrame(CWnd*, CRect);
	~LeftTopFrame();
	CEdit* EQEditOld;
	CStatic* EQLabel;
public:
	afx_msg void OnShift2();
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
	~LeftBottomFrame();
	CStatic* InfoLabel;
};

class RightBottomFrame: public CFrameWnd
{
public:
	RightBottomFrame(CWnd*, CRect);
};

#endif