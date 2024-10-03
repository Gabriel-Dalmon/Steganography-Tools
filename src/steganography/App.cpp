#include "pch.h"

App* App::instance_ = nullptr;

App::App() {

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

void App::OnWindowCreateCallback(Window* pWindow)
{
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

	ButtonDescriptor encryptButtonDescriptor = { L"Encrypt", windowCenterX - (buttonWidth + centerPaddingHalf), buttonY, buttonWidth, buttonHeight };
	pWindow->CreateButton(&encryptButtonDescriptor, &App::Encrypt);

	ButtonDescriptor decryptButtonDescriptor = { L"Decrypt", windowCenterX + centerPaddingHalf, buttonY, buttonWidth, buttonHeight };
	pWindow->CreateButton(&decryptButtonDescriptor, &App::Decrypt);

	TextInputDescriptor textInputDescriptor = { L"Enter the file path here", 10, 10, 300, 25 };
	pWindow->CreateTextInput(&textInputDescriptor, &App::LoadFile);
}

void App::Encrypt(Button* pButtonClicked)
{
	MessageBox(0, L"Encrypt button clicked", 0, 0);
}

void App::Decrypt(Button* pButtonClicked)
{
	MessageBox(0, L"Decrypt button clicked", 0, 0);
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
	wchar_t* filePath = pTextInput->GetText();
	if (std::filesystem::exists(filePath) && std::filesystem::path(filePath).extension() == ".bmp") {
		MessageBox(0, L"File found", 0, 0);
		Bitmap bitmap;
		bitmap.Init(filePath);
		ImageDescriptor imageDescriptor = { windowCenterX - (imageWidth + centerPaddingHalf), imageY, imageWidth, imageHeight };
		window.CreateImage(&bitmap, &imageDescriptor);
		window.Redraw();
	}
}
