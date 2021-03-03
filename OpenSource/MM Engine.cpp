#include <Windows.h>

#include "Variables.h"
#include "Memory.hpp"
#include "Functions.hpp"

extern "C" HMODULE hGame = LoadLibrary("Game.dll");

FARPROC procGameMain = NULL;

LPSTR l_lpInfo;
HINSTANCE l_hInstance = GetModuleHandle(NULL);
HBITMAP l_hBmp;

HWND WINAPI CreateWindowExA_Proxy(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
BOOL WINAPI SetWindowTextA_Proxy(HWND hWnd, LPCSTR lpString);
HCURSOR WINAPI LoadCursorA_Proxy(HINSTANCE hInstance, LPCSTR lpCursorName);
HANDLE WINAPI LoadImageA_Proxy(HINSTANCE hInst, LPCSTR name, UINT type, int cx, int cy, UINT fuLoad);

void ShowLogo(LPCSTR l_lpMod, LPCSTR l_lpFileName);
LRESULT CALLBACK LogoWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int __cdecl SStrVPrintf_Proxy(char* dest, size_t size, const char* format, void* a...);

extern "C" {
	std::string& GetRace(UINT index);

	std::string& GetRaceName(UINT index);

	size_t GetRacesCount();

	size_t GetRacesNamesCount();

	void RaceUI();

	void RaceSounds();

	void RaceLoadingScreen();

	void RaceName();

	void RaceScoreScreen();

	void RaceOrder();

	void RaceSlot();

	void RaceStartUnits();

	void RaceBlocked();

	void RaceIncreaser();

	void RaceInit();
}

//---------------------------------------------------------------------------------------------

BOOL WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPSTR lpCmdLine, int nCmdShow)
{
	if (!hGame)
		return FALSE;

	if (!(procGameMain = GetProcAddress(hGame, "GameMain")))
		return FALSE;

	cmdline = new CCmdLine(lpCmdLine);
	engine = new CEngine();

	HMODULE hUser32 = GetModuleHandle("user32.dll");
	Exploit(hGame, hUser32, "CreateWindowExA", CreateWindowExA_Proxy);
	Exploit(hGame, hUser32, "SetWindowTextA", SetWindowTextA_Proxy);
	Exploit(hGame, hUser32, "LoadCursorA", LoadCursorA_Proxy);
	Exploit(hGame, hUser32, "LoadImageA", LoadImageA_Proxy);

	jmp(MakePtr(hGame, 0x3a2840), RaceUI);
	jmp(MakePtr(hGame, 0x31f5d0), RaceSounds);
	jmp(MakePtr(hGame, 0x5a3d84), RaceLoadingScreen);
	jmp(MakePtr(hGame, 0x58aa02), RaceName);
	jmp(MakePtr(hGame, 0x39f710), RaceScoreScreen);
	jmp(MakePtr(hGame, 0x559580), RaceOrder);
	jmp(MakePtr(hGame, 0x5bed8e), MakePtr(hGame, 0x5bedab));
	jmp(MakePtr(hGame, 0x559260), RaceSlot);
	jmp(MakePtr(hGame, 0x3a31a0), RaceStartUnits);
	jmp(MakePtr(hGame, 0x599bcc), RaceBlocked);
	fill(MakePtr(hGame, 0x5c0a1b), 0x90, 6);
	fill(MakePtr(hGame, 0x5c0a25), 0x90, 5);
	jmp(MakePtr(hGame, 0x3c11d0), RaceIncreaser);
	jmp(MakePtr(hGame, 0x3c11a0), RaceInit);
	patch(MakePtr(hGame, 0x5bf4e3), 0, 1);

	Exploit(hGame, GetModuleHandle("storm.dll"), (LPCSTR)578, SStrVPrintf_Proxy);

	engine->LoadMod(cmdline->GetValue("Mod"));

	if (engine->GetData("Enable"))
		ShowLogo((LPSTR)engine->GetData("Mod"), (LPSTR)engine->GetData("Logo"));

	reinterpret_cast<BOOL(CALLBACK*)(HMODULE)>(procGameMain)(hGame);
	FreeLibrary(hGame);

	delete engine;
	delete cmdline;

	return FALSE;
}

//---------------------------------------------------------------------------------------------

