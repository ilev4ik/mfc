#include "frame.h"
#include "res.h"
#include "automato.h"


// убрать
#include "mathground.h"
#include <iostream>

LeftTopFrame::LeftTopFrame(CWnd* pWnd, CRect rect) 
{
	this->Create(
		NULL, TEXT(""),
		WS_BORDER | WS_CHILD,
		rect,
		pWnd
		);
	this->ShowWindow(SW_SHOW);

	EQLabel = new CStatic();
	CRect r_lbl(
		(rect.right - rect.left) / 3, 
		(rect.bottom - rect.top) / 3, 
		rect.right - (rect.right - rect.left) / 3, 
		(rect.bottom - rect.top) /3 + 20
		);

	CRect r_edit(
		(rect.right - rect.left) / 6,
		(rect.bottom - rect.top) / 3 + 20 + 2,
		rect.right - (rect.right - rect.left) / 6,
		(rect.bottom - rect.top) / 3 + 20 + 20 + 2
		);

	EQLabel->Create(TEXT("Введите уравнение: "), WS_BORDER, r_lbl, this);
	EQLabel->ShowWindow(SW_RESTORE);

	
	EQEditOld = new CEdit();
	EQEditOld->Create(WS_BORDER, r_edit, this, NULL);
	this->LoadAccelTable(MAKEINTRESOURCE(IDR_ACCELERATOR1));
	EQEditOld->ShowWindow(SW_RESTORE);

}

RightTopFrame::RightTopFrame(CWnd* pWnd, CRect rect)
{
	this->Create(
		NULL, TEXT(""),
		WS_BORDER | WS_CHILD,
		rect,
		pWnd
		);
	this->ShowWindow(SW_SHOW);

	
}

LeftBottomFrame::LeftBottomFrame(CWnd* pWnd, CRect rect)
{
	this->Create(
		NULL, TEXT(""),
		WS_BORDER | WS_CHILD,
		rect,
		pWnd
		);
	this->ShowWindow(SW_SHOW);
}

RightBottomFrame::RightBottomFrame(CWnd* pWnd, CRect rect)
{
	this->Create(
		NULL, TEXT(""),
		WS_BORDER | WS_CHILD,
		rect,
		pWnd
		);
	this->ShowWindow(SW_SHOW);

}

BEGIN_MESSAGE_MAP(LeftTopFrame, CFrameWnd)
	ON_COMMAND(ID_SHIFT2, LeftTopFrame::OnShift2)
	ON_COMMAND(ID_ENTER, LeftTopFrame::OnEnter)
END_MESSAGE_MAP()

afx_msg void LeftTopFrame::OnShift2()
{
	INT nLength = this->EQEditOld->GetWindowTextLengthW();
	this->EQEditOld->SetSel(nLength, nLength);
	this->EQEditOld->ReplaceSel(TEXT("^2"));
}

afx_msg void LeftTopFrame::OnEnter()
{
	std::map <CString, DOUBLE> eq_map;

	CString EQ;
	this->EQEditOld->GetWindowTextW(EQ);

	// Конечный автомат для разбора строки
	FiniteAutomato CurveEq(EQ);
	CurveEq.START();

	this->EQEditOld->SetWindowTextW(CurveEq.getResult());
	EQEditOld->SetFocus();
	EQEditOld->SetSel(0, -1);	// select all text and move cursor at the end
	EQEditOld->SetSel(-1);		// caret is on the last position

	MathTool tool(CurveEq.eq_map);
}