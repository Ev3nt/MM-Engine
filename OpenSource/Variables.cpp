#include "Variables.h"

extern "C" HMODULE gameBase = LoadLibrary("game.dll");
CEngine* engine = NULL;