HWND WINAPI CreateWindowExA_Proxy(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	if (!_strcmpi(lpWindowName, "warcraft iii"))
		return CreateWindowEx(dwExStyle, lpClassName, (LPCSTR)engine->GetData("ModName"), dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

	return CreateWindowEx(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

BOOL WINAPI SetWindowTextA_Proxy(HWND hWnd, LPCSTR lpString)
{
	if (!_strcmpi(lpString, "warcraft iii"))
		return SetWindowText(hWnd, (LPCSTR)engine->GetData("ModName"));

	return SetWindowText(hWnd, lpString);
}

HCURSOR WINAPI LoadCursorA_Proxy(HINSTANCE hInstance, LPCSTR lpCursorName)
{
	if (!_strcmpi(lpCursorName, "blizzardcursor.cur"))
		return LoadCursor(hInstance, "MMEngineCursor.cur");

	return LoadCursor(hInstance, lpCursorName);
}

HANDLE WINAPI LoadImageA_Proxy(HINSTANCE hInst, LPCSTR name, UINT type, int cx, int cy, UINT fuLoad)
{
	if (!_strcmpi(name, "war3x.ico"))
	{
		LPCSTR icon = (LPCSTR)engine->GetData("ModIcon");

		if (!_strcmpi(icon, "MMEngine.ico"))
			return LoadImage(hInst, icon, type, cx, cy, fuLoad);
		else if (FileExists(icon))
			return LoadImage(NULL, icon, IMAGE_ICON, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		else
			return LoadImage(hInst, "MMEngine.ico", type, cx, cy, fuLoad);
	}

	return LoadImage(hInst, name, type, cx, cy, fuLoad);
}

int SStrVPrintf_Analog(char* dest, size_t size, void* a ...)
{
	return SStrVPrintf(dest, size, (LPCSTR)engine->GetData("ModVersion"), a);
}

int __cdecl SStrVPrintf_Proxy(char* dest, size_t size, const char* format, void* a ...)
{
	if (!strcmp(format, "%d.%d.%d.%d"))
		return SStrVPrintf_Analog(dest, size, a);

	return SStrVPrintf(dest, size, format, a);
}

extern "C" {
	std::string& GetRace(UINT index)
	{
		return (*(std::vector<std::string>*)engine->GetData("Skins"))[index];
	}

	std::string& GetRaceName(UINT index)
	{
		return (*(std::vector<std::string>*)engine->GetData("Keys"))[index];
	}

	size_t GetRacesCount()
	{
		return (*(std::vector<std::string>*)engine->GetData("Skins")).size();
	}

	size_t GetRacesNamesCount()
	{
		return (*(std::vector<std::string>*)engine->GetData("Keys")).size();
	}
}

void ShowLogo(LPCSTR lpMod, LPCSTR lpFileName)
{
	std::string Logo = ".\\Mods\\" + std::string(lpMod ? lpMod : "") + "\\" + (lpFileName ? lpFileName : "") + ".bmp";

	if (!lpFileName || !FileExists(Logo.c_str()))
		l_hBmp = (HBITMAP)LoadImageA(l_hInstance, "MMEngineBitmap.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	else
		l_hBmp = (HBITMAP)LoadImageA(0, Logo.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (!l_hBmp)
		return;

	l_lpInfo = (LPSTR)engine->GetData("Info");

	for (size_t i = 0; i < strlen(l_lpInfo); i++)
		if (!strncmp(&l_lpInfo[i], "\\n", 2))
		{
			l_lpInfo[i] = ' ';
			l_lpInfo[i + 1] = '\n';
		}

	WNDCLASSEX LogoClass;
	HWND hLogo;
	MSG Msg;

	LogoClass.cbSize = sizeof(LogoClass);
	LogoClass.lpszClassName = "LogoClass";
	LogoClass.lpfnWndProc = LogoWndProc;
	LogoClass.style = CS_HREDRAW | CS_VREDRAW;
	LogoClass.hInstance = l_hInstance;
	LogoClass.hIcon = LoadIconA(LogoClass.hInstance, "MMEngine.ico");
	LogoClass.hIconSm = LoadIconA(LogoClass.hInstance, "MMEngine.ico");
	LogoClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
	LogoClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	LogoClass.lpszMenuName = NULL;
	LogoClass.cbClsExtra = NULL;
	LogoClass.cbWndExtra = NULL;

	RegisterClassExA(&LogoClass);

	RECT Window;
	GetClientRect(GetDesktopWindow(), &Window);
	int LogoWidth = 500;
	int LogoHeight = 400;
	int LogoX = (Window.right - LogoWidth) / 2;
	int LogoY = (Window.bottom - LogoHeight) / 2;
	hLogo = CreateWindowExA(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, "LogoClass", "MM Engine - Mod Logo", WS_POPUP, LogoX, LogoY, LogoWidth, LogoHeight, NULL, NULL, l_hInstance, NULL);

	ShowWindow(hLogo, SW_SHOWNORMAL);
	UpdateWindow(hLogo);

	HANDLE hTimer = CreateEvent(NULL, TRUE, FALSE, NULL);
	while (WaitForSingleObject(hTimer, 1000) != WAIT_TIMEOUT && GetMessage(&Msg, hLogo, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	CloseHandle(hTimer);
	DestroyWindow(hLogo);
}

LRESULT CALLBACK LogoWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HDC hMemDC = CreateCompatibleDC(hdc);

		BITMAP bmp;
		GetObject(l_hBmp, sizeof(BITMAP), &bmp);

		SelectObject(hMemDC, l_hBmp);
		BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCCOPY);

		SetTextColor(hdc, 0xC8F0);
		SetBkMode(hdc, TRANSPARENT);

		RECT rect;
		GetClientRect(hWnd, &rect);

		size_t lines = 1;
		for (size_t i = 0; i < strlen(l_lpInfo); i++)
			if (l_lpInfo[i] == '\n')
				lines++;

		rect.top += 400 - 25 * lines + 9 * (lines - 1);
		DrawTextA(hdc, l_lpInfo, -1, &rect, DT_CENTER | DT_TOP | DT_WORDBREAK);

		DeleteDC(hMemDC);
		EndPaint(hWnd, &ps);

		break;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}