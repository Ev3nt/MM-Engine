#pragma once

#include <Windows.h>

template <typename Address>
inline void write(Address destination, DWORD data, size_t size)
{
	DWORD oldProtect;

	if (VirtualProtect((LPVOID)destination, size, PAGE_EXECUTE_READWRITE, &oldProtect))
	{
		CopyMemory((LPVOID)destination, &data, size);
		VirtualProtect((LPVOID)destination, size, oldProtect, NULL);
	}
}

template <typename Address>
inline void fill(Address destination, BYTE data, size_t size)
{
	DWORD oldProtect;

	if (VirtualProtect((LPVOID)destination, size, PAGE_EXECUTE_READWRITE, &oldProtect))
	{
		for (size_t i = 0; i < size; i++)
		{
			*(BYTE*)((DWORD)destination + i) = data;
		}

		VirtualProtect((LPVOID)destination, size, oldProtect, NULL);
	}
}

template <typename Address, typename Function>
inline void call(Address destination, Function function)
{
	DWORD oldProtect;

	if (VirtualProtect((LPVOID)destination, 5, PAGE_EXECUTE_READWRITE, &oldProtect))
	{
		*(BYTE*)destination = 0xe8;
		*(DWORD*)((DWORD)destination + 1) = (DWORD)function - (DWORD)destination - 5;
		VirtualProtect((LPVOID)destination, 5, oldProtect, NULL);
	}
}

template <typename Address, typename Function>
inline void jmp(Address destination, Function function)
{
	DWORD oldProtect;

	if (VirtualProtect((LPVOID)destination, 5, PAGE_EXECUTE_READWRITE, &oldProtect))
	{
		*(BYTE*)destination = 0xe9;
		*(DWORD*)((DWORD)destination + 1) = (DWORD)function - (DWORD)destination - 5;
		VirtualProtect((LPVOID)destination, 5, oldProtect, NULL);
	}
}