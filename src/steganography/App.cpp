#include "pch.h"

App* App::instance_ = nullptr;

App::App() {
	m_pMainWindow = nullptr;
	m_pOriginalBitmap = nullptr;
	m_pOutputBitmap = nullptr;
	m_pOriginalImage = nullptr;
	m_pOutputImage = nullptr;
	m_pFilePathTextInput = nullptr;
	m_pEncryptionTextInput = nullptr;
}
App& App::instance()
{
    if (!instance_) {
        instance_ = new App();
    };
    return *instance_;
}

int App::Init(HINSTANCE hInstance) {

	WindowClassDescriptor windowClassDescriptor;
	WindowDescriptor windowDescriptor = { L"Steganography Tools", 1200, 800, &App::OnWindowCreateCallback };

	m_pMainWindow = new Window();
	m_pMainWindow->Init(hInstance, &windowClassDescriptor);
	m_pMainWindow->Create(&windowDescriptor);
	m_pMainWindow->Run();
	return 0;
}

void App::Release() {
}

void App::OnWindowCreateCallback(Window* pWindow)
{
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

	ButtonDescriptor encryptButtonDescriptor = { 
		L"Encrypt", 
		windowCenterX - (buttonWidth + centerPaddingHalf), 
		buttonY, 
		buttonWidth, 
		buttonHeight, 
		&App::Encrypt 
	};
	pWindow->CreateButton(&encryptButtonDescriptor);

	ButtonDescriptor decryptButtonDescriptor = { 
		L"Save", 
		windowCenterX + centerPaddingHalf, 
		buttonY, 
		buttonWidth, 
		buttonHeight,
		&App::SaveOutput
	};
	pWindow->CreateButton(&decryptButtonDescriptor);

	TextInputDescriptor textInputDescriptor = { L"Enter the file path here.", 10, 10, 300, 25 };
	app.m_pFilePathTextInput = pWindow->CreateTextInput(&textInputDescriptor, &App::LoadFile);	
	
	TextInputDescriptor encryptiontextInputDescriptor = { L"Enter encryption data here.", 10, 50, 300, 25 };
	app.m_pEncryptionTextInput = pWindow->CreateTextInput(&encryptiontextInputDescriptor, nullptr);
}

void App::Encrypt(Button* pButtonClicked)
{
	OutputDebugString(L"Encrypting...\n");
	App& app = App::instance();
	if(app.m_pOriginalBitmap == nullptr) {
		MessageBox(0, L"Please load a file first.", 0, 0);
		return;
	}
	if (app.m_pOutputBitmap == nullptr) {
		app.m_pOutputBitmap = new Bitmap();
	}
	else {
		//app.m_pOriginalBitmap->Release();
		delete app.m_pOutputBitmap;
		app.m_pOutputBitmap = new Bitmap();
	}
	Bitmap& outputBitmap = *app.m_pOutputBitmap;
	outputBitmap.Init(*app.m_pOriginalBitmap);
	const char* textToEncrypt = app.m_pEncryptionTextInput->GetText();
	if(!outputBitmap.EncryptText(textToEncrypt)) {
		MessageBox(0, L"Failed to encrypt text.", 0, 0);
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
	ImageDescriptor imageDescriptor = { windowCenterX + centerPaddingHalf, imageY, imageWidth, imageHeight };
	app.m_pOutputImage = window.CreateImage(&outputBitmap, &imageDescriptor);
	window.Redraw();
}

void App::SaveOutput(Button* pButtonClicked)
{
	App& app = App::instance();
	if(app.m_pOutputBitmap == nullptr) {
		MessageBox(0, L"Please encrypt data into a bitmap first.", 0, 0);
		return;
	}
	/*const char* path = "output.bmp";
	app.m_pOutputBitmap->Save(path);*/
	Bitmap& outputBitmap = *app.m_pOutputBitmap;
	File outputFile;
	outputFile.Open("output.bmp", "wb+");
	outputFile.Write(outputBitmap.GetBuffer(), outputBitmap.GetFileInfo().bfSize);
	outputFile.Close();
	MessageBox(0, L"File saved.", 0, 0);
}

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
		return;
	}
	if(app.m_pOriginalBitmap == nullptr) {
		app.m_pOriginalBitmap = new Bitmap();
	}
	else {
		//app.m_pOriginalBitmap->Release();
		delete app.m_pOriginalBitmap;
		app.m_pOriginalBitmap = new Bitmap();
	}
	Bitmap& orinalBitmap = *app.m_pOriginalBitmap;
	orinalBitmap = Bitmap();
	orinalBitmap.Init(filePath);
	if (orinalBitmap.CheckSignEncrypted()) {
		MessageBox(0, L"File is already encrypted. \n Click OK to decrypt it.", 0, 0);
		int encryptedDataLength = orinalBitmap.ReadTextHeader();
		const char* encryptedData = orinalBitmap.ReadEncryptedText(encryptedDataLength);
		std::cout << encryptedData << std::endl;
		app.m_pEncryptionTextInput->SetText(encryptedData);
	}
	if(app.m_pOriginalImage != nullptr) {
		app.m_pMainWindow->DeleteImage(app.m_pOriginalImage);
	}
	ImageDescriptor imageDescriptor = { windowCenterX - (imageWidth + centerPaddingHalf), imageY, imageWidth, imageHeight };
	app.m_pOriginalImage = window.CreateImage(&orinalBitmap, &imageDescriptor);
	window.Redraw();
}
