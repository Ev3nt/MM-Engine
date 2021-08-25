#pragma once

#include <Windows.h>

// user32.dll
//-------------------------------------------------------------------------------

HWND WINAPI CreateWindowExA_Detour(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y,
	int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);

BOOL WINAPI SetWindowTextA_Detour(HWND hWnd, LPCSTR lpString);

HCURSOR WINAPI LoadCursorA_Detour(HINSTANCE hInstance, LPCSTR lpCursorName);

HANDLE WINAPI LoadImageA_Detour(HINSTANCE hInst, LPCSTR name, UINT type, int cx, int cy, UINT fuLoad);

// storm.dll
//-------------------------------------------------------------------------------

BOOL __cdecl SStrVPrintf_Detour(LPSTR dest, size_t size, LPCSTR format, LPVOID a...);