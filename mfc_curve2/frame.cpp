#include "frame.h"
#include "res.h"
#include "curvetypes.h"

LeftTopFrame::LeftTopFrame(CWnd* pWnd, CRect rect) 
{
	this->Create(
		NULL, TEXT(""),
		WS_BORDER | WS_CHILD,
		rect,
		pWnd
		);
	this->ShowWindow(SW_SHOW);

	LoadAccelTable(MAKEINTRESOURCE(IDR_LEFTTOPFRAME));
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

	EQLabel->Create(TEXT("������� ���������: "), WS_BORDER, r_lbl, this);
	EQLabel->ShowWindow(SW_RESTORE);

	EQEditOld = new CEdit();
	EQEditOld->Create(WS_BORDER, r_edit, this, NULL);
	EQEditOld->ShowWindow(SW_RESTORE);
}

BEGIN_MESSAGE_MAP(LeftTopFrame, CFrameWnd)
	ON_COMMAND(ID_SHIFT2, LeftTopFrame::OnShift2)
	ON_WM_PAINT()
END_MESSAGE_MAP()

afx_msg void LeftTopFrame::OnShift2()
{
	INT nLength = this->EQEditOld->GetWindowTextLengthW();
	this->EQEditOld->SetSel(nLength, nLength);
	this->EQEditOld->ReplaceSel(TEXT("^2"));
}

LeftTopFrame::~LeftTopFrame()
{
	delete EQEditOld;
	delete EQLabel;
}

RightTopFrame::RightTopFrame(CWnd* pWnd, CRect r)
{
	p_isdefined = FALSE;

	this->Create(
		NULL, TEXT(""),
		WS_BORDER | WS_CHILD,
		r,
		pWnd
		);

	// ������ �������� ���������� (������ �������� ������)
	this->GetClientRect(this->rect);

	// ����� ������������ ���
	this->O.x = (rect.right - rect.left) / 2;
	this->O.y = (rect.bottom - rect.top) / 2;

	// ������ ���������� �����
	this->step = (rect.bottom - rect.top) / 8;	// 1 [step] = 1 [cm]

	// ��������� ������������ ����
	// ������� ������� ������
	maxX = ::GetSystemMetrics(SM_CXSCREEN);
	maxY = ::GetSystemMetrics(SM_CYSCREEN);
	CClientDC dc(this);

	// �������� ������������ ��������� ���������� �
	// �������� ������

	m_memDC.CreateCompatibleDC(&dc);	// ��� ����
	m_bmp.CreateCompatibleBitmap(&dc, maxX, maxY);

	// ������ ������� ������ �� ���������� ��� m_mbp,
	// �������������� ������, ���� ��� ������.
	m_memDC.SelectObject(&m_bmp);
	m_memDC.PatBlt(0, 0, maxX, maxY, PATCOPY);

	this->setBackground();

	m_picDC.CreateCompatibleDC(&dc);	// ��� �����������
	p_bmp.CreateCompatibleBitmap(&dc, maxX, maxY);
	m_picDC.SelectObject(&p_bmp);
	m_picDC.PatBlt(0, 0, maxX, maxY, PATCOPY);

	this->ShowWindow(SW_RESTORE);
}

void RightTopFrame::setBackground()
{
	// ������ pen ��� ���������
	// � �������� ������� � �������� ��������

	CPen pen_axes(PS_SOLID, 1, BLACK_PEN);
	m_memDC.SelectObject(pen_axes);

	// ������������ ���
	m_memDC.MoveTo(O.x, 0);
	m_memDC.LineTo(O.x, rect.bottom);
	m_memDC.MoveTo(0, O.y);
	m_memDC.LineTo(rect.right, O.y);

	// �������� ������� Y � X ��������������
	m_memDC.MoveTo(O.x, 0);
	m_memDC.LineTo(O.x - 5, rect.top + 10);
	m_memDC.MoveTo(O.x, 0);
	m_memDC.LineTo(O.x + 5, rect.top + 10);
	
	m_memDC.MoveTo(rect.right, O.y);
	m_memDC.LineTo(rect.right - 10, O.y - 5);
	m_memDC.MoveTo(rect.right, O.y);
	m_memDC.LineTo(rect.right - 10, O.y + 5);

	// ����� ������ �����:
	CPen pen_grid(PS_SOLID, 1, RGB(180, 180, 180));
	m_memDC.SelectObject(pen_grid);

	// �� Y
	for (INT i = step; i <= rect.bottom; i += step)
	{
		m_memDC.MoveTo(0, O.y + i);	// ����
		m_memDC.LineTo(rect.right, O.y + i);
		m_memDC.MoveTo(0, O.y - i);	// �����
		m_memDC.LineTo(rect.right, O.y - i);
	}

	// �� X
	for (INT j = step; j <= rect.right; j += step)
	{
		m_memDC.MoveTo(O.x + j, 0);	// ������
		m_memDC.LineTo(O.x + j, rect.bottom);
		m_memDC.MoveTo(O.x - j, 0);	// �����
		m_memDC.LineTo(O.x - j, rect.bottom);
	}
}


