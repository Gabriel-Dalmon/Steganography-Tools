#define ENCRYPTSIGN 19319

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

	void SetBits(byte value, byte numOfBits, byte* place);

	byte ReadBits(byte numOfBits, byte* place);

	void SetBytes(unsigned int value, byte size, byte* place);

	unsigned int ReadBytes(byte size, byte* place);

	void SetSignEncrypted();

	void setTextHeader(unsigned int lengthOfText);

	void EncryptText(const char* text);

	bool CheckSignEncrypted();

	unsigned int ReadTextHeader();

	const char* ReadEncryptedText(int textLength);

	void uninit();

	~Bitmap() {};
};

