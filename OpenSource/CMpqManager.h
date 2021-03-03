#pragma once

#include <Windows.h>
#pragma comment(lib, "Storm.lib")
#include <Storm.h>
#include <vector>

class CMpqManager
{
private:
	std::vector<HANDLE> m_Mpqs;
public:
	CMpqManager();
	~CMpqManager();

	bool OpenMpq(LPCSTR lpArchiveName, DWORD dwPriority = 0, DWORD dwFlags = 0);
};