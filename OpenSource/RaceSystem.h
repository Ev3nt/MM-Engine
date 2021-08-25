#pragma once

#include <Windows.h>
#include <string>
#include "Variables.h"

extern "C" {
	std::string& getRace(UINT index)
	{
		return (*engine->getData<std::vector<std::string>*>("Skins"))[index];
	}

	std::string& getRaceName(UINT index)
	{
		return (*engine->getData<std::vector<std::string>*>("Keys"))[index];
	}

	size_t getRacesCount()
	{
		return engine->getData<std::vector<std::string>*>("Skins")->size();
	}

	size_t getRacesNamesCount()
	{
		return engine->getData<std::vector<std::string>*>("Keys")->size();
	}

	void raceUI();

	void raceSounds();

	void raceLoadingScreen();

	void raceName();

	void raceScoreScreen();

	void raceOrder();

	void raceSlot();

	void raceStartUnits();

	void raceBlocked();

	void raceIncreaser();

	void raceInit();
}