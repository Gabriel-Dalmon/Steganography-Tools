#include "pch.h"

Window::Window()
{
}

Window::~Window()
{
}

int Window::Initialize(HINSTANCE hAppInstance, WindowDescriptor* windowDescriptor)
{
    m_hAppInstance = hAppInstance;
    m_windowWidth = windowDescriptor->windowWidth;
    m_windowHeight = windowDescriptor->windowHeight;
    RegisterWindowClass(windowDescriptor);

    m_hWnd = CreateWindow(
        windowDescriptor->hAppClassName,
        windowDescriptor->hAppTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        m_windowWidth,
        m_windowHeight,
        0,
        0,
        m_hAppInstance,
        0
    );

    if (m_hWnd == NULL)
    {
        MessageBox(0, L"CreateWindow FAILED", 0, 0);
        return -1;
    }

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
    return 0;
}

ATOM Window::RegisterWindowClass(WindowDescriptor* windowDescriptor)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProcess;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hAppInstance;
    //wcex.hIcon = windowDescriptor->hAppIcon == nullptr ? LoadIcon(m_hAppInstance, MAKEINTRESOURCE(IDI_SLEEPYENGINE)) : windowDescriptor->hAppIcon;
    wcex.hCursor = windowDescriptor->hAppCursor == nullptr ? LoadCursor(nullptr, IDC_ARROW) : windowDescriptor->hAppCursor;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    //wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SLEEPYENGINE);
    wcex.lpszClassName = windowDescriptor->hAppClassName;
    //wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

void Window::Release()
{
    // Close window
    DestroyWindow(m_hWnd);
}

LRESULT CALLBACK Window::WindowProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
    {
        if (wParam == BN_CLICKED) {
            //OnButtonClicked(wParam, lParam);
            /*const HWND buttonHWnd = (HWND)lParam;
            const Button& button = GetWindowLongPtr(buttonHWnd, GWLP_USERDATA);
            button.execute();*/
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: GetDC and Add bitmaps drawing here
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}