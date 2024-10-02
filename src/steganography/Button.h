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
	int Init(const Window* parentWindow, ButtonDescriptor* buttonDescriptor, void (*callback)());

	inline void Execute() const { if (m_callback)m_callback(); };

private:
	void (*m_callback)() = nullptr;
};