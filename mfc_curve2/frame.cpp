// Порядок не менять!!!
#define _USE_MATH_DEFINES

#include <math.h>
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

	// Загрузить в таблицу аксселераторов
	LoadAccelTable(MAKEINTRESOURCE(IDR_LEFTTOPFRAME));

	// введите уравнение
	EQLabel = new CStatic();
	// rectangle label
	CRect r_lbl(
		(rect.right - rect.left) / 3, 
		(rect.bottom - rect.top) / 3, 
		rect.right - (rect.right - rect.left) / 3, 
		(rect.bottom - rect.top) /3 + 20 // высота курсора
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
	// узнаём текущую позицию курсора
	DWORD pos = this->EQEditOld->GetSel();
	// сперва выделить, потом поменять
	this->EQEditOld->SetSel(pos, pos);
	this->EQEditOld->ReplaceSel(TEXT("^2"));
}

LeftTopFrame::~LeftTopFrame()
{
	delete EQEditOld;
	delete EQLabel;
}

// height and width of bitmaps
RightTopFrame::RightTopFrame(CWnd* pWnd, CRect r)
{
	p_isdefined = FALSE;

	this->Create(
		NULL, TEXT(""),
		WS_BORDER | WS_CHILD,
		r,
		pWnd
		);

	//всплывающее меню на правую кнопку мыши
	hPopupMenu = CreatePopupMenu();
	AppendMenu(hPopupMenu, MF_STRING, ID_TANGENT, TEXT("Касательная/поляра"));
	AppendMenu(hPopupMenu, MF_STRING, ID_NORMAL, TEXT("Нормаль"));
	AppendMenu(hPopupMenu, MF_STRING, ID_DIAMETER, TEXT("Диаметр"));

	// создаём контекст устройства (нашего текущего фрейма)
	this->GetClientRect(this->rect);

	// центр координатной оси
	this->O.x = (rect.right - rect.left) / 2;
	this->O.y = (rect.bottom - rect.top) / 2;

	// ширина квадратной сетки
	this->step = (rect.bottom - rect.top) / 8;	// 1 [step] = 1 [cm]

	// Поддержка виртуального окна
	// Получим разрешение экрана
	maxX = ::GetSystemMetrics(SM_CXSCREEN);
	maxY = ::GetSystemMetrics(SM_CYSCREEN);

	// Получить контекст клиентской части правого верхнего окна
	CClientDC dc(this);

	// Создание совместимого контекста устройства и
	// битового образа
	// создаём битовый шаблон на устройстве для m_mbp,
	// предварительно создав, куда его класть.
	gDC.CreateCompatibleDC(&dc);	// для фона (grid)
	g_bmp.CreateCompatibleBitmap(&dc, maxX, maxY);
	gDC.SelectObject(&g_bmp);
	gDC.PatBlt(0, 0, maxX, maxY, PATCOPY);

	this->setBackground();

	cDC.CreateCompatibleDC(&dc);	// для канонических коорд
	c_bmp.CreateCompatibleBitmap(&dc, maxX, maxY);
	cDC.SelectObject(&c_bmp);
	cDC.PatBlt(0, 0, maxX, maxY, PATCOPY);

	dDC.CreateCompatibleDC(&dc);	// для декартовых коорд
	d_bmp.CreateCompatibleBitmap(&dc, maxX, maxY);
	dDC.SelectObject(&d_bmp);
	dDC.PatBlt(0, 0, maxX, maxY, PATCOPY);

	// popup
	tip = new CStatic();
	tip->Create(0, WS_BORDER, CRect(0, 0, step*3, step+2), this);
	tip->ShowWindow(SW_RESTORE);

	this->ShowWindow(SW_RESTORE);
}

