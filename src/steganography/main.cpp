#ifndef UNICODE
#define UNICODE
#endif 

#include "pch.h"
#include "Bitmap.h"
#include "File.h"
#include <iostream>
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
	AllocConsole();
	FILE* consoleOut;
	freopen_s(&consoleOut, "CONOUT$", "w", stdout);

	Bitmap bitMP;
	bitMP.Init("imgBMPTest.bmp");
	if (bitMP.CheckSignEncrypted()) {
		int length2 = bitMP.ReadTextHeader();
		std::cout << bitMP.ReadEncryptedText(length2) << std::endl;
	}
	else {
		std::cout << "No encrypted text" << std::endl;
	}
	bitMP.EncryptText("TEST");
	File testFile;
	testFile.Open("C:/Users/mzins/Downloads/copy.bmp", "wb+");
	testFile.Write(bitMP.GetBuffer(), bitMP.GetFileInfo().bfSize);
	testFile.Close();

	Bitmap bitMP2;
	bitMP2.Init("C:/Users/mzins/Downloads/copy.bmp");


	std::cout << bitMP2.CheckSignEncrypted() << std::endl;
	if (bitMP2.CheckSignEncrypted()) {
		int length = bitMP2.ReadTextHeader();
		std::cout << bitMP2.ReadEncryptedText(length) << std::endl;
	}
	else {
		std::cout << "No encrypted text" << std::endl;
	}

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