void RightTopFrame::plotCoinciding()
{
	m_picDC.MoveTo(O.x, 0);
	m_picDC.LineTo(O.x, rect.bottom);
}

void RightTopFrame::plotDot()
{
	// +- 3 ����� ���� �����
	m_picDC.Ellipse(O.x - 3, O.y - 3, O.x + 3, O.y + 3);
}

void RightTopFrame::plotEllipse()
{
	m_picDC.Ellipse(
		(int)(O.x - step*pf.ca), (int)(O.y - step*pf.cb), 
		(int)(O.x + step*pf.ca), (int)(O.y + step*pf.cb)
		);
}

void RightTopFrame::plotHiperbola()
{
	// ������� 2 ���� :)
	for (DOUBLE x = O.x + pf.ca*step; x < rect.right; ++x)
	{
		m_picDC.SetPixel((int)x, (int)ceil(O.y - step*pf.cb*sqrt((x - O.x)*(x - O.x) / (step*step*pf.ca*pf.ca) - 1)), BLACK_PEN);
		m_picDC.SetPixel((int)x, (int)ceil(O.y + step*pf.cb*sqrt((x - O.x)*(x - O.x) / (step*step*pf.ca*pf.ca) - 1)), BLACK_PEN);
	}

	for (DOUBLE x = O.x - pf.ca*step; x >= rect.left; --x)
	{
		m_picDC.SetPixel((int)x, (int)ceil(O.y - step*pf.cb*sqrt((x - O.x)*(x - O.x) / (step*step*pf.ca*pf.ca) - 1)), BLACK_PEN);
		m_picDC.SetPixel((int)x, (int)ceil(O.y + step*pf.cb*sqrt((x - O.x)*(x - O.x) / (step*step*pf.ca*pf.ca) - 1)), BLACK_PEN);
	}
}

void RightTopFrame::plotParabola()
{
	if (pf.cp < 0)
		pf.cp *= -1;	// �.�. ���������� �������
	for (DOUBLE x = O.x; x <= rect.right; ++x)
	{
		m_picDC.SetPixel((int)x, (int)(O.y - sqrt(2 * pf.cp*step*(x - O.x))), BLACK_PEN);	// �������
		m_picDC.SetPixel((int)x, (int)(O.y + sqrt(2 * pf.cp*step*(x - O.x))), BLACK_PEN);	// ������
	}
}

void RightTopFrame::plotParallel()
{
	m_picDC.MoveTo((int)(O.x - step*pf.ca), 0);
	m_picDC.LineTo((int)(O.x - step*pf.ca), rect.bottom);
	m_picDC.MoveTo((int)(O.x + step*pf.ca), 0);
	m_picDC.LineTo((int)(O.x + step*pf.ca), rect.bottom);
}

