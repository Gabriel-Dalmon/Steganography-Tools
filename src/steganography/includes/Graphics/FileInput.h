#pragma once

struct FileInputDescriptor : public ComponentDescriptor {
	FileInputDescriptor(
		const wchar_t* defaultValue,
		int centerX,
		int centerY,
		int textInputWidth,
		int height,
		int paddingX,
		const wchar_t* fileTypeFilter,
		void (*onFileLoadedCallback)(const wchar_t* filePath)
	) : 
		ComponentDescriptor(),
		defaultValue(defaultValue),
		centerX(centerX),
		centerY(centerY),
		textInputWidth(textInputWidth),
		height(height),
		paddingX(paddingX),
		fileTypeFilter(fileTypeFilter),
		onFileLoadedCallback(onFileLoadedCallback)
	{};
	const wchar_t* defaultValue = 0;
	int centerX = 0;
	int centerY = 0;
	int textInputWidth = 0;
	int height = 0;
	int paddingX = 0;
	const wchar_t* fileTypeFilter = L"*.*"; // Should ideally be able to specify multiple file types and a list of different configs.
	void (*onFileLoadedCallback)(const wchar_t* filePath) = nullptr;
};

class FileInput : public CRTPComponent<FileInput> {
public:
	FileInput();
	~FileInput();

	int Init(Window* pParentWindow, FileInputDescriptor* descriptor);
	void Release();

	const wchar_t* GetWFilePath() const;
	const wchar_t* GetAllowedWFileExtension() const;
	void SetInputText(const wchar_t* text);

	static void OnValidateButtonClicked(Button* pButton);
	static void OnFileDialogButtonClicked(Button* pButton);
	void OnFileSelected(const wchar_t* filePath);

private:

private:
	TextInput* m_pTextInput;
	Button* m_pValidateButton;
	Button* m_pFileDialogButton;
	const wchar_t* m_allowedFileExtension;
	void (*m_onFileLoadedCallback)(const wchar_t* filePath) = nullptr;
};