void RightTopFrame::setBackground()
{
	// создаём pen для рисования
	// и загрузим рисунок к буферный контекст
	CPen pen_axes(PS_SOLID, 1, BLACK_PEN); // стиль, ширина, макрос цвета или RGB(r,g,b)
	gDC.SelectObject(pen_axes);

	// координатная ось
	gDC.MoveTo(O.x, 0);
	gDC.LineTo(O.x, rect.bottom);
	gDC.MoveTo(0, O.y);
	gDC.LineTo(rect.right, O.y);

	// Нарисуем стрелки Y
	gDC.MoveTo(O.x, 0);
	gDC.LineTo(O.x - 5, rect.top + 10);
	gDC.MoveTo(O.x, 0);
	gDC.LineTo(O.x + 5, rect.top + 10);
	
	// подпишем ось Y
	gDC.TextOutW(O.x + 8, rect.top, CString("y"));
	// и ось X
	gDC.TextOutW(rect.right - 10, O.y + 5, CString("x"));

	// Нарисуем стрелки X
	gDC.MoveTo(rect.right, O.y);
	gDC.LineTo(rect.right - 10, O.y - 5);
	gDC.MoveTo(rect.right, O.y);
	gDC.LineTo(rect.right - 10, O.y + 5);

	// сетка серого цвета:
	CPen pen_grid(PS_SOLID, 1, RGB(180, 180, 180));
	gDC.SelectObject(pen_grid);

	// по Y
	for (INT i = step; i <= rect.bottom; i += step)
	{
		gDC.MoveTo(0, O.y + i);	// вниз
		gDC.LineTo(rect.right, O.y + i);
		gDC.MoveTo(0, O.y - i);	// вверх
		gDC.LineTo(rect.right, O.y - i);
	}

	// по X
	for (INT j = step; j <= rect.right; j += step)
	{
		gDC.MoveTo(O.x + j, 0);	// вправо
		gDC.LineTo(O.x + j, rect.bottom);
		gDC.MoveTo(O.x - j, 0);	// влево
		gDC.LineTo(O.x - j, rect.bottom);
	}
}

void RightTopFrame::plotCoinciding()
{
	cDC.MoveTo(O.x, 0);
	cDC.LineTo(O.x, rect.bottom);
}

void RightTopFrame::plotDot()
{
	// +- 3 чтобы было видно
	cDC.Ellipse(O.x - 3, O.y - 3, O.x + 3, O.y + 3);
}

// по умолчанию строим в канонических координатах
void RightTopFrame::plotEllipse(CPoint C, int state = 0)
{

	// перо для фигур
	CPen cpen(PS_DOT, 1, RGB(0, 0, 0));		// в канонических координатах
	CPen dpen(PS_SOLID, 2, RGB(0, 0, 0));	// в декартовых координатах
	int a = step*pf.ca, b = step*pf.cb;

	switch (state)
	{
	case 0:
		cDC.SelectStockObject(NULL_BRUSH); // прозрачная кисть
		cDC.SelectObject(&cpen);
		cDC.Ellipse(
			(int)(C.x - a), (int)(C.y - b),
			(int)(C.x + a), (int)(C.y + b)
		);
		break;
	case 1:
	{
		int coef = 1;
		if ((C == Z) && (pf.a(1, 1) > pf.a(2, 2)))
		{
			std::swap(a, b);
			coef *= -1;
		}

		dDC.SelectStockObject(NULL_BRUSH);
		dDC.SelectObject(&dpen);
		dDC.Ellipse(
			(int)(C.x - a), (int)(C.y - b),
			(int)(C.x + a), (int)(C.y + b));
		// повернуть
		this->plotRotatedBM(coef*atan(pf.a(1, 2)));
		break;
	}
	default:
		break;
	}
	
}

void RightTopFrame::plotHiperbola()
{
	for (DOUBLE x = O.x + pf.ca*step; x < rect.right; ++x)
	{
		cDC.SetPixel((int)x, (int)ceil(O.y - step*pf.cb*sqrt((x - O.x)*(x - O.x) / (step*step*pf.ca*pf.ca) - 1)), BLACK_PEN);
		cDC.SetPixel((int)x, (int)ceil(O.y + step*pf.cb*sqrt((x - O.x)*(x - O.x) / (step*step*pf.ca*pf.ca) - 1)), BLACK_PEN);
	}

	for (DOUBLE x = O.x - pf.ca*step; x >= rect.left; --x)
	{
		cDC.SetPixel((int)x, (int)ceil(O.y - step*pf.cb*sqrt((x - O.x)*(x - O.x) / (step*step*pf.ca*pf.ca) - 1)), BLACK_PEN);
		cDC.SetPixel((int)x, (int)ceil(O.y + step*pf.cb*sqrt((x - O.x)*(x - O.x) / (step*step*pf.ca*pf.ca) - 1)), BLACK_PEN);
	}
}

