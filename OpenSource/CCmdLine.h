#pragma once

#include <Windows.h>
#include <map>

class CCmdLine {
private:
	std::map<LPCSTR, LPCSTR> m_CmdLine;
public:
	CCmdLine(LPCSTR lpCmdLine);
	~CCmdLine();

	LPCSTR GetValue(LPCSTR lpKey);
};