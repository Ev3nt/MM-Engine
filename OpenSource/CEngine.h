#pragma once

#include <Windows.h>
#include <map>
#include <string>
#include "CImportTable.h"
#include "CMpqManager.h"
#pragma comment(lib, "version.lib")

typedef BOOL(WINAPI* _gameMain)(HMODULE);

#ifndef _CEngine_h
#define _CEngine_h
class CEngine
{
	HMODULE m_gameBase;
	_gameMain m_gameMain;

	CImportTable m_importTable;
	CMpqManager m_mpqManager;

	std::map<std::string, DWORD> m_dataTable;

	std::string m_modName;
public:
	CEngine(HMODULE gameBase);

	void loadMod(std::string modName);
	BOOL startGame();

	template <typename ReturnType>
	ReturnType getData(std::string key)
	{
		for (const auto& pair : m_dataTable)
		{
			if (pair.first == key)
			{
				return *(ReturnType*)&pair.second;
			}
		}

		return (ReturnType)NULL;
	}

	template <typename DataType>
	void setData(std::string key, DataType value);

	std::string& getModName();
	CMpqManager& getMpqManager();
};
#endif