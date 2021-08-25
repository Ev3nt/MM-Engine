#pragma once

#include <string>
#include <map>

#ifndef _CCmdLine_h
#define _CCmdLine_h
class CCmdLine {
	std::map<std::string, std::string> m_arguments;
public:
	CCmdLine(std::string cmdLine);

	std::string operator[](std::string key);
};
#endif