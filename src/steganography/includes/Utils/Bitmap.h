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
	bool Save(const char* path);
	void Release();

	BITMAPINFOHEADER GetMapInfo();

	BITMAPFILEHEADER GetFileInfo();

	HBITMAP GenerateHBitMap(HDC hdc);

	byte* GetBuffer();

	void DoubleSize();

	inline byte* GetPixelsBuffer() { return m_colorBits; };
	inline size_t GetPixelsBufferSize() { return m_bfh.bfSize - m_bfh.bfOffBits - 24; };

	void uninit();

	~Bitmap() {};
};

