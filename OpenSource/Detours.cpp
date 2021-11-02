#include "Detours.h"
#include "Variables.h"
#include "Utils.h"
#include <Storm.h>
#pragma comment(lib, "wsock32.lib")

HWND gameWindow = NULL;

// user32.dll
//-------------------------------------------------------------------------------

HWND WINAPI CreateWindowExA_Detour(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	return gameWindow = CreateWindowEx(dwExStyle, lpClassName, engine->getData<std::string*>("ModName")->c_str(), dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
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
	std::string iconPath = ".\\Mods\\" + engine->getModName() + "\\" + *(engine->getData<std::string*>("ModIcon")) + ".ico";

	if (!fileExists(iconPath))
	{
		return LoadImage(hInst, "MMEngine.ico", type, cx, cy, fuLoad);
	}
	else
	{
		return LoadImage(NULL, iconPath.c_str(), IMAGE_ICON, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTSIZE);
	}
}

// kernel32.dll
//-------------------------------------------------------------------------------

HMODULE WINAPI LoadLibraryA_Detour(LPCSTR lpLibFileName)
{
	LPCSTR lpFileName = &lpLibFileName[strlen(lpLibFileName) - 4];

	char file1[] = "redist\\miles\\Mp3dec.asi";
	char file2[] = "redist\\miles\\Mssdolby.m3d";
	char file3[] = "redist\\miles\\Msseax2.m3d";
	char file4[] = "redist\\miles\\Mssfast.m3d";
	char file5[] = "redist\\miles\\Reverb3.flt";

	return (!(!_strcmpi(&lpFileName[strlen(lpFileName) - strlen(file1)], file1) ||
		!_strcmpi(&lpFileName[strlen(lpFileName) - strlen(file2)], file2) ||
		!_strcmpi(&lpFileName[strlen(lpFileName) - strlen(file3)], file3) ||
		!_strcmpi(&lpFileName[strlen(lpFileName) - strlen(file4)], file4) ||
		!_strcmpi(&lpFileName[strlen(lpFileName) - strlen(file5)], file5))) ? 
		(!_strcmpi(lpFileName, ".mix") || !_strcmpi(lpFileName, ".asi") || !_strcmpi(lpFileName, ".flt") ||
		!_strcmpi(lpFileName, ".m3d")) ? NULL : LoadLibrary(lpLibFileName) : LoadLibrary(lpLibFileName);
}

HANDLE WINAPI CreateEventA_Detour(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName)
{
	HANDLE retval = CreateEvent(lpEventAttributes, bManualReset, bInitialState, lpName);

	if (lpName && !strcmp(lpName, "Warcraft III Game Application"))
	{
		SetLastError(NULL);
	}

	return retval;
}

// wsock32.dll
//-------------------------------------------------------------------------------

WORD tcp_port;
DWORD mmeID = 'M' + ('M' << 8) + ('E' << 16);

BOOL WINAPI bind_Detour(SOCKET s, const sockaddr* name, int namelen)
{
	int optVal;
	int optLen = sizeof(int);

	if (getsockopt(s, SOL_SOCKET, SO_TYPE, (LPSTR)&optVal, &optLen) != SOCKET_ERROR)
	{
		WORD& socketPort = ((struct sockaddr_in*)name)->sin_port;
		if (optVal == SOCK_DGRAM && socketPort == 0xE017)
		{
			BOOL optval = TRUE;
			setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (LPCSTR)&optval, sizeof(optval));
			
			return bind(s, name, namelen);
		}
		else if (optVal == SOCK_STREAM)
		{
			BOOL optval = FALSE;
			setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));
			
			WORD port = ntohs(socketPort);

			while (true)
			{
				if (bind(s, name, namelen) != SOCKET_ERROR)
				{
					tcp_port = port;

					return FALSE;
				}

				socketPort = htons(++port);
			}
		}
	}

	return bind(s, name, namelen);
}

#define W3GS_HEADER_CONSTANT 247
#define W3GS_GAMEINFO 48
#define W3GS_SEARCHGAME 47

BOOL WINAPI sendto_Detour(SOCKET s, const char* buf, int len, int flags, const sockaddr* to, int tolen)
{
	struct war3Packet
	{
		BYTE war3HeaderConstant;
		BYTE protocol;
		WORD size;
	};

	war3Packet* data = (war3Packet*)buf;
	if (data->war3HeaderConstant == W3GS_HEADER_CONSTANT && len >= sizeof(war3Packet))
	{
		switch (data->protocol)
		{
		case W3GS_GAMEINFO:
		{
			*(WORD*)(&buf[data->size - 2]) = tcp_port;
			sockaddr_in addr = *(sockaddr_in*)to;
			addr.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");

			return sendto(s, (LPCSTR)buf, len, flags, (const sockaddr*)&addr, sizeof(sockaddr_in));
		}
		case W3GS_SEARCHGAME:
		{
			CopyMemory((LPVOID)&buf[sizeof(war3Packet)], (LPVOID)&mmeID, 4);
		}
		default:
			break;
		}
	}

	return sendto(s, buf, len, flags, to, tolen);
}

// storm.dll
//-------------------------------------------------------------------------------

