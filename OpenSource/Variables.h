#pragma once

#include <Windows.h>

#include "CCmdLine.h"
#include "CEngine.h"

extern "C" HMODULE hGame;
static HMODULE hStorm = GetModuleHandle("Storm.dll");
static CCmdLine* cmdline;
static CEngine* engine;

#define MME_MAJOR "2"
#define MME_MINOR "0"
#define MME_RELEASE "4"
#define MME_BUG_FIX "1"

#define MME_POSTFIX "(Dev Build)"

#define MME_VERSION MME_MAJOR "." MME_MINOR "." MME_RELEASE "." MME_BUG_FIX
#define MME "MM Engine - Version " MME_VERSION " " MME_POSTFIX