afx_msg void RightTopFrame::OnPaint()
{
	CPaintDC paintDC(this);

	// �������� ������ m_memDC � ������� �������� paintDC
	// ������ ��������� ����������� ������ ��� :)
	paintDC.BitBlt(0, 0, rect.right, rect.bottom, &m_memDC, 0, 0, SRCCOPY);

	// ���� ��� �����
	CPen pen(PS_SOLID, 2, RGB(57,34,100));
	m_picDC.SelectStockObject(NULL_BRUSH);
	m_picDC.SelectObject(pen);


	if (p_isdefined)
	{
		switch (pf.CURVE_STATE)	
		{
		case ELLIPS:
			this->plotEllipse();
			break;
		case PARABOLA:
			this->plotParabola();
			break;
		case HIPERBOLA:
			this->plotHiperbola();
			break;
		case COINCIDING:
			this->plotCoinciding();
			break;
		case DOT:
			this->plotDot();
			break;
		case PARALLEL:
			this->plotParallel();
			break;
		default:
			MessageBox(TEXT("��������� ������ ������� ������� ������� �������!"),
				TEXT("WARNING"), MB_ICONWARNING | MB_OK);
			m_picDC.FillSolidRect(0, 0, maxX, maxY, RGB(255, 255, 255));
			return;
		}
	}

	// ��������� � �������� � �������
	paintDC.BitBlt(0, 0, rect.right, rect.bottom, &m_picDC, 0, 0, SRCAND);
	// ������� ����� (�����) ���������� ������� � ��� features
	m_picDC.FillSolidRect(0, 0, maxX, maxY, RGB(255, 255, 255));
}

BEGIN_MESSAGE_MAP(RightTopFrame, CFrameWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

RightBottomFrame::RightBottomFrame(CWnd* pWnd, CRect rect)
{
	this->Create(
		NULL, TEXT(""),
		WS_BORDER | WS_CHILD,
		rect,
		pWnd
		);
	this->ShowWindow(SW_SHOW);

	CRect r;
	this->GetClientRect(r);

	CRect p_lbl(
		(r.right - r.left) / 10,
		(r.bottom - r.top) / 6,
		r.right - (r.right - r.left) / 10,
		(r.bottom - r.top) / 6 + 20
		);

	CRect e_lbl(
		p_lbl.left,
		p_lbl.bottom + 10,
		p_lbl.right,
		p_lbl.bottom + 30
		);

	polar = new CStatic();
	polar->Create(TEXT("��������� � �������� �����������..."), WS_BORDER, p_lbl, this);
	polar->ShowWindow(SW_RESTORE);

	excent = new CStatic();
	excent->Create(TEXT("��������� ����� ��������������..."), WS_BORDER, e_lbl, this);
	excent->ShowWindow(SW_RESTORE);

	CRect sp_rect(
		e_lbl.left,
		e_lbl.bottom + 10,
		e_lbl.right / 2,
		e_lbl.bottom + 50
		);

	CRect se_rect(
		sp_rect.left,
		sp_rect.bottom + 10,
		e_lbl.right / 2,
		sp_rect.bottom + 50
		);

	pSlider = new CSliderCtrl();
	pSlider->Create(TBS_TOOLTIPS | WS_BORDER, sp_rect, this, ID_PSLIDER);
	pSlider->SetRange(0, 50, TRUE);
	pSlider->ShowWindow(SW_RESTORE);


	eSlider = new CSliderCtrl();
	eSlider->Create(TBS_TOOLTIPS | WS_BORDER, se_rect, this, ID_ESLIDER);
	eSlider->SetRange(0, 50, TRUE);
	eSlider->ShowWindow(SW_RESTORE);

	e = new CStatic();
	CRect rp(
		sp_rect.right + 5,
		sp_rect.top,
		p_lbl.right,
		sp_rect.bottom
		);

	CRect re(
		se_rect.right + 5,
		se_rect.top,
		p_lbl.right,
		se_rect.bottom
		);

	p = new CStatic();
	p->Create(TEXT("<-- ������������ ��������� ���������� ���������"), NULL, rp, this);
	p->ShowWindow(SW_RESTORE);

	e = new CStatic();
	e->Create(TEXT("<-- ������������ ��������� ���������������"), NULL, re, this);
	e->ShowWindow(SW_RESTORE);
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

	CRect rr;
	this->GetClientRect(rr);
	CRect r_lbl(
		rr.left + 5,
		(rr.bottom - rr.top)/11,
		rr.right - 5,
		rr.bottom - (rr.bottom - rr.top) / 11
		);

	InfoLabel = new CStatic();
	InfoLabel->Create(TEXT("����� �������� � ������:\n\n"), WS_BORDER, r_lbl, this);
	InfoLabel->ShowWindow(SW_RESTORE);
}

LeftBottomFrame::~LeftBottomFrame()
{
	delete InfoLabel;
}
