#include "pch.h"

//------------------------------------------------------------------------------
App* App::instance_ = nullptr;

//------------------------------------------------------------------------------
App::App() {
	m_pMainWindow = nullptr;
	m_pOriginalBitmap = nullptr;
	m_pOutputBitmap = nullptr;
	m_pOriginalImage = nullptr;
	m_pOutputImage = nullptr;
	m_pFilePathTextInput = nullptr;
	m_pEncryptionTextInput = nullptr;
}

//------------------------------------------------------------------------------
/** Singleton implementation to easily access the App from callbacks and 
* WndProcs. */
App& App::instance()
{
    if (!instance_) {
        instance_ = new App();
    };
    return *instance_;
}

//------------------------------------------------------------------------------
int App::Init(HINSTANCE hInstance) {
	WindowClassDescriptor windowClassDescriptor;
	WindowDescriptor windowDescriptor = { L"Steganography Tools", 1200, 800, RGB(240,240,255), & App::OnWindowCreateCallback};

	m_pMainWindow = new Window();
	m_pMainWindow->Init(hInstance, &windowClassDescriptor);
	m_pMainWindow->Create(&windowDescriptor);
	m_pMainWindow->Run();
	return 0;
}

//------------------------------------------------------------------------------
void App::Release() {
	//--------------------------------------------------------------------------
	if (m_pOriginalBitmap != nullptr) {
		m_pOriginalBitmap->Release();
		delete m_pOriginalBitmap;
	}
	if (m_pOutputBitmap != nullptr) {
		m_pOutputBitmap->Release();
		delete m_pOutputBitmap;
	}
	//--------------------------------------------------------------------------
	m_pMainWindow->Release();
	delete m_pMainWindow;
	m_pMainWindow = nullptr;

	//--------------------------------------------------------------------------
	/** These window components are destroyed by the window Release(). */
	m_pEncryptionTextInput = nullptr;
	m_pFilePathTextInput = nullptr;

	//--------------------------------------------------------------------------
	/** Delete the singleton instance. */
	delete instance_;
	instance_ = nullptr;
}

//------------------------------------------------------------------------------
void App::OnWindowCreateCallback(Window* pWindow)
{
	//--------------------------------------------------------------------------
	App& app = App::instance();
	int windowWidth = pWindow->GetWindowWidth();
	int windowHeight = pWindow->GetWindowHeight();
	int windowCenterX = windowWidth / 2;
	int windowCenterY = windowHeight / 2;
	int buttonWidth = 200;
	int buttonHeight = 50;
	int paddingBottom = windowHeight / 4;
	int buttonY = windowHeight - buttonHeight - paddingBottom;
	int imageWidth = 400;
	int imageHeight = 400;
	int imageY = buttonY - imageHeight - 50;
	int centerPaddingHalf = 50;

	//--------------------------------------------------------------------------
	ButtonDescriptor encryptButtonDescriptor { 
		L"Encrypt", 
		windowCenterX - (buttonWidth + centerPaddingHalf), 
		buttonY, 
		buttonWidth, 
		buttonHeight, 
		RGB(255, 255, 255),
		RGB(0, 128, 255),
		&App::Encrypt
	};
	ButtonDescriptor decryptButtonDescriptor { 
		L"Decrypt", 
		windowCenterX + centerPaddingHalf, 
		buttonY, 
		buttonWidth, 
		buttonHeight,
		RGB(255, 255, 255),
		RGB(0, 128, 255),
		&App::Decrypt
	};
	ButtonDescriptor saveButtonDescriptor{
		L"Save",
		windowCenterX + centerPaddingHalf + buttonWidth + centerPaddingHalf * 2,
		buttonY,
		buttonWidth,
		buttonHeight,
		RGB(255, 255, 255),
		RGB(0, 128, 255),
		&App::SaveOutput
	};
	pWindow->CreateComponent<Button>(&encryptButtonDescriptor);
	pWindow->CreateComponent<Button>(&decryptButtonDescriptor);
	pWindow->CreateComponent<Button>(&saveButtonDescriptor);

	//--------------------------------------------------------------------------
	TextInputDescriptor textInputDescriptor { L"Enter the file path here.", 10, 10, 300, 25, &App::LoadFile };
	TextInputDescriptor encryptTextInputDescriptor { L"Enter encryption data here.", 10, 50, 300, 25, nullptr };
	
	app.m_pFilePathTextInput = pWindow->CreateComponent<TextInput>(&textInputDescriptor);
	app.m_pEncryptionTextInput = pWindow->CreateComponent<TextInput>(&encryptTextInputDescriptor);
}

