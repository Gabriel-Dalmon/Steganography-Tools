#pragma once

class App {
public: 
	static App& instance();
	int Init(HINSTANCE hInstance);
	void Release();
	
	inline Window& GetWindow() { return *m_pMainWindow; };
	
	static void OnWindowCreateCallback(Window* pWindow);
	static void Encrypt(Button* pButtonClicked);
	static void Decrypt(Button* pButtonClicked);
	static void SaveOutput(Button* pButtonClicked);
	static void OnFileSelected(const wchar_t* filePath);

	Window* m_pMainWindow;
	FileInput* m_pFilePathInput;
private:
	App();
	static App* instance_;

	TextInput* m_pEncryptionTextInput;

	Bitmap* m_pOriginalBitmap;
	ImageResource* m_pOriginalImage;
	
	Bitmap* m_pOutputBitmap;
	ImageResource* m_pOutputImage;
};