void RightTopFrame::plotParabola()
{
	if (pf.cp < 0)
		pf.cp *= -1;	// т.к. существует поворот
	for (DOUBLE x = O.x; x <= rect.right; ++x)
	{
		cDC.SetPixel((int)x, (int)(O.y - sqrt(2 * pf.cp*step*(x - O.x))), BLACK_PEN);	// верхняя
		cDC.SetPixel((int)x, (int)(O.y + sqrt(2 * pf.cp*step*(x - O.x))), BLACK_PEN);	// нижняя
	}
}

void RightTopFrame::plotParallel()
{
	cDC.MoveTo((int)(O.x - step*pf.ca), 0);
	cDC.LineTo((int)(O.x - step*pf.ca), rect.bottom);
	cDC.MoveTo((int)(O.x + step*pf.ca), 0);
	cDC.LineTo((int)(O.x + step*pf.ca), rect.bottom);
}

void RightTopFrame::plotIntersecting()
{
	int x1 = rect.left, x2 = rect.right;
	cDC.MoveTo(x1, pf.k*(O.x-x1) + O.y);
	cDC.LineTo(x2, pf.k*(O.x-x2) + O.y);

	cDC.MoveTo(x1, -pf.k*(O.x - x1) + O.y);
	cDC.LineTo(x2, -pf.k*(O.x - x2) + O.y);
}

BOOL RightTopFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{	
	CPoint pos;
	GetCursorPos(&pos);
	ScreenToClient(&pos);

	for (int i = 0; i < pf.focus.size(); ++i)
	{
		if (abs(pos.x - (O.x + step*pf.focus[i].x)) < 5 &&
			abs(pos.y - (O.y + step*pf.focus[i].y)) < 5)
		{
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_UPARROW));
			return TRUE;
		}
		else ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}

	if (pf.center != nullptr)
	{
		if (abs(O.x + step*pf.center->x - pos.x) < 5 &
			abs(O.y - step*pf.center->y - pos.y) < 5)
		{
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_UPARROW));
			return TRUE;
		}
		else ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}

	return TRUE;
}

afx_msg void RightTopFrame::OnRButtonDown(UINT, CPoint p)
{
	extra = p;
	ClientToScreen(&p);
	TrackPopupMenuEx(hPopupMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this->m_hWnd, NULL);
}

afx_msg void RightTopFrame::OnMouseMove(UINT, CPoint pos)
{
	for (int i = 0; i < pf.focus.size(); ++i)
	{
		if (abs(pos.x - (O.x + step*pf.focus[i].x)) < 5 &&
			abs(pos.y - (O.y + step*pf.focus[i].y)) < 5)
		{
			tip->SetWindowTextW(CString("Фокус"));
			return;
		}
		else
		{
			tip->SetWindowTextW(TEXT(""));
		}
	}

	if (pf.center != nullptr)
	{
		if (abs(O.x + step*pf.center->x - pos.x) < 5 &
			abs(O.y - step*pf.center->y - pos.y) < 5)
		{
			tip->SetWindowTextW(TEXT("Центр"));
			return;
		}
		else
		{
			tip->SetWindowTextW(TEXT(""));
		}
	}

	for (int i = 0; i < pf.dir.size(); ++i)
	{
		if (abs(pos.x - (O.x + step*pf.dir[i])) < 5)
		{
			tip->SetWindowTextW(TEXT("Директриса"));
			return;
		}
		else
		{
			tip->SetWindowTextW(TEXT(""));
		}
	}

}

