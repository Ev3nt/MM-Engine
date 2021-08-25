#include "Utils.h"
#include <fstream>

bool fileExists(std::string fileName)
{
	std::ifstream file(fileName);

	if (!file)
	{
		return false;
	}

	file.close();

	return true;
}