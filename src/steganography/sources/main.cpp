#include "pch.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	AllocConsole();
	FILE* consoleOut;
	freopen_s(&consoleOut, "CONOUT$", "w", stdout);
#endif

	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"COM Initialization failed", L"Error", MB_OK);
		return 1;
	}

	App& app = App::instance();
	app.Init(hInstance);

	app.Release();

	CoUninitialize();

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif


	return 0;
}