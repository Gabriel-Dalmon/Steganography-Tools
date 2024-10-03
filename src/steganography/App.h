#pragma once

class App {
public: 
	static App& instance();
	int Init(HINSTANCE hInstance);
	void Release();
	
	inline Window& GetWindow() { return *m_pMainWindow; };
	
	static void OnWindowCreateCallback(Window* pWindow);
	static void Encrypt(Button* pButtonClicked);
	static void SaveOutput(Button* pButtonClicked);
	static void LoadFile(TextInput* pTextInput);
	Window* m_pMainWindow;
private:
	App();
	static App* instance_;

	TextInput* m_pFilePathTextInput;
	TextInput* m_pEncryptionTextInput;

	Bitmap* m_pOriginalBitmap;
	Image* m_pOriginalImage;	
	
	Bitmap* m_pOutputBitmap;
	Image* m_pOutputImage;
};