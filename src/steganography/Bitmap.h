class Bitmap {
	byte* m_buffer;
	BITMAPFILEHEADER m_bfh;
	BITMAPINFOHEADER m_bih;
	byte* m_colorBits;

public:
	
	Bitmap();

	bool Init(const char* path);

	BITMAPINFOHEADER GetMapInfo();

	HBITMAP GenerateHBitMap(HDC hdc);

	byte* GetBuffer();

	void setTextHeader(SHORT lengthOfText);

	~Bitmap() {};
};