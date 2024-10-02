#ifndef UNICODE
#define UNICODE
#endif 

#include "pch.h"
#include "Bitmap.h"
#include "File.h"
void OnPaint(HDC hdc) {
    
    Bitmap bitMP;
    bitMP.Init("imgBMPTest.bmp");

    HBITMAP bmpTest = bitMP.GenerateHBitMap(hdc);
    HDC newhdc = CreateCompatibleDC(hdc);

    SelectObject(newhdc, bmpTest);
    BITMAPINFOHEADER bih = bitMP.GetMapInfo();
    BitBlt(hdc, 10, 10, bih.biWidth, bih.biHeight, newhdc, 0, 0, SRCCOPY);

    File testFile;
    testFile.Open("copy.bmp", "wb+");
    testFile.Write(bitMP.GetBuffer(), bitMP.GetFileInfo().bfSize);
    testFile.Close();
}

void EncryptButtonCallback()
{
	MessageBox(0, L"Encrypt button clicked", 0, 0);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	WindowClassDescriptor windowClassDescriptor;
	WindowDescriptor windowDescriptor = { L"Window", 800, 600 };
	Window window;
	window.Init(hInstance, &windowClassDescriptor);
	window.Create(&windowDescriptor);

	ButtonDescriptor encryptButtonDescriptor = { L"Encrypt", 100, 100, 200, 50 };
	window.CreateButton(&encryptButtonDescriptor, &EncryptButtonCallback);

	ButtonDescriptor decryptButtonDescriptor = { L"Decrypt", 350, 100, 200, 50 };
	window.CreateButton(&decryptButtonDescriptor, nullptr);

	TextInputDescriptor textInputDescriptor = { L"Enter the file path here", 10, 10, 300, 25 };
	window.CreateTextInput(&textInputDescriptor);

	window.Run();
	return 0;
}