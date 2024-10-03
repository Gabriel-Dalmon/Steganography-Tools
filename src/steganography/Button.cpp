#include "pch.h"

Button::Button()
{
}

Button::~Button()
{
}

int Button::Init(const Window* parentWindow, ButtonDescriptor* buttonDescriptor, void (*onClickCallback)(Button* button), int windowId) {
	const HWND& parentHWnd = parentWindow->GetWindowHandle();
	const HINSTANCE& parentHInstance = parentWindow->GetHInstance();
	m_onClickCallback = onClickCallback;
	m_hWnd = CreateWindow(
		L"BUTTON",
		buttonDescriptor->text,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		buttonDescriptor->x,
		buttonDescriptor->y,
		buttonDescriptor->width,
		buttonDescriptor->height,
		parentHWnd,
		(HMENU)windowId,
		parentHInstance,
		NULL
	);

	if(m_hWnd == NULL) {
		return 1;
	}

	SetInstance();
	return 0;
}