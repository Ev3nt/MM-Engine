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