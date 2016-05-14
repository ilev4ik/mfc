#ifndef FRAMES
#define FRAMES

#include <afxwin.h>
#include <afxcmn.h>	// CSliderCtrl
#include <afxtooltipctrl.h>
#include "plot.h"	// info to plot :)

class LeftTopFrame: public CFrameWnd
{
public:
	friend class CMainWin;
	LeftTopFrame(CWnd*, CRect);
	~LeftTopFrame();
private:
	CEdit* EQEditOld;
	CStatic* EQLabel;

	afx_msg void OnShift2();

	DECLARE_MESSAGE_MAP()
};

class RightTopFrame: public CFrameWnd
{
public:
	friend class CMainWin;
	RightTopFrame(CWnd*, CRect);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT, CPoint);
	afx_msg void OnRButtonDown(UINT, CPoint);
	afx_msg BOOL OnSetCursor(CWnd*, UINT, UINT);

	void setBackground();	// здесь делаем координатные оси
	CPaintDC* pDC;
private:
	void clearGraphics();

	void plotEllipse();
	void plotHiperbola();
	void plotParabola();
	void plotDot();
	void plotParallel();
	void plotCoinciding();
	void plotIntersecting();

	void plotTangent();
	void plotNormal();
	void plotDiameter();

	// point for context menu (menu point)
	CPoint mp;
	// popup menu
	HMENU hPopupMenu;

	CStatic* tip;
	// всякие точки и директрисы
	void plotFeatures();

	int step;

	BOOL p_isdefined;
	PlotFeatures pf;
	CPoint O;
	CPoint extra;
	CRect rect;

	CDC m_memDC;
	CDC m_picDC;

	CBitmap m_bmp;
	CBitmap p_bmp;
	
	int maxX, maxY;
	COLORREF m_textColor;

	DECLARE_MESSAGE_MAP()
};

class LeftBottomFrame: public CFrameWnd
{
public:
	friend class CMainWin;
	LeftBottomFrame(CWnd*, CRect);
	~LeftBottomFrame();
private:
	CStatic* InfoLabel;
};

class RightBottomFrame: public CFrameWnd
{
public:
	friend class CMainWin;
	RightBottomFrame(CWnd*, CRect);
private:
	CSliderCtrl* pSlider;
	CSliderCtrl* eSlider;
	CStatic* polar;
	CStatic* excent;
	CStatic* p;
	CStatic* e;
};

#endif