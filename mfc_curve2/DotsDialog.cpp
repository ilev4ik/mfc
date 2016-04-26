// DotsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DotsDialog.h"
#include "afxdialogex.h"
#include "res.h"

// DotsDialog dialog

IMPLEMENT_DYNAMIC(DotsDialog, CDialog)

DotsDialog::DotsDialog(CWnd* pParent): CDialog(DotsDialog::IDD, pParent) {}

DotsDialog::~DotsDialog() {}

void DotsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DotsDialog, CDialog)
	ON_BN_CLICKED(IDC_CANCEL, &DotsDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ACCEPT, &DotsDialog::OnBnClickedAccept)
END_MESSAGE_MAP()


// DotsDialog message handlers

void DotsDialog::OnBnClickedCancel()
{

}


void DotsDialog::OnBnClickedAccept()
{
	// TODO: Add your control notification handler code here
}
