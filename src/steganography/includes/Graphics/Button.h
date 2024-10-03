#pragma once

struct ButtonDescriptor : public ComponentDescriptor {
	ButtonDescriptor(
		const wchar_t* text, 
		int x, 
		int y, 
		int width, 
		int height, 
		COLORREF textColor, 
		COLORREF backgroundColor, 
		void (*onClickCallback)(Button* button)
	) :
		ComponentDescriptor(),
		text(text),
		x(x),
		y(y),
		width(width),
		height(height),
		textColor(textColor),
		backgroundColor(backgroundColor),
		onClickCallback(onClickCallback)
	{};
	const wchar_t* text = L"Button";
	int x = 0;
	int y = 0;
	int width = 100;
	int height = 30;
	COLORREF textColor = RGB(255, 255, 255);
	COLORREF backgroundColor = RGB(0, 128, 255);
	void (*onClickCallback)(Button* button) = nullptr;
};

class Button : public CRTPComponent<Button> {
public:
	Button();
	~Button();
	int Init(const Window* parentWindow, ButtonDescriptor* buttonDescriptor, int windowId = MAIN_WINDOW_ID);
	void Draw(HDC compatibleDevice, RECT* rcItem);
	void Release();

	inline void Execute() { if (m_onClickCallback)m_onClickCallback(this); };

private:
	const wchar_t* m_text = L"Button";
	COLORREF m_textColor = RGB(255, 255, 255);
	COLORREF m_backgroundColor = RGB(0, 128, 255);
	bool m_isDisabled = false;
	void (*m_onClickCallback)(Button* button) = nullptr;
};