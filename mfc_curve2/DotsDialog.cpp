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
	// ������ pen ��� ���������
	// � �������� ������� � �������� ��������
	CPen pen_axes(PS_SOLID, 1, BLACK_PEN);
	DC.SelectObject(pen_axes);

	// ������������ ���
	DC.MoveTo(O.x, 0);
	DC.LineTo(O.x, rect.bottom);
	DC.MoveTo(0, O.y);
	DC.LineTo(rect.right, O.y);

	// �������� ������� Y � X ��������������
	DC.MoveTo(O.x, 0);
	DC.LineTo(O.x - 5, rect.top + 10);
	DC.MoveTo(O.x, 0);
	DC.LineTo(O.x + 5, rect.top + 10);

	// �������� ��� Y
	DC.TextOutW(O.x + 8, rect.top, CString("y"));
	// � ��� X
	DC.TextOutW(rect.right - 10, O.y + 5, CString("x"));

	DC.MoveTo(rect.right, O.y);
	DC.LineTo(rect.right - 10, O.y - 5);
	DC.MoveTo(rect.right, O.y);
	DC.LineTo(rect.right - 10, O.y + 5);

	// ����� ������ �����:
	CPen pen_grid(PS_SOLID, 1, RGB(180, 180, 180));
	DC.SelectObject(pen_grid);

	// �� Y
	for (INT i = step; i <= rect.bottom; i += step)
	{
		DC.MoveTo(0, O.y + i);	// ����
		DC.LineTo(rect.right, O.y + i);
		DC.MoveTo(0, O.y - i);	// �����
		DC.LineTo(rect.right, O.y - i);
	}

	// �� X
	for (INT j = step; j <= rect.right; j += step)
	{
		DC.MoveTo(O.x + j, 0);	// ������
		DC.LineTo(O.x + j, rect.bottom);
		DC.MoveTo(O.x - j, 0);	// �����
		DC.LineTo(O.x - j, rect.bottom);
	}
}

DotsDialog::~DotsDialog() {}

void DotsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// �������� ����� � �����
}

BEGIN_MESSAGE_MAP(DotsDialog, CDialog)
	ON_BN_CLICKED(IDC_CANCEL, &DotsDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ACCEPT, &DotsDialog::OnBnClickedAccept)
END_MESSAGE_MAP()


// DotsDialog message handlers

void DotsDialog::OnPaint()
{
	// ��������� ������������ ����
	// ������� ������� ������
	int maxX = ::GetSystemMetrics(SM_CXSCREEN);
	int maxY = ::GetSystemMetrics(SM_CYSCREEN);


	// ����� ������������ ���
	this->O.x = (rect.right - rect.left) / 2;
	this->O.y = (rect.bottom - rect.top) / 2;

	CClientDC dc(this);

	// �������� ������������ ��������� ���������� �
	// �������� ������

	DC.CreateCompatibleDC(&dc);	// ��� ����
	m_bmp.CreateCompatibleBitmap(&dc, maxX, maxY);

	// ������ ������� ������ �� ���������� ��� m_mbp,
	// �������������� ������, ���� ��� ������.
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
