class Bitmap {
	byte* m_buffer;
	BITMAPFILEHEADER m_bfh;
	BITMAPINFOHEADER m_bih;
	byte* m_colorBits;

public:
	
	Bitmap();

	bool Init(const char* path);

	BITMAPINFOHEADER GetMapInfo();

	BITMAPFILEHEADER GetFileInfo();

	HBITMAP GenerateHBitMap(HDC hdc);

	byte* GetBuffer();

	void setTextHeader(unsigned int lengthOfText);

	~Bitmap() {};
};