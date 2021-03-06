#ifndef FRAMES
#define FRAMES

#include <afxwin.h>
#include <afxcmn.h>	// CSliderCtrl
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

	void setBackground();	// ����� ������ ������������ ���
	CPaintDC* pDC;
private:
	void clearGraphics();

	void plotEllipse(CPoint, int);
	void plotHiperbola();
	void plotParabola();
	void plotDot();
	void plotParallel();
	void plotCoinciding();
	void plotIntersecting();

	void plotTangent();
	void plotNormal();
	void plotDiameter();


	// graphics transformation functions
	void plotRotatedBM(float);

	// point for context menu (menu point)
	CPoint mp;	// ����� � ���������� ����
	// popup menu
	HMENU hPopupMenu;

	CStatic* tip;
	// ������ ����� � ����������
	void plotFeatures();

	int step;

	BOOL p_isdefined;
	PlotFeatures pf;
	CPoint O;		// ����� ���������� ���������
	CPoint Z;		// ����� ������ ������� �������
	CPoint extra;	// ����� � ����������� ������������ �� �������� �� O (��. ����)
	CRect rect;

	CDC gDC;		// grid
	CDC dDC;		// decart
	CDC cDC;		// canon

	CBitmap g_bmp;	
	CBitmap d_bmp;	
	CBitmap c_bmp;	

	int maxX, maxY;
	COLORREF m_textColor;

	// ����� ��������� ������ ���� ��������� � ����� ����� �������� ������
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