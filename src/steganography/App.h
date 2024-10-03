#pragma once

class App {
public: 
	static App& instance();
	int Init(HINSTANCE hInstance);
	
	inline Window& GetWindow() { return *m_pMainWindow; };
	
	static void OnWindowCreateCallback(Window* pWindow);
	static void Encrypt(Button* pButtonClicked);
	static void Decrypt(Button* pButtonClicked);
	static void LoadFile(TextInput* pTextInput);
	Window* m_pMainWindow;
private:
	App();
	static App* instance_;

	Bitmap* loadedBitmap;
	Image* loadedImageResource;
};