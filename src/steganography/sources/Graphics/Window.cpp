#include "pch.h"

//------------------------------------------------------------------------------
Window::Window()
{
    m_hWnd = NULL;
	m_hInstance = NULL;
	m_windowWidth = 0;
	m_windowHeight = 0;
	m_onWindowCreateCallback = nullptr;
	m_windowClassName = L"";
	m_hWnd = NULL;
	m_hInstance = NULL;
	m_windowWidth = 0;
	m_windowHeight = 0;
	m_onWindowCreateCallback = nullptr;
	m_windowClassName = L"";
	m_graphicResources.clear();
}

//------------------------------------------------------------------------------
Window::~Window()
{
}

//------------------------------------------------------------------------------
int Window::Init(HINSTANCE hInstance, WindowClassDescriptor* windowClassDescriptor)
{
    m_hInstance = hInstance;
    m_windowClassName = windowClassDescriptor->className;
    RegisterWindowClass(windowClassDescriptor);
    return 0;
}

//------------------------------------------------------------------------------
int Window::Create(WindowDescriptor* windowDescriptor)
{
    m_windowWidth = windowDescriptor->width;
    m_windowHeight = windowDescriptor->height;
    m_backgroundColor = windowDescriptor->backgroundColor;
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
        this
    );

    if (m_hWnd == NULL)
    {
        MessageBox(0, L"CreateWindow FAILED", 0, 0);
        return 1;
    }
    
    return 0;
}

//------------------------------------------------------------------------------
void Window::Run()
{
    MSG msg = { 0 };
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
void Window::Release()
{
    // Close subwindows
    for(auto& Component : m_components)
	{
		Component->Release();
        delete Component;
	}
    m_components.clear();

    // Destroy graphic resources
    for(auto& GraphicResource : m_graphicResources)
	{
		GraphicResource->Release();
        delete GraphicResource;
	}
    m_graphicResources.clear();
    DestroyWindow(m_hWnd);
}

//------------------------------------------------------------------------------
ImageResource* Window::CreateImage(Bitmap* bitmap, ImageResourceDescriptor* imageDescriptor)
{
    ImageResource* image = new ImageResource();
    image->Init(bitmap, imageDescriptor, GetDC(m_hWnd));
    m_graphicResources.insert(image);
    return image;
}

void Window::DeleteComponent(Component* component)
{
    m_components.erase(component);
	component->Release();
	delete component;
}

void Window::DeleteImage(ImageResource* image)
{
    m_graphicResources.erase(image);
    image->Release();
	delete image;
}

//------------------------------------------------------------------------------
void Window::Redraw() {
    RedrawWindow(GetWindowHandle(), NULL, NULL, RDW_INVALIDATE);
}

//------------------------------------------------------------------------------
LRESULT CALLBACK Window::WindowProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window& window = *Window::GetInstance(hWnd);
    switch (uMsg)
    {
    case WM_CREATE:
    {
        //----------------------------------------------------------------------
        /** Prior to WM_CREATE Window::GetInstance(hWnd) will return null. The
        * Window class instance is passed in lParam Create Struct for the 
        * WM_CREATE Message */
        const LPCREATESTRUCT& createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        Window& initWindow = *reinterpret_cast<Window*>(createStruct->lpCreateParams);
        initWindow.OnWindowMessageCreate(hWnd);
        break;
    }
    case WM_COMMAND:
        window.OnWindowMessageCommand(wParam, lParam);
    break;
    case WM_DRAWITEM:
		window.OnWindowMessageDrawItem(wParam, lParam);
    break;
    case WM_PAINT:
        window.OnWindowMessagePaint();
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_ERASEBKGND:
		window.OnWindowMessageEraseBackground(reinterpret_cast<HDC>(wParam));
        return 1;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

//------------------------------------------------------------------------------
void Window::OnWindowMessageCreate(HWND hWnd)
{
    //--------------------------------------------------------------------------
    /** Generic Window Class instructions to execute on WM_CREATE */
    m_hWnd = hWnd;
    SetInstance();
    ShowWindow(m_hWnd, SW_SHOW);
    //--------------------------------------------------------------------------
    /** App specific instructions to execute on WM_CREATE */
    if(m_onWindowCreateCallback) m_onWindowCreateCallback(this);
    //--------------------------------------------------------------------------
    UpdateWindow(m_hWnd);
}

void Window::OnWindowMessageEraseBackground(HDC hdc)
{
    HBRUSH hBrush = CreateSolidBrush(m_backgroundColor);  // Red background
    RECT rect;
    GetClientRect(m_hWnd, &rect);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
    return;
}

//------------------------------------------------------------------------------
void Window::OnWindowMessageCommand(WPARAM wParam, LPARAM lParam)
{
    //--------------------------------------------------------------------------
    const HWND& controlHWnd = reinterpret_cast<HWND>(lParam);
    /** Return if controlHWnd is null or LOWORD(wParam) indicates the control 
    * belongs to another window. */
    if (!controlHWnd) return;
    if (LOWORD(wParam) != MAIN_WINDOW_ID) return;

    //--------------------------------------------------------------------------
    switch (HIWORD(wParam)) {
    case EN_CHANGE:
    {
        TextInput* textInput = TextInput::GetInstance(controlHWnd);
        if (m_components.find(textInput) == m_components.end()) return;
        textInput->OnChange();
        return;
    }
    case BN_CLICKED:
        Button& button = *Button::GetInstance(controlHWnd);
        button.Execute();
        return;
    }
}

void Window::OnWindowMessageDrawItem(WPARAM wParam, LPARAM lParam)
{
    LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lParam;
	if (lpDrawItem->CtlType == ODT_BUTTON) {
		Button& button = *Button::GetInstance(lpDrawItem->hwndItem);
		button.Draw(lpDrawItem->hDC, &lpDrawItem->rcItem);
	}
	return;
}

//------------------------------------------------------------------------------
void Window::OnWindowMessagePaint()
{
    //--------------------------------------------------------------------------
    /** BeginPaint and EndPaint must both be called otherwise unexpected
    * behavior may occur. (ex: MessageBoxes not being displayed) */
    PAINTSTRUCT ps;
    HDC compatibleDevice = BeginPaint(m_hWnd, &ps);
    HDC sourceCompatibleDevice = CreateCompatibleDC(compatibleDevice);
    for (const auto& graphicResource : m_graphicResources) {
        graphicResource->Draw(compatibleDevice, sourceCompatibleDevice);
    }
    DeleteDC(sourceCompatibleDevice);
    EndPaint(m_hWnd, &ps);
}