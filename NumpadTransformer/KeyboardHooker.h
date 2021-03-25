#pragma once

#ifndef KEYBOARD_HOOKER_H

#define KEYBOARD_HOOKER_H

#include <functional>
#include "Win32Basic.h"

class KeyBoardHooker
{
public:
	
	bool BeginHook();
	void EndHook();
	~KeyBoardHooker()
	{
		EndHook();
		m_hHook = NULL;
	}

	void SetHookCall(const std::function<void(WPARAM, LPARAM)>& hookCall) { this->hookCall = hookCall; }

	static KeyBoardHooker& GetObj();

private:

	HHOOK m_hHook = NULL;
	std::function<void(WPARAM, LPARAM)> hookCall = [](WPARAM, LPARAM) {};
	KeyBoardHooker() {}		// I don't know how to encapsule the static function SetWindowsHook. Just singleton pattern!
	KeyBoardHooker(const KeyBoardHooker&) = delete;
	KeyBoardHooker& operator=(const KeyBoardHooker&) = delete;

	static LRESULT CALLBACK LowKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);
};

#endif // !KEYBOARD_HOOKER_H
