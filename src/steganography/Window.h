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
	void (*onWindowCreateCallback)(Window* pWindow) = nullptr;
};

template<typename DerivedWindowType>
class AbstractWindow {
public:
	void SetInstance() {
		SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	};
	
	static DerivedWindowType* GetInstance(HWND hWnd) {
		return reinterpret_cast<DerivedWindowType*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		//return (instance->type == typeid(DerivedWindowType)) ? instance : nullptr;
	};

protected:
	HWND m_hWnd = nullptr;
	//type_info type = typeid(DerivedWindowType);
private:
	//AbstractWindow();
	friend DerivedWindowType;
};

class Window : public AbstractWindow<Window>
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

	// Subwindow Factories
	Button* CreateButton(ButtonDescriptor* buttonDescriptor);
	TextInput* CreateTextInput(TextInputDescriptor* buttonDescriptor, void(*onChangeCallback)(TextInput* textInput) = nullptr);
	Image* CreateImage(Bitmap* bitmap, ImageDescriptor* imageDescriptor);

	void DeleteImage(Image* image);

	void Redraw();

private:
	ATOM RegisterWindowClass(WindowClassDescriptor* windowClassDescriptor);
	static LRESULT __stdcall WindowProcess(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void OnWindowMessageCreate(HWND hWnd);
	void OnWindowMessageCommand(WPARAM wParam, LPARAM lParam);
	void OnWindowMessagePaint();

private:
	HINSTANCE m_hInstance = nullptr;
	LPCWSTR m_windowClassName = nullptr;
	int m_windowWidth = 0;
	int m_windowHeight = 0;

	void (*m_onWindowCreateCallback)(Window* pWindow) = nullptr;

	std::unordered_set<Button*> m_buttons = {};
	std::unordered_set<TextInput*> m_textInputs = {};
	std::unordered_set<GraphicResource*> m_graphicResources = {};
};