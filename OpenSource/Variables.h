#pragma once

#include "CEngine.h"

extern "C" HMODULE gameBase;
extern CEngine* engine;

#define MAJOR "2"
#define MINOR "1"
#define RELEASE "0"

#define MME_POSTFIX "(Release)"

#define MME_VERSION MAJOR "." MINOR "." RELEASE " " MME_POSTFIX
#define MME "MM Engine - Version " MME_VERSION