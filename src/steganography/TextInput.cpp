#include "pch.h"

TextInput::TextInput()
{
}

TextInput::~TextInput()
{
}

int TextInput::Init(const Window* parentWindow, TextInputDescriptor* textInputDescriptor, void(*onChangeCallback)(TextInput* textInput), int windowId)
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
		(HMENU)windowId,
		parentHInstance,
		NULL
	);

	if (m_hWnd == NULL) {
		return 1;
	}

	SetInstance();
	return 0;
}

wchar_t* TextInput::GetWText() const
{
	size_t textLength = GetWindowTextLength(m_hWnd) + 1;
	wchar_t* buffer = new wchar_t[textLength];
	GetWindowText(m_hWnd, buffer, textLength);
	return buffer;
}

char* TextInput::GetText() const
{
	size_t textLength = GetWindowTextLength(m_hWnd);
	wchar_t* wtext = new wchar_t[textLength];
	GetWindowText(m_hWnd, wtext, textLength);
	size_t outSize;
	char* text = new char[textLength];
	wcstombs_s(&outSize, text, textLength, wtext, textLength-1);
	return text;
}


void TextInput::SetText(wchar_t* text)
{
	SetWindowText(m_hWnd, text);
}

void TextInput::SetText(const char* text)
{
	size_t outSize;
	size_t textLength = std::strlen(text);
	wchar_t* wtext = new wchar_t[textLength];
	mbstowcs_s(&outSize, wtext, textLength, text, textLength-1);
	SetWindowText(m_hWnd, wtext);
}