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

	EQLabel->Create(TEXT("Введите уравнение: "), WS_BORDER, r_lbl, this);
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

	// создаём контекст устройства (нашего текущего фрейма)
	this->GetClientRect(this->rect);

	// центр координатной оси
	this->O.x = (rect.right - rect.left) / 2;
	this->O.y = (rect.bottom - rect.top) / 2;

	// Поддержка виртуального окна
	// Получим размеры экрана
	maxX = ::GetSystemMetrics(SM_CXSCREEN);
	maxY = ::GetSystemMetrics(SM_CYSCREEN);
	CClientDC dc(this);

	// Создание совместимого контекста устройства и
	// битового образа

	m_memDC.CreateCompatibleDC(&dc);
	m_bmp.CreateCompatibleBitmap(&dc, maxX, maxY);
	

	// создаём битовый шаблон на устройстве для m_mbp
	// придварительно создав, куда класть его
	m_memDC.SelectObject(&m_bmp);
	m_memDC.PatBlt(0, 0, maxX, maxY, PATCOPY);
	this->setBackground();

	this->ShowWindow(SW_RESTORE);
}

void RightTopFrame::setBackground()
{
	// создаём pen для рисования
	// и загрузим рисунок к буферный контекст

	CPen pen(PS_SOLID, 1, BLACK_PEN);
	m_memDC.SelectObject(pen);

	// Рисуем координатные оси и отметки важные
	// вертикально:
	for (INT i = rect.left; i < rect.right; i += (rect.right - rect.left) / 8)
	{
		m_memDC.MoveTo(i, 0);
		m_memDC.LineTo(i, rect.bottom);
	}

	for (INT j = rect.top; j < rect.bottom; j += (rect.bottom - rect.top) / 6)
	{
		m_memDC.MoveTo(0, j);
		m_memDC.LineTo(rect.right, j);
	}
	// Нарисуем стрелки Y и X соответственно
	m_memDC.MoveTo(O.x, 0);
	m_memDC.LineTo(O.x - 5, rect.top + 10);
	m_memDC.MoveTo(O.x, 0);
	m_memDC.LineTo(O.x + 5, rect.top + 10);

	m_memDC.MoveTo(rect.right, O.y);
	m_memDC.LineTo(rect.right - 10, O.y - 5);
	m_memDC.MoveTo(rect.right, O.y);
	m_memDC.LineTo(rect.right - 10, O.y + 5);
}

afx_msg void RightTopFrame::OnPaint()
{
	CPaintDC paintDC(this);

	// копируем битмап в текущий контекст
	// вместо трудоёмкой перерисовки каждый раз :)
	paintDC.BitBlt(0, 0, rect.right, rect.bottom, &m_memDC, 0, 0, SRCCOPY);

	if (p_isdefined)
	{
		switch (pf.CURVE_STATE)	
		{
		case ELLIPS:
			paintDC.Ellipse(100,100,200,200);
			break;
		case PARABOLA:
			break;
		case HIPERBOLA:
			break;
		case COINCIDING:
			break;
		case DOT:
			break;
		case PARALLEL:
			break;
		default:
			return;
		}
	}
}

BEGIN_MESSAGE_MAP(RightTopFrame, CFrameWnd)
	ON_WM_ERASEBKGND()
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
	polar->Create(TEXT("Уравнение в полярных координатах..."), WS_BORDER, p_lbl, this);
	polar->ShowWindow(SW_RESTORE);

	excent = new CStatic();
	excent->Create(TEXT("Уравнение через эксцентриситет..."), WS_BORDER, e_lbl, this);
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
	p->Create(TEXT("<-- динамическое изменение фокального параметра"), NULL, rp, this);
	p->ShowWindow(SW_RESTORE);

	e = new CStatic();
	e->Create(TEXT("<-- динамическое изменение эксцентриситета"), NULL, re, this);
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
	InfoLabel->Create(TEXT("Общие сведения о кривой:\n\n"), WS_BORDER, r_lbl, this);
	InfoLabel->ShowWindow(SW_RESTORE);
}

LeftBottomFrame::~LeftBottomFrame()
{
	delete InfoLabel;
}
