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

std::weak_ptr<Button> Window::CreateButton(ButtonDescriptor* buttonDescriptor, void(*onClickedCallback)())
{
    std::shared_ptr<Button> button = std::make_shared<Button>();
    button->Init(this, buttonDescriptor, onClickedCallback);
    m_buttons.insert(button);
    return std::weak_ptr<Button>(button);
}

std::weak_ptr<TextInput> Window::CreateTextInput(TextInputDescriptor* textInputDescriptor, void(*onChangeCallback)())
{
    std::shared_ptr<TextInput> textInput = std::make_shared<TextInput>();
    textInput->Init(this, textInputDescriptor, onChangeCallback);
    m_textInputs.insert(textInput);
    return std::weak_ptr<TextInput>(textInput);
}

std::weak_ptr<Image> Window::CreateImage(Bitmap* bitmap, ImageDescriptor* imageDescriptor)
{
    std::shared_ptr<Image> image = std::make_shared<Image>();
    image->Init(bitmap, imageDescriptor, GetDC(m_hWnd));
    m_graphicResources.insert(image);
    return std::weak_ptr<Image>(image);
}

LRESULT CALLBACK Window::WindowProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* window = Window::GetInstance(hWnd);
    switch (uMsg)
    {
    case WM_COMMAND:
    {
        const HWND& controlHWnd = reinterpret_cast<HWND>(lParam);
        if (!controlHWnd) break;
        switch(wParam) {
        case EN_CHANGE:            
            window->OnEditControlChanged(controlHWnd);
            break;
        case BN_CLICKED:
            window->OnButtonClicked(controlHWnd);
            break;
        }
        break;
    }
    break;
    case WM_PAINT:
    {
        OutputDebugString(L"WMPAIN called\n");
        if (window->m_graphicResources.size() == 0) {
            break;
        }
        OutputDebugString(L"Has Graphic Resources\n");
        PAINTSTRUCT ps;
        HDC compatibleDevice = BeginPaint(hWnd, &ps);
        HDC sourceCompatibleDevice = CreateCompatibleDC(compatibleDevice);
        for (const auto& graphicResource : window->m_graphicResources) {
            graphicResource->Draw(compatibleDevice, sourceCompatibleDevice);
        }
        DeleteDC(sourceCompatibleDevice);
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

void Window::OnEditControlChanged(HWND controlHWnd)
{
    const TextInput& textInput = *TextInput::GetInstance(controlHWnd);
    textInput.OnChange();
}