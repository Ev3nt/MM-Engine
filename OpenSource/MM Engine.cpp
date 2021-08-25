#include <Windows.h>
#include "Variables.h"
#include "CCmdLine.h"

BOOL WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPSTR lpCmdLine, int nCmdShow)
{
	try {
		engine = new CEngine(gameBase);
	}
	catch(std::string& exception)
	{
		MessageBox(NULL, exception.c_str(), "Error", MB_ICONHAND | MB_SYSTEMMODAL);
	
		return FALSE;
	}

	CCmdLine cmdline(lpCmdLine);

	engine->loadMod(cmdline["Mod"]);
	engine->startGame();
	delete engine;

	return FALSE;
}