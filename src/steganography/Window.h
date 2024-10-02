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
};

template<typename WindowType>
class AbstractWindow {
public:
	void SetInstance() {
		SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	};
	
	static WindowType* GetInstance(HWND hWnd) {
		return reinterpret_cast<WindowType*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	};

protected:
	HWND m_hWnd = nullptr;
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

	// Subwindow Factories
	std::weak_ptr<Button> CreateButton(ButtonDescriptor* buttonDescriptor, void(*onClickedCallback)());
	std::weak_ptr<TextInput> CreateTextInput(TextInputDescriptor* buttonDescriptor, void(*onChangeCallback)() = nullptr);
	std::weak_ptr<Image> CreateImage(Bitmap* bitmap, ImageDescriptor* imageDescriptor);


private:
	ATOM RegisterWindowClass(WindowClassDescriptor* windowClassDescriptor);
	static LRESULT __stdcall WindowProcess(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void OnButtonClicked(HWND buttonHWnd);
	void OnEditControlChanged(HWND controlHWnd);

private:
	HINSTANCE m_hInstance = nullptr;
	LPCWSTR m_windowClassName = nullptr;
	int m_windowWidth = 0;
	int m_windowHeight = 0;

	std::unordered_set<std::shared_ptr<Button>> m_buttons = {};
	std::unordered_set<std::shared_ptr<TextInput>> m_textInputs = {};
	std::unordered_set<std::shared_ptr<GraphicResource>> m_graphicResources = {};
};