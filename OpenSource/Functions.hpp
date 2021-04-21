#pragma once

#include <Windows.h>

inline bool FileExists(LPCSTR lpFileName)
{
	FILE* file;

	if (!fopen_s(&file, lpFileName, "r"))
	{
		fclose(file);

		return true;
	}

	return false;
}

inline LPSTR strcopy(LPCSTR str)
{
	size_t size = strlen(str) + 1;
	char* retval = new char[size];
	strcpy_s(retval, size, str);

	return retval;
}