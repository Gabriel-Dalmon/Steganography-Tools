#pragma once

struct ButtonDescriptor {
	const wchar_t* text;
	int x;
	int y;
	int width;
	int height;
};

class Button : public AbstractWindow<Button> {
public:
	Button();
	~Button();
	int Init(const Window* parentWindow, ButtonDescriptor* buttonDescriptor, void (*callback)(Button* button), int windowId = MAIN_WINDOW_ID);

	inline void Execute() { if (m_onClickCallback)m_onClickCallback(this); };

private:
	void (*m_onClickCallback)(Button* button) = nullptr;
};