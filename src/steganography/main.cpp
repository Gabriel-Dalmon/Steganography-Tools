#include "pch.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	App& app = App::instance();
	app.Init(hInstance);

	return 0;
}