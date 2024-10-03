#include "pch.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	AllocConsole();
	FILE* consoleOut;
	freopen_s(&consoleOut, "CONOUT$", "w", stdout);
#endif

	App& app = App::instance();
	app.Init(hInstance);

	app.Release();

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif


	return 0;
}