void RightTopFrame::plotTangent()
{
	double x, y;
	dDC.SelectStockObject(BLACK_PEN);

	if (p_isdefined)
	{
		x = extra.x - O.x;
		y = O.y - extra.y;

		this->pf.calcTangentInPoint(Point(x / step, y / step));
		double b = pf.tc;
		double k = pf.tk;

		double x1 = rect.left, x2 = rect.right;
		this->dDC.MoveTo(x1, k*(O.x - x1) + floor(O.y - step*b));
		this->dDC.LineTo(x2, k*(O.x - x2) + floor(O.y - step*b));
	}
	else MessageBox(TEXT("Кривая не определена!"),
		TEXT("WARNING"), MB_ICONERROR | MB_OK);

	pDC->BitBlt(0, 0, rect.right, rect.bottom, &dDC, 0, 0, SRCAND);
	this->clearGraphics();
}

void RightTopFrame::plotNormal()
{
	double x, y;
	dDC.SelectStockObject(BLACK_PEN);

	if (p_isdefined)
	{
		x = extra.x - O.x;
		y = O.y - extra.y;

		this->pf.calcNormalInPoint(Point(x / step, y / step));
		double b = pf.nc;
		double k = pf.nk;

		double x1 = rect.left, x2 = rect.right;
		this->dDC.MoveTo(x1, k*(O.x - x1) + (O.y - step*b));
		this->dDC.LineTo(x2, k*(O.x - x2) + (O.y - step*b));
	}
	else MessageBox(TEXT("Кривая не определена!"),
		TEXT("WARNING"), MB_ICONERROR | MB_OK);

	pDC->BitBlt(0, 0, rect.right, rect.bottom, &dDC, 0, 0, SRCAND);
	this->clearGraphics();
}

void RightTopFrame::plotDiameter()
{
	double x, y;
	dDC.SelectStockObject(BLACK_PEN);

	if (p_isdefined)
	{
		x = extra.x - O.x;
		y = O.y - extra.y;

		this->pf.calcDiameterInPoint(Point(x / step, y / step));
		double b = pf.dc;
		double k = pf.dk;

		double x1 = rect.left, x2 = rect.right;
		this->dDC.MoveTo(x1, k*(O.x - x1) + (O.y - step*b));
		this->dDC.LineTo(x2, k*(O.x - x2) + (O.y - step*b));
	}
	else MessageBox(TEXT("Кривая не определена!"),
		TEXT("WARNING"), MB_ICONERROR | MB_OK);

	pDC->BitBlt(0, 0, rect.right, rect.bottom, &dDC, 0, 0, SRCAND);
	this->clearGraphics();
}

void RightTopFrame::plotFeatures()
{
	// brushes
	CBrush brush_focus, brush_center;
	// hot points
	CPen pen_focus(PS_SOLID, 2, RGB(255, 0, 0));
	CPen pen_center(PS_SOLID, 2, RGB(0, 255, 0));
	CPen pen_dir(PS_SOLID, 2, RGB(0, 0, 255));

	brush_focus.CreateSolidBrush(RGB(255, 0, 0));
	brush_center.CreateSolidBrush(RGB(0, 255, 0));

	cDC.SelectObject(pen_dir);
	cDC.SelectObject(&c_bmp);

	// директриса
	for (INT i = 0; i < pf.dir.size(); ++i)
	{
		int a = pf.dir[i];
		cDC.MoveTo((int)(O.x + step*this->pf.dir[i]), 0);
		cDC.LineTo((int)(O.x + step*this->pf.dir[i]), rect.bottom);
	}

	cDC.SelectObject(pen_focus);
	cDC.SelectObject(brush_focus);

	// фокусы
	for (INT j = 0; j < pf.focus.size(); ++j)
	{
		cDC.Ellipse(
			O.x + step*pf.focus[j].x - 4, O.y - step*pf.focus[j].y - 4,
			O.x + step*pf.focus[j].x + 4, O.y - step*pf.focus[j].y + 4
			);
	}

	// центр
	dDC.SelectObject(pen_center);
	dDC.SelectObject(brush_center);

	if (pf.center != nullptr)
	{
		this->Z.x = (int)(step*pf.center->x + O.x);
		this->Z.y = (int)(O.y - step*pf.center->y);

		dDC.Ellipse(
			Z.x - 4, Z.y - 4,
			Z.x + 4, Z.y + 4
			);
	}
}

