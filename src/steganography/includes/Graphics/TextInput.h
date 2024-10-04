#pragma once

struct TextInputDescriptor : public ComponentDescriptor {
	/** Looks like when the constructor is not explicitly defined, 
	* the constructor of the base class is called with all constructor data. */
	TextInputDescriptor(const wchar_t* defaultText, int x, int y, int width, int height, void(*onChangeCallback)(TextInput* textInput)) :
		ComponentDescriptor(), 
		defaultText(defaultText),
		x(x), 
		y(y), 
		width(width),
		height(height),
		onChangeCallback(onChangeCallback)
	{};
	const wchar_t* defaultText = L"FileInput";
	int x = 0;
	int y = 0;
	int width = 100;
	int height = 20;
	void(*onChangeCallback)(TextInput* textInput) = nullptr;
};


class TextInput : public CRTPComponent<TextInput> {
public:
	TextInput();
	~TextInput();

	int Init(const Window* parentWindow, TextInputDescriptor* textInputDescriptor, int windowId = MAIN_WINDOW_ID);
	void Release();
	
	char* GetText() const;
	wchar_t* GetWText() const;
	void SetText(const char* text);
	void SetText(const wchar_t* text);
	inline void OnChange() { if (m_onChangeCallback)m_onChangeCallback(this); };

private:
	void(*m_onChangeCallback)(TextInput* textInput) = nullptr;
};