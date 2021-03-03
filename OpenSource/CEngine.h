#pragma once

#include <Windows.h>
#include <map>

#include "CMpqManager.h"

class CEngine {
private:
	std::map<LPCSTR, DWORD> m_Data;
	CMpqManager* mpqmanager;
public:
	CEngine();
	~CEngine();

	void LoadMod(LPCSTR lpModName);

	DWORD GetData(LPCSTR lpKey);
};