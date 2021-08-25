#include "CImportTable.h"

CImportTable::CImportTable(HMODULE module): m_module(module)
{
	PIMAGE_DOS_HEADER dosHeader = PIMAGE_DOS_HEADER(m_module);
	if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return;
	}

	m_ntHeaders = PIMAGE_NT_HEADERS((DWORD)m_module + dosHeader->e_lfanew);
	if (m_ntHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		return;
	}
}

BOOL CImportTable::selectApi(HMODULE apiModule)
{
	m_apiModule = apiModule;

	PIMAGE_IMPORT_DESCRIPTOR descriptor = PIMAGE_IMPORT_DESCRIPTOR((DWORD)m_module + m_ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	for (; descriptor->FirstThunk; descriptor++)
	{
		if (GetModuleHandle((LPSTR)((DWORD)m_module + descriptor->Name)) == apiModule)
		{
			m_thunk = (PIMAGE_THUNK_DATA)((DWORD)m_module + descriptor->FirstThunk);

			return TRUE;
		}
	}

	return FALSE;
}

void CImportTable::detour(std::string functionName, LPVOID function)
{
	FARPROC oldFunction = GetProcAddress(m_apiModule, functionName[0] > '0' && functionName[0] <= '9' ? (LPCSTR)std::stoi(functionName) : functionName.c_str());

	if (!m_thunk || !oldFunction)
	{
		return;
	}

	PIMAGE_THUNK_DATA thunk = m_thunk;
	for (; thunk->u1.Function; thunk++)
	{
		DWORD* address = (DWORD*)&thunk->u1.Function;
		if (*address == (DWORD)oldFunction)
		{
			DWORD dwOldProtect = NULL;

			VirtualProtect((LPVOID)address, 4, PAGE_WRITECOPY, &dwOldProtect);
			*address = (DWORD)function;
			VirtualProtect((LPVOID)address, 4, dwOldProtect, NULL);

			return;
		}
	}
}