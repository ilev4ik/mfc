// DotsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DotsDialog.h"
#include "afxdialogex.h"
#include "res.h"

// DotsDialog dialog

IMPLEMENT_DYNAMIC(DotsDialog, CDialog)

DotsDialog::DotsDialog(CWnd* pParent): CDialog(DotsDialog::type::IDD)
{

}


void DotsDialog::setBackground()
{
	// создаём pen для рисования
	// и загрузим рисунок к буферный контекст
	CPen pen_axes(PS_SOLID, 1, BLACK_PEN);
	DC.SelectObject(pen_axes);

	// координатная ось
	DC.MoveTo(O.x, 0);
	DC.LineTo(O.x, rect.bottom);
	DC.MoveTo(0, O.y);
	DC.LineTo(rect.right, O.y);

	// Нарисуем стрелки Y и X соответственно
	DC.MoveTo(O.x, 0);
	DC.LineTo(O.x - 5, rect.top + 10);
	DC.MoveTo(O.x, 0);
	DC.LineTo(O.x + 5, rect.top + 10);

	// подпишем ось Y
	DC.TextOutW(O.x + 8, rect.top, CString("y"));
	// и ось X
	DC.TextOutW(rect.right - 10, O.y + 5, CString("x"));

	DC.MoveTo(rect.right, O.y);
	DC.LineTo(rect.right - 10, O.y - 5);
	DC.MoveTo(rect.right, O.y);
	DC.LineTo(rect.right - 10, O.y + 5);

	// сетка серого цвета:
	CPen pen_grid(PS_SOLID, 1, RGB(180, 180, 180));
	DC.SelectObject(pen_grid);

	// по Y
	for (INT i = step; i <= rect.bottom; i += step)
	{
		DC.MoveTo(0, O.y + i);	// вниз
		DC.LineTo(rect.right, O.y + i);
		DC.MoveTo(0, O.y - i);	// вверх
		DC.LineTo(rect.right, O.y - i);
	}

	// по X
	for (INT j = step; j <= rect.right; j += step)
	{
		DC.MoveTo(O.x + j, 0);	// вправо
		DC.LineTo(O.x + j, rect.bottom);
		DC.MoveTo(O.x - j, 0);	// влево
		DC.LineTo(O.x - j, rect.bottom);
	}
}

DotsDialog::~DotsDialog() {}

void DotsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// обратная связь с папой
}

BEGIN_MESSAGE_MAP(DotsDialog, CDialog)
	ON_BN_CLICKED(IDC_CANCEL, &DotsDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ACCEPT, &DotsDialog::OnBnClickedAccept)
END_MESSAGE_MAP()


// DotsDialog message handlers

void DotsDialog::OnPaint()
{
	// Поддержка виртуального окна
	// Получим размеры экрана
	int maxX = ::GetSystemMetrics(SM_CXSCREEN);
	int maxY = ::GetSystemMetrics(SM_CYSCREEN);


	// центр координатной оси
	this->O.x = (rect.right - rect.left) / 2;
	this->O.y = (rect.bottom - rect.top) / 2;

	CClientDC dc(this);

	// Создание совместимого контекста устройства и
	// битового образа

	DC.CreateCompatibleDC(&dc);	// для фона
	m_bmp.CreateCompatibleBitmap(&dc, maxX, maxY);

	// создаём битовый шаблон на устройстве для m_mbp,
	// предварительно создав, куда его класть.
	DC.SelectObject(&m_bmp);
	DC.PatBlt(0, 0, maxX, maxY, PATCOPY);

	this->step = (rect.bottom - rect.top) / 8;
	this->setBackground();
}
void DotsDialog::OnBnClickedCancel()
{
	this->EndModalLoop(DotsDialog::r_button::CANCEL);
}


void DotsDialog::OnBnClickedAccept()
{
	this->EndModalLoop(DotsDialog::r_button::ACCEPT);
}
