#pragma once

#include <Windows.h>

#define MakePtr(Address, Offset) (DWORD)((DWORD)Address + (DWORD)Offset)

template<typename R, typename F, typename ...A>
inline R stdcall(F f, A ...a)
{
	return reinterpret_cast<R(CALLBACK*)(A...)>(f)(a...);
}

inline void Exploit(HMODULE lib, HMODULE api, LPCSTR name, LPVOID proc)
{
	if (!lib || !name || !proc)
		return;

	DWORD oldProc = (DWORD)GetProcAddress(api, name);
	if (!oldProc)
		return;

	if (PIMAGE_DOS_HEADER(lib)->e_magic != IMAGE_DOS_SIGNATURE)
		return;

	PIMAGE_NT_HEADERS headers = PIMAGE_NT_HEADERS(MakePtr(lib, PIMAGE_DOS_HEADER(lib)->e_lfanew));
	if (headers->Signature != IMAGE_NT_SIGNATURE)
		return;

	PIMAGE_IMPORT_DESCRIPTOR desc = (PIMAGE_IMPORT_DESCRIPTOR)(MakePtr(lib, headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));
	for (; desc->FirstThunk; desc++)
		if (GetModuleHandle((LPSTR)(MakePtr(lib, desc->Name))) == api)
			break;

	PIMAGE_THUNK_DATA thunk = (PIMAGE_THUNK_DATA)(MakePtr(lib, desc->FirstThunk));
	if (!thunk)
		return;

	for (; thunk->u1.Function; thunk++)
	{
		DWORD* address = (DWORD*)&thunk->u1.Function;
		if (*address == oldProc)
		{
			DWORD dwOldProtect = NULL;

			VirtualProtect((LPVOID)address, 4, PAGE_WRITECOPY, &dwOldProtect);
			*address = (DWORD)proc;
			VirtualProtect((LPVOID)address, 4, dwOldProtect, NULL);

			return;
		}
	}
}

inline bool patch(DWORD address, DWORD data, size_t size)
{
	if (!address || !size)
		return false;

	DWORD dwOldProtect;
	if (VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &dwOldProtect))
	{
		memcpy((LPVOID)address, (LPVOID)&data, size);
		VirtualProtect((LPVOID)address, size, dwOldProtect, nullptr);

		return true;
	}

	return false;
}

inline bool fill(DWORD address, DWORD dwBytes, SIZE_T nSize)
{
	bool retval = true;

	for (UINT i = 0; i < nSize; i++)
		if (retval)
			retval = patch(address + i, dwBytes, 1);
		else
			return false;

	return true;
}

template <typename A>
inline bool call(DWORD address, A proc)
{
	return patch(address, 0xe8, 1) && patch(address + 1, (DWORD)proc - (address + 5), 4);
}

template <typename A>
inline bool jmp(DWORD address, A proc)
{
	return patch(address, 0xe9, 1) && patch(address + 1, (DWORD)proc - (address + 5), 4);
}