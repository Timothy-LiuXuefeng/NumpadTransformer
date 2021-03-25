#pragma once

#ifndef UI_H

#define UI_H

#include "BasicModelessDialog.h"
#include <vector>
#include <utility>
#include <map>
#include <mutex>

class UI final : BasicModelessDialog
{
public:

	UI();

private:

	virtual bool MessageProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) override;
	
	std::vector<std::pair<int, std::_tstring>> posToKey;
	std::map<int, int> listBoxIDToVkCode;
	std::map<int, int> orgVkCodeToNew;
	mutable std::mutex orgVkCodeToNewMutex;

	static const LPCTSTR aboutInfo;
};

#endif // ! UI_H
