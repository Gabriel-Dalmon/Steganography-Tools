#include "pch.h"

Button::Button()
{
}

Button::~Button()
{
}

int Button::Init(const Window* parentWindow, ButtonDescriptor* buttonDescriptor, void (*callback)()) {
	const HWND& parentHWnd = parentWindow->GetWindowHandle();
	const HINSTANCE& parentHInstance = parentWindow->GetHInstance();
	m_callback = callback;
	m_hWnd = CreateWindow(
		L"BUTTON",
		buttonDescriptor->text,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		buttonDescriptor->x,
		buttonDescriptor->y,
		buttonDescriptor->width,
		buttonDescriptor->height,
		parentHWnd,
		(HMENU)0,
		parentHInstance,
		NULL
	);

	if(m_hWnd == NULL) {
		return 1;
	}

	SetInstance();
	return 0;
}

/*inline Button* Button::GetButtonInstance(HWND hwnd)
{
	return reinterpret_cast<Button*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
}*/
