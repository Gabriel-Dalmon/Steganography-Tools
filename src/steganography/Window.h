#pragma once

struct WindowDescriptor {
	int windowWidth = 800;
	int	windowHeight = 600;
	LPCWSTR hAppTitle = L"Windows app";
	HICON hAppIcon = nullptr;
	HICON hAppIconSmall = nullptr;
	HCURSOR hAppCursor = nullptr;
	LPCWSTR hAppClassName = L"WindowsApp";
};

class Window
{
public:
	Window();
	~Window();

	//
	int Initialize(HINSTANCE hInstance, WindowDescriptor* windowDescriptor);
	ATOM RegisterWindowClass(WindowDescriptor* windowDescriptor);
	void Release();

	// Getters/Setters
	inline HWND GetWindowHandle() const { return m_hWnd; };
	inline int GetWindowWidth() const { return m_windowWidth; };
	inline int GetWindowHeight() const { return m_windowHeight; };

	// Subwindows Factory
	bool CreateButton() const;
	bool CreateTextInput() const;
	bool CreateImage() const;


private:
	void OnButtonClicked(WPARAM wParam, LPARAM lParam);
	static LRESULT __stdcall WindowProcess(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hWnd = nullptr;
	HINSTANCE m_hAppInstance = nullptr;
	int m_windowWidth = 0;
	int m_windowHeight = 0;
};