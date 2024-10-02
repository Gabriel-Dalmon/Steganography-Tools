#ifndef UNICODE
#define UNICODE
#endif 

#include "pch.h"




void EncryptButtonCallback()
{
	MessageBox(0, L"Encrypt button clicked", 0, 0);
}

void TextInputCallback()
{
	MessageBox(0, L"TextInput changed", 0, 0);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	OutputDebugString(L"Output Debug String Entered Main\n");
	WindowClassDescriptor windowClassDescriptor;
	WindowDescriptor windowDescriptor = { L"Window", 1200, 800 };
	Window window;
	window.Init(hInstance, &windowClassDescriptor);
	window.Create(&windowDescriptor);

	int windowWidth = window.GetWindowWidth();
	int windowHeight = window.GetWindowHeight();
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

	ButtonDescriptor encryptButtonDescriptor = { L"Encrypt", windowCenterX - (buttonWidth + centerPaddingHalf), buttonY, buttonWidth, buttonHeight};
	window.CreateButton(&encryptButtonDescriptor, &EncryptButtonCallback);

	ButtonDescriptor decryptButtonDescriptor = { L"Decrypt", windowCenterX + centerPaddingHalf, buttonY, buttonWidth, buttonHeight };
	window.CreateButton(&decryptButtonDescriptor, nullptr);

	TextInputDescriptor textInputDescriptor = { L"Enter the file path here", 10, 10, 300, 25 };
	window.CreateTextInput(&textInputDescriptor, &TextInputCallback);
	Bitmap bitmap;
	bitmap.Init("./bigImageTest.bmp");
	ImageDescriptor imageDescriptor = { windowCenterX - (imageWidth + centerPaddingHalf), imageY, imageWidth, imageHeight };
	window.CreateImage(&bitmap, &imageDescriptor);

	Bitmap bitmap2;
	bitmap2.Init("./imgBMPTest.bmp");
	ImageDescriptor imageDescriptor2 = { windowCenterX + centerPaddingHalf, imageY, imageWidth, imageHeight };
	window.CreateImage(&bitmap2, &imageDescriptor2);

	window.Run();
	return 0;
}