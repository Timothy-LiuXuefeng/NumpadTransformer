#include "KeyboardHooker.h"

bool KeyBoardHooker::RebeginHook()
{
	EndHook();
	m_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowKeyBoardProc, GetModuleHandle(NULL), 0);
	return static_cast<bool>(m_hHook);
}

void KeyBoardHooker::EndHook()
{
	if (m_hHook)
	{
		UnhookWindowsHookEx(m_hHook);
		m_hHook = NULL;
	}
}

LRESULT CALLBACK KeyBoardHooker::LowKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) return CallNextHookEx(KeyBoardHooker::GetObj().m_hHook, nCode, wParam, lParam);
	KeyBoardHooker::GetObj().hookCall(wParam, lParam);
	return CallNextHookEx(KeyBoardHooker::GetObj().m_hHook, nCode, wParam, lParam);
}

KeyBoardHooker& KeyBoardHooker::GetObj()
{
	static KeyBoardHooker keyBoardHooker;
	return keyBoardHooker;
}
