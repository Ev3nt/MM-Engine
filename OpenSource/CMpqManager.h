#pragma once

#include <vector>
#include <Storm.h>
#pragma comment(lib, "storm.lib")

#ifndef _CMpqManager_h
#define _CMpqManager_h
class CMpqManager {
	std::vector<HANDLE> m_mpqList;
public:
	CMpqManager();
	~CMpqManager();

	BOOL loadMpq(std::string mpqName, DWORD priority = 0, DWORD flags = 0);
};
#endif