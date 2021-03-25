#pragma once

#ifndef BASIC_MODAL_DIALOG_H

#define BASIC_MODAL_DIALOG_H

#include "Win32Basic.h"

class BasicModelessDialog
{
protected:

	HWND m_hDlg = NULL;
	HINSTANCE m_hInst = NULL;
	BOOL Init(HINSTANCE hInstance, LPCTSTR c_lpszTemplateName, HWND hWndParent);
	virtual bool MessageProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) = 0;

	int GetListBoxSelectPos(int listBoxID);
	bool SetButtonText(int buttonID, LPCTSTR lpText);

private:

	static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif	//#ifndef BASIC_MODAL_DIALOG_H
