#include "Detours.h"
#include "Variables.h"
#include "Utils.h"
#include <Storm.h>

// user32.dll
//-------------------------------------------------------------------------------

HWND WINAPI CreateWindowExA_Detour(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	return CreateWindowEx(dwExStyle, lpClassName, engine->getData<std::string*>("ModName")->c_str(), dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

BOOL WINAPI SetWindowTextA_Detour(HWND hWnd, LPCSTR lpString)
{
	return SetWindowText(hWnd, engine->getData<std::string*>("ModName")->c_str());
}

HCURSOR WINAPI LoadCursorA_Detour(HINSTANCE hInstance, LPCSTR lpCursorName)
{
	return LoadCursor(hInstance, "MMEngineCursor.cur");
}

HANDLE WINAPI LoadImageA_Detour(HINSTANCE hInst, LPCSTR name, UINT type, int cx, int cy, UINT fuLoad)
{
	std::string iconName = engine->getData<std::string*>("ModIcon")->append(".ico");
	std::string iconPath = ".\\Mods\\" + engine->getModName() + "\\" + *(engine->getData<std::string*>("ModIcon")) + ".ico";

	if (!fileExists(iconPath))
	{
		return LoadImage(hInst, iconName.c_str(), type, cx, cy, fuLoad);
	}
	else
	{
		return LoadImage(NULL, iconPath.c_str(), IMAGE_ICON, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTSIZE);
	}
}

// storm.dll
//-------------------------------------------------------------------------------

BOOL __cdecl SStrVPrintf_Detour(LPSTR dest, size_t size, LPCSTR format, LPVOID a...)
{
	return SStrVPrintf(dest, size, engine->getData<std::string*>("ModVersion")->c_str(), a);
}