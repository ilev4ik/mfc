#pragma once
#include "res.h"	// здесь определены ID-шники ресурсов
#include <afxwin.h>

// DotsDialog dialog

class DotsDialog: public CDialog
{
	DECLARE_DYNAMIC(DotsDialog)
public:
	DotsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~DotsDialog();

	afx_msg void OnPaint();
// Dialog Data
	enum type {IDD = IDD_OLE_PROPPAGE_SMALL};
	enum r_button {CANCEL, ACCEPT};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedAccept();
private:
	void DotsDialog::setBackground();
	CBitmap m_bmp;
	CDC DC;
	CPoint O;
	CRect rect;
	int step;
};
