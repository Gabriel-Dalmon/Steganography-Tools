#include "pch.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	AllocConsole();
	FILE* consoleOut;
	freopen_s(&consoleOut, "CONOUT$", "w", stdout);

	App& app = App::instance();
	app.Init(hInstance);

	app.Release();

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif


	return 0;
}