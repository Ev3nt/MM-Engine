#pragma once

#include <Windows.h>

// user32.dll
//-------------------------------------------------------------------------------

HWND WINAPI CreateWindowExA_Detour(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y,
	int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);

BOOL WINAPI SetWindowTextA_Detour(HWND hWnd, LPCSTR lpString);

HCURSOR WINAPI LoadCursorA_Detour(HINSTANCE hInstance, LPCSTR lpCursorName);

HANDLE WINAPI LoadImageA_Detour(HINSTANCE hInst, LPCSTR name, UINT type, int cx, int cy, UINT fuLoad);

// kernel32.dll
//-------------------------------------------------------------------------------

HMODULE WINAPI LoadLibraryA_Detour(LPCSTR lpLibFileName);

HANDLE WINAPI CreateEventA_Detour(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName);

// wsock32.dll
//-------------------------------------------------------------------------------

BOOL WINAPI bind_Detour(SOCKET s, const sockaddr* name, int namelen);

BOOL WINAPI sendto_Detour(SOCKET s, const char* buf, int len, int flags, const sockaddr* to, int tolen);

// storm.dll
//-------------------------------------------------------------------------------

BOOL __cdecl SStrVPrintf_Detour(LPSTR dest, size_t size, LPCSTR format, LPVOID a...);

BOOL WINAPI StormOpenArchive_Detour(LPCSTR lpArchiveName, DWORD dwPriority, DWORD dwFlags, HANDLE* hMPQ);

BOOL WINAPI StormOpenFileAsArchive_Detour(DWORD a, LPCSTR lpArchiveName, DWORD dwPriority, DWORD dwFlags, HANDLE* hMPQ);

BOOL WINAPI StormLoadFile_Detour(LPCSTR lpFileName, LPVOID lpBuffer, size_t* pSize, size_t extraSizeToAlocate, LPOVERLAPPED lpOverlapped);

LONG WINAPI StormGetFileSize_Detour(HANDLE hFile, LPDWORD lpFileSizeHigh);

BOOL WINAPI TextureExistsChecking(DWORD dwFlag);

// game.dll
//-------------------------------------------------------------------------------

void __fastcall CreateMatrixPerspectiveFov_Detour(UINT outMatrix, UINT unused, float fovY, float aspectRatio, float nearZ, float farZ);

void __fastcall BuildHPBars_Detour(UINT a1, UINT unused, UINT a2, UINT a3);

DWORD WINAPI setWarcraftID();