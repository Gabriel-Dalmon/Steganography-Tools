#include "pch.h"

FileInput::FileInput()
{
	m_pFileDialogButton = nullptr;
	m_pValidateButton = nullptr;
	m_pTextInput = nullptr;
}

FileInput::~FileInput()
{
}

int FileInput::Init(Window* pParentWindow, FileInputDescriptor* pDescriptor)
{
	m_allowedFileExtension = pDescriptor->fileTypeFilter;
	m_onFileLoadedCallback = pDescriptor->onFileLoadedCallback;

	const int& halfTextInputWidth = pDescriptor->textInputWidth / 2;
	const int& buttonWidth = pDescriptor->height;
	const int& height = pDescriptor->height;
	const int& y = pDescriptor->centerY - pDescriptor->height / 2;
	TextInputDescriptor textInputDescriptor {
		pDescriptor->defaultValue,
		pDescriptor->centerX - halfTextInputWidth,
		y,
		pDescriptor->textInputWidth,
		height,
		nullptr
	};
	m_pTextInput = pParentWindow->CreateComponent<TextInput>(&textInputDescriptor, false);

	ButtonDescriptor validateButtonDescriptor {
		L"->",
		pDescriptor->centerX + halfTextInputWidth + pDescriptor->paddingX,
		y,
		buttonWidth,
		height,
		RGB(0, 0, 0),
		RGB(235, 235, 235),
		&FileInput::OnValidateButtonClicked
	};
	m_pValidateButton = pParentWindow->CreateComponent<Button>(&validateButtonDescriptor, false);

	ButtonDescriptor fileDialogButtonDescriptor{
	L"Q",
	pDescriptor->centerX - halfTextInputWidth - pDescriptor->paddingX - buttonWidth,
	y,
	buttonWidth,
	height,
	RGB(0, 0, 0),
	RGB(235, 235, 235),
	&FileInput::OnFileDialogButtonClicked
	};
	m_pFileDialogButton = pParentWindow->CreateComponent<Button>(&fileDialogButtonDescriptor, false);

	return 0;
}

void FileInput::Release()
{
	if (m_pTextInput) {
		m_pTextInput->Release();
		delete m_pTextInput;
	}
	if (m_pValidateButton) {
		m_pValidateButton->Release();
		delete m_pValidateButton;
	}
	if (m_pFileDialogButton) {
		m_pFileDialogButton->Release();
		delete m_pFileDialogButton;
	}
	m_pTextInput = nullptr;
	m_pValidateButton = nullptr;
	m_pFileDialogButton = nullptr;
	m_onFileLoadedCallback = nullptr;
}

const wchar_t* FileInput::GetWFilePath() const
{
	return m_pTextInput->GetWText();
}

const wchar_t* FileInput::GetAllowedWFileExtension() const
{
	return m_allowedFileExtension;
}

void FileInput::SetInputText(const wchar_t* text)
{
	m_pTextInput->SetText(text);
}

void FileInput::OnValidateButtonClicked(Button* pButton)
{
	App& app = App::instance();
	
	// FileInput should be retrieved differently as it could be stored elsewhere in different apps. Ideally it should be passed as a parameter.
	// Or the parent window should be stored in the sub windows to be able to retrieve the FileInput from the Button.
	FileInput* pFileInput = app.m_pFilePathInput;
	const wchar_t* filePath = pFileInput->GetWFilePath();
	if (!std::filesystem::exists(filePath) || std::filesystem::path(filePath).extension() != pFileInput->GetAllowedWFileExtension()) {
		Window& window = *app.m_pMainWindow;
		MessageBox(window.GetWindowHandle(), L"Invalid file path. No .bmp found.", 0, MB_OK);
		delete filePath;
		return;
	}
	pFileInput->OnFileSelected(filePath);
}

// Ideally FileDialog logic should be in a separate class.
void FileInput::OnFileDialogButtonClicked(Button* pButton)
{
	App& app = App::instance();
	FileInput* pFileInput = app.m_pFilePathInput;
	IFileDialog* pFileDialog = NULL;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));

	if (!SUCCEEDED(hr))
	{
		MessageBox(NULL, L"Failed to create FileDialog", L"Error", MB_OK);
		return;
	}
	const wchar_t* extensionLeftPart = L"*";
	const wchar_t* extensionRightPart = pFileInput->GetAllowedWFileExtension();
	const wchar_t* filterTitle = L"Authorized files ";
	size_t leftPartLength = std::wcslen(extensionLeftPart);
	size_t rightPartLength = std::wcslen(extensionRightPart);
	const wchar_t* extension = new wchar_t[leftPartLength + rightPartLength + 1];
	wcscpy_s((wchar_t*)extension, leftPartLength + rightPartLength + 1, extensionLeftPart);
	wcscat_s((wchar_t*)extension, leftPartLength + rightPartLength + 1, extensionRightPart);

	size_t filterTitleLength = std::wcslen(filterTitle);
	const wchar_t* filter = new wchar_t[filterTitleLength + leftPartLength + rightPartLength + 1];
	wcscpy_s((wchar_t*)filter, filterTitleLength + leftPartLength + rightPartLength + 1, filterTitle);
	wcscat_s((wchar_t*)filter, filterTitleLength + leftPartLength + rightPartLength + 1, extension);
	
	COMDLG_FILTERSPEC fileTypes[] = {
		{ filter, extension } // { L"Authorized files *.bmp", "*.bmp" }
	};
	pFileDialog->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
	delete[] extension;
	delete[] filter;

	hr = pFileDialog->Show(NULL);
	if (!SUCCEEDED(hr)) {
		if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED)) return;
		MessageBox(NULL, L"Failed to show FileDialog", L"Error", MB_OK);
		return;
	}
	
	IShellItem* pItem;
	hr = pFileDialog->GetResult(&pItem);
	if (!SUCCEEDED(hr))
	{
		MessageBox(NULL, L"Failed to get result from FileDialog", L"Error", MB_OK);
		return;
	}
	
	LPWSTR coFilePath;
	hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &coFilePath);
	if (!SUCCEEDED(hr))
	{
		MessageBox(NULL, L"Failed to get file path from FileDialog", L"Error", MB_OK);
		CoTaskMemFree(coFilePath);
		pItem->Release();
		return;
	}

	// We want to be able to call a simple delete[] outside of this scope 
	//since we won't know coFilePath was created from COM objects.
	const wchar_t* filePath = _wcsdup(coFilePath); 
	CoTaskMemFree(coFilePath);
	pItem->Release();
	pFileDialog->Release();
	pFileInput->SetInputText(filePath);
	pFileInput->OnFileSelected(filePath);
}

void FileInput::OnFileSelected(const wchar_t* filePath)
{
	if (m_onFileLoadedCallback)m_onFileLoadedCallback(filePath);
}
