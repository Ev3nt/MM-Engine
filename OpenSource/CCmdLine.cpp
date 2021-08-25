#include "CCmdLine.h"

CCmdLine::CCmdLine(std::string cmdLine)
{
	size_t it;
	while ((it = cmdLine.find('-')) != -1)
	{
		cmdLine = cmdLine.substr(it + 1, cmdLine.size() - it - 1);
		if (cmdLine[0] == ' ')
		{
			continue;
		}

		std::string key = cmdLine.substr(0, (it = cmdLine.find(' ')) != -1 ? it : it = cmdLine.size());
		cmdLine = it != cmdLine.size() ? cmdLine.substr(it, cmdLine.size() - it) : "";

		for (it = 0; it < cmdLine.size(); it++)
		{
			if (cmdLine[it - 1] == ' ' && cmdLine[it] == '-' && cmdLine[it + 1] != ' ')
			{
				it--;

				break;
			}
		}

		std::string value = cmdLine.substr(0, it);

		size_t i;
		for (i = 0; i < value.size() && value[i] == ' '; i++);
		value.erase(0, i);

		for (i = value.size(); i > 0 && value[i - 1] == ' '; i--);
		value.erase(i, value.size() - i);

		cmdLine = cmdLine.substr(it, cmdLine.size() - it);

		m_arguments[key] = value;
	}
}

std::string CCmdLine::operator[](std::string key)
{
	for (const auto& item : m_arguments)
	{
		if (!_strcmpi(item.first.c_str(), key.c_str()))
		{
			return item.second;
		}
	}

	return "";
}