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
    m_onWindowCreateCallback = windowDescriptor->onWindowCreateCallback;
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

Button* Window::CreateButton(ButtonDescriptor* buttonDescriptor, void(*onClickedCallback)(Button* button))
{
    Button* button = new Button();
    button->Init(this, buttonDescriptor, onClickedCallback);
    m_buttons.insert(button);
    return button;
}

TextInput* Window::CreateTextInput(TextInputDescriptor* textInputDescriptor, void(*onChangeCallback)(TextInput* textInput))
{
    TextInput* textInput = new TextInput();
    textInput->Init(this, textInputDescriptor, onChangeCallback);
    m_textInputs.insert(textInput);
    return textInput;
}

Image* Window::CreateImage(Bitmap* bitmap, ImageDescriptor* imageDescriptor)
{
    Image* image = new Image();
    image->Init(bitmap, imageDescriptor, GetDC(m_hWnd));
    m_graphicResources.insert(image);
    return image;
}

void Window::Redraw() {
    RedrawWindow(GetWindowHandle(), NULL, NULL, RDW_INVALIDATE);
}

LRESULT CALLBACK Window::WindowProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window& window = *Window::GetInstance(hWnd);
    App& app = App::instance();
    Window& window2 = *app.m_pMainWindow;
    switch (uMsg)
    {
    case WM_CREATE:
    {
        int a = 5;
        //window = *app.m_pMainWindow;
        window2.SetWindowHandle(hWnd);
        window2.SetInstance();
        window2.OnWindowMessageCreate();
        break;
    }
    case WM_COMMAND:
        window.OnWindowMessageCommand(wParam, lParam);
    break;
    case WM_PAINT:
        window.OnWindowMessagePaint();
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

void Window::OnWindowMessageCreate()
{
    SetInstance();
    ShowWindow(m_hWnd, SW_SHOW);
    if(m_onWindowCreateCallback) m_onWindowCreateCallback(this);
    UpdateWindow(m_hWnd);
}

void Window::OnWindowMessageCommand(WPARAM wParam, LPARAM lParam)
{
    const HWND& controlHWnd = reinterpret_cast<HWND>(lParam);
    if (!controlHWnd) return;
    if (LOWORD(wParam) != MAIN_WINDOW_ID) return;
    switch (HIWORD(wParam)) {
    case EN_CHANGE:
        TextInput* textInput = TextInput::GetInstance(controlHWnd);
        if (m_textInputs.find(textInput) == m_textInputs.end()) return;
        textInput->OnChange();
        break;
    }
    switch (wParam) {
    case BN_CLICKED:
        Button& button = *Button::GetInstance(controlHWnd);
        button.Execute();
        break;
    }
    return;
}

void Window::OnWindowMessagePaint()
{
    if (m_graphicResources.size() == 0) {
        return;
    }
    PAINTSTRUCT ps;
    HDC compatibleDevice = BeginPaint(m_hWnd, &ps);
    HDC sourceCompatibleDevice = CreateCompatibleDC(compatibleDevice);
    for (const auto& graphicResource : m_graphicResources) {
        graphicResource->Draw(compatibleDevice, sourceCompatibleDevice);
    }
    DeleteDC(sourceCompatibleDevice);
    EndPaint(m_hWnd, &ps);
}