#include "CEngine.h"
#include "Detours.h"
#include "Memory.h"
#include "Bitmap.h"
#include "RaceSystem.h"

struct Offsets
{
	Offsets(HMODULE gameBase): m_gameBase((DWORD)gameBase) {}
	DWORD m_gameBase;

	DWORD setGameVersion = m_gameBase + 0x58bf8d;
	DWORD setRaceUI = m_gameBase + 0x3a2840;
	DWORD setRaceSounds = m_gameBase + 0x31f5d0;
	DWORD setRaceLoadingScreen = m_gameBase + 0x5a3d84;
	DWORD setRaceName = m_gameBase + 0x58aa02;
	DWORD setRaceScoreScreen = m_gameBase + 0x39f710;
	DWORD setRaceOrder = m_gameBase + 0x559580;
	DWORD setRaceSlot = m_gameBase + 0x559260;
	DWORD setRaceStartUnits = m_gameBase + 0x3a31a0;
	DWORD setRaceBlocked = m_gameBase + 0x599bcc;
	DWORD setRaceIncreaser = m_gameBase + 0x3c11d0;
	DWORD setRaceInit = m_gameBase + 0x3c11a0;

	DWORD someOpcodeFixes1 = m_gameBase + 0x5bed8e;
	DWORD someOpcodeFixes2 = m_gameBase + 0x5bed91;
	DWORD someOpcodeFixes3 = m_gameBase + 0x5bed96;
	DWORD someOpcodeFixes4 = m_gameBase + 0x5beda8;
	DWORD someOpcodeFixes5 = m_gameBase + 0x5c0a1b;
	DWORD someOpcodeFixes6 = m_gameBase + 0x5c0a25;
	DWORD someOpcodeFixes7 = m_gameBase + 0x5bf4e3;
	DWORD someOpcodeFixes8 = m_gameBase + 0x59b3e4;
};

CEngine::CEngine(HMODULE gameBase) : m_gameBase(gameBase), m_importTable(m_gameBase), m_mpqManager()
{
	if (!m_gameBase)
	{
		throw std::string("Couldn't open game.dll.");

		return;
	}

	m_gameMain = GetProcAddress(m_gameBase, "GameMain");
	if (!m_gameMain)
	{
		throw std::string("Game.dll is corrupt.");

		return;
	}

	DWORD handle;
	DWORD size = GetFileVersionInfoSize("game.dll", &handle);

	LPSTR buffer = new char[size];
	GetFileVersionInfo("game.dll", handle, size, buffer);

	VS_FIXEDFILEINFO* verInfo;
	size = sizeof(VS_FIXEDFILEINFO);
	VerQueryValue(buffer, "\\", (LPVOID*)&verInfo, (UINT*)&size);
	delete[] buffer;

	if (((verInfo->dwFileVersionMS >> 16) & 0xffff) != 1 || ((verInfo->dwFileVersionMS >> 0) & 0xffff) != 26 ||
		((verInfo->dwFileVersionLS >> 16) & 0xffff) != 0 || ((verInfo->dwFileVersionLS >> 0) & 0xffff) != 6401)
	{
		throw std::string("Invalid version of game.dll.");

		return;
	}

	if (!m_mpqManager.loadMpq("MM Engine.mpq", 9, 0))
	{
		throw std::string("Unable to load MM Engine.mpq.");

		return;
	}

	setData("ModName", new std::string("MM Engine"));
	setData("ModIcon", new std::string("MMEngine"));
	setData("ModVersion", new std::string("MM Engine - Version 2.0.5 (OOP Build)"));
	setData("Bitmap", new std::string("MMEngineBitmap"));
	setData("Info", new std::string("© (2021) The Mod Makers"));
	setData("Enable", true);
	setData("Skins", new std::vector<std::string>{ "Human", "Orc", "Undead", "NightElf" });
	setData("Keys", new std::vector<std::string>{ "HUMAN", "ORC", "UNDEAD", "NIGHT_ELF" });
}

void CEngine::loadMod(std::string modName)
{
	if (m_importTable.selectApi(GetModuleHandle("user32.dll")))
	{
		m_importTable.detour("CreateWindowExA", CreateWindowExA_Detour);
		m_importTable.detour("SetWindowTextA", SetWindowTextA_Detour);
		m_importTable.detour("LoadImageA", LoadImageA_Detour);
		m_importTable.detour("LoadCursorA", LoadCursorA_Detour);
	}
	
	Offsets offsets(m_gameBase);
	call(offsets.setGameVersion, SStrVPrintf_Detour);

	// I forgot some of the addresses
	jmp(offsets.setRaceUI, raceUI);
	jmp(offsets.setRaceSounds, raceSounds);
	jmp(offsets.setRaceLoadingScreen, raceLoadingScreen);
	jmp(offsets.setRaceName, raceName);
	jmp(offsets.setRaceScoreScreen, raceScoreScreen);
	jmp(offsets.setRaceOrder, raceOrder);
	fill(offsets.someOpcodeFixes1, 0x90, 3);
	write(offsets.someOpcodeFixes2, 0x7364f883, 4);
	fill(offsets.someOpcodeFixes3, 0x90, 14);
	write(offsets.someOpcodeFixes4, 0x64c083, 3);
	jmp(offsets.setRaceSlot, raceSlot); // 0x560ef6 esi + 9*2 + 6		0x5c305a
	jmp(offsets.setRaceStartUnits, raceStartUnits);
	jmp(offsets.setRaceBlocked, raceBlocked);
	fill(offsets.someOpcodeFixes5, 0x90, 6);
	fill(offsets.someOpcodeFixes6, 0x90, 5);
	jmp(offsets.setRaceIncreaser, raceIncreaser);
	jmp(offsets.setRaceInit, raceInit);
	write(offsets.someOpcodeFixes7, 0, 1);
	fill(offsets.someOpcodeFixes8, 0x90, 8);

	m_modName = modName;
	showBitmap(*getData<std::string*>("Bitmap"), *getData<std::string*>("Info"));
}

BOOL CEngine::startGame()
{
	BOOL result = ((BOOL(__stdcall*)(HMODULE))(m_gameMain))(m_gameBase);
	FreeLibrary(m_gameBase);

	return result;
}

template <typename DataType>
void CEngine::setData(std::string key, DataType value)
{
	m_dataTable[key] = (DWORD)value;
}

std::string& CEngine::getModName()
{
	return m_modName;
}