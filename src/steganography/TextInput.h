#pragma once

struct TextInputDescriptor {
	const wchar_t* defaultText;
	int x;
	int y;
	int width;
	int height;
};


class TextInput : public AbstractWindow<TextInput> {
public:
	TextInput();
	~TextInput();

	int Init(const Window* parentWindow, TextInputDescriptor* textInputDescriptor);
	wchar_t* GetText() const;

private:
};

/* // Compiles but feels weird FileInput inherits from both AbstractWindow<FileInput>
* and TextInput->AbstractWindow<TextInput>
* /
class FileInput : public TextInput, public AbstractWindow<FileInput> {
public:
	FileInput();
	~FileInput();

	int Init(const Window* parentWindow, TextInputDescriptor* textInputDescriptor);
	wchar_t* GetText() const;
private:
	Button* m_button = nullptr;
};*/