void RightTopFrame::plotRotatedBM(float fangle)
{
	// получить безопасный хэндлер для поворота
	HDC hDc = pDC->GetSafeHdc();
	dDC.SelectObject(&d_bmp);
	int nGraphicsMode = SetGraphicsMode(hDc, GM_ADVANCED);
	XFORM xform;

	if (fangle != 0)
	{
		//double fangle = (double)m_iAngle / 180. * 3.1415926;
		xform.eM11 = (float)cos(fangle);
		xform.eM12 = (float)sin(fangle);
		xform.eM21 = (float)-sin(fangle);
		xform.eM22 = (float)cos(fangle);
		xform.eDx = (float)(Z.x - cos(fangle)*Z.x + sin(fangle)*Z.y);
		xform.eDy = (float)(Z.y - cos(fangle)*Z.y - sin(fangle)*Z.x);

		SetWorldTransform(hDc, &xform);

		dDC.BitBlt(0, 0, rect.right, rect.bottom, &dDC, 0, 0, SRCCOPY);	// скопировать в pDC из dDC
		pDC->BitBlt(0, 0, rect.right, rect.bottom, &dDC, 0, 0, SRCAND);
		dDC.FillSolidRect(0, 0, maxX, maxY, RGB(255, 255, 255));

		// восстановить настройки контекста (теперь все не повернуто, но картнку сохранили)
		xform.eM11 = (float)1.0;
		xform.eM12 = (float)0;
		xform.eM21 = (float)0;
		xform.eM22 = (float)1.0;
		xform.eDx = (float)0;
		xform.eDy = (float)0;

		SetWorldTransform(hDc, &xform);
	}
	SetGraphicsMode(hDc, nGraphicsMode);
}

void RightTopFrame::clearGraphics()
{
	// очищаем буфер (белым) собственно графика и его features
	// все контексты, кроме сетки
	dDC.FillSolidRect(0, 0, maxX, maxY, RGB(255, 255, 255));
	cDC.FillSolidRect(0, 0, maxX, maxY, RGB(255, 255, 255));
}

afx_msg void RightTopFrame::OnPaint()
{
	pDC = new CPaintDC(this);
	// копируем битмап gDC (grid -- сетка) в текущий контекст paintDC
	// вместо трудоёмкой перерисовки каждый раз :)
	pDC->BitBlt(0, 0, rect.right, rect.bottom, &gDC, 0, 0, SRCCOPY);


	if (p_isdefined)
	{
		// сперва вызываем это обязательно
		this->plotFeatures();

		switch (pf.CURVE_STATE)	
		{
		case ELLIPS:
		{
			this->plotEllipse(O);	// канонические
			this->plotEllipse(Z, 1);	// в декартовых
			break;
		}
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
		case INTERSECTING:
			this->plotIntersecting();
			break;
		case _ERROR:
			MessageBox(TEXT("Ни одной вещественной точки не определено!"),
				TEXT("WARNING"), MB_ICONWARNING | MB_OK);
			break;
		default:
			MessageBox(TEXT("Уравнение кривой второго порядка введено неверно!"),
				TEXT("WARNING"), MB_ICONWARNING | MB_OK);
			clearGraphics();
			return;
		}
	}

	// обновляем с рисунком и точками
	dDC.BitBlt(0, 0, rect.right, rect.bottom, &cDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, rect.right, rect.bottom, &dDC, 0, 0, SRCAND);
	this->clearGraphics();
}

BEGIN_MESSAGE_MAP(RightTopFrame, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_TANGENT, RightTopFrame::plotTangent)
	ON_COMMAND(ID_NORMAL, RightTopFrame::plotNormal)
	ON_COMMAND(ID_DIAMETER, RightTopFrame::plotDiameter)
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
		(rr.bottom - rr.top)/15,
		rr.right - 5,
		rr.bottom - (rr.bottom - rr.top) / 15
		);

	InfoLabel = new CStatic();
	InfoLabel->Create(TEXT("Общие сведения о кривой:\n\n"), WS_BORDER, r_lbl, this);
	InfoLabel->ShowWindow(SW_RESTORE);
}

LeftBottomFrame::~LeftBottomFrame()
{
	delete InfoLabel;
}
