#pragma once

struct WindowClassDescriptor {
	HICON hWindowIcon = nullptr;
	HICON hWindowIconSmall = nullptr;
	HCURSOR hWindowCursor = nullptr;
	LPCWSTR className = L"WindowsAppClassName";
};

struct WindowDescriptor {
	LPCWSTR title = L"Windows App Title";
	int width = 800;
	int	height = 600;
	COLORREF backgroundColor = RGB(255, 255, 255);
	void (*onWindowCreateCallback)(Window* pWindow) = nullptr;
};

class Window : public CRTPComponent<Window>
{
public:
	Window();
	~Window();

	//
	int Init(HINSTANCE hInstance, WindowClassDescriptor* windowClassDescriptor);
	int Create(WindowDescriptor* windowDescriptor);
	void Run();
	void Release();

	// Getters/Setters
	inline HWND GetWindowHandle() const { return m_hWnd; };
	inline HINSTANCE GetHInstance() const { return m_hInstance; };
	inline int GetWindowWidth() const { return m_windowWidth; };
	inline int GetWindowHeight() const { return m_windowHeight; };

	inline void SetWindowHandle(HWND hWnd) { m_hWnd = hWnd; };

	// Interface Factories
	template<typename ComponentType>
	ComponentType* CreateComponent(ComponentDescriptor* pComponentDescriptor) {
		static_assert(std::is_base_of<CRTPComponent<ComponentType>, ComponentType>::value, "ComponentType must inherit from Component");
		ComponentType* component = new ComponentType();
		component->Init(this, static_cast<GetComponentDescriptorType<ComponentType>::type*>(pComponentDescriptor));
		m_components.insert(component);
		return component;
	};
	ImageResource* CreateImage(Bitmap* bitmap, ImageResourceDescriptor* imageDescriptor);

	void DeleteComponent(Component* component);
	void DeleteImage(ImageResource* image);

	void Redraw();

private:
	ATOM RegisterWindowClass(WindowClassDescriptor* windowClassDescriptor);
	static LRESULT __stdcall WindowProcess(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void OnWindowMessageCreate(HWND hWnd);
	void OnWindowMessageEraseBackground(HDC hdc);
	void OnWindowMessageCommand(WPARAM wParam, LPARAM lParam);
	void OnWindowMessageDrawItem(WPARAM wParam, LPARAM lParam);
	void OnWindowMessagePaint();

private:
	HINSTANCE m_hInstance = nullptr;
	LPCWSTR m_windowClassName = nullptr;
	int m_windowWidth = 0;
	int m_windowHeight = 0;
	COLORREF m_backgroundColor = RGB(255, 255, 255);

	void (*m_onWindowCreateCallback)(Window* pWindow) = nullptr;

	std::unordered_set<Component*> m_components = {};
	std::unordered_set<GraphicResource*> m_graphicResources = {};
};

