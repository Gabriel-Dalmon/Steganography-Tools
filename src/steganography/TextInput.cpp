#include "pch.h"

TextInput::TextInput()
{
}

TextInput::~TextInput()
{
}

int TextInput::Init(const Window* parentWindow, TextInputDescriptor* textInputDescriptor, void(*onChangeCallback)())
{
	m_onChangeCallback = onChangeCallback;
	const HWND& parentHWnd = parentWindow->GetWindowHandle();
	const HINSTANCE& parentHInstance = parentWindow->GetHInstance();
	m_hWnd = CreateWindow(
		L"EDIT",
		textInputDescriptor->defaultText,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL,
		textInputDescriptor->x,
		textInputDescriptor->y,
		textInputDescriptor->width,
		textInputDescriptor->height,
		parentHWnd,
		(HMENU)0,
		parentHInstance,
		NULL
	);

	if (m_hWnd == NULL) {
		return 1;
	}

	SetInstance();
	return 0;
}

wchar_t* TextInput::GetText() const
{
	wchar_t* buffer = new wchar_t[256];
	GetWindowText(m_hWnd, buffer, 256);
	return buffer;
}
