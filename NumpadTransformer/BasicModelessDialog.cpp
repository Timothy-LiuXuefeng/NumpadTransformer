#include "BasicModelessDialog.h"

BOOL BasicModelessDialog::Init(HINSTANCE hInstance, LPCTSTR c_lpszTemplateName, HWND hWndParent)
{
	m_hInst = hInstance;
	//return (BOOL)DialogBoxParam(hInstance, c_lpszTemplateName, hWndParent, DlgProc, (LPARAM)this);
	m_hDlg = CreateDialogParam(hInstance, c_lpszTemplateName, hWndParent, DlgProc, (LPARAM)this);
	return m_hDlg ? TRUE : FALSE;
}

INT_PTR CALLBACK BasicModelessDialog::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	BasicModelessDialog* thisDlg = NULL;
	if (message == WM_INITDIALOG)
	{
		thisDlg = (BasicModelessDialog*)lParam;
		thisDlg->m_hDlg = hDlg;
		SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR)thisDlg);
	}
	thisDlg = (BasicModelessDialog*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
	if (thisDlg) return (INT_PTR)thisDlg->MessageProc(hDlg, message, wParam, lParam);
	return 0;
}

int BasicModelessDialog::GetListBoxSelectPos(int listBoxID)
{
	int cnt = SendDlgItemMessage(m_hDlg, listBoxID, LB_GETCOUNT, 0, 0);
	for (int i = 0; i < cnt; ++i)
	{
		if (SendDlgItemMessage(m_hDlg, listBoxID, LB_GETSEL, i, 0)) return i;
	}
	return -1;
}

bool BasicModelessDialog::SetButtonText(int buttonID, LPCTSTR lpText)
{
	return SendDlgItemMessage(m_hDlg, buttonID, WM_SETTEXT, 0, (LPARAM)lpText);
}
