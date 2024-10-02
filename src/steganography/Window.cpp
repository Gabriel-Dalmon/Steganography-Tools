#include "pch.h"

Window::Window()
{
}

Window::~Window()
{
}

int Window::Init(HINSTANCE hInstance, WindowClassDescriptor* windowClassDescriptor)
{
    m_hInstance = hInstance;
    m_windowClassName = windowClassDescriptor->className;
    RegisterWindowClass(windowClassDescriptor);
    return 0;
}

int Window::Create(WindowDescriptor* windowDescriptor)
{
    m_windowWidth = windowDescriptor->width;
    m_windowHeight = windowDescriptor->height;
    m_hWnd = CreateWindow(
        m_windowClassName,
        windowDescriptor->title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        m_windowWidth,
        m_windowHeight,
        0,
        0,
        m_hInstance,
        0
    );

    if (m_hWnd == NULL)
    {
        MessageBox(0, L"CreateWindow FAILED", 0, 0);
        return -1;
    }

    // Store the Window instance in the window handle user data.
    SetInstance();
    ShowWindow(m_hWnd, SW_SHOW);
    return 0;
}

void Window::Run()
{
    MSG msg = { 0 };
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

ATOM Window::RegisterWindowClass(WindowClassDescriptor* windowClassDescriptor)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProcess;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = windowClassDescriptor->hWindowIcon;
    wcex.hCursor = windowClassDescriptor->hWindowCursor;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = windowClassDescriptor->className;
    wcex.hIconSm = NULL;

    return RegisterClassExW(&wcex);
}

void Window::Release()
{
    // Close window
    DestroyWindow(m_hWnd);
}

std::weak_ptr<Button> Window::CreateButton(ButtonDescriptor* buttonDescriptor, void(*callback)())
{
    std::shared_ptr<Button> button = std::make_shared<Button>();
    button->Init(this, buttonDescriptor, callback);
    m_buttons.insert(button);
    return std::weak_ptr<Button>(button);
}

std::weak_ptr<TextInput> Window::CreateTextInput(TextInputDescriptor* textInputDescriptor)
{
    std::shared_ptr<TextInput> textInput = std::make_shared<TextInput>();
    textInput->Init(this, textInputDescriptor);
    m_textInputs.insert(textInput);
    return std::weak_ptr<TextInput>(textInput);
}

LRESULT CALLBACK Window::WindowProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* window = Window::GetInstance(hWnd);
    switch (uMsg)
    {
    case WM_COMMAND:
    {
        if (wParam == BN_CLICKED) {
            const HWND& buttonHWnd = reinterpret_cast<HWND>(lParam);
            if (!buttonHWnd) break;
            window->OnButtonClicked(buttonHWnd);
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

void Window::OnButtonClicked(HWND buttonHWnd)
{
    const Button& button = *Button::GetInstance(buttonHWnd);
    button.Execute();
}
