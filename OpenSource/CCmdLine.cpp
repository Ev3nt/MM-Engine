#include "CCmdLine.h"

CCmdLine::CCmdLine(LPCSTR lpCmdLine)
{
	UINT i_key = 0;
	UINT i_value = 0;

	UINT cmdlinelenght = strlen(lpCmdLine);

	for (UINT i = 0; i < cmdlinelenght; i++)
		if (lpCmdLine[i] == '-' && lpCmdLine[i + 1] != ' ' && (i == 0 || lpCmdLine[i - 1] == ' '))
		{
			LPSTR key = NULL;
			LPSTR value = NULL;

			i_key = i + 1;
			i_value = 0;

			for (; i <= cmdlinelenght; i++)
				if (i == cmdlinelenght)
					key = _strdup(&lpCmdLine[i_key]);
				else if (lpCmdLine[i] == ' ')
				{
					UINT lenght = i - i_key + 1;
					key = (LPSTR)malloc(lenght);
					ZeroMemory(key, lenght);
					strncpy_s(key, lenght, &lpCmdLine[i_key], lenght - 1);

					break;
				}

			for (; i < cmdlinelenght && (cmdlinelenght > i + 2 ? !(lpCmdLine[i] == ' ' && lpCmdLine[i + 1] == '-' && lpCmdLine[i + 2] != ' ') : false); i++)
				if (lpCmdLine[i] != ' ')
				{
					i_value = i;

					break;
				}

			for (const auto& item : m_CmdLine)
				if (!_strcmpi(key, item.first))
				{
					delete[] m_CmdLine[item.first];
					delete[] key;
					key = (LPSTR)item.first;

					break;
				}

			if (!i_value)
			{
				m_CmdLine[key] = NULL;

				continue;
			}

			for (; i <= cmdlinelenght; i++)
				if (i == cmdlinelenght)
					value = _strdup(&lpCmdLine[i_value]);
				else if (lpCmdLine[i] == ' ')
				{
					UINT lenght = i - i_value + 1;
					value = (LPSTR)malloc(lenght);
					ZeroMemory(value, lenght);
					strncpy_s(value, lenght, &lpCmdLine[i_value], lenght - 1);

					break;
				}

			m_CmdLine[key] = value;
		}
}

CCmdLine::~CCmdLine()
{ }

LPCSTR CCmdLine::GetValue(LPCSTR lpKey)
{
	for (const auto& item : m_CmdLine)
		if (!_strcmpi(lpKey, item.first))
			return item.second;

	return "";
}