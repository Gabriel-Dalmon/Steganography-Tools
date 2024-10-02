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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        OnPaint(hdc);
        // All painting occurs here, between BeginPaint and EndPaint.

        //FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
            
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}