#include "Bitmap.h"
#include "Variables.h"
#include "Utils.h"
#include <Windows.h>

std::string information;
HBITMAP bitmap;
HINSTANCE instance = GetModuleHandle(NULL);

LRESULT CALLBACK bitmapWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//----------------------------------------------------------------

void showBitmap(std::string& fileName, std::string& info)
{
	std::string bitmapName = ".\\Mods\\" + engine->getModName() + "\\" + fileName + ".bmp";

	if (!fileExists(bitmapName))
	{
		bitmap = (HBITMAP)LoadImageA(instance, "MMEngineBitmap.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	}
	else
	{
		bitmap = (HBITMAP)LoadImageA(NULL, bitmapName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}

	if (!bitmap)
	{
		return;
	}

	information = info;

	for (size_t i = 0; i < information.size(); i++)
	{
		if (!information.compare(i, 2, "\\n"))
		{
			information.replace(i, 2, 1, '\n');
		}
	}

	WNDCLASSEX bitmapClass;
	HWND bitmapWindow;
	MSG msg;

	bitmapClass.cbSize = sizeof(bitmapClass);
	bitmapClass.lpszClassName = "bitmapClass";
	bitmapClass.lpfnWndProc = bitmapWndProc;
	bitmapClass.style = CS_HREDRAW | CS_VREDRAW;
	bitmapClass.hInstance = instance;
	bitmapClass.hIcon = LoadIcon(bitmapClass.hInstance, "MMEngine.ico");
	bitmapClass.hIconSm = LoadIcon(bitmapClass.hInstance, "MMEngine.ico");
	bitmapClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	bitmapClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	bitmapClass.lpszMenuName = NULL;
	bitmapClass.cbClsExtra = NULL;
	bitmapClass.cbWndExtra = NULL;

	RegisterClassExA(&bitmapClass);

	RECT window;
	GetClientRect(GetDesktopWindow(), &window);
	int bitmapWidth = 500;
	int bitmapHeight = 400;
	int bitmapX = (window.right - bitmapWidth) / 2;
	int bitmapY = (window.bottom - bitmapHeight) / 2;
	bitmapWindow = CreateWindowExA(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, "bitmapClass", "MM Engine - Mod Logo", WS_POPUP, bitmapX, bitmapY, bitmapWidth, bitmapHeight, NULL, NULL, instance, NULL);

	ShowWindow(bitmapWindow, SW_SHOWNORMAL);
	UpdateWindow(bitmapWindow);

	HANDLE hTimer = CreateEvent(NULL, TRUE, FALSE, NULL);
	while (WaitForSingleObject(hTimer, 1000) != WAIT_TIMEOUT && GetMessage(&msg, bitmapWindow, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CloseHandle(hTimer);
	DestroyWindow(bitmapWindow);
}

//----------------------------------------------------------------

LRESULT CALLBACK bitmapWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HDC memDC = CreateCompatibleDC(hdc);

		BITMAP bmp;
		GetObject(bitmap, sizeof(BITMAP), &bmp);

		SelectObject(memDC, bitmap);
		BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, memDC, 0, 0, SRCCOPY);

		SetTextColor(hdc, 0xC8F0);
		SetBkMode(hdc, TRANSPARENT);

		RECT rect;
		GetClientRect(hWnd, &rect);

		size_t lines = 1;
		for (size_t i = 0; i < information.size(); i++)
		{
			if (information[i] == '\n')
			{
				lines++;
			}
		}

		rect.top += 400 - 25 * lines + 9 * (lines - 1);
		DrawTextA(hdc, information.c_str(), -1, &rect, DT_CENTER | DT_TOP | DT_WORDBREAK);

		DeleteDC(memDC);
		EndPaint(hWnd, &ps);

		break;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}