//------------------------------------------------------------------------------
void App::Encrypt(Button* pButtonClicked)
{
	App& app = App::instance();
	if(app.m_pOriginalBitmap == nullptr) {
		MessageBox(0, L"Please load a file first.", 0, 0);
		return;
	}
	if (app.m_pOutputBitmap == nullptr) {
		app.m_pOutputBitmap = new Bitmap();
	}
	else {
		app.m_pOutputBitmap->Release();
		delete app.m_pOutputBitmap;
		app.m_pOutputBitmap = new Bitmap();
	}
	Bitmap& outputBitmap = *app.m_pOutputBitmap;
	outputBitmap.Init(*app.m_pOriginalBitmap);
	const char* textToEncrypt = app.m_pEncryptionTextInput->GetText();
	bool isEncrypted = outputBitmap.EncryptText(textToEncrypt);
	delete textToEncrypt;
	if(!isEncrypted) {
		MessageBox(0, L"Failed to encrypt text.", 0, 0);
		return;
	}

	Window& window = *app.m_pMainWindow;
	#pragma region ImagesDisplaySpecifications
	int windowWidth = window.GetWindowWidth();
	int windowHeight = window.GetWindowHeight();
	int windowCenterX = windowWidth / 2;
	int buttonHeight = 50;
	int paddingBottom = windowHeight / 4;
	int buttonY = windowHeight - buttonHeight - paddingBottom;
	int imageWidth = 400;
	int imageHeight = 400;
	int imageY = buttonY - imageHeight - 50;
	int centerPaddingHalf = 50;
	#pragma endregion ImagesDisplaySpecifications
	if (app.m_pOutputImage != nullptr) {
		window.DeleteImage(app.m_pOutputImage);
	}
	ImageResourceDescriptor imageDescriptor = { windowCenterX + centerPaddingHalf, imageY, imageWidth, imageHeight };
	app.m_pOutputImage = window.CreateImage(&outputBitmap, &imageDescriptor);
	window.Redraw();
}

//------------------------------------------------------------------------------
void App::Decrypt(Button* pButtonClicked)
{
	App& app = App::instance();
	Bitmap& originalBitmap = *app.m_pOriginalBitmap;
	//check if originalBitmap is null
	if(app.m_pOriginalBitmap == nullptr) {
		MessageBox(0, L"A bitmap must be loaded to be decrypted.", 0, 0);
		return;
	}
	if (originalBitmap.CheckSignEncrypted() == false) {
		MessageBox(0, L"This bitmap has no data encrypted in its pixels.", 0, 0);
		return;
	}
	const char* encryptedData = originalBitmap.ReadEncryptedText();
	app.m_pEncryptionTextInput->SetText(encryptedData);
	delete encryptedData;
}

//------------------------------------------------------------------------------
void App::SaveOutput(Button* pButtonClicked)
{
	App& app = App::instance();
	if(app.m_pOutputBitmap == nullptr) {
		MessageBox(0, L"Please encrypt data into a bitmap first.", 0, 0);
		return;
	}
	const char* path = "output.bmp";
	app.m_pOutputBitmap->Save(path);
	MessageBox(0, L"File saved.", 0, 0);
}

