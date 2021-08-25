#pragma once

#include <Windows.h>
#include <string>

#ifndef _CImportTable_h
#define _CImportTable_h
class CImportTable {
	HMODULE m_module;
	HMODULE m_apiModule;
	PIMAGE_NT_HEADERS m_ntHeaders;
	PIMAGE_THUNK_DATA m_thunk;
public:
	CImportTable(HMODULE module);

	BOOL selectApi(HMODULE apiModule);
	void detour(std::string functionName, LPVOID function);
};
#endif