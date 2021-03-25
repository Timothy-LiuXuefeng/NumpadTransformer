#include "UI.h"
#include "resource.h"
#include "KeyboardHooker.h"

#include <iostream>
#include <thread>

const LPCTSTR UI::aboutInfo = 
TEXT(R"(Author: Timothy Liu
Platform: Windows 10
Version: 1.0.2.0
Copyleft (C) 2021
)");

UI::UI()
{
	
	posToKey.emplace_back(0, TEXT("NONE"));
	posToKey.emplace_back(VK_RETURN, TEXT("RETURN"));
	posToKey.emplace_back(VK_BACK, TEXT("BACKSPACE"));
	posToKey.emplace_back(VK_SPACE, TEXT("SPACE"));
	posToKey.emplace_back(VK_ESCAPE, TEXT("ESC"));
	//posToKey.emplace_back(VK_TAB, TEXT("TAB"));
	posToKey.emplace_back(VK_PRIOR, TEXT("PAGE UP"));
	posToKey.emplace_back(VK_NEXT, TEXT("PAGE DOWN"));
	posToKey.emplace_back(VK_HOME, TEXT("HOME"));
	posToKey.emplace_back(VK_END, TEXT("END"));
	posToKey.emplace_back(VK_DELETE, TEXT("DELETE"));

	TCHAR alphaNumKeyStr[2]{ TEXT('\0'), TEXT('\0') };
	for (int i = 'A'; i <= 'Z'; ++i)
	{
		alphaNumKeyStr[0] = i;
		posToKey.emplace_back(i, alphaNumKeyStr);
	}
	for (int i = '0'; i <= '9'; ++i)
	{
		alphaNumKeyStr[0] = i;
		posToKey.emplace_back(i, alphaNumKeyStr);
	}

	for (int i = VK_F1; i <= VK_F12; ++i)
	{
		std::_tstring fnKeyStr = TEXT("F");
		fnKeyStr += std::to_tstring(i - VK_F1 + 1);
		posToKey.emplace_back(i, fnKeyStr.c_str());
	}

	//posToKey.emplace_back(VK_LCONTROL, TEXT("LEFT CTRL"));
	//posToKey.emplace_back(VK_RCONTROL, TEXT("RIGHT CTRL"));
	//posToKey.emplace_back(VK_LMENU, TEXT("LEFT ALT"));
	//posToKey.emplace_back(VK_RMENU, TEXT("RIGHT ALT"));
	//posToKey.emplace_back(VK_LSHIFT, TEXT("LEFT SHIFT"));
	//posToKey.emplace_back(VK_RSHIFT, TEXT("RIGHT SHIFT"));

#define TRANSFORM_LISTBOX_AND_NUMPAD(i) (listBoxIDToVkCode[IDC_LISTNUMPAD##i] = VK_NUMPAD##i)
	
	TRANSFORM_LISTBOX_AND_NUMPAD(0);
	TRANSFORM_LISTBOX_AND_NUMPAD(1);
	TRANSFORM_LISTBOX_AND_NUMPAD(2);
	TRANSFORM_LISTBOX_AND_NUMPAD(3);
	TRANSFORM_LISTBOX_AND_NUMPAD(4);
	TRANSFORM_LISTBOX_AND_NUMPAD(5);
	TRANSFORM_LISTBOX_AND_NUMPAD(6);
	TRANSFORM_LISTBOX_AND_NUMPAD(7);
	TRANSFORM_LISTBOX_AND_NUMPAD(8);
	TRANSFORM_LISTBOX_AND_NUMPAD(9);

#undef TRANSFORM_LISTBOX_AND_NUMPAD

	if (Init(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MAINDIALOG), NULL))
	{
		ShowWindow(m_hDlg, SW_NORMAL);
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

bool UI::MessageProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		HICON hMainIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_MAINICON));
		SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hMainIcon);
		SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hMainIcon);
		auto InsertAllStrsToListBox = [this, hDlg](int listBoxID)
		{
			for (auto& item : posToKey)
			{
				SendDlgItemMessage(hDlg, listBoxID, LB_ADDSTRING, 0, (LPARAM)item.second.c_str());
			}
		};
		InsertAllStrsToListBox(IDC_LISTNUMPAD0);
		InsertAllStrsToListBox(IDC_LISTNUMPAD1);
		InsertAllStrsToListBox(IDC_LISTNUMPAD2);
		InsertAllStrsToListBox(IDC_LISTNUMPAD3);
		InsertAllStrsToListBox(IDC_LISTNUMPAD4);
		InsertAllStrsToListBox(IDC_LISTNUMPAD5);
		InsertAllStrsToListBox(IDC_LISTNUMPAD6);
		InsertAllStrsToListBox(IDC_LISTNUMPAD7);
		InsertAllStrsToListBox(IDC_LISTNUMPAD8);
		InsertAllStrsToListBox(IDC_LISTNUMPAD9);

		KeyBoardHooker::GetObj().SetHookCall
		(
			[this](WPARAM wParam, LPARAM lParam)
			{
				if (wParam == WM_KEYDOWN || wParam == WM_KEYUP)
				{
					std::thread
					(
						[this, wParam, lParam]()
						{
							int orgVk = ((LPKBDLLHOOKSTRUCT)lParam)->vkCode;
							std::lock_guard lg(orgVkCodeToNewMutex);
							if (auto itr = orgVkCodeToNew.find(orgVk); itr != orgVkCodeToNew.end())
							{
								keybd_event(static_cast<BYTE>(itr->second), 0, wParam == WM_KEYDOWN ? 0 : KEYEVENTF_KEYUP, 0);
							}
						}
					).detach();
				}
			}
		);
		break;
	}
	case WM_CLOSE:
	{
		goto quitDialog;
		break;
	}
	case WM_DESTROY:
	{
		KeyBoardHooker::GetObj().EndHook();
		PostQuitMessage(0);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
		{
		quitDialog:
			SendMessage(hDlg, WM_DESTROY, 0, 0);
			break;
		}
		case IDB_ABOUT:
		{
			MessageBox(m_hDlg, aboutInfo, TEXT("About"), MB_OK | MB_ICONINFORMATION);
			break;
		}
		case IDOK:
		{
			std::map<int, int> selPosToListBoxID;
			auto GetOneResult = [this, &selPosToListBoxID](int listBoxID)
			{
				auto pos = GetListBoxSelectPos(listBoxID);
				if (pos != -1 && pos != 0)
				{
					if (selPosToListBoxID.find(pos) != selPosToListBoxID.end())
					{
						return false;
					}
					else
					{
						selPosToListBoxID[pos] = listBoxID;
					}
				}
				return true;
			};

			if (!GetOneResult(IDC_LISTNUMPAD0)
				|| !GetOneResult(IDC_LISTNUMPAD1)
				|| !GetOneResult(IDC_LISTNUMPAD2)
				|| !GetOneResult(IDC_LISTNUMPAD3)
				|| !GetOneResult(IDC_LISTNUMPAD4)
				|| !GetOneResult(IDC_LISTNUMPAD5)
				|| !GetOneResult(IDC_LISTNUMPAD6)
				|| !GetOneResult(IDC_LISTNUMPAD7)
				|| !GetOneResult(IDC_LISTNUMPAD8)
				|| !GetOneResult(IDC_LISTNUMPAD9)
				)
			{
				MessageBox(m_hDlg, TEXT("Sorry, one key cannot substitute two or more other keys!"), TEXT("Sorry!"), MB_OK | MB_ICONEXCLAMATION);
			}
			else if (selPosToListBoxID.size() == 0)
			{
				MessageBox(m_hDlg, TEXT("Please select at least one key."), TEXT("Sorry!"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
			beginHook:
				if (!KeyBoardHooker::GetObj().RebeginHook())
				{
					std::_tstring errStr = TEXT("Fail to set hook! Error code: ");
					errStr += std::to_tstring(GetLastError());
					if (MessageBox(m_hDlg, errStr.c_str(), TEXT("Error!"), MB_RETRYCANCEL | MB_ICONERROR) == IDRETRY)
					{
						goto beginHook;
					}
				}
				else
				{
					{
						std::lock_guard lk(orgVkCodeToNewMutex);
						orgVkCodeToNew.clear();
						for (auto& item : selPosToListBoxID)
						{
							orgVkCodeToNew[posToKey[item.first].first] = listBoxIDToVkCode[item.second];
						}
					}
					if (!SetButtonText(IDOK, TEXT("Modify")))
					{
						MessageBox(m_hDlg, TEXT("Unknown error!"), TEXT("Error!"), MB_OK | MB_ICONERROR);
					}
					else
					{
						MessageBox(m_hDlg, TEXT("Success!"), TEXT("Success!"), MB_OK | MB_ICONMASK);
					}
				}
			}
			break;
		}
		default: return false;
		}
		break;
	}
	default: return false;
	}
	return true;
}