//------------------------------------------------------------------------------
void App::LoadFile(TextInput* pTextInput)
{
	App& app = App::instance();
	Window& window = *app.m_pMainWindow;

#pragma region ImagesDisplaySpecifications
	int windowWidth = window.GetWindowWidth();
	int windowHeight = window.GetWindowHeight();
	int windowCenterX = windowWidth / 2;
	int buttonHeight = 50;
	int paddingBottom = windowHeight / 4;
	int buttonY = windowHeight - buttonHeight - paddingBottom;
	int imageWidth = 400;
	int imageHeight = 400;
	int imageY = buttonY - imageHeight - 50;
	int centerPaddingHalf = 50;
#pragma endregion ImagesDisplaySpecifications
	wchar_t* filePath = pTextInput->GetWText();
	if (!std::filesystem::exists(filePath) || std::filesystem::path(filePath).extension() != ".bmp") {
		// File does not exist or is not a bitmap.	
		delete filePath;
		return;
	}
	if(app.m_pOriginalBitmap == nullptr) {
		app.m_pOriginalBitmap = new Bitmap();
	}
	else {
		app.m_pOriginalBitmap->Release();
		delete app.m_pOriginalBitmap;
		app.m_pOriginalBitmap = new Bitmap();
	}
	Bitmap& originalBitmap = *app.m_pOriginalBitmap;
	originalBitmap = Bitmap();
	originalBitmap.Init(filePath);
	delete filePath;
	if (originalBitmap.CheckSignEncrypted()) {
		MessageBox(0, L"File is already encrypted.", 0, 0);
		const char* encryptedData = originalBitmap.ReadEncryptedText();
		app.m_pEncryptionTextInput->SetText(encryptedData);
		delete encryptedData;
	}
	if(app.m_pOriginalImage != nullptr) {
		app.m_pMainWindow->DeleteImage(app.m_pOriginalImage);
	}
	ImageResourceDescriptor imageDescriptor = { windowCenterX - (imageWidth + centerPaddingHalf), imageY, imageWidth, imageHeight };
	app.m_pOriginalImage = window.CreateImage(&originalBitmap, &imageDescriptor);
	window.Redraw();
}

void App::FileDialogTest(Button* button) {
	App& app = App::instance();
	Window& window = *app.m_pMainWindow;

#pragma region ImagesDisplaySpecifications
	int windowWidth = window.GetWindowWidth();
	int windowHeight = window.GetWindowHeight();
	int windowCenterX = windowWidth / 2;
	int buttonHeight = 50;
	int paddingBottom = windowHeight / 4;
	int buttonY = windowHeight - buttonHeight - paddingBottom;
	int imageWidth = 400;
	int imageHeight = 400;
	int imageY = buttonY - imageHeight - 50;
	int centerPaddingHalf = 50;
#pragma endregion ImagesDisplaySpecifications
	// Initialize COM
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"COM Initialization failed", L"Error", MB_OK);
		return;
	}

	// Create the FileDialog
	IFileDialog* pFileDialog = NULL;
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));

	if (SUCCEEDED(hr))
	{
		// Set the file type filter to .bmp files
		COMDLG_FILTERSPEC fileTypes[] = {
			{ L"Bitmap Images (*.bmp)", L"*.bmp" }
		};
		pFileDialog->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);

		// Show the dialog
		hr = pFileDialog->Show(NULL);
		if (SUCCEEDED(hr))
		{
			// Get the result
			IShellItem* pItem;
			hr = pFileDialog->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				// Get the file path
				LPWSTR filePath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
				if (SUCCEEDED(hr))
				{
					// Use filePath to load your .bmp file
					if (app.m_pOriginalBitmap == nullptr) {
						app.m_pOriginalBitmap = new Bitmap();
					}
					else {
						app.m_pOriginalBitmap->Release();
						delete app.m_pOriginalBitmap;
						app.m_pOriginalBitmap = new Bitmap();
					}
					Bitmap& originalBitmap = *app.m_pOriginalBitmap;
					originalBitmap = Bitmap();
					originalBitmap.Init(filePath);
					app.m_pFilePathTextInput->SetText(filePath);
					CoTaskMemFree(filePath); // Free the memory allocated for the file path
					if (originalBitmap.CheckSignEncrypted()) {
						MessageBox(0, L"File is already encrypted.", 0, 0);
						const char* encryptedData = originalBitmap.ReadEncryptedText();
						app.m_pEncryptionTextInput->SetText(encryptedData);
						delete encryptedData;
					}
					if (app.m_pOriginalImage != nullptr) {
						app.m_pMainWindow->DeleteImage(app.m_pOriginalImage);
					}
					ImageResourceDescriptor imageDescriptor = { windowCenterX - (imageWidth + centerPaddingHalf), imageY, imageWidth, imageHeight };
					app.m_pOriginalImage = window.CreateImage(&originalBitmap, &imageDescriptor);
					window.Redraw();
					
				}
				pItem->Release(); // Release the IShellItem
			}
		}
		pFileDialog->Release(); // Release the IFileDialog
	}
	else
	{
		MessageBox(NULL, L"Failed to create FileDialog", L"Error", MB_OK);
	}

	// Uninitialize COM
	CoUninitialize();
}
