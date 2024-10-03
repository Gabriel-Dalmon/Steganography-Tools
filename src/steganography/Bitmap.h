#pragma once
#define ENCRYPTSIGN 38977
#define MAX16BITS 65535

class Bitmap {
	byte* m_buffer;
	BITMAPFILEHEADER m_bfh;
	BITMAPINFOHEADER m_bih;
	byte* m_colorBits;

public:
	
	Bitmap();

	bool Init(const char* path);
	bool Init(const wchar_t* path);
	bool Init(Bitmap& bitmap);

	BITMAPINFOHEADER GetMapInfo();

	BITMAPFILEHEADER GetFileInfo();

	HBITMAP GenerateHBitMap(HDC hdc);

	byte* GetBuffer();

	void SetBits(byte value, byte numOfBits, byte* place);

	byte ReadBits(byte numOfBits, byte* place);

	void DoubleSize();

	void SetBytes(unsigned int value, byte size, byte* place);

	unsigned int ReadBytes(byte size, byte* place);

	void SetSignEncrypted();

	inline BYTE* GetPixelsBuffer() const { return m_colorBits; };

	void setTextHeader(unsigned int lengthOfText);

	bool EncryptText(const char* text);

	bool CheckSignEncrypted();

	unsigned int ReadTextHeader();

	const char* ReadEncryptedText(int textLength);

	void uninit();

	~Bitmap() {};
};

