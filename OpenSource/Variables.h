#pragma once

#include <Windows.h>

#include "CCmdLine.h"
#include "CEngine.h"

extern "C" HMODULE hGame;
static HMODULE hStorm = GetModuleHandle("Storm.dll");
static CCmdLine* cmdline;
static CEngine* engine;