BOOL __cdecl SStrVPrintf_Detour(LPSTR dest, size_t size, LPCSTR format, LPVOID a...)
{
	return SStrVPrintf(dest, size, engine->getData<std::string*>("ModVersion")->c_str(), a);
}

std::string mapName;

BOOL WINAPI StormOpenArchive_Detour(LPCSTR lpArchiveName, DWORD dwPriority, DWORD dwFlags, HANDLE* hMPQ)
{
	mapName = lpArchiveName;

	return SFileOpenArchive(lpArchiveName, dwPriority, dwFlags, hMPQ);
}

BOOL WINAPI StormOpenFileAsArchive_Detour(DWORD a, LPCSTR lpArchiveName, DWORD dwPriority, DWORD dwFlags, HANDLE* hMPQ)
{
	mapName = lpArchiveName;

	return SFileOpenFileAsArchive(a, lpArchiveName, dwPriority, dwFlags, hMPQ);
}

BOOL WINAPI StormLoadFile_Detour(LPCSTR lpFileName, LPVOID lpBuffer, size_t* pSize, size_t extraSizeToAlocate, LPOVERLAPPED lpOverlapped)
{
	if (!_strnicmp(lpFileName, "ui\\widgets\\glues\\icon-map", 25))
	{
		HANDLE mpq;

		SFileOpenArchive(mapName.c_str(), 0, 0, &mpq);
		BOOL retval = SFileLoadFileEx(mpq, "war3mapIcon.blp", lpBuffer, pSize, extraSizeToAlocate, 0, lpOverlapped);

		SFileCloseArchive(mpq);

		if (retval)
		{
			return retval;
		}
	}

	return SFileLoadFile(lpFileName, lpBuffer, pSize, extraSizeToAlocate, lpOverlapped);
}

HMODULE stormBase = GetModuleHandle("storm.dll");

LONG WINAPI StormGetFileSize_Detour(HANDLE hFile, LPDWORD lpFileSizeHigh)
{
	LONG retval = SFileGetFileSize(hFile, lpFileSizeHigh);

	if (retval)
	{
		char fileName[MAX_PATH] = { 0 };
		SFileGetFileName(hFile, fileName, sizeof(fileName));
		if (!_strcmpi(&fileName[strlen(fileName) - 4], ".w3x") || !_strcmpi(&fileName[strlen(fileName) - 4], ".w3m"))
		{
			return TRUE;
		}
	}

	return retval;
}

BOOL WINAPI TextureExistsChecking(DWORD dwFlag)
{
	LPCSTR lpFileName;
	DWORD retval;

	_asm mov lpFileName, esi;

	retval = reinterpret_cast<BOOL(WINAPI*)(DWORD)>((DWORD)gameBase + 0x4e1410)(dwFlag); // Calling original function

	return !_strnicmp(lpFileName, "ui\\widgets\\glues\\icon-map", 25) ? 0 : retval;
}

// game.dll
//-------------------------------------------------------------------------------

float wideScreenMul = 1.0f;

void __fastcall CreateMatrixPerspectiveFov_Detour(UINT outMatrix, UINT unused, float fovY, float aspectRatio, float nearZ, float farZ)
{
	RECT rect;
	if (gameWindow && GetWindowRect(gameWindow, &rect))
	{
		float width = float(rect.right - rect.left);
		float height = 1.0f / (rect.bottom - rect.top);
		wideScreenMul = width * height * 0.75f;
	}

	float yScale = 1.0f / tan(fovY * 0.5f / sqrt(aspectRatio * aspectRatio + 1.0f));
	float xScale = yScale / (aspectRatio * wideScreenMul);

	*(float*)(outMatrix) = xScale;
	*(float*)(outMatrix + 16) = 0.0f;
	*(float*)(outMatrix + 32) = 0.0f;
	*(float*)(outMatrix + 48) = 0.0f;

	*(float*)(outMatrix + 4) = 0.0f;
	*(float*)(outMatrix + 20) = yScale;
	*(float*)(outMatrix + 36) = 0.0f;
	*(float*)(outMatrix + 52) = 0.0f;

	*(float*)(outMatrix + 8) = 0.0f;
	*(float*)(outMatrix + 24) = 0.0f;
	*(float*)(outMatrix + 40) = (nearZ + farZ) / (farZ - nearZ);
	*(float*)(outMatrix + 56) = (-2.0f * nearZ * farZ) / (farZ - nearZ);

	*(float*)(outMatrix + 12) = 0.0f;
	*(float*)(outMatrix + 28) = 0.0f;
	*(float*)(outMatrix + 44) = 1.0f;
	*(float*)(outMatrix + 60) = 0.0f;
}

void __fastcall BuildHPBars_Detour(UINT a1, UINT unused, UINT a2, UINT a3)
{
	reinterpret_cast<void(__fastcall*)(UINT, UINT, UINT, UINT)>((DWORD)gameBase + 0x379a30)(a1, unused, a2, a3);

	UINT pHPBarFrame = *((UINT*)a1 + 3);
	if (pHPBarFrame)
	{
		*((float*)pHPBarFrame + 22) /= wideScreenMul;
	}
}

DWORD WINAPI setWarcraftID()
{
	return mmeID;
}