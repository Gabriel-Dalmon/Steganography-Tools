#include "pch.h"


Bitmap::Bitmap() {
	m_buffer = nullptr;
	memset(&m_bfh, 0, sizeof(BITMAPFILEHEADER));
	memset(&m_bih, 0, sizeof(BITMAPINFOHEADER));
	m_colorBits = nullptr;
}

bool Bitmap::Init(const char* path) {
	File file;
	file.Open(path, "rb");
	long int size = file.GetSize();
	m_buffer = new byte[size];
	
	file.Read(m_buffer);
	memcpy(&m_bfh, m_buffer, sizeof(BITMAPFILEHEADER));
	memcpy(&m_bih, m_buffer+ sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));	
	m_colorBits = m_buffer + m_bfh.bfOffBits;
	file.Close();
	return true;
}
bool Bitmap::Init(Bitmap& bitmap)
{
	/*m_bfh = bitmap.m_bfh;
	m_bih = bitmap.m_bih;
	m_colorBits = m_buffer + m_bfh.bfOffBits;*/
	m_buffer = new byte[bitmap.m_bfh.bfSize];
	memcpy(m_buffer, bitmap.m_buffer, bitmap.m_bfh.bfSize);
	memcpy(&m_bfh, m_buffer, sizeof(BITMAPFILEHEADER));
	memcpy(&m_bih, m_buffer + sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));
	m_colorBits = m_buffer + m_bfh.bfOffBits;
	return true;
}
bool Bitmap::Init(const wchar_t* path) {
	File file;
	file.Open(path, L"rb");
	long int size = file.GetSize();
	m_buffer = new byte[size];

	file.Read(m_buffer);
	memcpy(&m_bfh, m_buffer, sizeof(BITMAPFILEHEADER));
	memcpy(&m_bih, m_buffer + sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));
	m_colorBits = m_buffer + m_bfh.bfOffBits;

	return true;
}

bool Bitmap::Init(Bitmap& bitmap)
{
	m_buffer = new byte[bitmap.m_bfh.bfSize];
	memcpy(m_buffer, bitmap.m_buffer, bitmap.m_bfh.bfSize);
	memcpy(&m_bfh, m_buffer, sizeof(BITMAPFILEHEADER));
	memcpy(&m_bih, m_buffer + sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));
	m_colorBits = m_buffer + m_bfh.bfOffBits;
	return true;
}

bool Bitmap::Save(const char* path)
{
	File outputFile;
	bool isFileOpened = outputFile.Open(path, "wb+");
	if(!isFileOpened) {
		outputFile.Close();
		return false;
	}
	outputFile.Write(GetBuffer(), GetFileInfo().bfSize);
	outputFile.Close();
	return true;
}

void Bitmap::Release()
{
	delete[] m_buffer;
	m_buffer = nullptr;
	m_colorBits = nullptr;
	memset(&m_bfh, 0, sizeof(BITMAPFILEHEADER));
	memset(&m_bih, 0, sizeof(BITMAPINFOHEADER));
}

HBITMAP Bitmap::GenerateHBitMap(HDC hdc) {
	HBITMAP bmpTest = CreateDIBitmap(hdc, &m_bih, CBM_INIT, m_colorBits, (BITMAPINFO*)&m_bih, DIB_RGB_COLORS);
	return bmpTest;
}

BITMAPINFOHEADER Bitmap::GetMapInfo() {
	return m_bih;
}
BITMAPFILEHEADER Bitmap::GetFileInfo() {
	return m_bfh;
}

byte* Bitmap::GetBuffer() {
	return m_buffer;
}


bool Bitmap::EncryptText(const char* text) {
	unsigned int size = std::strlen(text);
	if (size <= MAX16BITS) {
		int bits = m_bfh.bfSize - m_bfh.bfOffBits - 24;
		while (size * 6 > bits) {
			DoubleSize();
			bits = m_bfh.bfSize - m_bfh.bfOffBits - 24;
		}

		BitwiseOperationsHelper::SetBytes(size, 2, m_colorBits + 12);
		byte* place = m_colorBits + 24;
		for (int i = 0; i < size; i++) {
			BitwiseOperationsHelper::SetBytes((unsigned int)text[i], 2, place);
			place += 6;
		}
		place = m_colorBits;
		BitwiseOperationsHelper::SetSignEncrypted(place);
		return true;
	}
	else {
		return false;
	}
}

bool Bitmap::CheckSignEncrypted() {
	unsigned int sign = ENCRYPTSIGN;
	unsigned int sign2 = (unsigned int)BitwiseOperationsHelper::ReadBytes(2, m_colorBits);
	if (sign2 == sign) {
		return true;
	}
	return false;
}

const char* Bitmap::ReadEncryptedText() {
	if (CheckSignEncrypted()) {
		unsigned int textLength = BitwiseOperationsHelper::ReadBytes(2, m_colorBits + 12);
		char* returnArray = new char[textLength + 1];
		byte* place = m_colorBits + 24;
		for (int i = 0; i < textLength; i++) {
			returnArray[i] = (char)BitwiseOperationsHelper::ReadBytes(1, place);
			place += 6;
		}
		returnArray[textLength] = '\0';
		return returnArray;
	}
	return nullptr;
}

void Bitmap::DoubleSize() {
	BITMAPINFOHEADER newBIH = m_bih;
	BITMAPFILEHEADER newBFH = m_bfh;

	unsigned int oldSize = m_bfh.bfSize;
	unsigned int oldWidth = m_bih.biWidth;
	unsigned int oldHeight = m_bih.biHeight;
	newBIH.biWidth = oldWidth * 2;
	newBIH.biHeight = oldHeight * 2;
	newBIH.biSizeImage = newBIH.biWidth * newBIH.biHeight * 3;
	int size = newBIH.biSizeImage + m_bfh.bfOffBits;
	newBFH.bfSize = size;

	byte* newBuffer = new byte[size];
	memset(newBuffer, 0, size);

	memcpy(newBuffer, &newBFH, sizeof(BITMAPFILEHEADER));
	byte* cpyPnter = newBuffer + sizeof(BITMAPFILEHEADER);
	memcpy(cpyPnter, &newBIH , sizeof(BITMAPINFOHEADER));
	
	for (int y = 0; y < newBIH.biHeight; y++) {
		for (int x = 0; x < newBIH.biWidth; x++) {
			int newIndex = y * newBIH.biWidth * 3 + x * 3 + m_bfh.bfOffBits;
			int xs = x/2;
			int ys = y/2;
			int oldIndex = ys * m_bih.biWidth * 3 + xs * 3 + m_bfh.bfOffBits;
			//int newXYCoords = y * newBIH.biWidth * 3 + x * 3 + m_bfh.bfOffBits;
			//int oldXYCoords = y * m_bih.biWidth * 3 + x * 3 + m_bfh.bfOffBits;
			memcpy(newBuffer + newIndex, m_buffer + oldIndex, 3);
		}
	}
	delete[] m_buffer;
	m_buffer = newBuffer;
	m_bih = newBIH;
	m_bfh = newBFH;
	m_colorBits = m_buffer + m_bfh.bfOffBits;
}