#pragma once

struct ButtonDescriptor {
	const wchar_t* text = L"Button";
	int x = 0;
	int y = 0;
	int width = 10;
	int height = 10;
	void (*onClickCallback)(Button* button) = nullptr;
};

class Button : public AbstractWindow<Button> {
public:
	Button();
	~Button();
	int Init(const Window* parentWindow, ButtonDescriptor* buttonDescriptor, int windowId = MAIN_WINDOW_ID);

	inline void Execute() { if (m_onClickCallback)m_onClickCallback(this); };

private:
	void (*m_onClickCallback)(Button* button) = nullptr;
};