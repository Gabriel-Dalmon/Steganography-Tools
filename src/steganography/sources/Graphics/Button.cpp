#include "pch.h"

Button::Button()
{
	m_hWnd = NULL;
	m_onClickCallback = nullptr;
	m_text = L"Button";
	m_textColor = RGB(255, 255, 255);
	m_backgroundColor = RGB(0, 128, 255);
}

Button::~Button()
{
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;
	m_onClickCallback = nullptr;
}

int Button::Init(const Window* parentWindow, ButtonDescriptor* buttonDescriptor, int windowId) {
	const HWND& parentHWnd = parentWindow->GetWindowHandle();
	const HINSTANCE& parentHInstance = parentWindow->GetHInstance();
	m_text = buttonDescriptor->text;
	m_textColor = buttonDescriptor->textColor;
	m_backgroundColor = buttonDescriptor->backgroundColor;
	m_onClickCallback = buttonDescriptor->onClickCallback;
	m_hWnd = CreateWindow(
		L"BUTTON",
		m_text,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_OWNERDRAW,
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

void Button::Draw(HDC compatibleDevice, RECT* rcItem)
{
	HBRUSH hBrush = CreateSolidBrush(m_backgroundColor);
	SetBkColor(compatibleDevice, m_backgroundColor);
	FillRect(compatibleDevice, rcItem, hBrush);

	SetTextColor(compatibleDevice, m_textColor);
	DrawText(compatibleDevice, m_text, -1, rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DeleteObject(hBrush);
}

void Button::Release()
{
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;
	m_onClickCallback = nullptr;
}
